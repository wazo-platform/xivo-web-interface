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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.	If not, see <http://www.gnu.org/licenses/>.
#

spl_autoload_register(function ($class) {
	// project-specific namespace prefix
	$prefix = 'PhpAmqpLib\\';
	// base directory for the namespace prefix
	$base_dir = '/usr/share/php/PhpAmqpLib/';
	// does the class use the namespace prefix?
	$len = strlen($prefix);
	if (strncmp($prefix, $class, $len) !== 0) {
		// no, move to the next registered autoloader
		return;
	}
	// get the relative class name
	$relative_class = substr($class, $len);
	// replace the namespace prefix with the base directory, replace namespace
	// separators with directory separators in the relative class name, append
	// with .php
	$file = $base_dir . str_replace('\\', '/', $relative_class) . '.php';
	// if the file exists, require it
	if (file_exists($file)) {
		require_once($file);
	}
});

use PhpAmqpLib\Connection\AMQPConnection;
use PhpAmqpLib\Message\AMQPMessage;

class MessageQueue {

	private $key = 'bus_msg_to_send';

	function add_msg($msg) {
		if (!isset($_SESSION[$this->key])) {
			$_SESSION[$this->key] = array();
		}

		array_push($_SESSION[$this->key], $msg);
	}

	function get_messages() {
		if (!isset($_SESSION[$this->key])) {
			return array();
		}

		return $_SESSION[$this->key];
	}

	function clear() {
		if (isset($_SESSION[$this->key])) {
			unset($_SESSION[$this->key]);
		}
	}

}

class Publisher {

	private $host = null;
	private $port = null;
	private $username = null;
	private $password = null;
	private $exchange_name = null;
	private $exchange_type = 'topic';

	function __construct($host, $port, $username, $password, $exchange_name) {
		$this->host = $host;
		$this->port = $port;
		$this->username = $username;
		$this->password = $password;
		$this->exchange_name = $exchange_name;
	}

	static function from_config() {
		$config = dwho::load_init(XIVO_PATH_CONF.DWHO_SEP_DIR.'ipbx.ini');

		if (!array_key_exists('bus', $config)
			|| !array_key_exists('host', $config['bus'])
			|| !array_key_exists('port', $config['bus'])
			|| !array_key_exists('username', $config['bus'])
			|| !array_key_exists('password', $config['bus'])
			|| !array_key_exists('exchange_name', $config['bus'])) {
			return false;
		}

		$host = $config['bus']['host'];
		$port = $config['bus']['port'];
		$username = $config['bus']['username'];
		$password = $config['bus']['password'];
		$exchange_name = $config['bus']['exchange_name'];

		return new Publisher($host, $port, $username, $password, $exchange_name);
	}

	function publish_messages($messages) {
		$connection = new AMQPConnection($this->host, $this->port, $this->username, $this->password);
		$channel = $connection->channel();
		$channel->exchange_declare($this->exchange_name, $this->exchange_type, false, true, false);
		foreach ($messages as &$msg) {
			$body = $msg->get_amqp_message();
			$routing_key = $msg->get_routing_key();;
			$channel->basic_publish($body, $this->exchange_name, $routing_key);
		}
		$channel->close();
		$connection->close();
	}

	function flush() {
		if (dwho_report::has('error') === false) {
			$queue = new MessageQueue();
			$this->publish_messages($queue->get_messages());
		}
		$queue->clear();
	}

}

class BusMessage {

	private $routing_key = null;
	private $amqp_message = null;

	function __construct($amqp_message, $routing_key) {
		$this->amqp_message = $amqp_message;
		$this->routing_key = $routing_key;
	}

	function get_routing_key() {
		return $this->routing_key;
	}

	function get_amqp_message() {
		return $this->amqp_message;
	}
}


class MessageFactory {

	private $xivo_uuid = null;
	private $msg_params = null;

	function __construct() {
		$this->xivo_uuid = getenv('XIVO_UUID');
		$this->msg_params = array('content_type' => 'application/json');
	}

	function new_user_line_associated_msg($user_id, $line_id) {
		$body = json_encode(array(
			'name' => 'line_associated',
			'origin_uuid' => $this->xivo_uuid,
			'data' => array(
				'user_id' => $user_id,
				'line_id' => $line_id,
				'main_line' => true,
				'main_user' => true)));

		$amqp_message = new AMQPMessage($body, $this->msg_params);
		$routing_key = 'config.user_line_association.created';

		return new BusMessage($amqp_message, $routing_key);
	}

}

?>
