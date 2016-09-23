<?php

#
# XiVO Web-Interface
# Copyright (C) 2015-2016 Avencall
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

class ConfdClient extends Client
{
	static function default_config($raise=false) {
		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT, 'confd', 'abstract', 'config');

		if(($config = dwho_gat::get_file($dir)) === false) {
			trigger_error("could not load default config for confd");
			return(false);
		}

		$confd_config = $config['confd'];
		$url = "http://" . $confd_config['host']
			. ":" . (string)$confd_config['port']
			. "/" . $confd_config['protocol'];

		return new Client($url, $confd_config['connect_timeout'], $confd_config['timeout'], $raise);
	}
}

?>