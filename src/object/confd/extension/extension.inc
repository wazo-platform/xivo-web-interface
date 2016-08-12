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

class ExtensionResource {

	private $client = null;

	function __construct($client) {
		$this->client = $client;
	}

	function get($id) {
		$request = Request::json_get("/extensions/" . $id);
		$response = $this->client->send($request);
		return $response->decode_or_raise();
	}

	function create($extension)
	{
		$request = Request::json_post("/extensions", $extension);
		$response = $this->client->send($request);
		return $response->decode_or_raise();
	}

	function update($extension)
	{
		$request = Request::json_put("/extensions/" . $extension['id'], $extension);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function delete($id) {
		$request = Request::delete("/extensions/" . $id);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	function list_line_associations($id)
	{
		$request = Request::json_get("/extensions/" . $id . "/line");
		$response = $this->client->send($request);
		if($response->get_status() == 404) {
			return null;
		}
		return $response->decode_or_raise();
	}
}
