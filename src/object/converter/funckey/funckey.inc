<?php
#
# XiVO Web-Interface
# Copyright (C) 2015  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'funckey', 'mapper.inc'));

class FuncKeyConverter {

	protected $confd = null;
	protected $mapper = null;

	public function __construct($confd, $mapper) {
		$this->confd = $confd;
		$this->mapper = $mapper;
	}

	public function get_user_func_keys($user_id) {
		$funckeys = $this->confd->get_user_func_keys($user_id);
		if ($funckeys === null) {
			return array();
		}
		return $this->mapper->map_to_legacy($user_id, $funckeys);
	}

}

?>
