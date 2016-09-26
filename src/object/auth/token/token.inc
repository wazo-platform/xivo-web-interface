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

class TokenResource {

	private $client = null;
	private $backend = null;

	function __construct($client) {
		$this->client = $client;
		$this->backend = $client->get_backend();
	}

	public function create($login, $password, $expiration=null)
	{
		$expiration = $expiration === null ? 3600 * 24 : $expiration;

		$body = array('backend' => $this->backend, 'expiration' => $expiration);

		$request = Request::json_post('/token', $body);
		$request->set_basic_auth($login, $password);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	public function get($token)
	{
		$request = Request::json_get('/token/'.$token);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	public function delete($token)
	{
		$request = Request::delete('/token/'.$token);
		$this->client->send($request);
	}
}

?>