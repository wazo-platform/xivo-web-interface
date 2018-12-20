<?php

#
# XiVO Web-Interface
# Copyright 2006-2018 The Wazo Authors  (see the AUTHORS file)
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

class xivo_user
{
	var $_ini	= array();
	var $_conf	= array();

	public function __construct()
	{
		$this->_conf = &dwho_gat::get('user');

		$ini = &xivo_user::load_get_init();

		if(dwho_issa('general',$ini) === false)
			trigger_error('Invalid user init file',E_USER_ERROR);

		$this->_ini = &$ini;

		$this->_chk_ini_general();

		if(isset($_SESSION['_USR']) === false)
			$_SESSION['_USR'] = (object) null;

		if(isset($_SESSION['_USR']->_type) === false
		|| xivo_user::chk_type($_SESSION['_USR']->_type) === false)
			$_SESSION['_USR']->_type = 0;

		$meta = xivo_user::type_to_meta($_SESSION['_USR']->_type);

		if(($class = $this->load_class($meta)) === false)
			trigger_error('Failed to load user class',E_USER_ERROR);

		dwho_gct::set(new $class(),'_USR');
	}

	function _chk_ini_general()
	{
		$cfg_gen = &$this->_ini['general'];

		if(isset($cfg_gen['time_cache']) === true)
			$this->_conf['option']['time_cache'] = dwho_uint($cfg_gen['time_cache']);

	}

	public static function &load_get_init()
	{
		static $ini = null;

		$path = XIVO_PATH_CONF.DWHO_SEP_DIR.'user.ini';

		if($ini === null && ($ini = xivo::load_init($path)) === false)
			trigger_error('User init file not found or unreadable',E_USER_ERROR);

		return $ini;
	}

	public static function get_info($name)
	{
		$name = (string) $name;

		if(isset($_SESSION['_USR'],$_SESSION['_USR']->{$name}) === true && $name{0} !== '_')
			return($_SESSION['_USR']->{$name});

		return(null);
	}

	public static function is_valid()
	{
		if(isset($_SESSION['_USR'],$_SESSION['_USR']->_valid) === true
		&& $_SESSION['_USR']->_valid === true)
			return(true);

		return(false);
	}

	public static function chk_active()
	{
		$cfg = dwho_gat::get('user');

		if(xivo_user::is_valid() === false)
			return(false);
		else if($cfg['option']['time_cache'] <= 0
		|| ($cfg['option']['time_cache'] + $_SESSION['_USR']->_time) > gmstrftime('%s'))
			return(true);

		return(null);
	}

	public static function chk_type($type,$real=false)
	{
		$cfg = dwho_gat::get('user');

		if(($r = isset($cfg['type'][$type])) === true
		&& (bool) $real === true)
			return(($type > 0));

		return($r);
	}

	public static function chk_meta($meta,$real=false)
	{
		if(($type = xivo_user::meta_to_type($meta)) !== false
		&& ($meta = xivo_user::type_to_meta($type,$real)) !== false)
			return($meta);

		return(false);
	}

	public static function type_to_meta($type,$real=false)
	{
		$cfg = dwho_gat::get('user');

		if(xivo_user::chk_type($type,$real) === true
		&& isset($cfg['type'][$type]['meta']) === true)
			return($cfg['type'][$type]['meta']);

		return(false);
	}

	public static function meta_to_type($meta)
	{
		$meta = strtoupper($meta);

		if(defined('XIVO_USER__'.$meta) === true)
			return(constant('XIVO_USER__'.$meta));

		return(false);
	}

	public static function chk_authorize($users,$meta='',$real=false)
	{
		$users = (array) $users;

		if(xivo_user::is_valid() === false
		|| empty($users) === true)
			return(false);
		else if($meta === '')
			$meta = $_SESSION['_USR']->meta;

		if(xivo_user::chk_meta($meta) === false)
			return(false);

		foreach($users as $value)
		{
			if(($umeta = xivo_user::chk_meta($value,$real)) === false)
				break;
			else if($meta === $umeta)
				return(true);
		}

		return(false);
	}

	public static function chk_acl($category='',$subcategory='',$section='')
	{
	        return(true);
	}

	public static function chk_acl_section($section)
	{
	        return(true);
	}

	public static function logoff()
	{
		session_unset();
	}

	public static function load_class($meta)
	{
		if(($meta = xivo_user::chk_meta($meta)) === false)
			return(false);

		$class = 'xivo_user_'.$meta;
		$dir = XIVO_PATH_OBJECT.DWHO_SEP_DIR.'user';

		if(xivo::load_class($class,$dir,$meta,false) === false)
			trigger_error('Failed to load user class type: '.$meta,E_USER_ERROR);

		return($class);
	}
}

?>
