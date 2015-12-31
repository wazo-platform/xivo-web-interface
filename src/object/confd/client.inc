<?php

#
# XiVO Web-Interface
# Copyright (C) 2015 Avencall
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


class Request {

	private $verb = null;
	private $url = null;
	private $body = null;
	private $headers = null;

	function __construct($verb, $url, $body="", $headers=null)
	{
		$this->verb = $verb;
		$this->url = $url;
		$this->body = $body;
		$this->headers = array();
		if ($headers !== null) {
			$this->headers = array_merge($this->headers, $headers);
		}
	}

	static public function post($url, $body, $headers=null)
	{
		return new Request("POST", $url, $body, $headers);
	}

	public function get_verb()
	{
		return $this->verb;
	}

	public function get_headers() 
	{
		return $this->headers;
	}

	public function build_url($root)
	{
		return $root . $this->url;
	}

	public function get_body()
	{
		return $this->body;
	}

}


class Response {

	private $status_code = null;
	private $body = null;
	private $error = "";

	function __construct($status_code, $body, $error)
	{
		$this->status_code = $status_code;
		$this->body = $body;
		$this->error = $error;

	}

	public function status_ok()
	{
		if ($this->error !== "") {
			return false;
		}
		if (!in_array($this->status_code, array(200, 201, 204))) {
			return false;
		}

		return true;
	}

	public function decode_json()
	{
		return json_decode($this->body, true);
	}

	public function get_errors()
	{
		if ($this->error !== "") {
			return array($this->error);
		} else {
			return $this->decode_json();
		}
	}

	public function get_body()
	{
		return $this->body;
	}

}


class Client {

	private $rooturl = null;
	private $connect_timeout = null;
	private $timeout = null;

	function __construct($rooturl, $connect_timeout=10, $timeout=10) {
		$this->rooturl = $rooturl; 
		$this->connect_timeout = $connect_timeout;
		$this->timeout = $timeout;
	}

	static function default_config() {
		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT, 'confd', 'abstract', 'config');

		if(($config = dwho_gat::get_file($dir)) === false) {
			trigger_error("could not load default config for confd");
			return(false);
		}

		$confd_config = $config['confd'];
		$url = "http://" . $confd_config['host'] 
			. ":" . (string)$confd_config['port'] 
			. "/" . $confd_config['protocol'];

		return new Client($url, $confd_config['connect_timeout'], $confd_config['timeout']);
	}

	function send($request)
	{
		$curl = $this->prepare_curl($request);
		return $this->build_response($curl);
	}

	private function prepare_curl($request) 
	{
		$curl = curl_init();
		curl_setopt($curl, CURLOPT_HTTPHEADER, $request->get_headers());
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_FORBID_REUSE, true);
		curl_setopt($curl, CURLOPT_URL, $request->build_url($this->rooturl));
		curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, $this->connect_timeout);
		curl_setopt($curl, CURLOPT_TIMEOUT, $this->timeout);

		$verb = $request->get_verb();
		if($verb == "GET") {
			curl_setopt($curl, CURLOPT_HTTPGET, true);
		} else if ($verb == "POST") {
			curl_setopt($curl, CURLOPT_POST, true);
			curl_setopt($curl, CURLOPT_POSTFIELDS, $request->get_body());
		} else if ($verb == "PUT") {
			curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'PUT');
			curl_setopt($curl, CURLOPT_POSTFIELDS, $request->get_body());
		} else if ($verb == "DELETE") {
			curl_setopt($curl, CURLOPT_CUSTOMREQUEST, 'DELETE');
		}

		return $curl;
	}

	private function build_response($curl) {
		$body = curl_exec($curl);
		$status_code = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		$error = curl_error($curl);
		curl_close($curl);
		return new Response($status_code, $body, $error);
	}

}

?>
