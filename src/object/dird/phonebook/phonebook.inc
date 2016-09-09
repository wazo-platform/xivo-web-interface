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

	function __construct($client) {
		$this->client = $client;
	}

	public function list_phonebooks($entity) {
        $url = sprintf("/tenants/%s/phonebooks", $entity);
        $headers = array('X-Auth-Token: ' . $_SESSION['_USR']->token);
		$request = Request::json_get($url,"",null,$headers);
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
