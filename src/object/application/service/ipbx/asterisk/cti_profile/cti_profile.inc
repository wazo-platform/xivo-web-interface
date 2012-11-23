<?php
#
# XiVO Web-Interface
# Copyright (C) 2006-2012  Avencall
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

class xivo_application_service_asterisk_cti_profile extends xivo_application_asterisk_abstract
{
	var $_cti_profile = false;
	var $_cti_profile_service = false;
	var $_cti_profile_preference = false;
	var $_cti_profile_xlet = false;
	var $_userfeatures = false;

	public function __construct(&$service,$param=null,$components=true)
	{
		parent::__construct($service);

		if(($this->_cti_profile = &$this->_service->get_module('cti_profile')) === false
		|| ($this->_userfeatures = &$this->_service->get_module('userfeatures')) === false)
			return(false);
		else if((bool) $components === true)
			$this->load_components();

		return(true);
	}

	function load_components()
	{
		$this->_components = true;

		if(($this->_cti_profile_service = &$this->_service->get_module('cti_profile_service')) === false
		|| ($this->_cti_profile_preference = &$this->_service->get_module('cti_profile_preference')) === false
		|| ($this->_cti_profile_xlet = &$this->_service->get_module('cti_profile_xlet')) === false)
			return(false);

		return(true);
	}

	function get($id,$disable=null,$nocomponents=null)
	{
		$this->_info = $this->_origin = array();

		$this->_info['cti_profile'] = $this->_cti_profile->get($id);

		if($this->_components === false)
			return($this->_info);

		if(dwho_ak('service',$nocomponents,true) === false)
			$this->_get_service($this->_info['cti_profile']['id']);

		if(dwho_ak('preference',$nocomponents,true) === false)
			$this->_get_preference($this->_info['cti_profile']['id']);

		if(dwho_ak('xlet',$nocomponents,true) === false)
			$this->_get_xlet($this->_info['cti_profile']['id']);

		return($this->_info);
	}

	function _get_service($profile_id)
	{
		$this->_info['service'] = false;
		$this->_origin['service'] = false;

		$where = array();
		$where['profile_id'] = $profile_id;

		if(is_object($this->_cti_profile_service) === false
		|| ($this->_info['service'] = $this->_cti_profile_service->get_all_where($where)) === false
		|| isset($this->_info['service'][0]) === false)
			return(false);

		$this->_origin['service'] = $this->_cti_profile_service->get_origin_list();

		return($this->_info['service']);
	}

	function _get_preference($profile_id)
	{
		$this->_info['preference'] = false;
		$this->_origin['preference'] = false;

		$where = array();
		$where['profile_id'] = $profile_id;

		if(is_object($this->_cti_profile_preference) === false
		|| ($this->_info['preference'] = $this->_cti_profile_preference->get_all_where($where)) === false
		|| isset($this->_info['preference'][0]) === false)
			return(false);

		$this->_origin['preference'] = $this->_cti_profile_preference->get_origin_list();

		return($this->_info['preference']);
	}

	function _get_xlet($profile_id)
	{
		$this->_info['xlet'] = false;
		$this->_origin['xlet'] = false;

		$where = array();
		$where['profile_id'] = $profile_id;

		if(is_object($this->_cti_profile_xlet) === false
		|| ($this->_info['xlet'] = $this->_cti_profile_xlet->get_all_where($where)) === false
		|| isset($this->_info['xlet'][0]) === false)
			return(false);

		$this->_origin['xlet'] = $this->_cti_profile_xlet->get_origin_list();

		return($this->_info['xlet']);
	}

	function get_elements()
	{
		$r = array();
		$r['cti_profile'] = $this->_cti_profile->get_element();
		$r['service'] = $this->_cti_profile_service->get_element();
		$r['preference'] = $this->_cti_profile_preference->get_element();
		$r['xlet'] = $this->_cti_profile_xlet->get_element();

		return($r);
	}

	function get_cti_profile_list($presence_id=null, $order=null,$limit=null,$assoc=false)
	{
		if($presence_id === null)
			$rs = $this->_cti_profile->get_all();
		else
			$rs = $this->_cti_profile->get_all_where(array('presence_id' => $presence_id));

		if($rs === false || ($nb = count($rs)) === 0)
		{
			$this->_cnt = $this->_cti_profile->get_cnt();
			return(false);
		}

		$r = $info = array();
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];
			$r[$ref['id']] = $this->_cti_profile->_prepare($ref);

			if($this->users_has_profile($ref['name']))
				$r[$ref['id']]['deletable'] = false;
			else
				$r[$ref['id']]['deletable'] = true;
		}

		if(empty($r) === true)
			return(false);
		else if((bool) $assoc === false)
			return(array_values($r));

		return($r);
	}

	function set_edit($arr)
	{
		if(empty($this->_info) === true)
			return(false);

		return($this->_set('edit',$arr));
	}

	function set_add($arr)
	{
		return($this->_set('add',$arr));
	}

	function _set($action,$arr)
	{
		$this->_reset();
		if(($action !== 'add' && $action !== 'edit') === true
		|| dwho_issa('cti_profile',$arr) === false)
			return(false);

		$this->set_cti_profile($arr['cti_profile']);

		if(dwho_issa('services',$arr) === true)
			$this->set_service($arr['services']);

		if(dwho_issa('preference',$arr) === true)
			$this->set_preference($arr['preference']);

		if(dwho_issa('xlet',$arr) === true)
			$this->set_xlet($arr['xlet']);

		return(($this->get_errnb() < 1));
	}

	function set_cti_profile($arr)
	{
		if(is_array($arr) === false)
		{
			$this->_set_result('cti_profile',null);
			$this->_set_error('cti_profile','invalid data');
			return(false);
		}
		else if(($rs = $this->_cti_profile->chk_values($arr)) === false)
		{
			$this->_set_result('cti_profile',$this->_cti_profile->get_filter_result());
			$this->_set_error('cti_profile',$this->_cti_profile->get_filter_error());
			return(false);
		}
		$this->_set_result('cti_profile',$rs);

		return(true);
	}

	function set_service($arr)
	{
		if(is_array($arr) === false
		|| $this->_cti_profile_service === false)
			return(false);

		if(empty($arr))
			return(true);

		$nb = count($arr);
		$errnb = 0;
		$res = $error = $data = array();
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$arr[$i];

			$data['profile_id'] = 0;
			$data['service_id'] = (int) $ref;

			if(($res[$i] = $this->_cti_profile_service->chk_values($data)) === false)
			{
				$errnb++;
				$res[$i] = $this->_cti_profile_service->get_filter_result();
				$error[$i] = $this->_cti_profile_service->get_filter_error();
			}
		}

		$this->_set_result('service',$res);

		if ($errnb >> 0)
		{
			$this->_set_errors('service',$error);
			return(false);
		}

		return(true);
	}

	function set_preference($arr)
	{
		if(is_array($arr) === false
		|| $this->_cti_profile_preference === false)
			return(false);

		$arr = $this->_prepare_list_data_from_form($arr);

		if(empty($arr))
			return(true);

		$nb = count($arr);
		$errnb = 0;
		$res = $error = $data = array();
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$arr[$i];

			$data['profile_id'] = 0;
			$data['preference_id'] = (int) $ref['id'];
			$data['value'] = $ref['value'];

			if(($res[$i] = $this->_cti_profile_preference->chk_values($data)) === false)
			{
				$errnb++;
				$res[$i] = $this->_cti_profile_preference->get_filter_result();
				$error[$i] = $this->_cti_profile_preference->get_filter_error();
			}
		}

		$this->_set_result('preference',$res);

		if ($errnb >> 0)
		{
			$this->_set_errors('preference',$error);
			return(false);
		}

		return(true);
	}

	function set_xlet($arr)
	{
		if(is_array($arr) === false
		|| $this->_cti_profile_xlet === false)
			return(false);

		$arr = $this->_prepare_list_data_from_form($arr);

		if(empty($arr))
			return(true);

		$nb = count($arr);
		$errnb = 0;
		$res = $error = array();
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$arr[$i];

			$data = $ref;
			$data['profile_id'] = 0;
			$data['xlet_id'] = (int) $ref['id'];
			$data['layout_id'] = (int) $ref['layout'];

			if(($res[$i] = $this->_cti_profile_xlet->chk_values($data)) === false)
			{
				$errnb++;
				$res[$i] = $this->_cti_profile_xlet->get_filter_result();
				$error[$i] = $this->_cti_profile_xlet->get_filter_error();
			}
		}

		$this->_set_result('xlet',$res);

		if ($errnb >> 0)
		{
			$this->_set_errors('xlet',$error);
			return(false);
		}

		return(true);
	}

	function add()
	{
		$r = true;

		$this->_status = 'add';
		$this->_return = array();

		if($this->get_errnb() > 0
		|| ($profile_id = $this->_add_cti_profile()) === false
		|| $this->_add_service($profile_id) === false
		|| $this->_add_preference($profile_id) === false
		|| $this->_add_xlet($profile_id) === false)
		{
			$r = false;
			if($profile_id !== null)
				$this->_reverse_add();
		}

		$this->_status = '';

		return($r);
	}

	private function _add_cti_profile()
	{
		if(($rs = $this->get_result('cti_profile')) === null
		|| $rs === false)
			return(false);

		$rs['deletable'] = 'true';
		$this->_return['cti_profile'] = $this->_cti_profile->add($rs);
		return($this->_return['cti_profile']);
	}

	private function _add_service($profile_id)
	{
		if(($rs = $this->get_result('service')) === null)
			return($rs);

		if($rs === false)
			return(false);

		$this->_return['service'] = array();

		$nb = count($rs);
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];
			$ref['profile_id'] = (int) $profile_id;
			if (($this->_return['service'][$ref['service_id']] = $this->_cti_profile_service->add($ref)) === false)
			{
				dwho_report::push('error','error during create service');
				return(false);
			}
		}

		return($this->_return['service']);
	}

	private function _add_preference($profile_id)
	{
		if(($rs = $this->get_result('preference')) === null)
			return($rs);

		if($rs === false)
			return(false);

		$this->_return['preference'] = array();

		$nb = count($rs);
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];
			$ref['profile_id'] = (int) $profile_id;
			if (($this->_return['preference'][$ref['preference_id']] = $this->_cti_profile_preference->add($ref)) === false)
			{
				dwho_report::push('error','error during create preference');
				return(false);
			}
		}

		return($this->_return['preference']);
	}

	private function _add_xlet($profile_id)
	{
		if(($rs = $this->get_result('xlet')) === null)
			return($rs);

		if($rs === false)
			return(false);

		$this->_return['xlet'] = array();

		$nb = count($rs);
		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];
			$ref['profile_id'] = (int) $profile_id;
			if (($this->_return['xlet'][$ref['xlet_id']] = $this->_cti_profile_xlet->add($ref)) === false)
			{
				dwho_report::push('error','error during create xlet');
				return(false);
			}
		}

		return($this->_return['xlet']);
	}

	function _reverse_add()
	{
		if($this->get_errnb() > 0
		|| $this->_status !== 'add'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'cti_profile':
					$this->_cti_profile->delete($val);
					break;
				case 'service':
					if(is_array($val) === false)
						continue;

					foreach($val as $v)
						$this->_cti_profile_service->delete($v);
					break;
				case 'preference':
					if(is_array($val) === false)
						continue;

					foreach($val as $v)
						$this->_cti_profile_preference->delete($v);
					break;
				case 'xlet':
					if(is_array($val) === false)
						continue;

					foreach($val as $v)
						$this->_cti_profile_xlet->delete($v);
			}
		}

		return(true);
	}

	function edit()
	{
		$r = true;

		$this->_status = 'edit';
		$this->_return = array();

		$n = $this->get_errnb();
		if($this->get_errnb() > 0
		|| $this->_edit_cti_profile() === false
		|| $this->_edit_service($this->_info['cti_profile']['id']) === false
		|| $this->_edit_preference($this->_info['cti_profile']['id']) === false
		|| $this->_edit_xlet($this->_info['cti_profile']['id']) === false)
		{
			$r = false;
			$this->_reverse_edit();
		}

		$this->_status = '';

		return($r);
	}

	private function _edit_cti_profile()
	{
		if(dwho_issa('cti_profile',$this->_info) === false
		|| ($rs = $this->get_result('cti_profile')) === null
		|| $rs === false)
			return(false);

		$this->_return['cti_profile'] = $this->_cti_profile->edit($this->_info['cti_profile']['id'],$rs);
		return($this->_return['cti_profile']);
	}

	private function _edit_service($profile_id)
	{
		if ($this->_status === 'edit'
		&& $this->_info['service'] === false
		&& $this->get_result('service') === null)
			return(true);
		elseif($this->_info['service'] === false)
			return($this->_add_service($profile_id));

		$arr = array();
		$arr['profile_id'] = (int) $profile_id;
		$this->_cti_profile_service->delete_where($arr);

		return($this->_add_service($profile_id));
	}

	private function _edit_preference($profile_id)
	{
		if ($this->_status === 'edit'
		&& $this->_info['preference'] === false
		&& $this->get_result('preference') === null)
			return(true);
		elseif($this->_info['preference'] === false)
			return($this->_add_preference($profile_id));

		$arr = array();
		$arr['profile_id'] = (int) $profile_id;
		$this->_cti_profile_preference->delete_where($arr);

		return($this->_add_preference($profile_id));
	}

	private function _edit_xlet($profile_id)
	{
		if ($this->_status === 'edit'
		&& $this->_info['xlet'] === false
		&& $this->get_result('xlet') === null)
			return(true);
		elseif($this->_info['xlet'] === false)
			return($this->_add_xlet($profile_id));

		$arr = array();
		$arr['profile_id'] = (int) $profile_id;
		$this->_cti_profile_xlet->delete_where($arr);

		return($this->_add_xlet($profile_id));
	}

	function _reverse_edit()
	{
		if($this->get_errnb() > 0
		|| $this->_status !== 'edit'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'presences':
					if(isset($this->_origin['presences']) === false
					|| $this->_origin['presences'] === false)
						$this->delete_presences();
					else
						$this->_ctipresences->edit_origin($this->_origin['presences']);
					break;
			}
		}
		return(true);
	}

	public function users_has_profile($profile_name)
	{
		return($this->_userfeatures->users_has_profile($profile_name));
	}

	function update_users_with_profile($name)
	{
		$r = true;

		$this->_status = 'update-cti_profile';
		$this->_return = array();

		if($this->get_errnb() > 0
		|| $this->update_users_profile($name) === false)
		{
			$r = false;
			$this->_reverse_update($name);
		}

		$this->_status = '';
		return($r);
	}

	function update_users_profile($name)
	{
		if(dwho_issa('cti_profile',$this->_info) === false
		|| ($rs = $this->get_result('cti_profile')) === null
		|| $rs === false)
			return(false);

		$from = array();
		$from['profileclient'] = $name;

		$to = array();
		$to['profileclient'] = $rs['name'];

		$this->_return['cti_profile'] = $this->_userfeatures->edit_where($from, $to);

		return($this->_return['cti_profile']);
	}

	function _reverse_update($name)
	{
		$from = array();
		$from['profileclient'] = $rs['name'];

		$to = array();
		$to['profileclient'] = $name;

		return($this->_userfeatures->edit_where($from, $to));
	}

	function disable_users_with_profile($name)
	{
		if(dwho_issa('cti_profile',$this->_info) === false)
			return false;

		$from = array();
		$from['profileclient'] = $this->_info['cti_profile']['name'];

		$to = array();
		$to['profileclient'] = '';
		$to['enableclient'] = 0;

		$this->_return['cti_profile'] = $this->_userfeatures->edit_where($from, $to);

		return($this->_return['cti_profile']);

	}

	function delete()
	{
		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if($this->get_errnb() > 0
				|| $this->delete_cti_profile() === false)
		{
			$r = false;
			$this->_reverse_delete();
		}

		$this->_status = '';

		return($r);
	}

	function delete_cti_profile()
	{
		if(dwho_issa('cti_profile',$this->_info) === false)
			return(false);

		if($this->users_has_profile($this->_info['cti_profile']['name']))
		{
			dwho_report::push('error','Can\'t delete this ctiprofile because it\'s used by a user');
			return(false);
		}

		$r = $this->_cti_profile->delete($this->_info['cti_profile']['id']);

		if($this->_cti_profile === 'delete')
			$this->_return['cti_profile'] = $r;

		return($r);
	}

	function _reverse_delete()
	{
		if($this->_status !== 'delete'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'cti_profile':
					if(isset($this->_origin['cti_profile']) === true
					&& $this->_origin['cti_profile'] !== false)
						$this->_cti_profile->add_origin($this->_origin['cti_profile']);
					break;
			}
		}
		return(true);
	}

	function enable()
	{
		$r = true;

		$this->_status = 'enable';
		$this->_return = array();

		if($this->enable_cti_profile() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return($r);
	}

	function disable()
	{
		$r = true;

		$this->_status = 'disable';
		$this->_return = array();

		if($this->disable_cti_profile() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return(true);
	}

	function enable_cti_profile()
	{
		return($this->_enable_disable_cti_profile(false));
	}

	function disable_cti_profile()
	{
		return($this->_enable_disable_cti_profile(true));
	}

	function _enable_disable_cti_profile($disable=false)
	{
		if(dwho_issa('cti_profile',$this->_info) === false)
			return(false);
		else if((bool) $disable === false)
			$r = $this->_cti_profile->enable($this->_info['cti_profile']['id']);
		else
			$r = $this->_cti_profile->disable($this->_info['cti_profile']['id']);

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['cti_profile'] = $r;

		return($r);
	}

	function _reverse_enable_disable()
	{
		if(($this->_status !== 'enable' && $this->_status !== 'disable') === true
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		$disable = $this->_status === 'enable';

		$this->_status = '';

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'cti_profile':
					$this->_enable_disable_cti_profile($disable);
					break;
			}
		}

		return(true);
	}
}
?>
