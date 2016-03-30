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
	private $line = null;
	private $sip = null;
	private $sccp = null;
	private $extension = null;
	private $custom = null;
	private $user = null;
	private $mapper = null;
	private $context_dso = null;

	function __construct($line_resource, $user_resource, $sip_resource, $sccp_resource, $custom_resource, $extension_resource, $line_mapper, $context_dso) {
		$this->line = $line_resource;
		$this->user = $user_resource;
		$this->sip = $sip_resource;
		$this->sccp = $sccp_resource;
		$this->custom = $custom_resource;
		$this->extension = $extension_resource;
		$this->mapper = $line_mapper;
		$this->context_dso = $context_dso;
	}

	public function get_main_line($user_id)
	{
		$userlines = $this->user->list_line_associations($user_id);
		foreach($userlines as $userline) {
			if ($userline['main_line'] == true) {
				$line = $this->line->get($userline['line_id']);
				if($this->entity_allowed($line)) {
					return $this->get_legacy_line($line);
				}
			}
		}
		return null;
	}

	private function entity_allowed($line)
	{
		if(defined('ENTITY_FILTER_CONTEXTS')) {
			$contexts = explode(',', ENTITY_FILTER_CONTEXTS);
			if(!in_array($line['context'], $contexts)) {
				return false;
			}
		}
		return true;
	}

	public function create($user_id, $lines)
	{
		$created_lines = array();
		$line_devices = array();

		foreach($lines as $form) {
			$created_line = $this->create_line($user_id, $form);
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

	public function update($user_id, $lines)
	{
		$updated_lines = array();
		$line_devices = array();

		foreach($lines as $form) {
			$updated_line = $this->update_line($user_id, $form);
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

	private function create_line($user_id, $form) {
		$protocol = $form['protocol'];

		$mapped_line = $this->mapper->map_form($form);
		$line = $this->line->create($mapped_line);
		$endpoint = $this->create_and_associate_endpoint($line, $protocol);

		$line['protocol'] = $protocol;
		return $this->mapper->map_line($line, $endpoint);
	}

	private function update_line($user_id, $form) {
		$protocol = $form['protocol'];

		$line = $this->line->get($form['id']);
		$mapped_line = $this->mapper->map_form($form);
		$this->line->update($mapped_line);

		return $this->get($line['id']);
	}

	private function create_and_associate_endpoint($line, $protocol)
	{
		if($protocol == "sip") {
			$endpoint = $this->sip->create(array());
			$this->line->associate_sip($line['id'], $endpoint['id']);
		} else if ($protocol == "sccp") {
			$endpoint = $this->sccp->create(array());
			$this->line->associate_sccp($line['id'], $endpoint['id']);
		} else if ($protocol == "custom") {
			$custom = $this->generate_custom();
			$endpoint = $this->custom->create($custom);
			$this->line->associate_custom($line['id'], $endpoint['id']);
		} else {
			throw new ClientException(array('Unknown endpoint protocol: ' . $protocol));
		}
		return $endpoint;
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
		$this->dissociate_device($line);
		$this->dissociate_users($line);
		$this->dissociate_extensions($line);
		$this->delete_endpoint($line);
	}

	private function dissociate_device($line)
	{
		$association = $this->line->get_device_association($line['id']);
		if($association !== null) {
			return  $this->line->dissociate_device($line['id'], $association['device_id']);
		}
	}

	private function dissociate_users($line)
	{
		$associations = $this->line->list_user_associations($line['id']);
		foreach($associations as $userline) {
			$this->line->dissociate_user($line['id'], $userline['user_id']);
		}
	}

	private function dissociate_extensions($line)
	{
		$internal = array();
		$associations = $this->line->list_extension_associations($line['id']);

		foreach($associations as $association) {
			$extension = $this->extension->get($association['extension_id']);
			if($this->is_internal_extension($extension)) {
				$internal[] = $extension;
			} else {
				$this->line->dissociate_extension($line['id'], $extension['id']);
			}
		}

		foreach($internal as $extension) {
			$this->line->dissociate_extension($line['id'], $extension['id']);
		}
	}

	private function delete_endpoint($line)
	{
		$protocol = $line['protocol'];

		if($protocol == 'sip') {
			$association = $this->line->get_sip_association($line['id']);
			$this->line->dissociate_sip($line['id'], $association['endpoint_id']);
			return $this->sip->delete($association['endpoint_id']);
		} else if ($protocol == 'sccp') {
			$association = $this->line->get_sccp_association($line['id']);
			$this->line->dissociate_sccp($line['id'], $association['endpoint_id']);
			return $this->sccp->delete($association['endpoint_id']);
		} else if ($protocol == 'custom') {
			$association = $this->line->get_custom_association($line['id']);
			$this->line->dissociate_custom($line['id'], $association['endpoint_id']);
			return $this->custom->delete($association['endpoint_id']);
		} else {
			throw new ClientException(array('Unknown endpoint protocol: ' . $protocol));
		}
	}

	function get($line_id)
	{
		$line = $this->line->get($line_id);
		return $this->get_legacy_line($line);
	}

	private function get_legacy_line($line)
	{
		$endpoint = $this->get_endpoint($line);
		$extension = $this->get_internal_extension($line);
		return $this->mapper->map_line($line, $endpoint, $extension);
	}

	private function get_endpoint($line) {
		$protocol = $line['protocol'];

		if($protocol == "sip") {
			$association = $this->line->get_sip_association($line['id']);
			if($association !== null) {
				$sip = $this->sip->get($association['endpoint_id']);
				return $sip;
			}
		} else if ($protocol == "sccp") {
			$association = $this->line->get_sccp_association($line['id']);
			if($association !== null) {
				return $this->sccp->get($association['endpoint_id']);
			}
		} else if ($protocol == "custom") {
			$association = $this->line->get_custom_association($line['id']);
			if($association !== null) {
				return $this->custom->get($association['endpoint_id']);
			}
		}

		return null;
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

}

?>
