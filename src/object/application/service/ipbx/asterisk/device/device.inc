<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2014  Avencall
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
dwho::load_class('dwho_http');

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);

class xivo_application_service_asterisk_device extends xivo_application_asterisk_abstract
{
	protected $_linefeatures 	= false;
	protected $_provdplugin 	= false;
	protected $_sccpdevice		= false;
	protected $_device_api		= false;

	public function __construct(&$service,$param=null,$components=true)
	{
		parent::__construct($service);

		if(($this->_linefeatures = &$this->_service->get_module('linefeatures')) === false
		|| ($this->_sccpdevice = &$this->_service->get_module('sccpdevice')) === false
		|| ($_XOBJ = &dwho_gct::get('xivo_object')) === false
		|| ($this->_provdplugin = &$_XOBJ->get_module('provdplugin')) === false
		|| ($this->_provdconfig = &$_XOBJ->get_module('provdconfig')) === false
		|| ($_RAPI = &dwho_gct::get('xivo_restapi')) === false
		|| ($this->_device_api = $_RAPI->get_ressource('device')) === false)
			return(false);

		return(true);
	}

	public function get($id,$nocomponents=null)
	{
		$this->_info = $this->_origin = array();

		if(($this->_info['device'] = $this->_device_api->get($id)) === false)
			return(false);

		if($nocomponents === true)
			return($this->_info);

		$this->get_capabilities();
		$this->get_sep();

		return($this->_info);
	}

	public function get_nb($arr=null)
	{
		return($this->_dso->get_nb($arr));
	}

	public function is_sccp()
	{
		if ($this->_info['device'] === false
		|| isset($this->_info['device']['plugin']) === false)
			return(false);

		return((bool) preg_match('/'.XIVO_SRE_IPBX_AST_PROTO_SCCP.'/',$this->_info['device']['plugin']));
	}

	public function get_capabilities()
	{
		$this->_info['capabilities'] = false;

		if (isset($this->_info['device']['plugin']) === false
		|| isset($this->_info['device']['model']) === false
		|| ($plugin = $this->_info['device']['plugin']) === null
		|| ($rs = $this->_provdplugin->get($plugin)) === false
		|| ($info = $rs['info']) === null
		|| isset($info['capabilities']) === false
		|| ($nb = count($info['capabilities'])) === 0)
			return(false);

		$r = array();
		foreach ($info['capabilities'] as $k => $v)
		{
			if(preg_match('/'.$this->_info['device']['model'].'/', $k) === 0)
				continue;
			$r = $v;
			break;
		}

		$this->_info['capabilities'] = $r;
	}

	public function get_capabilities_by_plugin($pluginid)
	{
		if (isset($this->_info['device']) === false
		|| $this->_info['device'] === false
		|| ($rs = $this->_provdplugin->get($pluginid)) === false
		|| ($info = $rs['info']) === null
		|| isset($info['capabilities']) === false
		|| ($nb = count($info['capabilities'])) === 0)
			return(false);

		foreach ($info['capabilities'] as $k => $v)
		{
			if(isset($this->_info['device']['model']) === false
			|| preg_match('/'.$this->_info['device']['model'].'/', $k) === 0)
				continue;
			return($v);
		}
		return(false);
	}

	public function get_sep()
	{
		$this->_info['sep'] = '';
		if (isset($this->_info['device']['mac']) === false
		|| $this->is_sccp() === false)
			return(false);
		$this->_info['sep'] = 'SEP'.strtoupper(str_replace(':','', $this->_info['device']['mac']));
	}

	public function mode_autoprov($synchronize=false)
	{
		if(dwho_issa('device',$this->_info) === false)
			return(false);

		if ($this->_device_api->reset_to_autoprov($this->_info['device']['id']) === false)
			return(false);
		elseif($this->is_sccp() === true)
			$this->_sccpdevice->init_device($this->_info['device']['mac'],'');

		if ($synchronize === true)
			$this->synchronize();

		return(true);
	}

	public function synchronize()
	{
		if(dwho_issa('device',$this->_info) === false)
			return(false);

		return($this->_device_api->synchronize($this->_info['device']['id']));
	}

	public function remove_line($line_id, $synchronize=false)
	{
		if(dwho_issa('device',$this->_info) === false)
			return(false);

		$rs = $this->_device_api->remove_line($this->_info['device']['id'], $line_id);

		if ($synchronize === true)
			$this->synchronize();

		return($rs);
	}

	public function associate_line($line_id, $synchronize=false)
	{
		if(dwho_issa('device',$this->_info) === false)
			return(false);
		elseif (($conf = $this->_provdconfig->build_device_config($this->_info['device']['id'], $line_id)) === false)
		{
			dwho_report::push('error', 'fail to build config device: '.$this->_info['device']['id']);
			return(false);
		}
		elseif ($this->_provdconfig->edit($this->_info['device']['id'], $conf) === false)
		{
			dwho_report::push('error', 'fail to edit config for this device: '.$this->_info['device']['id']);
			return(false);
		}
		elseif (($rs = $this->_device_api->associate_line($this->_info['device']['id'], $line_id)) === false)
		{
			dwho_report::push('error', 'fail to associate line to device: '.$this->_info['device']['id']);
			return(false);
		}
		elseif ($synchronize === true)
			$this->synchronize();

		return($rs);
	}

	public function get_elements()
	{
		$r = array();

		return($r);
	}

	public function set_add($arr)
	{
		return($this->_set('add',$arr));
	}

	public function set_edit($arr)
	{
		if(empty($this->_info) === true)
			return(false);

		return($this->_set('edit',$arr));
	}

	private function _set($action,$arr)
	{
		$this->_reset();

		if(($action !== 'add' && $action !== 'edit') === true
		|| dwho_issa('device',$arr) === false)
			return(false);

		$this->set_device($arr['device']);

		if($this->get_errnb() > 0)
			$r = false;
		else
			$r = true;

		return($r);
	}

	function set_device($arr)
	{
		if(is_array($arr) === false)
		{
			$this->_set_result('device',null);
			$this->_set_error('device','invalid data');
			return(false);
		}

		if (!empty($arr['mac']))
			$arr['mac'] = strtolower($arr['mac']);
		else
			unset($arr['mac']);

		if (empty($arr['ip']))
			unset($arr['ip']);

		if (empty($arr['plugin']))
			unset($arr['plugin']);

		if (!isset($arr['options']))
			$arr['options'] = array();

		if (isset($arr['options']['switchboard']))
			$arr['options']['switchboard'] = dwho_bool($arr['options']['switchboard']);
		else
			$arr['options']['switchboard'] = false;

		if (!empty($this->_info['device']['options']))
		{
			$arr['options'] = array_merge($this->_info['device']['options'], $arr['options']);
		}

		$this->_set_result('device',$arr);

		return(true);
	}

	function add()
	{
		$r = true;

		$this->_status = 'add';
		$this->_return = array();
		$id = null;

		if($this->get_errnb() > 0
		|| ($id = $this->_add_device()) === false)
			$r = false;

		$this->_status = '';

		return($r);
	}

	private function _add_device()
	{
		if(($rs = $this->get_result('device')) === null
		|| $rs === false)
			return(false);

		return($this->_device_api->add($rs));
	}

	function edit()
	{
		$this->_status = 'edit';
		$this->_return = array();

		if(is_array($this->_info) === false
		|| empty($this->_info) === true
		|| $this->get_errnb() > 0
		|| ($this->_edit_device()) === false)
		{
			$this->_status = '';
			return(false);
		}

		$this->_status = '';
		return(true);
	}

	private function _edit_device()
	{
		if(dwho_issa('device',$this->_info) === false
		|| ($rs = $this->get_result('device')) === null)
			return(false);

		$this->_device_api->edit($this->_info['device']['id'],$rs);

		return($this->_return['device']);
	}

	function delete()
	{
		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if($this->_linefeatures->reset_device($this->_info['device']['id']) === false
		||$this->delete_device() === false)
			$r = false;

		$this->_status = '';

		return($r);
	}

	function delete_device()
	{
		if(dwho_issa('device',$this->_info) === false)
			return(false);

		$r = $this->_device_api->delete($this->_info['device']['id']);
		$this->_sccpdevice->delete_by_mac($this->_info['device']['mac']);

		if($this->_status === 'delete')
			$this->_return['device'] = $r;

		return($r);
	}
}

?>
