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
		$funckeys = $this->mapper->map_to_legacy($user_id, $funckeys);
		return $funckeys;
	}

	public function add_user_func_keys($user_id, $legacy_funckeys) {
		$funckeys = $this->mapper->map_to_confd($user_id, $legacy_funckeys);
		$this->remove_stale_func_keys($user_id, $funckeys);
		$updated_funckeys = $this->update_func_keys($user_id, $funckeys);
		return $this->mapper->map_legacy_return($user_id, $updated_funckeys);
	}

	private function remove_stale_func_keys($user_id, $funckeys) {
		$old_funckeys = $this->confd->get_user_func_keys($user_id);
		if ($old_funckeys === null) {
			return;
		}

		foreach(array_keys($funckeys) as $position) {
			if (!array_key_exists($position, $old_funckeys)) {
				$this->confd->delete_user_func_key($user_id, $position);
			}
		}
	}

	private function update_func_keys($user_id, $funckeys) {
		$updated_funckeys = array();
		foreach($funckeys as $position => $funckey) {
			$success = $this->confd->update_user_func_key($user_id, $position, $funckey);
			if ($success) {
				$updated_funckeys[] = $funckey;
			}
		}
		return $updated_funckeys;
	}

	public function delete_user_func_key($user_id, $position) {
		return $this->confd->delete_user_func_key($user_id, $position);
	}
}

?>
