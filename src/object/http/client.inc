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

class ClientException extends Exception
{
	private $messages = null;

	public function __construct($messages, $code = 0, $previous = null)
	{
		$message = implode(',', $messages);
		parent::__construct($message, $code, $previous);
		$this->messages = $messages;
	}

	public function report_errors()
	{
		foreach($this->messages as $error) {
			dwho_report::push('error', $error);
		}
	}

}


class Request {

	private $verb = null;
	private $url = null;
	private $body = null;
	private $headers = null;

	static private $json_headers = array(
		'Content-Type: application/json',
		'Accept: application/json'
	);

	function __construct($verb, $url, $body="", $query=null, $headers=null)
	{
		$this->verb = $verb;
		$this->url = $url;
		$this->body = $body;
		$this->query = array();
		if ($query !== null) {
			$this->query = $query;
		}
		$this->headers = array();
		if ($headers !== null) {
			$this->headers = $headers;
		}
	}

	static public function post($url, $body, $query=null, $headers=null)
	{
		return new Request("POST", $url, $body, $query, $headers);
	}

	static public function put($url, $body="", $query=null, $headers=null)
	{
		return new Request("PUT", $url, $body, $query, $headers);
	}

	static public function get($url, $body="", $query=null, $headers=null)
	{
		return new Request("GET", $url, $body, $query, $headers);
	}

	static public function delete($url, $body="", $query=null, $headers=null)
	{
		return new Request("DELETE", $url, $body, $query, $headers);
	}

	static public function json_get($url, $body="", $query=null)
	{
		return new Request("GET", $url, json_encode($body), $query, self::$json_headers);
	}

	static public function json_post($url, $content, $query=null)
	{
		return new Request("POST", $url, json_encode($content), $query, self::$json_headers);
	}

	static public function json_put($url, $body, $query=null)
	{
		return new Request("PUT", $url, json_encode($body), $query, self::$json_headers);
	}

	public function get_verb()
	{
		return $this->verb;
	}

	public function get_headers()
	{
		return $this->headers;
	}

	public function get_url()
	{
		return $this->url;
	}

	public function build_url($root)
	{
		$url = $root . $this->url;
		$query_string = http_build_query($this->query);
		if (! empty($query_string)) {
			$url = $url . "?" . $query_string;
		}
		return $url;
	}

	public function get_body()
	{
		return $this->body;
	}

}


class Response {

	private $status_code = null;
	private $body = null;
	private $request = null;
	private $error = "";
	private $raise = false;

	function __construct($status_code, $body, $error, $request, $raise=false)
	{
		$this->status_code = $status_code;
		$this->body = $body;
		$this->error = $error;
		$this->raise = $raise;
		$this->request = $request;
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

	public function report_errors()
	{
		foreach($this->get_errors() as $error) {
			$msg = $this->request->get_verb()
				. " "
				. $this->request->get_url()
				. " - "
				. $error;
			dwho_report::push('error', $msg);
		}
	}

	public function get_status()
	{
		return $this->status_code;
	}

	public function get_body()
	{
		return $this->body;
	}

	public function decode_or_raise()
	{
		if ($this->status_ok()) {
			return $this->decode_json();
		} else {
			$this->handle_error();
		}
		return null;
	}

	public function status_ok_or_raise()
	{
		if($this->status_ok()) {
			return true;
		} else {
			$this->handle_error();
		}
		return false;
	}

	private function handle_error()
	{
		if($this->raise == true) {
			throw new ClientException($this->get_errors(), $this->status_code);
		} else {
			$this->report_errors();
		}
	}

}


class Client {

	private $rooturl = null;
	private $connect_timeout = null;
	private $timeout = null;

	function __construct($rooturl, $connect_timeout=10, $timeout=10, $raise=false) {
		$this->rooturl = $rooturl;
		$this->connect_timeout = $connect_timeout;
		$this->timeout = $timeout;
		$this->raise = $raise;
	}

	function send($request)
	{
		$curl = $this->prepare_curl($request);
		return $this->build_response($curl, $request);
	}

	private function prepare_curl($request)
	{
		$curl = curl_init();
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($curl, CURLOPT_FORBID_REUSE, true);
		curl_setopt($curl, CURLOPT_CONNECTTIMEOUT, $this->connect_timeout);
		curl_setopt($curl, CURLOPT_TIMEOUT, $this->timeout);
		curl_setopt($curl, CURLOPT_URL, $request->build_url($this->rooturl));
		curl_setopt($curl, CURLOPT_HTTPHEADER, $request->get_headers());

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

	private function build_response($curl, $request) {
		$body = curl_exec($curl);
		$status_code = curl_getinfo($curl, CURLINFO_HTTP_CODE);
		$error = curl_error($curl);
		curl_close($curl);
		return new Response($status_code, $body, $error, $request, $this->raise);
	}

}

?>
