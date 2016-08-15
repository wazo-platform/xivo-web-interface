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

xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_user_line extends xivo_service_asterisk_abstract
{
	var $_dso		= null;
	var $_name		= 'user_line';
	var $_filter	= false;
	var $_origin	= false;

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

	function _prepare($data)
	{
		$data['id'] = (int) $data['id'];
		$data['user_id'] = (int) $data['user_id'];
		$data['line_id'] = (int) $data['line_id'];
		$data['main_user'] = (bool) $data['main_user'];
		$data['main_line'] = (bool) $data['main_line'];

		return($data);
	}

	function add($arr,$id=0)
	{
		if(is_array($arr) === false)
			return(false);

		$data = $this->chk_values($arr);

		return(parent::add($data,$id));
	}

	function edit($id,$arr,$provisioning=false,$disable=null)
	{
		if(is_array($arr) === false)
			return(false);

		$data = $this->chk_values($arr);

		return(parent::edit($id,$data,$disable));
	}

	function find_by_user_id($user_id)
	{
		$where = array();
		$where['user_id'] = (int) $user_id;

		return($this->get_all_where($where));
	}

	function get_master_user_id_by_line_id($line_id)
	{
		if (($user_id = $this->_dso->find_main_user_id($line_id)) === false)
			return false;

		return (int) $user_id;
	}
}

?>
