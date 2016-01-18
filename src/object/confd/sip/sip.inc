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


class SipResource {

	private $client = null;

	function __construct($client) {
		$this->client = $client;
	}

	public function get($id)
	{
		$request = Request::json_get("/endpoints/sip/" . $id);
		$response = $this->client->send($request);

		if($response->status_ok()) {
			return $response->decode_json();
		} else if ($response->get_status() == 404) {
			return null;
		} else {
			$this->report_errors($response);
			return null;
		}
	}

	private function report_errors($response)
	{
		foreach($response->get_errors() as $error) {
			dwho_report::push('error', $error);
		}
	}

	public function update($id, $parameters)
	{
		$request = Request::json_put("/endpoints/sip/" . $id, $parameters);
		$response = $this->client->send($request);

		if($response->status_ok()) {
			return true;
		} else {
			$this->report_errors($response);
			return false;
		}
	}

}

?>
