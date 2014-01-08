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

class xivo_application_service_asterisk_dialpattern_abstract extends xivo_application_asterisk_abstract
{
	var $_dialpattern	= false;
	var $_extensions	= false;
	var $_param			= array('type'	=> '');

	public function __construct(&$service,$param=array())
	{
		parent::__construct($service);

		if(($this->_dialpattern = &$this->_service->get_module('dialpattern')) === false
		|| ($this->_extensions = &$this->_service->get_module('extensions')) === false)
			return(false);

		return($this->load_init($param));
	}

	function load_init($param=array())
	{
		$this->_reset_info();
		$this->_reset_params();
		$this->_reset();

		if($this->set_params($param) === false
		|| isset($param['type']) === false)
		#|| $this->_dialpattern->chk_element_value('type',$param['type']) === false)
		{
			$this->_param['type'] = '';
			return(false);
		}

		$this->_param['type'] = $param['type'];

		return(true);
	}

	function get($id,$context='')
	{
		$this->_reset();
		$this->_info = false;
		$this->_origin = false;

		if(dwho_has_len($id) === false)
			return(false);

		$where = array();
		$where['id'] = $id;
		$where['type'] = $this->_param['type'];

		if(($this->_info['dialpattern'] = $this->_dialpattern->get($id)) === false
		|| $this->_get_exten($context) === false)
			return(false);

		$this->_origin['dialpattern'] = $this->_dialpattern->get_origin();

		return($this->_info);
	}

	function _get_exten($context)
	{
		$this->_info['exten'] = false;
		$this->_origin['exten'] = false;

		if(($this->_info['exten'] = $this->_extensions->get_exten($this->_info['dialpattern']['exten'],
										 $context)) !== false)
			$this->_origin['exten'] = $this->_extensions->get_origin();

		return($this->_info['exten']);
	}

	function get_list($outcallid,$assoc=false)
	{
		$this->_cnt = 0;

		if(dwho_has_len($outcallid) === false
		|| ($outcallid = (int) $outcallid) === 0)
			return(false);

		$where = array();
		$where['type'] = $this->_param['type'];
		$where['typeid'] = $outcallid;

		$rs = $this->_dialpattern->get_all_where($where);

		$this->_cnt = $this->_dialpattern->get_cnt();

		if($rs === false || ($nb = count($rs)) === 0)
			return(false);

		$r = array();

		for($i = 0;$i < $nb;$i++)
		{
			$r[$rs[$i]['id']] = $rs[$i];
			$ref = &$r[$rs[$i]['id']];
		}

		if(empty($r) === true)
			return(false);
		else if((bool) $assoc === false)
			return(array_values($r));

		return($r);
	}

	function get_element()
	{
		return($this->_dialpattern->get_element());
	}

	function set($action,$arr,$context='',$outcallerid='')
	{
		$this->_reset();

		$arr['type'] = $this->_param['type'];

		$number = (string) $arr['exten'];
		$context = (string) $context;

		if(($rsexten = $this->chk_exten('exten',
												$number,
												$context,
												'outcall',
												0)) !== false)
			$this->_set_result('exten',$rsexten);

		if(($rs = $this->_dialpattern->chk_values($arr)) === false)
		{
			$this->_set_result('dialpattern',$this->_dialpattern->get_filter_result());
			$this->_set_error('dialpattern',$this->_dialpattern->get_filter_error());
			return(false);
		}

		$this->_set_result('dialpattern',$rs);

		return(($this->get_errnb() < 1));
	}

	function _set_exten($action,$outcallid)
	{
		if(($outcallid = dwho_ulongint($outcallid)) === 0
		|| ($rs = $this->get_result('exten')) === null
		|| $rs === false
		|| ($rsexten = $this->set_exten($action,
										'exten',
										$rs['exten'],
										$rs['context'],
										'outcall',
										$outcallid)) === false)
			return(false);

		$this->_set_result('exten',$rsexten);

		return(true);
	}

	function add()
	{
		if(($rs = $this->get_result('dialpattern')) === null || $rs === false)
			return($rs);

		if(($dialpatternid = $this->_dialpattern->add($rs)) === false
		|| $this->add_exten($dialpatternid) === false)
			return(false);

		return($dialpatternid);
	}

	function add_exten($outcallid)
	{
		if($this->_set_exten('add',$outcallid) !== true)
			return(false);

		return(parent::add_exten('exten'));
	}

	function reverse_add($status)
	{
		if($this->get_errnb() > 0
		|| $status !== 'add'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'dialpattern':
					$this->_dialpattern->delete($val);
					break;
				case 'exten':
					$this->_extensions->delete_exten($val);
					break;
			}
		}

		return(true);
	}

	function edit()
	{
		if(empty($this->_info) === true)
			return(false);

		$this->_return = false;
		$dialpatternid = $this->_info['dialpattern']['id'];

		if(($rs = $this->get_result('dialpattern')) === null || $rs === false)
			return($rs);

		if($this->_dialpattern->edit($dialpatternid,$rs) === false
		|| $this->edit_exten($dialpatternid) === false)
			$this->_return = $dialpatternid;

		return($this->_return);
	}

	function edit_exten($dialpatternid)
	{
		if(dwho_issa('exten',$this->_info) === false
		|| $this->_set_exten('edit',$dialpatternid) !== true)
			return(false);

		return(parent::edit_exten('exten'));
	}

	function reverse_edit($status)
	{
		if($this->get_errnb() > 0
		|| $status !== 'edit'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'dialpattern':
					$this->_dialpattern->edit_origin($this->_origin['dialpattern']);
					break;
				case 'exten':
					if(isset($this->_origin['exten']) === false
					|| $this->_origin['exten'] === false)
						$this->delete_exten('exten');
					else
						$this->_extensions->edit_origin($this->_origin['exten']);
					break;
			}
		}

		return(true);
	}

	function delete()
	{
		if(empty($this->_info) === true)
			return(null);

		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if ($this->_dialpattern->delete($this->_info['dialpattern']['id']) === false
		|| $this->delete_exten('exten') === false)
		{
			$r = false;
			$this->_reverse_delete();
		}

		$this->_status = '';

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
				case 'dialpattern':
					if(isset($this->_origin['dialpattern']) === true
					&& $this->_origin['dialpattern'] !== false)
						$this->_dialpattern->add_origin($this->_origin['dialpattern']);
					break;
				case 'exten':
					if(isset($this->_origin['exten']) === true
					&& $this->_origin['exten'] !== false)
						$this->_extensions->add_origin($this->_origin['exten']);
					break;
			}
		}

		return(true);
	}
}

?>
