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

	public function add_user($tenant_uuid, $user_uuid)
	{
		$request = Request::put('/tenants/'.$tenant_uuid.'/users/'.$user_uuid, null, null, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return(true);
	}

	public function create($entity)
	{
		$body = $this->_entity_to_tenant($entity);

		$request = Request::json_post('/tenants', $body, null, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	public function search($name)
	{
		$query = array('name' => $name);
		$request = Request::get('/tenants', "", $query, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return($response->decode_or_raise());
	}

	public function delete($uuid)
	{
		$request = Request::delete('/tenants/'.$uuid, null, null, $this->_headers);
		$response = $this->client->send($request);
		if($response->status_ok() === false) {
			return(false);
		}
		return(true);
	}

	public function edit($uuid, $entity)
	{
		$body = $this->_entity_to_tenant($entity);

		$request = Request::json_put('/tenants/'.$uuid, $body, null, $this->_headers);
		$response = $this->client->send($request);
		if ($response->status_ok() === false) {
			return(false);
		}
		return(true);
	}

	private function _entity_to_tenant($entity)
	{
		$body = array();
		if(!empty($entity['name'])) {
			$body['name'] = $entity['name'];
		}
		if(!empty($entity['phonenumber'])) {
			$body['phone'] = $entity['phonenumber'];
		}

		$address = array();
		if(!empty($entity['city'])) {
			$address['city'] = $entity['city'];
		}
		if(!empty($entity['country'])) {
			$address['country'] = $entity['country'];
		}
		if(!empty($entity['address1'])) {
			$address['line_1'] = $entity['address1'];
		}
		if(!empty($entity['address2'])) {
			$address['line_2'] = $entity['address2'];
		}
		if(!empty($entity['state'])) {
			$address['state'] = $entity['state'];
		}
		if(!empty($entity['zipcode'])) {
			$address['zip_code'] = $entity['zipcode'];
		}

		if(!empty($address)) {
			$body['address'] = $address;
		}

		return($body);
	}

}

?>
