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

xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_extensions extends xivo_service_asterisk_abstract
{
	var $_dso			= null;
	var $_name			= 'extensions';
	var $_filter		= false;
	var $_origin		= false;
	var $_origin_list	= false;
	var $_exten			= array();
	var $_context		= array();

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_USER_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_load_config();
	}

	function _load_config()
	{
		if(empty($this->_conf) === false)
			return(null);

		$dir = dirname(__FILE__).DWHO_SEP_DIR.'config';

		if(($this->_conf = dwho_gat::get_file($dir)) === false)
			$this->_conf = array();
	}

	function _prepare($data)
	{
		$data['id'] = (int) $data['id'];
		$data['typeval'] = (string) $data['typeval'];
		$data['commented'] = (bool) $data['commented'];
		$data['extenbrut'] = $data['exten'];
		$data['exten'] = $this->_sre->display_extension($data['exten']);

		return($data);
	}

	function get_element_where($where)
	{
		if(is_array($where) === false)
			return(false);
		else if(isset($where['exten']) === true)
			$where['exten'] = $this->_sre->clean_extension($where['exten']);

		return(parent::get_element_where($where));
	}

	function add($arr,$id=0)
	{
		if(is_array($arr) === false)
			return(false);
		else if(dwho_has_len($arr,'exten') === false
		|| ($exten = $this->_sre->chk_extension($arr['exten'])) === false)
			return(false);

		$r = array();

		$autoid = true;
		if(($id = dwho_ulongint($id)) !== 0)
		{
			$r['id'] = $id;
			$autoid = false;
		}

		$r['context'] = $arr['context'];
		$r['exten'] = $this->_sre->clean_extension($arr['exten']);
		$r['typeval'] = (string) $arr['typeval'];

		if(isset($arr['commented']) === false)
			$arr['commented'] = false;

		if(isset($arr['type']) === true)
			$r['type'] = (string) $arr['type'];

		$r['commented'] = intval((bool) $arr['commented']);

		return($this->_dso->add($r, $autoid));
	}

	function edit($id,$arr,$disable=null)
	{
		if(($id = dwho_ulongint($id)) === 0 || is_array($arr) === false)
			return(false);

		$r = array();

		if(dwho_has_len($arr,'exten') === true)
		{
			if(($exten = $this->_sre->chk_extension($arr['exten'])) === false)
				return(false);

			$r['exten'] = $this->_sre->clean_extension($arr['exten']);
		}

		$r['typeval'] = (string) $arr['typeval'];

		if(isset($arr['context']) === true)
			$r['context'] = (string) $arr['context'];

		if(isset($arr['commented']) === true)
			$r['commented'] = intval((bool) $arr['commented']);

		if(isset($arr['type']) === true)
			$r['type'] = (string) $arr['type'];

		if(isset($arr['typeval']) === true)
			$r['typeval'] = (string) $arr['typeval'];

		return($this->_dso->edit($id,$r,$disable));
	}

	function edit_where($where,$arr,$disable=null,$id=null,$chkunique=true)
	{
		if(is_array($arr) === false)
			return(false);
		else if(isset($arr['exten']) === true)
			$arr['exten'] = $this->_sre->clean_extension($arr['exten']);

		return(parent::edit_where($where,$arr,$disable,$id,$chkunique));
	}

	function new_exten($exten,$context,$type,$typeval,$arr=null)
	{
		return($this->set_exten($type,$typeval,0,$arr,$exten,$context));
	}

	function set_exten($type,$typeval,$id=0,$arr=null,$exten='',$context='')
	{
		if(($id = dwho_uint($id)) === 0)
		{
			if(dwho_has_len($exten) === false
			|| dwho_has_len($context) === false)
				return(false);

			$id = count($this->_exten) + 1;
			$this->_exten[$id] = array();
		}
		else if(dwho_issa($id,$this->_exten) === false
		|| dwho_issa(0,$this->_exten[$id]) === false)
			return(false);
		else
		{
			$exten = $this->_exten[$id][0]['exten'];
			$context = $this->_exten[$id][0]['context'];
		}

		if(($r = $this->chk_exten($exten,$context,$type,$typeval)) === false)
			return(false);

		$this->_exten[$id][] = $r;

		return($id);
	}

	function chk_exten($exten,$context,$type,$typeval='')
	{
		return($this->_chk_element_value($exten,$context,$type,$typeval));
	}

	function _chk_element_value($exten,$context,$type,$typeval='')
	{
		$r = array();

		$r['exten'] = $this->_sre->clean_extension($exten);
		$r['context'] = $context;
		$r['type'] = (string) $type;
		$r['typeval'] = (string) $typeval;

		return($this->chk_values($r,true,false,false));
	}

	function get_loaded_exten($id)
	{
		if(is_array($this->_exten) === false
		|| ($id = dwho_uint($id)) === 0
		|| dwho_issa($id,$this->_exten) === false
		|| dwho_issa(0,$this->_exten[$id]) === false)
			return(false);

		return($this->_exten[$id]);
	}

	function get_exten($exten,$context)
	{
		$where = array();
		$where['exten'] = $this->_sre->clean_extension($exten);
		$where['context'] = $context;

		return($this->get_where($where));
	}

	function add_exten($id)
	{
		if(is_array($this->_exten) === false
		|| ($id = dwho_uint($id)) === 0
		|| dwho_issa($id,$this->_exten) === false
		|| dwho_issa(0,$this->_exten[$id]) === false)
			return(false);

		$nb = count($this->_exten[$id]);

		$r = array();

		for($i = 0;$i < $nb;$i++)
		{
			if(($ret = $this->add($this->_exten[$id][$i])) !== false)
			{
				$this->_exten[$id][$i]['id'] = (int) $ret;
				$r[] = $ret;
			}
		}
		if(isset($r[0]) === false)
			return(false);
		else if($ret === false)
		{
			for($j = 0;$j < $i;$j++)
				$this->delete($r[$j]);
		}

		return($id);
	}

	function delete_exten($id)
	{
		if(is_array($this->_exten) === false
		|| ($id = dwho_uint($id)) === 0
		|| dwho_issa($id,$this->_exten) === false
		|| dwho_issa(0,$this->_exten[$id]) === false)
			return(false);

		$where = array();
		$where['exten'] = $this->_exten[$id][0]['exten'];
		$where['context'] = $this->_exten[$id][0]['context'];

		return($this->delete_where($where));
	}

	function new_context($context,$exten,$arr=null)
	{
		return($this->set_context($exten,0,$arr,$context));
	}

	function set_context($exten,$id=0,$arr=null,$context='')
	{
		if(($id = dwho_uint($id)) === 0)
		{
			if(dwho_has_len($exten) === false
			|| dwho_has_len($context) === false)
				return(false);

			$id = count($this->_context) + 1;
			$this->_context[$id] = array();
		}
		else if(dwho_issa($id,$this->_context) === false
		|| dwho_issa(0,$this->_context[$id]) === false)
			return(false);
		else
			$context = $this->_context[$id][0]['context'];

		if(($r = $this->chk_context($context,$exten,$arr)) === false)
			return(false);

		$this->_context[$id][] = $r;

		return($id);
	}

	function chk_context($context,$exten,$arr=null)
	{
		return($this->_chk_element_value($exten,$context,$arr));
	}

	function get_loaded_context($id)
	{
		if(is_array($this->_context) === false
		|| ($id = dwho_uint($id)) === 0
		|| dwho_issa($id,$this->_context) === false
		|| dwho_issa(0,$this->_context[$id]) === false)
			return(false);

		return($this->_context[$id]);
	}

	function get_context($context,$exten='',$arr=null,$order=null)
	{
		if(is_array($arr) === false)
			$arr = array();

		$where = array();
		$where['context'] = $context;

		if(dwho_has_len($exten) === true)
			$where['exten'] = $this->_sre->clean_extension($exten);

		if(isset($arr['typeval']) === true)
			$where['typeval'] = $arr['typeval'];

		return($this->get_all_where($where,null,true,$order));
	}

	function add_context($id)
	{
		if(is_array($this->_context) === false
		|| ($id = dwho_uint($id)) === 0
		|| dwho_issa($id,$this->_context) === false
		|| dwho_issa(0,$this->_context[$id]) === false)
			return(false);

		$nb = count($this->_context[$id]);

		$r = array();

		for($i = 0;$i < $nb;$i++)
		{
			if(($ret = $this->add($this->_context[$id][$i])) !== false)
				$r[] = $ret;
		}

		if(isset($r[0]) === false)
			return(false);
		else if($ret === false)
		{
			for($j = 0;$j < $i;$j++)
				$this->delete($r[$j]);
		}

		return($id);
	}

	function delete_context($id)
	{
		if(is_array($this->_context) === false
		|| ($id = dwho_uint($id)) === 0
		|| dwho_issa($id,$this->_context) === false
		|| dwho_issa(0,$this->_context[$id]) === false)
			return(false);

		$where = array();
		$where['context'] = $this->_context[$id][0]['context'];

		return($this->delete_where($where));
	}

	function get_all_name_by_context($arr,$disable=null,$display=false)
	{
		$display = (bool) $display;

		if(is_array($arr) === false
		|| isset($arr['context']) === false
		|| ($list = $this->get_all_where($arr,$disable)) === false)
			return(false);

		$r = array();

		$nb = count($list);

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$list[$i];

			if(isset($r[$ref['typeval']]) === false)
			{
				$r[$ref['typeval']] = array();
				$r[$ref['typeval']]['name'] = $ref['typeval'];

				if($display === true)
					$r[$ref['typeval']]['exten'] = $this->_sre->display_extension($ref['exten']);
				else
					$r[$ref['typeval']]['exten'] = $ref['exten'];

				$r[$ref['typeval']]['context'] = $ref['context'];
				$r[$ref['typeval']]['commented'] = $ref['commented'];
				$r[$ref['typeval']]['line'] = array();
			}

			$r[$ref['typeval']]['line'][1] = $ref;
		}

		if(empty($r) === true)
			$r = false;

		return($r);
	}

	function search($exten,$context='',$type='',$typeval='')
	{
		if(($extenchk = $this->_sre->chk_extension($exten)) === false)
			return(false);

		$type = (string) $type;
		$typeval = (string) $typeval;

		if(($list = $this->_dso->search($exten,$context,$type,$typeval)) === false)
			return(false);

		$nb = count($list);

		$r = array();

		for($i = 0;$i < $nb;$i++)
			$r[] = $this->_prepare($list[$i]);

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}
}

?>
