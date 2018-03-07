<?php

#
# XiVO Web-Interface
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
#

class AuthUserResource {

	private $client = null;
	private $_headers = array();

	function __construct($client) {
		$this->client = $client;
		$this->_headers[] = 'X-Auth-Token: ' . $_SESSION['token'];
	}

	public function create($uuid, $username, $password, $firstname, $lastname, $email, $enable_login)
	{
		$body = array(
			'uuid' => $uuid,
			'username' => $username,
			'firstname' => $firstname,
			'lastname' => $lastname,
			'email_address' => $email,
			'enable_login' => $enable_login,
		);
		if (empty($password) === false) {
			$body['password'] = $password;
		}
		$request = Request::json_post('/users', $body, null, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	public function get($uuid)
	{
		$request = Request::json_get('/users/'.$uuid, "", null, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	public function edit($uuid, $username, $firstname, $lastname, $enable_login)
	{
		$body = array(
			'username' => $username,
			'firstname' => $firstname,
			'lastname' => $lastname,
			'enable_login' => $enable_login,
		);
		$request = Request::json_put('/users/'.$uuid, $body, null, $this->_headers);
		$response = $this->client->send($request);
		if ($response->status_ok() === false) {
			return(false);
		}
		return(true);
	}

	public function edit_password($uuid, $password)
	{
		$query = array(
			'user_uuid' => $uuid
		);
		$body = array(
			'password' => $password,
		);
		$request = Request::json_post('/users/password/reset', $body, $query, $this->_headers);
		$response = $this->client->send($request);
		if ($response->status_ok() === false) {
			return(false);
		}
		return(true);
	}

	public function edit_email($uuid, $email)
	{
		$emails = array();
		if ($email) {
			array_push($emails, array(
				'address' => $email,
				'confirmed' => true,
				'main' => true
			));
		}

		$body = array('emails' => $emails);
		$request = Request::json_put('/admin/users/'.$uuid.'/emails', $body, null, $this->_headers);
		$response = $this->client->send($request);
		if ($response->status_ok() === false) {
			return(false);
		}
		return(true);
	}

	public function delete($uuid)
	{
		$request = Request::delete('/users/'.$uuid, "", null, $this->_headers);
		$response = $this->client->send($request);
		return(true);
	}
}

?>
