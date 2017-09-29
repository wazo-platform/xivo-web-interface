<?php

#
# Copyright 2015-2017 The Wazo Authors  (see the AUTHORS file)
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'http', 'client.inc'));

class AuthClient extends Client
{
	private $auth_backend = null;

	function __construct($url, $connect_timeout, $timeout, $verify, $raise, $backend)
	{
		parent::__construct($url, $connect_timeout, $timeout, $verify, $raise);
		$this->auth_backend = $backend;
	}

	public function get_backend()
	{
		return $this->auth_backend;
	}

	static function default_config($raise=false) {
		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT, 'auth', 'abstract', 'config');

		if(($config = dwho_gat::get_file($dir)) === false) {
			trigger_error("could not load default config for wazo-auth");
			return(false);
		}

		$default_auth_config = $config['auth'];
		$ipbx_ini_config = dwho::load_init(XIVO_PATH_CONF.DWHO_SEP_DIR.'ipbx.ini');
		if(isset($ipbx_ini_config['xivoauth']) === false) {
			$auth_config = $default_auth_config;
		} else {
			$auth_config = array_merge(
				$default_auth_config,
				$ipbx_ini_config['xivoauth']);
		}

		$url = $auth_config['scheme']. "://" . $auth_config['host']
			. ":" . (string)$auth_config['port']
			. "/" . $auth_config['protocol'];

		return new AuthClient(
			$url,
			$auth_config['connect_timeout'],
			$auth_config['timeout'],
			$auth_config['verify_certificate'],
			$raise,
			$auth_config['backend']);
	}
}

?>