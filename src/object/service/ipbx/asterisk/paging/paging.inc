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

xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_paging extends xivo_service_asterisk_abstract
{
	var $_dso		= null;
	var $_name		= 'paging';
	var $_filter	= false;
	var $_origin	= false;
	var $_identity	= false;
	protected $_func_key_manager = false;

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_USER_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_load_config();

		if(($_MGR = dwho_gct::get('xivo_manager')) === false
		|| ($this->_func_key_manager = &$_MGR->get_ressource('func_key')) === false)
			return false;
	}

	function _prepare($data)
	{
		$data['id'] = (int) $data['id'];
		$data['timeout'] = (int) $data['timeout'];
		$data['duplex'] = (bool) $data['duplex'];
		$data['ignore'] = (bool) $data['ignore'];
		$data['record'] = (bool) $data['record'];
		$data['quiet'] = (bool) $data['quiet'];
		$data['announcement_play'] = (bool) $data['announcement_play'];
		$data['announcement_caller'] = (bool) $data['announcement_caller'];
		$data['commented'] = (bool) $data['commented'];

		return($data);
	}

	function add($arr,$id=0)
	{
		if(is_array($arr) === false)
			return(false);

		if(($paging_id = parent::add($arr,$id)) === false
		|| $this->_add_func_key_dest($paging_id) === false)
		{
			return(false);
		}

		return($paging_id);
	}

	function edit($id,$arr,$disable=null)
	{
		if(is_array($arr) === false)
			return(false);

		return(parent::edit($id,$arr,$disable));
	}

	private function _add_func_key_dest($paging_id)
	{
		if (! $this->_func_key_manager->add_dst_for_paging($paging_id))
			return(false);

		return(true);
	}

	function delete($id,$disable=null)
	{
		if($id === false
		|| $this->_delete_func_key_dest($id) === false)
			return(false);

		return(parent::delete($id,$disable));
	}

	private function _delete_func_key_dest($paging_id)
	{
		if($this->_func_key_manager->delete_dst_for_paging($paging_id) === false)
			return(false);

		return(true);
	}
}

?>
