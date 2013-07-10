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

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk','realstatic','abstract.inc'),true);

class xivo_application_service_asterisk_realstatic_features_featuremap extends xivo_application_service_asterisk_realstatic_abstract
{
	var $_name			= 'featuremap';
	var $_modulename	= 'features';
	var $_category		= 'featuremap';
	var $_cat_metric	= 1;
	var $_filename		= 'features.conf';

	public function __construct(&$service)
	{
		if(parent::_init($service) === false)
			return(false);

		return(true);
	}

	function _load_config()
	{
		return(parent::_load_config(dirname(__FILE__)));
	}

	function get($name,$disable=null)
	{
		$this->_info = $this->_origin = array();

		$this->_realstatic_get($name,$disable);

		return($this->_info);
	}

	function set($arr)
	{
		parent::set($arr);

		if(is_array($arr) === false || isset($arr['var_name']) === false)
			return(false);

		$number = (string) $this->get_result_var('var_val');
		$context = '';

		return(($this->get_errnb() < 1));
	}

	function save()
	{
		$r = true;

		$this->_status = 'save';
		$this->_return = array();

		if(is_array($this->_info) === false
		|| empty($this->_info) === true
		|| $this->get_errnb() > 0
		|| ($name = $this->_realstatic_save()) === false)
		{
			$r = false;
			$this->_reverse_save();
		}

		$this->_status = '';

		return($r);
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
				case $this->_name:
					$this->_realstatic_reverse($val);
					break;
			}
		}
	}
}

?>
