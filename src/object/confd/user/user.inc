<?php

#
# XiVO Web-Interface
# Copyright (C) 2015-2016 Avencall
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


class UserResource {

	private $client = null;

	function __construct($client) {
		$this->client = $client;
	}

	public function update($user)
	{
		$request = Request::json_put("/users/" . $user['id'], $user);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

	public function import_csv($csvdata)
	{
		$request = $this->build_csv_request("POST", $csvdata);
		$response = $this->client->send($request);
		return $this->process_response($response);
	}

	public function update_csv($csvdata)
	{
		$request = $this->build_csv_request("PUT", $csvdata);
		$response = $this->client->send($request);
		return $this->process_response($response);
	}

	public function export_csv()
	{
		$request = Request::get("/users/export");
		$response = $this->client->send($request);
		if(!$response->status_ok()) {
			$response->report_errors();
			return false;
		}
		return $response->get_body();
	}

	public function list_line_associations($id)
	{
		$url = "/users/" . $id . "/lines";
		$request = Request::json_get($url);
		$response = $this->client->send($request);
		$content = $response->decode_or_raise();
		return $content === null ? array() : $content['items'];
	}

	private function build_csv_request($verb, $csvdata)
	{
		$headers = array('Content-Type: text/csv; charset=utf-8');
		$query = null;
		return new Request($verb, "/users/import", $csvdata, $query, $headers);
	}

	private function process_response($response)
	{
		if (!$response->status_ok()) {
			return $this->extract_errors($response);
		} else {
			return true;
		}
	}

	private function extract_errors($response)
	{
		$errors = $response->get_errors();
		if (array_key_exists('errors', $errors)) {
			return $this->format_errors($errors['errors']);
		} else {
			return $errors;
		}
	}

	private function format_errors($errors)
	{
		$formatted_errors = array();
		foreach($errors as $error) {
			$msg = "CSV Line " . $error['details']['row_number'] . ": " . $error['message'];
			$formatted_errors[] = $msg;
		}
		return $formatted_errors;
	}

}

?>
