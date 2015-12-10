<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2015  Avencall
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'voicemail', 'mapper.inc'));

class xivo_application_service_asterisk_voicemail extends xivo_application_asterisk_abstract
{
	var $_zonemessages	= null;
	var $_timezones		= array();
	var $_contextlist	= array();
	var $_modules		= array();
	var $_confd			= null;
	var $_context		= null;
	var $_entity		= null;
	var $_vm_mapper		= null;

	public function __construct(&$service,&$dso,$param=null,$components=true)
	{
		parent::__construct($service);

		$_RAPI = dwho_gct::get('xivo_confd');
		if($_RAPI === false) {
			trigger_error('Invalid CONFD object',E_USER_ERROR);
			return(false);
		}

		$_XOBJ = dwho_gct::get('xivo_object');
		if($_XOBJ === false) {
			trigger_error('Invalid XiVO object',E_USER_ERROR);
			return(false);
		}

		$this->_confd = $_RAPI->get_ressource('voicemail');
		if($this->_confd === false) {
			trigger_error('Cannot load voicemail confd resource',E_USER_ERROR);
			return(false);
		}

		$this->_context = $this->_service->get_module('context');
		if($this->_context === false) {
			trigger_error('error', 'Cannot load context module', E_USER_ERROR);
			return(false);
		}

		$this->_entity = $_XOBJ->get_module('entity');
		if($this->_entity === false) {
			trigger_error('error', 'Cannot load entity module', E_USER_ERROR);
			return(false);
		}

		$this->_vm_mapper = new VoicemailMapper();

		if((bool) $components === true)
			$this->load_components();

		return(true);
	}

	function load_components()
	{
		$this->_components = true;

		if(($appvoicemail = &$this->_service->get_apprealstatic('voicemail')) !== false)
			$this->_zonemessages = &$appvoicemail->get_module('zonemessages');

		return(true);
	}

	function get($id)
	{
		$voicemail = $this->_confd->get($id);
		if($voicemail === false) {
			dwho_report::push('error', 'Cannot get voicemail ', $id);
			return(false);
		}
		return $this->map_voicemail($voicemail);
	}
			
	function get_nb($arr=null,$disable=null)
	{
		$voicemails = $this->_confd->find_all();
		if($voicemails === false) {
			dwho_report::push('error', 'Cannot get number of voicemails');
			return(false);
		}

		if($disable !== null) {
			$enabled = !((bool)$disable);
			$nb = 0;
			foreach($voicemails as $voicemail) {
				if($voicemail['enabled'] === $enabled) {
					$nb++;
				}
			}
			return $nb;
		}

		return count($voicemails);
	}

	function get_timezones()
	{
		if(is_object($this->_zonemessages) === false)
			return(false);
		else if(is_array($this->_timezones) === true
		&& empty($this->_timezones) === false)
			return($this->_timezones);

		return(($this->_timezones = $this->_zonemessages->get_all_name()));
	}

	function get_voicemail_search($search='',$order=null,$limit=null)
	{
		$voicemails = $this->_confd->find_all($search, $order, $limit);
		if($voicemails === false) {
			dwho_report::push('error', 'Cannot search for voicemails');
			return(false);
		}

		$voicemails = $this->filter_on_entity($voicemails);
		$voicemails = $this->map_voicemails($voicemails);
		return $voicemails;
	}

	function get_search_total()
	{
		return $this->_confd->get_total();
	}

	private function filter_on_entity(&$voicemails) 
	{
		if(!defined('ENTITY_FILTER_CONTEXTS'))
			return $voicemails;

		$contexts = explode(',', ENTITY_FILTER_CONTEXTS);

		$filtered = array();
		foreach($voicemails as &$voicemail) {
			if(in_array($voicemail['context'], $contexts)) {
				$filtered[] = $voicemail;
			}
		}

		return $filtered;
	}

	private function map_voicemails(&$voicemails) 
	{
		foreach($voicemails as &$voicemail) {
			$voicemail = $this->map_voicemail($voicemail);
		}

		return $voicemails;
	}

	private function map_voicemail(&$voicemail) {
		$voicemail['identity'] = "${voicemail['name']} (${voicemail['number']}@${voicemail['context']})";

		$context = $this->_context->get($voicemail['context']);
		if($context === false)
			return(false);

		$entity = $this->_entity->get_where(array('name' => $context['entity']));
		if($entity === false) {
			$voicemail['entity_displayname'] = '';
		} else {
			$voicemail['entity_displayname'] = $entity['displayname'];
		}

		return $voicemail;
	}

	function add($form)
	{
		$voicemail = $this->convert_form($form);
		$voicemail_id = $this->_confd->add($voicemail);
		if($voicemail_id === false) {
			dwho_report::push('error', 'Cannot create voicemail');
			return(false);
		}
		return($voicemail_id);
	}

	function edit($voicemail_id, $form)
	{
		$voicemail = $this->convert_form($form);

		$status = $this->_confd->edit($voicemail_id, $voicemail);
		if($status === false) {
			dwho_report::push('error', 'Cannot edit voicemail');
			return(false);
		}
		return(true);
	}

	private function convert_form($form) {
		return $this->_vm_mapper->map_form($form['voicemail']);
	}

	function delete($voicemail_id)
	{
		$success = $this->dissociate_users_from_voicemail($voicemail_id);
		if($success === false) {
			dwho_report::push('error', 'Cannot dissociate users from voicemail');
			return(false);
		}

		$status = $this->_confd->delete($voicemail_id);
		if($status === false) {
			dwho_report::push('error', 'Cannot delete voicemail');
			return(false);
		}

		return $this->unlinks($voicemail_id);
	}

	function unlinks($voicemail_id)
	{
		if(dwho_issa('voicemail',$this->_info) === false)
			return(false);
		else if(($dialaction = &$this->_service->get_module('dialaction')) !== false)
		{
			$where = array();
			$where['action'] = 'voicemail';
			$where['actionarg1'] = (string) $voicemail_id;

			$dialaction->unlinked_where($where);
		}

		if(($userfeatures = &$this->_service->get_module('userfeatures')) !== false)
		{
			$where = array();
			$where['voicemailid'] = $voicemail_id;

			$userfeatures->edit_where($where,array('voicemailid' => 0,
												'enablevoicemail' => 0));
		}

		return(true);
	}

	function enable($voicemail_id)
	{
		$voicemail = array('enabled' => true);
		$status = $this->_confd->edit($voicemail_id, $voicemail);
		if($status === false) {
			dwho_report::push('error', 'Cannot enable voicemail ' . $voicemail_id);
			return(false);
		}
	}

	function disable($voicemail_id)
	{
		$voicemail = array('enabled' => false);
		$status = $this->_confd->edit($voicemail_id, $voicemail);
		if($status === false) {
			dwho_report::push('error', 'Cannot disable voicemail ' . $voicemail_id);
			return(false);
		}
	}

	function associate($voicemail_id, $user_id) 
	{
		return $this->_confd->associate_user_voicemail($user_id, $voicemail_id);
	}

	function dissociate($user_id)
	{
		$association = $this->_confd->get_user_voicemail($user_id);
		if($association !== false) {
			return $this->_confd->dissociate_user_voicemail($user_id);
		}
	}

	function dissociate_users_from_voicemail($voicemail_id)
	{
		$associations = $this->_confd->get_associations_for_voicemail($voicemail_id);
		if($associations === false) {
			dwho_report::push('error', 'Cannot get users associated to voicemail ' . $voicemail_id);
			return(false);
		}

		foreach($associations as $association) {
			$success = $this->_confd->dissociate_user_voicemail($association['user_id']);
			if($success === false) {
				dwho_report::push('error', 'Cannot dissociate voicemail from user'. $association['user_id']);
				return(false);
			}
		}
	}

	function get_voicemail_id_for_user($user_id)
	{
		$association = $this->_confd->get_user_voicemail($user_id);
		if($association === false)
			return false;

		return $association['voicemail_id'];

	}
}

?>
