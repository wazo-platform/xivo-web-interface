<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2014  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'manager', 'abstract.inc'));

class func_key_manager extends manager_abstract
{

	protected $_name			= 'func_key';
	protected $_extensions		= false;
	protected $_func_key		= false;
	protected $_func_key_type	= false;
	protected $_func_key_destination_type = false;
	protected $_func_key_dest_user	= false;
	protected $_func_key_dest_group	= false;
	protected $_func_key_dest_queue	= false;
	protected $_func_key_dest_conference = false;
	protected $_func_key_dest_forward = false;
	protected $_func_key_dest_park_position = false;
	protected $_func_key_dest_paging = false;
	protected $_func_key_dest_features = false;
	protected $_func_key_dest_agent = false;
	protected $_func_key_dest_custom = false;
	protected $_func_key_dest_bsfilter = false;
	protected $_func_key_mapping	= false;
	protected $_func_key_template	= false;
	protected $_callfiltermember = false;

	public function __construct($_MGR, $param=null)
	{
		parent::__construct($_MGR, $param);
		$this->_load_components();
	}

	private function _load_components()
	{
		$this->_extensions = $this->_service->get_module('ipbx','extensions');
		$this->_func_key = $this->_service->get_module('ipbx','func_key');
		$this->_func_key_type = $this->_service->get_module('ipbx','func_key_type');
		$this->_func_key_destination_type = $this->_service->get_module('ipbx','func_key_destination_type');
		$this->_func_key_dest_user = $this->_service->get_module('ipbx','func_key_dest_user');
		$this->_func_key_dest_group = $this->_service->get_module('ipbx','func_key_dest_group');
		$this->_func_key_dest_queue = $this->_service->get_module('ipbx','func_key_dest_queue');
		$this->_func_key_dest_conference = $this->_service->get_module('ipbx','func_key_dest_conference');
		$this->_func_key_dest_forward = $this->_service->get_module('ipbx','func_key_dest_forward');
		$this->_func_key_dest_park_position = $this->_service->get_module('ipbx', 'func_key_dest_park_position');
		$this->_func_key_dest_paging = $this->_service->get_module('ipbx', 'func_key_dest_paging');
		$this->_func_key_dest_features = $this->_service->get_module('ipbx', 'func_key_dest_features');
		$this->_func_key_dest_custom = $this->_service->get_module('ipbx', 'func_key_dest_custom');
		$this->_func_key_dest_agent = $this->_service->get_module('ipbx', 'func_key_dest_agent');
		$this->_func_key_dest_bsfilter = $this->_service->get_module('ipbx', 'func_key_dest_bsfilter');
		$this->_func_key_mapping = $this->_service->get_module('ipbx','func_key_mapping');
		$this->_func_key_template = $this->_service->get_module('ipbx','func_key_template');
		$this->_callfiltermember = $this->_service->get_module('ipbx','callfiltermember');
	}

	public function get_type_id_by_name($name)
	{
		if (($fk_type = $this->_func_key_type->get_where(array('name' => $name))) === false)
		{
			dwho_report::push('error', 'Error func_key_type with name "'.$name.'" not exist');
			return(false);
		}
		return((int) $fk_type['id']);
	}

	public function get_destination_type_id_by_name($name)
	{
		if (($fk_dst_type = $this->_func_key_destination_type->get_where(array('name' => $name))) === false)
		{
			dwho_report::push('error', 'Error func_key_destination_type with name "'.$name.'" not exist');
			return(false);
		}
		return((int) $fk_dst_type['id']);
	}

	public function get_destination_type_id_by_fk($func_key_id)
	{
		$fk = $this->_func_key->get($func_key_id);
		if ($fk === false)
		{
			dwho_report::push('error', 'Error func_key with id "'.$func_key_id.'" not exist');
			return(false);
		}
		return((int) $fk['destination_type_id']);
	}

	public function get_fk_id_with_dst($dst_type, $dst_val)
	{
		if($dst_type === 'service') {
			$func_key_id = $this->_func_key->get_func_key_id('service', $dst_val, 'extension_id');
		} else if ($dst_type === 'bsfilter') {
			$func_key_id = $this->_func_key->get_func_key_id('bsfilter', $dst_val, 'filtermember_id');
		} else {
			$func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id($dst_type, $dst_val);
		}

		if ((int) $func_key_id <= 0)
		{
			dwho_report::push('error', 'Error: func_key_id "'.$func_key_id.'" is not valid');
			return(false);
		}

		return((int) $func_key_id);
	}

	public function get_dst_id_for_extension($extension_type)
	{
		$dst_id = $this->_extensions->get_extenfeature_id_by_typeval($extension_type);
		if($dst_id === false) {
			dwho_report::push('error', 'did not find extension for service of type '.$service_type);
			return(false);
		}
		return($dst_id);
	}

	public function get_dst_id_for_features($feature_type)
	{
		$dst_id = $this->_func_key_dest_features->get_func_key_id_by_feature_type($feature_type);
		if ($dst_id === false) {
			dwho_report::push('error', 'did not find feature func key of type '.$feature_type);
			return(false);
		}

		return($dst_id);
	}

	public function get_dst_id_for_paging($number)
	{
		$dst_id = $this->_func_key_dest_paging->get_func_key_id_by_number($number);
		if($dst_id == false) {
			dwho_report::push('error', 'did not find paging func key with number '.$number);
			return(false);
		}
		return($dst_id);
	}

	public function get_dst_id_for_agent($agent_id, $action)
	{
		$dst_id = $this->_func_key_dest_agent->get_func_key_id_by_agent_and_action($agent_id, $action);
		if($dst_id == false) {
			dwho_report::push('error', 'did not find agent func key with id '.$agent_id.' and action '.$action);
			return(false);
		}
		return($dst_id);
	}

	public function add_fk_mapping($data)
	{
		$fk_dst_type_id = $this->get_destination_type_id_by_fk($data['func_key_id']);
		if($fk_dst_type_id === false)
			return(false);

		$func_key_mapping = array(
			'template_id' => (int) $data['template_id'],
			'func_key_id' => (int) $data['func_key_id'],
			'destination_type_id' => (int) $fk_dst_type_id,
			'position' => (int) $data['position'],
			'blf' => (bool) $data['blf'],
			'label' => (string) $data['label']
		);

		if(($rs = $this->_func_key_mapping->chk_values($func_key_mapping)) === false)
			return(false);

		$where = array(
			'template_id' => (int) $data['template_id'],
			'func_key_id' => (int) $data['func_key_id']
		);
		if (($this->_func_key_mapping->get_where($where)))
			$this->_func_key_mapping->edit($where, $rs);
		else
			$this->_func_key_mapping->add($rs);

		return(true);
	}

	public function delete_fk_in_template($template_id)
	{
		$func_keys = $this->_func_key->get_func_key_by_template_id($template_id);
		if($func_keys === false)
			return(true);

		foreach($func_keys as $func_key) {
			if($func_key['destination_type_name'] === 'forward')
			{
				if($this->delete_dst_for_forward($func_key['id']) === false)
					return(false);
			}
			elseif ($func_key['destination_type_name'] === 'park_position')
			{
				if($this->delete_dst_for_park_position($func_key['id']) === false)
					return(false);
			}
			elseif ($func_key['destination_type_name'] === 'custom')
			{
				if($this->delete_dst_for_custom($func_key['id']) === false)
					return(false);
			}
		}

		$where = array(
			'template_id' => (int) $template_id
		);
		$this->_func_key_mapping->delete_where($where);
	}

	public function add_private_tpl($name)
	{
		$func_key_template = array(
			'name' => $name,
			'private' => true
		);

		if(($rs = $this->_func_key_template->chk_values($func_key_template)) === false
		|| ($func_key_template_id = $this->_func_key_template->add($rs)) === false)
			return(false);

		return((int) $func_key_template_id);
	}

	public function delete_template($template_id)
	{
		if($this->delete_fk_in_template($template_id) === false
		   || $this->_func_key_template->delete($template_id) === false)
			return(false);
	}

	public function add_dst_for_user($user_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('user');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest_user = array(
			'user_id' => (int) $user_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_user->chk_values($func_key_dest_user)) === false
		|| $this->_func_key_dest_user->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_user($user_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('user', $user_id)))
			$this->_delete_func_key((int) $func_key_id, 'user');

		return(true);
	}

	public function add_dst_for_group($group_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('group');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'group_id' => (int) $group_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_group->chk_values($func_key_dest)) === false
		|| $this->_func_key_dest_group->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_group($group_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('group', $group_id)))
			$this->_delete_func_key((int) $func_key_id, 'group');

		return(true);
	}

	public function add_dst_for_queue($queue_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('queue');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'queue_id' => (int) $queue_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_queue->chk_values($func_key_dest)) === false
		|| $this->_func_key_dest_queue->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_queue($queue_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('queue', $queue_id)))
			$this->_delete_func_key((int) $func_key_id, 'queue');

		return(true);
	}

	public function add_dst_for_conference($conference_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('conference');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'conference_id' => (int) $conference_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_conference->chk_values($func_key_dest)) === false
		|| $this->_func_key_dest_conference->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_conference($conference_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('conference', $conference_id)))
			$this->_delete_func_key((int) $func_key_id, 'conference');

		return(true);
	}

	public function add_dst_for_forward($extension_id, $number=null)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('forward');

		if($fk_type_id === false || $fk_dst_type_id === false)
			return(false);

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'extension_id' => (int) $extension_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id,
			'number' => $number,
		);

		$rs = $this->_func_key_dest_forward->chk_values($func_key_dest);
		if($rs === false)
			return(false);

		if ($this->_func_key_dest_forward->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_forward($func_key_id)
	{
		return($this->_delete_func_key((int) $func_key_id, 'forward'));
	}

	public function add_dst_for_park_position($park_position)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('park_position');

		if($fk_type_id === false || $fk_dst_type_id === false)
			return(false);

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
		return(false);

		$func_key_dest = array(
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id,
			'park_position' => $park_position
		);

		$rs = $this->_func_key_dest_park_position->chk_values($func_key_dest);
		if($rs === false)
			return(false);

		if ($this->_func_key_dest_park_position->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_park_position($func_key_id)
	{
		return($this->_delete_func_key((int) $func_key_id, 'park_position'));
	}

	public function add_dst_for_paging($paging_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('paging');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'paging_id' => (int) $paging_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_paging->chk_values($func_key_dest)) === false
		|| $this->_func_key_dest_paging->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_paging($paging_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('paging', $paging_id)))
			$this->_delete_func_key((int) $func_key_id, 'paging');

		return(true);
	}

	public function add_dst_for_custom($exten)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('custom');

		if($fk_type_id === false || $fk_dst_type_id === false)
			return(false);

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id,
			'exten' => $exten,
		);

		$rs = $this->_func_key_dest_custom->chk_values($func_key_dest);
		if($rs === false)
			return(false);

		if ($this->_func_key_dest_custom->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_custom($func_key_id)
	{
		return($this->_delete_func_key((int) $func_key_id, 'custom'));
	}

	public function add_dst_for_agent($agent_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('agent');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		$actions = array('toggle', 'login', 'logoff');
		foreach($actions as $action)
		{
			if(($rs = $this->_func_key->chk_values($func_key)) === false
			|| ($func_key_id = $this->_func_key->add($rs)) === false)
				return(false);

			$func_key_dest = array(
				'agent_id' => (int) $agent_id,
				'action' => (string) $action,
				'func_key_id' => (int) $func_key_id,
				'destination_type_id' => (int) $fk_dst_type_id
			);

			$rs = $this->_func_key_dest_agent->chk_values($func_key_dest);
			$add_rs = $this->_func_key_dest_agent->add($rs);

			if($rs === false || $add_rs === false)
				return(false);
		}

		return((int) $func_key_id);
	}

	public function delete_dst_for_agent($agent_id)
	{
		$func_key_ids = $this->_func_key_dest_agent->func_key_ids_for_agent($agent_id);
		if($func_key_ids === false)
			return(false);

		foreach($func_key_ids as $func_key_id)
		{
			if($this->_delete_func_key((int) $func_key_id, 'agent') === false)
				return(false);
		}

		return(true);
	}

	public function add_dst_for_bsfilter($bsfilter_id)
	{
		$filtermember_ids = $this->_callfiltermember->get_secretary_member_ids_for_bsfilter($bsfilter_id);
		if($filtermembers_ids === false)
			return(false);

		foreach($filtermember_ids as $filtermember_id) {
			$rs = $this->create_bsfilter_func_key_for_filtermember_id($filtermember_id);
			if($rs === false)
				return(false);
		}
	}

	public function create_bsfilter_func_key_for_filtermember_id($filtermember_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('bsfilter');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'filtermember_id' => (int) $filtermember_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		$rs = $this->_func_key_dest_bsfilter->chk_values($func_key_dest);
		$add_rs = $this->_func_key_dest_bsfilter->add($rs);

		if($rs === false || $add_rs === false)
			return(false);
	}

	public function delete_dst_for_bsfilter($bsfilter_id)
	{
		$func_key_ids = $this->_func_key_dest_bsfilter->get_func_key_ids_for_bsfilter($bsfilter_id);
		if($func_key_ids === false)
			return(false);

		foreach($func_key_ids as $func_key_id)
		{
			if($this->_delete_func_key((int) $func_key_id, 'bsfilter') === false)
				return(false);
		}
	}

	public function delete_bsfilter_for_user($user_id)
	{
		$func_key_ids = $this->_func_key_dest_bsfilter->get_func_key_ids_for_user($user_id);
		if($func_key_ids === false)
			return(false);

		foreach($func_key_ids as $func_key_id)
		{
			if($this->_delete_func_key((int) $func_key_id, 'bsfilter') === false)
				return(false);
		}
	}

	private function _delete_bsfilter_func_key($filtermember_id)
	{
		$func_key_id = $this->_func_key_dest_bsfilter->get_func_key_id_for_filtermember_id($filtermember_id);
		if($func_key_id === false)
			return(false);

		if($this->_delete_func_key((int) $func_key_id, 'bsfilter') === false)
			return(false);
	}

	private function _delete_func_key($func_key_id, $dst_type)
	{
		$where_fk_id = array('func_key_id' => $func_key_id);
		if ($this->_func_key_mapping->delete_where($where_fk_id) === false)
			return(false);

		$fk_dst_type_id = $this->get_destination_type_id_by_name($dst_type);

		$where_func_key_dest = $where_fk_id;
		$where_func_key_dest['destination_type_id'] = (int) $fk_dst_type_id;

		switch ($dst_type)
		{
			case 'user':
				if ($this->_func_key_dest_user->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'group':
				if ($this->_func_key_dest_group->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'queue':
				if ($this->_func_key_dest_queue->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'conference':
				if ($this->_func_key_dest_conference->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'forward':
				if ($this->_func_key_dest_forward->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'park_position':
				if ($this->_func_key_dest_park_position->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'paging':
				if ($this->_func_key_dest_paging->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'custom':
				if ($this->_func_key_dest_custom->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'agent':
				if ($this->_func_key_dest_agent->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'bsfilter':
				if ($this->_func_key_dest_bsfilter->delete_where($where_func_key_dest) === false)
					return(false);
				break;
		}

		if ($this->_func_key->delete($func_key_id) === false)
			return(false);

		return(true);
	}
}

?>
