<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Avencall
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

class xivo_application_service_asterisk_agentgroup extends xivo_application_asterisk_abstract
{
	var $_agentgroup	= false;
	var $_agentmember	= null;
	var $_contextlist	= array();
	var $_musiconhold	= array();

	function xivo_application_service_asterisk_agentgroup(&$service,$param=null,$components=true)
	{
		$this->xivo_application_asterisk_abstract(&$service);

		if(($this->_agentgroup = &$this->_service->get_module('agentgroup')) === false)
			return(false);
		else if((bool) $components === true)
			$this->load_components();

		return(true);
	}

	function load_components()
	{
		$this->_components = true;

		if(($this->_agentmember = &$this->get_abstract('agentmember')) === false)
			return(false);

		return(true);
	}

	function get($id,$disable=null)
	{
		$this->_info = $this->_origin = array();

		if(($this->_info['agentgroup'] = $this->_agentgroup->get($id)) === false)
			return(false);

		$this->_origin['agentgroup'] = $this->_agentgroup->get_origin();

		if($this->_components === false)
			return($this->_info);

		$this->_get_agentmember();

		return($this->_info);
	}

	function get_nb($arr=null,$disable=null)
	{
		return($this->_agent->get_nb($arr,$disable));
	}

	function _get_agentmember()
	{
		$this->_info['agentmember'] = false;
		$this->_origin['agentmember'] = false;

		if(is_object($this->_agentmember) === true
		&& dwho_issa('agentgroup',$this->_info) === true
		&& ($this->_info['agentmember'] = $this->_agentmember->get($this->_info['agentgroup']['id'])) !== false)
			$this->_origin['agentmember'] = $this->_agentmember->get_origin();

		return($this->_info['agentmember']);
	}

	function get_elements()
	{
		$r = array();

		$r['agentgroup'] = $this->_agentgroup->get_element();

		return($r);
	}

	function get_agentgroups_list($disable=null,$order=null,$limit=null,$assoc=false)
	{
		if(($rs = $this->_agentgroup->get_all(null,false,true,$order,$limit)) === false
		|| ($nb = count($rs)) === 0)
		{
			$this->_cnt = $this->_agentgroup->get_cnt();
			return(false);
		}

		$this->_cnt = $this->_agentgroup->get_cnt();

		$agentfeatures = &$this->_service->get_module('agentfeatures');

		$r = $info = array();

		for($i = 0;$i < $nb;$i++)
		{
			$info['agentgroup'] = $rs[$i];

			if(($info['nb_amember'] = $agentfeatures->get_nb(
							array('numgroup' => $info['agentgroup']['id']),
							$disable)) !== false)
				$r[$info['agentgroup']['id']] = $info;
		}

		if(empty($r) === true)
			return(false);
		else if((bool) $assoc === false)
			return(array_values($r));

		return($r);
	}

	function set_add($arr)
	{
		return($this->_set('add',$arr));
	}

	function set_edit($arr)
	{
		if(empty($this->_info) === true)
			return(false);

		return($this->_set('edit',$arr));
	}

	function _set($action,$arr)
	{
		$this->_reset();
		if(($action !== 'add' && $action !== 'edit') === true
		|| dwho_issa('agentgroup',$arr) === false)
			return(false);

		$arr['agentgroup']['groupid'] = 0;
		$this->set_agentgroup($arr['agentgroup']);

		if(dwho_issa('agent-select',$arr) === true)
			$this->set_agentmember($arr['agent-select']);

		return(($this->get_errnb() < 1));
	}

	function set_agentgroup($arr)
	{
		if(is_array($arr) === false)
		{
			$this->_set_result('agentgroup',null);
			$this->_set_error('agentgroup','invalid data');
			return(false);
		}
		else if(($rs = $this->_agentgroup->chk_values($arr)) === false)
		{
			$this->_set_result('agentgroup',$this->_agentgroup->get_filter_result());
			$this->_set_error('agentgroup',$this->_agentgroup->get_filter_error());
			return(false);
		}

		$this->_set_result('agentgroup',$rs);

		return(true);
	}

	function set_agentmember($list)
	{
		if(is_object($this->_agentmember) === false)
			return(false);
		else if(($r = $this->_agentmember->set($list)) === false)
			$this->_set_error('agentmember',$this->_agentmember->get_error());

		$this->_set_result('agentmember',$this->_agentmember->get_result());

		return($r);
	}

	function add()
	{
		$r = true;

		$this->_status = 'add';
		$this->_return = array();

		if($this->get_errnb() > 0
		|| ($agentgroupid = $this->add_agentgroup()) === false
		|| $this->update_agentgroup($agentgroupid) === false
		|| $this->add_agentmember($agentgroupid) === false)
		{
			$r = false;
			if($agentgroupid !== null)
				$this->_reverse_add();
		}

		$this->_status = '';

		return($r);
	}

	function add_agentgroup()
	{
		if(($rs = $this->get_result('agentgroup')) === null
		|| $rs === false)
			return(false);

		$rs['groupid'] = 0;

		return(($this->_return['agentgroup'] = $this->_agentgroup->add($rs)));
	}

	function update_agentgroup($agentgroupid)
	{

		if(($agentgroupid = dwho_ulongint($agentgroupid)) === 0)
			return(false);

		return($this->_agentgroup->edit($agentgroupid,
						array('groupid'	=> $agentgroupid)));
	}

	function add_agentmember($agentgroupid)
	{
		if(is_object($this->_agentmember) === false)
			return(false);

		$r = $this->_agentmember->save($agentgroupid);

		$this->_return['agentmember'] = $this->_agentmember->get_return();

		return($r);
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
				case 'agentgroup':
					$this->_agentgroup->delete($val);
					break;
				case 'agentmember':
					$this->_agentmember->reverse_save($this->_status);
					break;
			}
		}

		return(true);
	}

	function edit()
	{
		$r = true;

		$this->_status = 'edit';
		$this->_return = array();

		if($this->get_errnb() > 0
		|| $this->edit_agentgroup() === false
		|| $this->edit_agentmember() === false)
		{
			$r = false;
			$this->_reverse_edit();
		}

		$this->_status = '';

		return($r);
	}

	function edit_agentgroup()
	{
		if(dwho_issa('agentgroup',$this->_info) === false
		|| ($rs = $this->get_result('agentgroup')) === null
		|| $rs === false)
			return(false);

		$rs['groupid'] = $this->_info['agentgroup']['groupid'];

		return(($this->_return['agentgroup'] = $this->_agentgroup->edit($this->_info['agentgroup']['id'],$rs)));
	}

	function edit_agentmember()
	{
		if(is_object($this->_agentmember) === false
		|| dwho_issa('agentgroup',$this->_info) === false)
			return(false);

		$r = $this->_agentmember->save($this->_info['agentgroup']['id']);

		$this->_return['agentmember'] = $this->_agentmember->get_return();

		return($r);
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
				case 'agentgroup':
					if(isset($this->_origin['agentgroup']) === false
					|| $this->_origin['agentgroup'] === false)
						$this->delete_agentgroup();
					else
						$this->_agentgroup->edit_origin($this->_origin['agentgroup']);
					break;
				case 'agentmember':
					$this->_agentmember->reverse_save($this->_status);
					break;
			}
		}

		return(true);
	}

	function delete()
	{
		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if($this->get_errnb() > 0
		|| $this->delete_agentgroup() === false
		|| $this->delete_agentmember() === false)
		{
			$r = false;
			$this->_reverse_delete();
		}

		$this->_status = '';

		return($r);
	}

	function delete_agentgroup()
	{
		if(dwho_issa('agentgroup',$this->_info) === false
		|| (string) $this->_info['agentgroup']['id'] === (string) XIVO_SRE_IPBX_AST_AGENT_GROUP_DEFAULT)
			return(false);

		$r = $this->_agentgroup->delete($this->_info['agentgroup']['id']);

		if($this->_status === 'delete')
			$this->_return['agentgroup'] = $r;

		return($r);
	}

	function delete_agentmember()
	{
		if(dwho_issa('agentmember',$this->_info) === false)
			return(null);
		else if(is_object($this->_agentmember) === false)
			return(false);
		else if(($r = $this->_agentmember->delete()) === null)
			return(null);
		else if($this->_status === 'delete')
			$this->_return['agentmember'] = $r;

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
				case 'agentgroup':
					if(dwho_issa('agentgroup',$this->_origin) === true)
						$this->_agentgroup->recover($this->_origin['agentgroup']['id']);
					break;
				case 'agentmember':
					$this->_agentmember->reverse_delete($this->_status);
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

		if($this->enable_agentgroup() === false
		|| $this->enable_agentmember() === false)
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

		if($this->disable_agentgroup() === false
		|| $this->disable_agentmember() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return(true);
	}

	function enable_agentgroup()
	{
		return($this->_enable_disable_agentgroup(false));
	}

	function disable_agentgroup()
	{
		return($this->_enable_disable_agentgroup(true));
	}

	function _enable_disable_agentgroup($disable=false)
	{
		if(dwho_issa('agentgroup',$this->_info) === false)
			return(false);
		else if((bool) $disable === false)
			$r = $this->_agentgroup->enable($this->_info['agentgroup']['id']);
		else
			$r = $this->_agentgroup->disable($this->_info['agentgroup']['id']);

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['agentgroup'] = $r;

		return($r);
	}

	function enable_agentmember()
	{
		return($this->_enable_disable_agentmember(false));
	}

	function disable_agentmember()
	{
		return($this->_enable_disable_agentmember(true));
	}

	function _enable_disable_agentmember($disable=false)
	{
		if(dwho_issa('agentmember',$this->_origin) === false)
			return(false);

		$disable = (bool) $disable;

		$rs = array();

		$r = true;

		foreach($this->_origin['agentmember'] as $val)
		{
			if($disable === false)
			{
				if(($r = $this->_agentmember->enable_agent($val['agentid'])) === false)
					break;
			}
			else if(($r = $this->_agentmember->disable_agent($val['agentid'])) === false)
				break;
		}

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['agentmember'] = $rs;

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
				case 'agentgroup':
					$this->_enable_disable_agentgroup($disable);
					break;
				case 'agentmember':
					$this->_enable_disable_agentmember($disable);
					break;
			}
		}
		return(true);
	}
}

?>
