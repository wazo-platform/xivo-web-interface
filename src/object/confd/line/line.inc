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
		return $response->decode_or_raise();
	}

	function create($line)
	{
		$request = Request::json_post("/lines", $line);
		$response = $this->client->send($request);
		return $response->decode_or_raise();
	}

	function update($line)
	{
		$request = Request::json_put("/lines/" . $line['id'], $line);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function delete($id) {
		$request = Request::delete("/lines/" . $id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function list_extension_associations($id)
	{
		$url = "/lines/" . $id . "/extensions";
		$request = Request::json_get($url);
		$response = $this->client->send($request);
		$content = $response->decode_or_raise();
		return $content === null ? null : $content['items'];
	}

	function get_device_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/devices");
		$response = $this->client->send($request);
		if($response->get_status() == 404) {
			return null;
		}
		return $response->decode_or_raise();
	}

	function list_user_associations($id)
	{
		$request = Request::json_get("/lines/" . $id . "/users");
		$response = $this->client->send($request);
		$content = $response->decode_or_raise();
		return $content === null ? null : $content['items'];
	}

	function get_sip_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/endpoints/sip");
		$response = $this->client->send($request);
		if($response->get_status() == 404) {
			return null;
		}
		return $response->decode_or_raise();
	}

	function get_sccp_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/endpoints/sccp");
		$response = $this->client->send($request);
		if($response->get_status() == 404) {
			return null;
		}
		return $response->decode_or_raise();
	}

	function get_custom_association($id)
	{
		$request = Request::json_get("/lines/" . $id . "/endpoints/custom");
		$response = $this->client->send($request);
		if($response->get_status() == 404) {
			return null;
		}
		return $response->decode_or_raise();
	}

	function associate_device($line_id, $device_id)
	{
		$request = Request::put("/lines/" . $line_id . "/devices/" . $device_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function associate_sip($line_id, $sip_id)
	{
		$request = Request::put("/lines/" . $line_id . "/endpoints/sip/" . $sip_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function associate_sccp($line_id, $sccp_id)
	{
		$request = Request::put("/lines/" . $line_id . "/endpoints/sccp/" . $sccp_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function associate_custom($line_id, $custom_id)
	{
		$request = Request::put("/lines/" . $line_id . "/endpoints/custom/" . $custom_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function associate_extension($line_id, $extension_id)
	{
		$request = Request::json_post(
			"/lines/" . $line_id . "/extensions",
			array('extension_id' => $extension_id)
		);

		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function associate_user($line_id, $user_id)
	{
		$request = Request::json_post(
			"/users/" . $user_id . "/lines",
			array('line_id' => $line_id)
		);

		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function dissociate_extension($line_id, $extension_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/extensions/" . $extension_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function dissociate_sip($line_id, $sip_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/endpoints/sip/" . $sip_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function dissociate_sccp($line_id, $sccp_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/endpoints/sccp/" . $sccp_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function dissociate_custom($line_id, $custom_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/endpoints/custom/" . $custom_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function dissociate_device($line_id, $device_id)
	{
		$request = Request::delete("/lines/" . $line_id . "/devices/" . $device_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function dissociate_user($line_id, $user_id)
	{
		$request = Request::delete("/users/" . $user_id . "/lines/" . $line_id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

}

?>
