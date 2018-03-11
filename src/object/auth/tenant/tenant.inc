<?php

# Copyright 2018 The Wazo Authors  (see the AUTHORS file)
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

class AuthTenantResource {

	private $client = null;
	private $_headers = array();

	function __construct($client) {
		$this->client = $client;
		$this->_headers[] = 'X-Auth-Token: ' . $_SESSION['token'];
	}

	public function create($name)
	{
		$body = array(
			'name' => $name,
		);

		$request = Request::json_post('/tenants', $body, null, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	// public function edit($uuid, $username, $firstname, $lastname, $enable_login)
	// {
	// 	$body = array(
	// 		'username' => $username,
	// 		'firstname' => $firstname,
	// 		'lastname' => $lastname,
	// 		'enable_login' => $enable_login,
	// 	);
	// 	$request = Request::json_put('/tenants/'.$uuid, $body, null, $this->_headers);
	// 	$response = $this->client->send($request);
	// 	if ($response->status_ok() === false) {
	// 		return(false);
	// 	}
	// 	return(true);
	// }

	// public function delete($uuid)
	// {
	// 	$request = Request::delete('/tenants/'.$uuid, "", null, $this->_headers);
	// 	$response = $this->client->send($request);
	// 	return(true);
	// }
}

?>
