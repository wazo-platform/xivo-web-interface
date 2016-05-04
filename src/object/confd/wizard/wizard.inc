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


class WizardResource {

	private $client = null;

	function __construct($client) {
		$this->client = $client;
	}

	public function discover() {
		$request = Request::json_get("/wizard/discover");
		$response = $this->client->send($request);
		$content = $response->decode_or_raise();
		return $content === null ? false : $content;
	}

	public function create($wizard)
	{
		$request = Request::json_post("/wizard", $wizard);
		$response = $this->client->send($request);
		return $response->status_ok_or_raise();
	}

}

?>
