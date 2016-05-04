<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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

dwho::load_class('dwho_network');
dwho::load_class('dwho_uri');
dwho::load_class('dwho_filter');
dwho::load_class('dwho_http');
dwho::load_class('dwho::file::csv');

dwho_file::required(array(XIVO_PATH_OBJECT,'service','ipbx','asterisk','config','config.inc'),true);
dwho_file::required(array(XIVO_PATH_OBJECT,'service','ipbx','asterisk.inc',),true);
dwho_file::required(array(XIVO_PATH_OBJECT,'application','xivo.inc'),true);

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'client.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'wizard', 'wizard.inc'));

class xivo_application_xivo_object_wizard extends xivo_application_xivo_object_abstract
{
	var $_conf             = array();
	var $_sysconfd         = false;
	var $_filter           = false;
	var $_uri              = false;
	var $_http_response    = false;
	var $_wizsess          = null;
	var $_ipbx             = null;
	var $_confd_wizard     = null;

	public function __construct(&$xobj,$param=null,$components=true)
	{
		parent::__construct($xobj);

		if(($this->_sysconfd = &$this->_xobj->get_module('sysconfd')) === false)
			trigger_error('Unable to load sysconfd module',E_USER_ERROR);

		$this->_uri = new dwho_uri();
		$this->_filter = new dwho_filter();
		$this->_http_response = dwho_http::factory('response');

		$nonraising_client = Client::default_config();
		$this->_confd_wizard = new WizardResource($nonraising_client);;

		if(dwho_issa('_wizard',$_SESSION) === false)
			$_SESSION['_wizard'] = array();

		$this->_load_config(dirname(__FILE__));

		$this->_wizsess = &$_SESSION['_wizard'];

	}

	function get_first_step()
	{
		reset($this->_conf['step']);
		return(key($this->_conf['step']));
	}

	function get_last_step()
	{
		end($this->_conf['step']);
		return(key($this->_conf['step']));
	}

	function get_current_step()
	{
		if(isset($this->_wizsess['step']) === false)
			$this->_wizsess['step'] = $this->get_first_step();

		return($this->_wizsess['step']);
	}

	function get_previous_sess_step()
	{
		if(isset($this->_wizsess['prevstep']) === false)
			return(false);

		return($this->_wizsess['prevstep']);
	}

	function get_previous_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];

		if(isset($this->_conf['step'][$step]) === false)
			return(false);

		if($step === $this->get_last_step())
		{
			end($this->_conf['step']);
			prev($this->_conf['step']);
		}
		else
		{
			reset($this->_conf['step']);
			while((list($key) = each($this->_conf['step']))
			&& $key !== $step);
			prev($this->_conf['step']);
			prev($this->_conf['step']);
		}

		return(key($this->_conf['step']));
	}

	function can_previous_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];

		if($step === $this->get_first_step())
			return(false);

		$prevstep = $this->get_previous_step($step);

		if(dwho_issa('commit',$this->_wizsess) === true
		&& dwho_issa('step',$this->_wizsess['commit']) === true
		&& isset($this->_wizsess['commit']['step'][$prevstep]) === true)
			return(false);
		else if($step === $this->get_last_step())
			return($this->get_previous_step($step));
		else if(isset($this->_conf['step'][$prevstep]) === false)
			return(false);

		return($prevstep);
	}

	function set_previous_step()
	{
		$step = $this->get_current_step();

		if(($step = $this->can_previous_step($step)) === false)
			return(false);

		$this->_wizsess['step'] = $step;
	}

	function get_next_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
		$step = $this->_wizsess['step'];

		if(isset($this->_conf['step'][$step]) === false)
			return(false);

		reset($this->_conf['step']);
		while((list($key) = each($this->_conf['step']))
		&& $key !== $step);

		return(key($this->_conf['step']));
	}

	function can_next_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];

		if($step === $this->get_last_step())
			return(false);

		return($step);
	}

	function get_step_element()
	{
		if(isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];
		else
			$step = 'welcome';

		switch($step)
		{
			case 'mainconfig':
				return $this->mainconfig_discover();
			case 'entitycontext':
				return(array('entity'	=> $this->_conf['element']['entity'],
							'context'	=> $this->_conf['element']['context']));
			default:
				return(null);
		}
	}

	function get_interface_list()
	{
		if(isset($this->_wizsess['interface_list']) === true) {
			return($this->_wizsess['interface_list']);
		}
		return(array());
	}

	function get_gateway_list()
	{
		if(isset($this->_wizsess['gateway_list']) === true) {
			return($this->_wizsess['gateway_list']);
		}
		return(array());
	}

	function mainconfig_discover()
	{
		$response = $this->_confd_wizard->discover();
		if($response === false) {
			return array();
		}
		$mainconfig = array();
		$mainconfig['hostname'] = $response['hostname'];
		$mainconfig['domain'] = $response['domain'];
		$resolvconf = array();
		$resolvconf['nameserver1'] = $response['nameservers'][0];
		$resolvconf['nameserver2'] = isset($response['nameservers'][1])?$response['nameservers'][1]:'';
		$netiface_id = array();
		$netiface_id['address'] = 0;
		$netiface_id['gateway'] = 0;

		$this->_wizsess['timezone'] = $response['timezone'];

		$i = 0;
		$this->_wizsess['interface_list'] = array();
		foreach($response['interfaces'] as $item)
		{
			$display = $item['ip_address'] . ' (' . $item['interface'] . ')';
			$this->_wizsess['interface_list'][$i] = $display;
			$this->_wizsess['interface_items'][$i]['address'] = $item['ip_address'];
			$this->_wizsess['interface_items'][$i]['netmask'] = $item['netmask'];
			$this->_wizsess['interface_items'][$i]['interface'] = $item['interface'];
			$i++;
		}

		$i = 0;
		$this->_wizsess['gateway_list'] = array();
		foreach($response['gateways'] as $item)
		{
			$display = $item['gateway'] . ' (' . $item['interface'] . ')';
			$this->_wizsess['gateway_list'][$i] = $display;
			$this->_wizsess['gateway_items'][$i]['gateway'] = $item['gateway'];
			$i++;
		}

		return(array('mainconfig'	=> $mainconfig,
					 'resolvconf'	=> $resolvconf,
					 'netiface_id'		=> $netiface_id));
	}

	function step_mainconfig()
	{
		$r = array();

		if(dwho_issa('mainconfig',$this->_wizsess) === true)
			$r['mainconfig'] = $this->_wizsess['mainconfig'];
		else
			$r['mainconfig'] = null;

		if(dwho_issa('netiface_id',$this->_wizsess) === true)
			$r['netiface_id'] = $this->_wizsess['netiface_id'];
		else
			$r['netiface_id'] = null;

		if(dwho_issa('resolvconf',$this->_wizsess) === true)
			$r['resolvconf'] = $this->_wizsess['resolvconf'];
		else
			$r['resolvconf'] = null;

		return($r);
	}

	function step_entitycontext()
	{
		$r = array();

		if(dwho_issa('entity',$this->_wizsess) === true)
			$r['entity'] = $this->_wizsess['entity'];
		else
			$r['entity'] = null;

		if(dwho_issa('context_internal',$this->_wizsess) === true)
			$r['context_internal'] = $this->_wizsess['context_internal'];
		else
			$r['context_internal'] = null;

		if(dwho_issa('context_incall',$this->_wizsess) === true)
			$r['context_incall'] = $this->_wizsess['context_incall'];
		else
			$r['context_incall'] = null;

		if(dwho_issa('context_outcall',$this->_wizsess) === true)
			$r['context_outcall'] = $this->_wizsess['context_outcall'];
		else
			$r['context_outcall'] = null;

		return($r);
	}

	function step_validate()
	{
		return($this->_wizsess);
	}

	function save_step_welcome($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		if(is_array($data) === false
		|| isset($data['hl']) === false)
		{
			$this->_set_error('welcome','language','missing');
			return(false);
		}

		$this->_wizsess['welcome'] = array();

		$_LANG = &dwho_gat::load_get('language',XIVO_PATH_OBJECTCONF);
		$language_available = dwho_array_intersect_key($_LANG,dwho_i18n::get_language_translated_list());

		if(isset($language_available[$data['hl']]) === false)
		{
			$this->_set_error('welcome','language','invalid');
			return(false);
		}

		$_SESSION['_i18n']['user'] = array();
		$_SESSION['_i18n']['user']['language'] = $data['hl'];

		$this->_wizsess['welcome']['language'] = $data['hl'];
		$this->_wizsess['step'] = $this->get_next_step('welcome');

		return(true);
	}

	function save_step_license($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		if(is_array($data) === false
		|| isset($data['license-agree']) === false
		|| (bool) $data['license-agree'] === false)
		{
			$this->_set_error('license','disagree');
			return(false);
		}

		$this->_wizsess['license'] = array();
		$this->_wizsess['license']['agree'] = true;

		$this->_wizsess['step'] = $this->get_next_step('license');

		return(true);
	}

	function save_mainconfig($data)
	{
		if(dwho_issa('mainconfig',$data) === false)
		{
			$this->_set_error('mainconfig','hostname','missing');
			return(false);
		}

		$this->_filter->set_validator($this->_conf['filter']['mainconfig']);

		if(($r = (bool) $this->_filter->chk($data['mainconfig'])) === false)
			$this->_set_error('mainconfig',$this->_filter->get_error());

		$this->_set_result('mainconfig',$this->_filter->get_result());

		return($r);
	}

	function save_netiface($data)
	{
		if(dwho_issa('netiface_id',$data) === false)
		{
			$this->_set_error('netiface','address','missing');
			return(false);
		}

		$interface_id = $data['netiface_id']['address'];
		$gateway_id = $data['netiface_id']['gateway'];
		$data['netiface'] = $this->_wizsess['interface_items'][$interface_id];
		$data['netiface']['gateway'] = $this->_wizsess['gateway_items'][$gateway_id]['gateway'];

		$this->_filter->set_validator($this->_conf['filter']['netiface']);

		if(($r = (bool) $this->_filter->chk($data['netiface'])) === false)
			$this->_set_error('netiface',$this->_filter->get_error());

		$resnetiface = $this->_filter->get_result();
		$this->_set_result('netiface',$resnetiface);
		$this->_set_result('netiface_id',$data['netiface_id']);

		return($r);
	}

	function save_resolvconf($data)
	{
		if(dwho_issa('resolvconf',$data) === false)
		{
			$this->_set_error('resolvconf','nameserver1','invalid');
			return(false);
		}

		$this->_filter->set_validator($this->_conf['filter']['resolvconf']);

		if(($r = (bool) $this->_filter->chk($data['resolvconf'])) === false)
			$this->_set_error('resolvconf',$this->_filter->get_error());

		$result = $this->_filter->get_result();

		$this->_set_result('resolvconf',$result);

		return($r);
	}

	function save_step_mainconfig($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		$this->save_mainconfig($data);
		$this->save_netiface($data);
		$this->save_resolvconf($data);

		if($this->get_errnb() > 0)
			return(false);

		$this->_wizsess['mainconfig'] = $this->get_result('mainconfig');
		$this->_wizsess['netiface'] = $this->get_result('netiface');
		$this->_wizsess['netiface_id'] = $this->get_result('netiface_id');
		$this->_wizsess['resolvconf'] = $this->get_result('resolvconf');
		$this->_wizsess['step'] = $this->get_next_step('mainconfig');

		return(true);
	}

	function save_entity($data)
	{
		if(dwho_issa('entity',$data) === false)
		{
			$this->_set_error('entity','name','missing');
			return(false);
		}

		$this->_filter->set_validator($this->_conf['filter']['entity']);

		if(($r = (bool) $this->_filter->chk($data['entity'])) === false) {
			$this->_set_error('entity',$this->_filter->get_error());
		}

		$result = $this->_filter->get_result();
		$this->_set_result('entity',$result);

		return($r);
	}

	function save_context($data)
	{
		if(dwho_issa('context',$data) === false)
		{
			$this->_set_error('context_internal','missing');
-			$this->_set_error('context_incall','missing');
-			$this->_set_error('context_outcall','missing');
			return(false);
		}

		if(dwho_issa('outcall',$data['context']) === false ||
			dwho_issa('internal', $data['context']) === false ||
			dwho_issa('incall', $data['context']) === false) {
			$this->_set_error('context','missing_context');
			return(false);
		}

		$resoutcall = false;
		$this->_filter->set_validator($this->_conf['filter']['context']['outcall']);
		if(($resoutcall = (bool) $this->_filter->chk($data['context']['outcall'])) === false) {
			$this->_set_error('context_outcall',$this->_filter->get_error());
		}
		$outcall = $this->_filter->get_result();
		$this->_set_result('context_outcall',$outcall);

		$resinternal = false;
		$this->_filter->set_validator($this->_conf['filter']['context']['internal']);
		if(($resinternal = (bool) $this->_filter->chk($data['context']['internal'])) === false) {
			$this->_set_error('context_internal',$this->_filter->get_error());
		}
		$internal = $this->_filter->get_result();
		$this->_set_result('context_internal',$internal);

		$resincall = false;
		$this->_filter->set_validator($this->_conf['filter']['context']['incall']);
		if(($resincall = (bool) $this->_filter->chk($data['context']['incall'])) === false) {
			$this->_set_error('context_incall',$this->_filter->get_error());
		}
		$incall = $this->_filter->get_result();
		$this->_set_result('context_incall',$incall);

		if($resoutcall === false
			|| $resinternal === false
			|| $resincall === false) {
			return(false);
		}

		$contexts = array('outcall' => $outcall,
					'internal' => $internal,
					'incall' => $incall);
		return($contexts);
	}

	function save_step_entitycontext($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		$this->save_entity($data);
		$rescontext = $this->save_context($data);

		if($this->get_errnb() > 0 || is_array($rescontext) === false)
			return(false);

		$this->_wizsess['entity'] = $this->get_result('entity');
		$this->_wizsess['context_internal'] = $this->get_result('context_internal');
		$this->_wizsess['context_outcall'] = $this->get_result('context_outcall');
		$this->_wizsess['context_incall'] = $this->get_result('context_incall');

		$this->_wizsess['step'] = $this->get_next_step('entitycontext');

		return(true);
	}

	function save_step_validate($data, $tpl)
	{
		if($this->post_confd_wizard() === false) {
			return(false);
		}

		session_destroy();
		return(true);
	}
	public function post_confd_wizard()
	{
		$wizsess = $this->_wizsess;
		if($wizsess['welcome']['language'] == 'fr') {
			$language = 'fr_FR';
		} else {
			$language = 'en_US';
		}
		$body = array('admin_password' => $wizsess['mainconfig']['adminpasswd'],
					'timezone' => $wizsess['timezone'],
					'language' => $language,
					'entity_name' => $wizsess['entity']['displayname'],
					'license' => $wizsess['license']['agree'],
					'network' => array('nameservers' => array($wizsess['resolvconf']['nameserver1']),
									'hostname' => $wizsess['mainconfig']['hostname'],
									'interface' => $wizsess['netiface']['interface'],
									'ip_address' => $wizsess['netiface']['address'],
									'netmask' => $wizsess['netiface']['netmask'],
									'domain' => $wizsess['mainconfig']['domain'],
									'gateway' => $wizsess['netiface']['gateway']),
					'context_incall' => array('display_name' => $wizsess['context_incall']['displayname'],
											'did_length' => $wizsess['context_incall']['didlength']),
					'context_internal' => array('display_name' => $wizsess['context_internal']['displayname'],
											'number_start' => $wizsess['context_internal']['numberbeg'],
											'number_end' => $wizsess['context_internal']['numberend']),
					'context_outcall' => array('display_name' => $wizsess['context_outcall']['displayname'])
				);
		if(empty($wizsess['resolvconf']['nameserver2']) === false) {
			array_push($body['network']['nameservers'], $wizsess['resolvconf']['nameserver2']);
		}

		if(empty($wizsess['context_incall']['numberbeg']) === false) {
			$body['context_incall']['number_start'] = $wizsess['context_incall']['numberbeg'];
		}

		if(empty($wizsess['context_incall']['numberend']) === false) {
			$body['context_incall']['number_end'] = $wizsess['context_incall']['numberend'];
		}

		$result = $this->_confd_wizard->create($body);
		return($result);
	}

	function save($data, $tpl)
	{
		$step = $this->get_current_step();

		if(isset($this->_conf['step'][$step]) === false)
			return(null);

		$save_step_function_name = 'save_step_'.$step;

		return($this->$save_step_function_name($data, $tpl));
	}
}

?>
