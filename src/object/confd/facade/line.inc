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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'extension', 'extension.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'facade', 'endpoint.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'line', 'line.inc'));

class LineFacade
{
	private $_line = null;
	private $_endpoint = null;
	private $_extension = null;

	function __construct($client)
	{
		// $client must not be configured to raise exception
		$this->_line = new LineResource($client);
		$this->_endpoint = new EndpointFacade($client);
		$this->_extension = new ExtensionResource($client);
	}

	public function get($line_id)
	{
		$line = $this->_line->get($line_id);
		if ($line === null || $line['protocol'] === null) {
			return null;
		}

		$protocol = $line['protocol'];
		$association = $this->_line->get_endpoint_association($line_id, $protocol);
		if ($association === null) {
			return null;
		}

		$endpoint = $this->_endpoint->get($association['endpoint_id'], $protocol);
		if ($endpoint === null) {
			return null;
		}

		return array('line' => $line, 'endpoint' => $endpoint);
	}

	public function create($line_args, $endpoint_args, $protocol)
	{
		$line = null;
		$endpoint = null;

		# caller_id_name and caller_id_num can't be passed during line creation
		unset($line_args['caller_id_name']);
		unset($line_args['caller_id_num']);
		$line = $this->_line->create($line_args);
		if ($line === null) {
			goto error;
		}

		$endpoint = $this->_endpoint->create($endpoint_args, $protocol);
		if ($endpoint === null) {
			goto error;
		}

		$associated = $this->_line->associate_endpoint($line['id'], $endpoint['id'], $protocol);
		if ($associated === false) {
			goto error;
		}

		return array('line' => $line, 'endpoint' => $endpoint);

error:
		if (!empty($endpoint)) {
			$this->_endpoint->delete($endpoint['id'], $protocol);
		}
		if (!empty($line)) {
			$this->_line->delete($line['id']);
		}

		return null;
	}

	public function delete($line_id)
	{
		$this->_dissociate_device($line_id);
		// dissociate the users before the extensions so that the incall extensions
		// are not listed in the line/extensions association
		$this->_dissociate_users($line_id);
		$this->_dissociate_and_delete_extensions($line_id);
		// deleting a line via confd also deletes the associated endpoint
		$this->_line->delete($line_id);
	}

	private function _dissociate_device($line_id)
	{
		$association = $this->_line->get_device_association($line_id);
		if ($association !== null) {
			$this->_line->dissociate_device($line_id, $association['device_id']);
		}
	}

	private function _dissociate_users($line_id)
	{
		$associations = $this->_line->list_user_associations($line_id);
		foreach($associations as $userline) {
			$this->_line->dissociate_user($line_id, $userline['user_id']);
		}
	}

	private function _dissociate_and_delete_extensions($line_id)
	{
		$associations = $this->_line->list_extension_associations($line_id);
		foreach($associations as $association) {
			$this->_line->dissociate_extension($line_id, $association['extension_id']);
			if ($this->_extension->list_line_associations($association['extension_id']) === null){
				$this->_extension->delete($association['extension_id']);
			}
		}
	}

	public function update_endpoint($endpoint_id, $endpoint, $protocol)
	{
		return $this->_endpoint->update($endpoint_id, $endpoint, $protocol);
	}
}
