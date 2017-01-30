<?php

#
# XiVO Web-Interface
# Copyright 2017 The Wazo Authors  (see the AUTHORS file)
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

class MOHResource {

	private $client = null;

	function __construct($client) {
		$this->client = $client;
	}

	function get_all()
	{
		$request = Request::json_get("/moh");
		$response = $this->client->send($request);
		return $response->decode_or_raise();
	}

	function create($moh)
	{
		$request = Request::json_post("/moh", $moh);
		$response = $this->client->send($request);
		return $response->decode_or_raise();
	}

	function get($uuid) {
		$request = Request::json_get("/moh/" . $uuid);
		$response = $this->client->send($request);
		return $response->decode_or_raise();
	}

	function update($moh)
	{
		$request = Request::json_put("/moh/" . $moh['uuid'], $moh);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function delete($uuid) {
		$request = Request::delete("/moh/" . $uuid);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function download_file($uuid, $filename) {
		$request = Request::get("/moh/" . $uuid . "/files/" . $filename);
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return $response->get_body();
	}

	function upload_file($uuid, $filename, $content) {
		$headers = array('Content-Type: application/octet-stream');
		$request = Request::put("/moh/" . $uuid . "/files/" . $filename, $content, null, $headers);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function delete_file($uuid, $filename) {
		$request = Request::delete("/moh/" . $uuid . "/files/" . $filename);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}
}

?>
