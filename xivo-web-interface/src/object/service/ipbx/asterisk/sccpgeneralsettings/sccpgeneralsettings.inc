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

xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_sccpgeneralsettings extends xivo_service_asterisk_abstract
{
	var $_dso	= null;
	var $_name	= 'sccpgeneralsettings';
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
		$result = array();

		foreach( $data as &$info ) {
			$key = $info['option_name'];
			$value = $info['option_value'];
			$result[$key] = $value;
		}

		return($result);
	}

	function get_options()
	{
		if(isset($this->_origin) === true)
			$this->_origin = false;

		if (($list = $this->_dso->get_all(null,false)) === false)
			return false;

		return($this->_prepare($list));
	}

	function save_sccp_general_option($option_name, $option_value)
	{
		$where = array(
			'option_name' => $option_name,
		);
		$option = array(
			'option_value' => $option_value
		);

		return($this->edit_where($where, $option));
	}

	function save_sccp_general_settings($options)
	{
		foreach ($options as $option_name => $option_value)
			if ($this->save_sccp_general_option($option_name, $option_value) === false)
				return(false);

		return(true);
	}

}

?>
