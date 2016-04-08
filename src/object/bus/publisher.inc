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
        require $file;
    }
});

use PhpAmqpLib\Connection\AMQPConnection;
use PhpAmqpLib\Message\AMQPMessage;

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

	static function default_config() {
		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT, 'bus', 'abstract', 'config');

		if(($config = dwho_gat::get_file($dir)) === false) {
			trigger_error("could not load default config for the bus publisher");
			return(false);
		}

		$host = $config['bus']['host'];
		$port = $config['bus']['port'];
		$username = $config['bus']['username'];
		$password = $config['bus']['password'];
		$exchange_name = $config['bus']['exchange_name'];
		return new Publisher($host, $port, $username, $password, $exchange_name);
	}

	function publish($msg) {
		$connection = new AMQPConnection($this->host, $this->port, $this->username, $this->password);
		$channel = $connection->channel();

		$channel->exchange_declare($this->exchange_name, $this->exchange_type, false, true, false);
		$channel->basic_publish($msg, $this->exchange_name);

		$channel->close();
		$connection->close();
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

		return new AMQPMessage($body, $this->msg_params);
	}

}

?>
