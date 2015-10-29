<?php

#
# XiVO Web-Interface
# Copyright (C) 2015 Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'auth', 'abstract.inc'));

abstract class xivo_authent_abstract_auth
{

	var $_auth = null;

	protected function _init($param=array())
	{

		$_RAPI = &dwho_gct::get('xivo_auth');
		if($_RAPI === false) {
			trigger_error('Invalid AUTH object',E_USER_ERROR);
			return(false);
		}

        	$this->_auth = $_RAPI->get_ressource('auth');
		if($this->_auth === false) {
			trigger_error('Cannot load auth resource',E_USER_ERROR);
			return(false);
		}

	}

	function chk($login,$pass,$meta=false)
	{
		if(isset($_SESSION['_USR']->token)) {
			$tk = $this->_auth->get_token($_SESSION['_USR']->token);
		} else {
			$tk = $this->_auth->create_token($login, $pass, 'xivo_admin');
		}

		$token = $tk['data']['token'];
		if ($token)
			$_SESSION['_USR']->token = $token;
			return(true);
		return(false);
	}

	function logoff()
	{
		$this->_auth->delete_token($_SESSION['_USR']->token);
	}
}

?>
