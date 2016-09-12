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


class PhonebookResource {

	private $client = null;
	private $_headers = array();

	function __construct($client) {
		$this->client = $client;
		$this->_headers[] = 'X-Auth-Token: ' . $_SESSION['_USR']->token;
	}

	public function delete_contact($entity, $phonebook_id, $contact_uuid)
	{
		$url = sprintf("/tenants/%s/phonebooks/%d/contacts/%s", $entity, $phonebook_id, $contact_uuid);
		$request = Request::delete($url,"",null,$this->_headers);
		$response = $this->client->send($request);
		return(true);
	}

	public function list_contacts($entity, $phonebook_id, $order=null, $limit=null)
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
