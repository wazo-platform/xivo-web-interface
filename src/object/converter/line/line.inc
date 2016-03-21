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

class LineResource {

	private $client = null;

	function __construct($client, $context_dso) {
		$this->client = $client;
		$this->context_dso = $context_dso;
	}

	function get($id) {
		$request = Request::json_get("/lines/" . $id);
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
		return null;
	}

	function delete($id) {
		$request = Request::delete("/lines/" . $id);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return true;
	}

	function get_endpoint($line) {
		$association = $this->get_endpoint_association($line);
		if($association != null) {
			$request = Request::json_get("/endpoints/" . $line['protocol'] . "/" . $association['endpoint_id']);
			$response = $this->client->send($request);
			if($response->status_ok()) {
				return $response->decode_json();
			}
		}
		return null;
	}

	function get_endpoint_association($line)
	{
		$url = "/lines/" . $line['id'] . "/endpoints/" . $line['protocol'];
		$request = Request::json_get($url);
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
		return null;
	}

	function get_internal_extension($line) {
		$associations = $this->list_extension_associations($line);
		foreach($associations as $association) {
			$extension = $this->get_extension($association);
			if($this->is_internal_extension($extension)) {
				return $extension;
			}
		}
		return null;
	}

	function list_extension_associations($line)
	{
		$url = "/lines/" . $line['id'] . "/extensions";
		$request = Request::json_get($url);
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json()['items'];
		}
		return array();
	}

	function get_device_association($line)
	{
		$request = Request::json_get("/lines/" . $line['id'] . "/devices");
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
		return null;
	}

	function list_user_associations($line)
	{
		$request = Request::json_get("/lines/" . $line['id'] . "/users");
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json()['items'];
		}
		return array();
	}

	function dissociate($line)
	{
		if($this->dissociate_device($line) === false) {
			dwho_report::push('error', 'error while dissociating device');
			return false;
		}

		if($this->dissociate_extensions($line) === false) {
			dwho_report::push('error', 'error while dissociating extensions');
			return false;
		}

		if($this->dissociate_users($line) === false) {
			dwho_report::push('error', 'error while dissociating users');
			return false;
		}

		if($this->dissociate_endpoint($line) === false) {
			dwho_report::push('error', 'error while dissociating endpoint');
			return false;
		}

		return true;
	}

	function dissociate_device($line)
	{
		$association = $this->get_device_association($line);
		if($association !== null) {
			$request = Request::delete("/lines/" . $line['id'] . "/devices/" . $association['device_id']);
			$response = $this->client->send($request);
			if(!$response->status_ok()) {
				$response->report_errors();
				return false;
			}
		}
		return true;
	}

	function dissociate_extensions($line)
	{
		$extension = $this->get_internal_extension($line);
		if($extension !== null) {
			$success = $this->dissociate_extension($line['id'], $extension['id']);
			if($success !== true) {
				return false;
			}
		}

		$associations = $this->list_extension_associations($line);
		foreach($associations as $association) {
			$success = $this->dissociate_extension($line['id'], $association['extension_id']);
			if($success !== false) {
				return false;
			}
		}
		return true;
	} 

	function dissociate_users($line)
	{
		$associations = $this->list_user_associations($line);
		foreach($associations as $association)
		{
			$request = Request::delete("/users/" . $association['user_id'] . "/lines/" . $line['id']);
			$response = $this->client->send($request);
			if(!$response->status_ok()) {
				$response->report_errors();
				return false;
			}
		}
	}

	function dissociate_endpoint($line)
	{
		$association = $this->get_endpoint_association($line);
		if($association !== null)
		{
			$url = "/lines/" . $line['id'] . "/endpoints/" . $association['endpoint'] . "/" . $association['endpoint_id'];
			$request = Request::delete($url);
			$response = $this->client->send($request);
			if(!$response->status_ok()) {
				$response->report_errors();
				return false;
			}
		}
		return true;
	}

	function get_main_line($user_id)
	{
		$associations = $this->list_user_line($user_id);
		foreach ($associations as $association) {
			if($association['main_line'] === true) {
				return $this->get($association['line_id']);
			}
		}
		return null;
	}

	function list_user_line($user_id)
	{
		$url = "/users/" . $user_id . "/lines";
		$request = Request::json_get($url);
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json()['items'];
		}
		return array();
	}

	private function dissociate_extension($line_id, $extension_id) 
	{
		$request = Request::delete("/lines/" . $line_id . "/extensions/" . $extension_id);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return true;
	}

	private function get_extension($association) {
		$request = Request::json_get("/extensions/" . $association['extension_id']);
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
	}

	private function is_internal_extension($extension) {
		$contexts = $this->context_dso->get_all(null, null, null, false, 'internal');
		foreach($contexts as $context) {
			if($context['name'] == $extension['context']) {
				return true;
			}
		}
		return false;
	}

}

?>
