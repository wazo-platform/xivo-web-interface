<?php

#
# Copyright 2016-2017 The Wazo Authors  (see the AUTHORS file)
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


class PhonebookResource {

	private $client = null;
	private $_headers = array();

	function __construct($client) {
		$this->client = $client;
		$this->_headers[] = 'X-Auth-Token: ' . $_SESSION['token'];
	}

	public function add_contact($entity, $phonebook_id, $fields)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts", $entity, $phonebook_id);
		$request = Request::json_post($url, $fields, null, $this->_headers);
		$response = $this->client->send($request);
	}

	public function add_phonebook($entity, $name, $description)
	{
		$url = sprintf("/tenants/%s/phonebooks", $entity);
		$fields = array('name' => $name);
		if(empty($description === false)) {
			$fields['description'] = $description;
		}
		$request = Request::json_post($url, $fields, null, $this->_headers);
		$response = $this->client->send($request);
	}

	public function edit_contact($entity, $phonebook_id, $contact_uuid, $fields)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts/%s", $entity, $phonebook_id, $contact_uuid);
		$request = Request::json_put($url,$fields,null,$this->_headers);
		$response = $this->client->send($request);
	}

	public function get_contact($entity, $phonebook_id, $contact_uuid)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts/%s", $entity, $phonebook_id, $contact_uuid);
		$request = Request::json_get($url,"",null,$this->_headers);
		$response = $this->client->send($request);

		if($response->status_ok()
		&& ($body = $response->decode_or_raise())
		&& empty($body) === false) {
			return($body);
		}

		return(false);
	}

	public function get_phonebook($entity, $phonebook_id)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d", $entity, $phonebook_id);
		$request = Request::json_get($url,"",null,$this->_headers);

		if(($response = $this->client->send($request))
			&& $response->status_ok()
			&& ($body = $response->decode_or_raise())
			&& empty($body) === false) {
			return($body);
		}

		return(false);
	}

	public function delete_contact($entity, $phonebook_id, $contact_uuid)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts/%s", $entity, $phonebook_id, $contact_uuid);
		$request = Request::delete($url,"",null,$this->_headers);
		$response = $this->client->send($request);
		return(true);
	}

	public function delete_phonebook($entity, $phonebook_id)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d", $entity, $phonebook_id);
		$request = Request::delete($url,"",null,$this->_headers);
		$response = $this->client->send($request);
		return(true);
	}

	public function import_csv_contacts($entity, $phonebook_id, $payload)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts/import", $entity, $phonebook_id);
		$headers = array(
			'Content-Type: text/csv;charset=utf-8',
			'X-Auth-Token: '.$_SESSION['token'],
			'Accept: application/json',
		);

		$request = Request::post($url, $payload, null, $headers);
		$response = $this->client->send($request);
		$body = $response->decode_or_raise();
		$failed = $body['failed'];
		if(is_array($failed) === true && count($failed) > 0) {
			foreach($failed as $contact) {
				dwho_report::push('error', implode(",", $contact));
			}
			dwho_report::push('error', 'Some contacts were not imported:');
			return(false);
		}
		return(true);
	}

	public function list_contacts($entity, $phonebook_id, $order=null, $limit=null, $search=null)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts", $entity, $phonebook_id);
		$query = array();
		if(is_array($order) && count($order) > 1) {
			$query['direction'] = $order[0] == SORT_ASC ? 'asc' : 'desc';
			$query['order'] = $order[1];
		}
		if(is_array($limit) && count($limit) > 1) {
			$query['offset'] = $limit[0];
			$query['limit'] = $limit[1];
		}
		if($search) {
			$query['search'] = $search;
		}
		$request = Request::json_get($url,"",$query,$this->_headers);
		$response = $this->client->send($request);
		$body = $response->status_ok() ? $response->decode_or_raise() : null;
		if(is_array($body) && array_key_exists('items', $body) && is_array($body['items'])) {
			return $body['items'];
		} else {
			return array();
		}
	}

	public function list_phonebooks($entity) {
		$url = sprintf("/tenants/%s/phonebooks", $entity);
		$request = Request::json_get($url,"",null,$this->_headers);
		$response = $this->client->send($request);
		$body = $response->status_ok() ? $response->decode_or_raise() : null;
		if(is_array($body) && array_key_exists('items', $body) && is_array($body['items'])) {
			return $body['items'];
		} else {
			return array();
		}
	}
}

?>
