<?php

#
# XiVO Web-Interface
# Copyright (C) 2016 Avencall
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

class DirdClient extends Client
{
	static function default_config($raise=false) {
		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT, 'dird', 'abstract', 'config');

		if(($config = dwho_gat::get_file($dir)) === false) {
			trigger_error("could not load default config for xivo-dird");
			return(false);
		}

		$default_dird_config = $config['dird'];
		$ipbx_ini_config = dwho::load_init(XIVO_PATH_CONF.DWHO_SEP_DIR.'ipbx.ini');
		if(isset($ipbx_ini_config['xivodird']) === false) {
			$dird_config = $default_dird_config;
		} else {
			$dird_config = array_merge(
				$default_dird_config,
				$ipbx_ini_config['xivodird']);
		}

		$url = $dird_config['scheme']. "://" . $dird_config['host']
			. ":" . (string)$dird_config['port']
			. "/" . $dird_config['protocol'];

		return new DirdClient(
			$url,
			$dird_config['connect_timeout'],
			$dird_config['timeout'],
			$dird_config['verify_certificate'],
			$raise);
	}
}

?>