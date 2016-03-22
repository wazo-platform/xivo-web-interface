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

	function __construct($client) {
		$this->client = $client;
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

	function list_extension_associations($id)
	{
		$url = "/lines/" . $id . "/extensions";
		$request = Request::json_get($url);
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json()['items'];
		}
		return array();
	}

	function get_device_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/devices");
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
		return null;
	}

	function list_user_associations($id)
	{
		$request = Request::json_get("/lines/" . $id . "/users");
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json()['items'];
		}
		return array();
	}

	function get_sip_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/endpoints/sip");
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
		return null;
	}

	function get_sccp_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/endpoints/sccp");
		$response = $this->client->send($request);
		if($response->status_ok()) {
			return $response->decode_json();
		}
		return null;
	}

	function dissociate_extension($line_id, $extension_id) 
	{
		$request = Request::delete("/lines/" . $line_id . "/extensions/" . $extension_id);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return true;
	}

	function dissociate_sip($line_id, $sip_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/endpoints/sip/" . $sip_id);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return true;
	}

	function dissociate_sccp($line_id, $sccp_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/endpoints/sccp/" . $sccp_id);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return true;
	}

	function dissociate_device($line_id, $device_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/devices/" . $device_id);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return true;
	}

}

?>
