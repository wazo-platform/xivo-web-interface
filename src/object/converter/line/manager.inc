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

class LineManager
{
	private $line_facade = null;
	private $line = null;
	private $extension = null;
	private $user = null;
	private $mapper = null;
	private $context_dso = null;

	function __construct($line_facade, $line_resource, $user_resource, $extension_resource, $line_mapper, $context_dso) {
		$this->line_facade = $line_facade;
		$this->line = $line_resource;
		$this->user = $user_resource;
		$this->extension = $extension_resource;
		$this->mapper = $line_mapper;
		$this->context_dso = $context_dso;
	}

	public function create($lines)
	{
		$created_lines = array();
		$line_devices = array();

		foreach($lines as $form) {
			$created_line = $this->create_line($form);
			$created_lines[] = $created_line;

			if($form['device'] !== '') {
				$line_devices[] = array(
					'line_id' => $created_line['id'],
					'device_id' => $form['device']
				);
			}
		}

		return array(
			'linefeatures' => $created_lines,
			'line_device' => $line_devices
		);
	}

	public function update($lines)
	{
		$updated_lines = array();
		$line_devices = array();

		foreach($lines as $form) {
			$updated_line = $this->update_line($form);
			$updated_lines[] = $updated_line;

			$line_devices[] = array(
				'line_id' => $updated_line['id'],
				'device_id' => $form['device']
			);
		}

		return array(
			'linefeatures' => $updated_lines,
			'line_device' => $line_devices
		);
	}

	private function create_line($form) {
		$protocol = $form['protocol'];

		$mapped_line = $this->mapper->map_form($form);
		switch ($protocol) {
			case 'sip':
			case 'sccp':
				$endpoint = array();
				break;
			case 'custom':
				$endpoint = $this->generate_custom();
				break;
			default:
				throw new ClientException(array('Unknown endpoint protocol: ' . $protocol));
		}

		$line = $this->line_facade->create($mapped_line, $endpoint, $protocol);
		if ($line === null) {
			throw new ClientException(array('Could not create line'));
		}

		$line['line']['protocol'] = $protocol;
		return $this->mapper->map_line($line['line'], $line['endpoint']);
	}

	private function update_line($form) {
		$protocol = $form['protocol'];

		$line_id = $form['id'];
		$mapped_line = $this->mapper->map_form($form);
		$this->line->update($mapped_line);

		return $this->get($line_id);
	}

	private function generate_custom()
	{
		return array(
			'interface' => substr(md5(microtime()), rand(0,26), 8)
		);
	}

	function associate_devices($line_devices)
	{
		foreach($line_devices as $line_device) {
			$this->update_device_association($line_device);
		}
	}

	function update_device_association($line_device)
	{
		$association = $this->line->get_device_association($line_device['line_id']);
		if($association === null) {
			if($line_device['device_id'] != "") {
				$this->line->associate_device(
					$line_device['line_id'],
					$line_device['device_id']
				);
			}
		} else {
			if($line_device['device_id'] == "") {
				$this->line->dissociate_device(
					$association['line_id'],
					$association['device_id']
				);
			} else if ($association['device_id'] != $line_device['device_id']) {
					$this->line->dissociate_device(
						$association['line_id'],
						$association['device_id']
					);
					$this->line->associate_device(
						$line_device['line_id'],
						$line_device['device_id']
					);
			}
		}
	}

	function delete($lines)
	{
		foreach($lines as $line) {
			$this->delete_line($line);
		}
	}

	private function delete_line($line)
	{
		$this->line_facade->delete($line['id']);
	}

	function get($line_id)
	{
		$line = $this->line_facade->get($line_id);
		if ($line === null) {
			throw new ClientException(array('Could not retrieve line'));
		}
		return $this->get_legacy_line($line['line'], $line['endpoint']);
	}

	private function get_legacy_line($line, $endpoint)
	{
		$extension = $this->get_internal_extension($line);
		return $this->mapper->map_line($line, $endpoint, $extension);
	}

	private function get_internal_extension($line) {
		$associations = $this->line->list_extension_associations($line['id']);
		foreach($associations as $association) {
			$extension = $this->extension->get($association['extension_id']);
			if($this->is_internal_extension($extension)) {
				return $extension;
			}
		}
		return null;
	}

	private function is_internal_extension($extension) {
		$contexts = $this->context_dso->get_all(null, null, null, false, 'internal');
		foreach($contexts as $context) {
			if($context['name'] == $extension['context']) {
				return true;
			}
		}
		return false;
	}

	function update_caller_id($user_id)
	{
		$this->user->update(array('id' => $user_id));
	}

	function associate_extension($line_id, $extension_id)
	{
		$associations = $this->line->list_extension_associations($line_id);

		if(empty($associations)) {
			$this->line->associate_extension($line_id, $extension_id);
		} else {
			foreach($associations as $association) {
				if ($association['extension_id'] == $extension_id) {
					return;
				}
			}

			foreach($associations as $association) {
				$this->line->dissociate_extension(
					$association['line_id'],
					$association['extension_id']
				);
			}
			$this->line->associate_extension($line_id, $extension_id);
		}
	}

	function associate_user($line_id, $user_id)
	{
		$this->line->associate_user($line_id, $user_id);
	}

	public function list_extension_associations($line_id)
	{
		return $this->line->list_extension_associations($line_id);
	}

}

?>
