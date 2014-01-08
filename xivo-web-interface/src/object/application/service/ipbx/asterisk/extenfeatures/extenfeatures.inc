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

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);

class xivo_application_service_asterisk_extenfeatures extends xivo_application_asterisk_abstract
{
	var $_extensions	= false;
	var $_staticsip		= null;
	var $_conf			= array();
	var $_filter		= false;
	var $_context		= XIVO_SRE_IPBX_AST_CONTEXT_FEATURES;

	public function __construct(&$service)
	{
		parent::__construct($service);

		if(($this->_extensions = &$this->_service->get_module('extensions')) === false)
			return(false);

		$this->_load_config(dirname(__FILE__));

		return(true);
	}

	function get($name)
	{
		$this->_info = $this->_origin = array();

		$this->_get_extenfeatures($name);

		return($this->_info);
	}

	function get_all_by_context($disable=null)
	{
		$arr = array();
		$arr['context'] = $this->_context;

		return($this->_extensions->get_all_name_by_context($arr,$disable,true));
	}

	function _get_extenfeatures($name,$disable=null)
	{
		$this->_info['extenfeatures'] = false;
		$this->_origin['extenfeatures'] = false;

		$arr = array();
		$arr['typeval'] = $name;
		$arr['context'] = $this->_context;

		if(($this->_info['extenfeatures'] = $this->_extensions->get_where($arr,$disable)) !== false)
			$this->_origin['extenfeatures'] = $this->_extensions->get_origin_list();

		return($this->_info['extenfeatures']);
	}

	function get_elements()
	{
		return($this->get_element());
	}

	function _chk_extenfeatures_exists($name,$cmd='')
	{
		$name = (string) $name;

		if($cmd !== '' && isset($this->_conf['element'][$name]) === false)
			return(false);

		return(true);
	}

	function set($arr)
	{
		$this->_reset();

		if(is_array($arr) === false || isset($arr['typeval']) === false)
			return(false);

		$this->get($arr['typeval']);

		if($this->get($arr['typeval']) !== false
		&& array_key_exists('commented',$arr) === false
		&& dwho_ak('commented',$this->_info['extenfeatures']) === true)
			$arr['commented'] = $this->_info['extenfeatures']['commented'];

		$this->set_extenfeatures($arr);

		$number = (string) $this->get_result_var('extenfeatures','exten');
		$context = '';

		return(($this->get_errnb() < 1));
	}

	function set_extenfeatures($arr)
	{
		if(is_array($arr) === false
		|| isset($arr['typeval'],$arr['exten']) === false
		|| $this->_chk_extenfeatures_exists(strval($arr['typeval'])) === false)
			return(false);

		$rs = array();
		$rs['type'] = 'extenfeatures';
		$rs['typeval'] = $arr['typeval'];
		$rs['commented'] = null;

		if(isset($arr['commented']) === true)
			$rs['commented'] = (bool) $arr['commented'];

		if(($exten = $this->extenfeatures_chk_value($arr['typeval'],$arr['exten'])) === false)
		{
			if(($exten = $this->get_filter_result()) !== false)
				$rs['exten'] = $exten[$arr['typeval']];

			$this->_set_result('extenfeatures',$rs);
			$this->_set_error('extenfeatures',$this->get_filter_error());
			return(false);
		}

		$rs['exten'] = $exten;

		$this->_set_result('extenfeatures',$rs);

		return(true);
	}

	function save()
	{
		$r = true;

		$this->_status = 'save';
		$this->_return = array();

		if(is_array($this->_info) === false
		|| empty($this->_info) === true
		|| $this->get_errnb() > 0
		|| ($name = $this->save_extenfeatures()) === false
		|| ($name === 'vmusermsg' && $this->save_sip_vmexten() === false) === true)
		{
			$r = false;
			$this->_reverse_save();
		}

		$this->_status = '';

		return($r);
	}

	function save_extenfeatures()
	{
		if(isset($this->_info['extenfeatures']) === false)
			return(false);
		else if($this->_info['extenfeatures'] === false)
			return($this->_add_extenfeatures());
		else if(($rs = $this->get_result('extenfeatures')) === null)
			return(false);

		$this->_return['extenfeatures'] = $rs['typeval'];

		if($this->_extensions->edit($this->_info['extenfeatures']['id'], $rs) === false)
			return(false);

		return($rs['typeval']);
	}

	function save_sip_vmexten()
	{
		if(is_object($this->_staticsip) === false
		&& ($this->_staticsip = &$this->_service->get_realstatic('staticsip')) === false)
		{
			$this->_set_error('extenfeatures','invalid staticsip module');
			return(false);
		}
		else if(($rs = $this->get_result('extenfeatures')) === false
		|| is_array($rs) === false
		|| isset($rs['typeval'],$rs['exten']) === false
		|| $rs['typeval'] !== 'vmusermsg')
		{
			$this->_set_error('extenfeatures','invalid data');
			return(false);
		}

		$where = array();
		$where['category'] = 'general';
		$where['var_name'] = 'vmexten';

		$value = array();
		$value['var_val'] = $this->_service->clean_extension($rs['exten']);

		return($this->_staticsip->edit_where($where,$value));
	}

	function _add_extenfeatures()
	{
		if(($rs = $this->get_result('extenfeatures')) === null
		|| $rs === false)
			return(false);
		else if($this->_extensions->add($rs) === false)
			$this->_return['extenfeatures'] = false;
		else
			$this->_return['extenfeatures'] = $rs['typeval'];

		return($this->_return['extenfeatures']);
	}

	function _reverse_save()
	{
		if($this->get_errnb() > 0
		|| $this->_status !== 'save'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'extenfeatures':
					$this->delete_extenfeatures($val);

					if(isset($this->_origin['extenfeatures']) === true
					&& $this->_origin['extenfeatures'] !== false)
						$this->_extensions->add_origin_list($this->_origin['extenfeatures']);
					break;
			}
		}
	}

	function extenfeatures_chk_value($key,$value)
	{
		return(parent::chk_value($key,$this->_service->clean_extension($value)));
	}

	function extenfeatures_set_chk_value($key,$value)
	{
		return(parent::set_chk_value($key,$this->_service->clean_extension($value)));
	}

	function delete_extenfeatures($name='')
	{
		if(dwho_issa('extenfeatures',$this->_info) === true)
			$name = $this->_info['extenfeatures']['typeval'];
		else if(is_scalar($name) === false)
			return(false);

		$where = array();
		$where['typeval'] = $name;

		$r = $this->_extensions->delete_where($where);

		if($this->_status === 'delete')
			$this->_return['extenfeatures'] = $r;

		return($r);
	}

	function edit_from_json()
	{
		if(($data = $this->_get_data_from_json()) === false
		|| $this->set($arr) === false
		|| $this->save() === false) {
			return(false);
		}

		return(true);
	}
}

?>
