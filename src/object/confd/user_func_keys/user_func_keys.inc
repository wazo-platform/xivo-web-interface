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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd','abstract','abstract.inc'));

class confd_ressource_user_func_keys extends xivo_confd_ressource_abstract {

	protected $_name = 'user_func_keys';
	protected $_base_url = 'users';

	protected $status_ok = array(200, 201, 204);

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
		$this->_httpheaders[] = dwho_json::get_header();
		$this->_httpheaders[] = 'Accept: application/json';
	}

	protected function fetch_response($response)
	{
		$this->_status_code = $this->_curl->get_info('http_code');
		$this->_curl->close();

		$decoded = dwho_json::decode(rtrim($response),true);

		if($decoded === null) {
			dwho_report::push('error', $response);
			return(null);
		}

		if (!(in_array($this->_status_code, $this->status_ok))) {
			dwho_report::push('error', $decoded);
			return(null);
		}

		return($decoded);
	}

	public function get_template($user_id) {
		$path = array($user_id, 'funckeys');
		$url = $this->_build_uri($path);
		return $this->request_get($url);
	}

	public function get_user_func_keys($user_id) {
		$template = $this->get_template($user_id);
		if ($template === null) {
			return(null);
		}

		$funckeys = array();
		foreach($template['keys'] as $position => $funckey) {
			if ($funckey['inherited'] === false) {
				$funckeys[$position] = $funckey;
			}
		}

		return $funckeys;
	}
}

?>
