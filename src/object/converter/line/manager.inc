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
	private $_line = null;
	private $_linefeatures = null;
	private $_userapp = null;
	private $line = null;
	private $sip = null;
	private $sccp = null;
	private $extension = null;
	private $user = null;
	private $mapper = null;
	private $context_dso = null;

	function __construct($lineapp, $linefeatures, $userapp, $line_resource, $user_resource, $sip_resource, $sccp_resource, $extension_resource, $line_mapper, $context_dso) {
		$this->_line = $lineapp;
		$this->_linefeatures = $linefeatures;
		$this->_userapp = $userapp;
		$this->line = $line_resource;
		$this->user = $user_resource;
		$this->sip = $sip_resource;
		$this->sccp = $sccp_resource;
		$this->extension = $extension_resource;
		$this->mapper = $line_mapper;
		$this->context_dso = $context_dso;
	}

	public function get_user_masterline($user_id)
	{
		$associations = $this->user->list_line_associations($user_id);
		foreach($associations as $association) {
			if ($association['main_line'] == true) {
				$line = $this->line->get($association['line_id']);
				if($line !== null && $this->entity_allowed($line)) {
					return $this->get_legacy_line($line);
				}
			}
		}
		return false;
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

	function edit_linefeatures($user_id, $lines, $action)
	{
		if($action == "add") {
			return $this->create($user_id, $lines);
		} else {
			return $this->update($user_id, $lines);
		}
	}

	private function create($user_id, $lines)
	{
		$created_lines = array();
		$line_devices = array();

		foreach($lines as $form) {
			$created_line = $this->create_line($user_id, $form);
			if($created_line === null) {
				return false;
			}
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

	private function update($user_id, $lines)
	{
		$updated_lines = array();
		$line_devices = array();

		foreach($lines as $form) {
			$updated_line = $this->update_line($user_id, $form);
			if($updated_line === null) {
				return false;
			}
			$update_lines[] = $updated_line;

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

		if($line !== null) {
			$endpoint = $this->create_endpoint($protocol);
			if($endpoint !== null) {
				$associated = $this->associate_endpoint($line, $protocol, $endpoint);
				if($associated == true) {
					$line['protocol'] = $protocol;
					$legacy_line = $this->mapper->map_line($line, $endpoint);
					return $legacy_line;
				}
			}
		}

		return false;
	}

	private function update_line($user_id, $form) {
		$protocol = $form['protocol'];
		$line = $this->line->get($form['id']);
		if($line !== null) {
			$line = $this->mapper->map_form($form);
			$line['id'] = $form['id'];
			$updated = $this->line->update($line);
			if ($updated == true) {
				$line = $this->line->get($line['id']);
				return $this->get_legacy_line($line);
			}
		}
		return false;
	}

	private function create_endpoint($protocol)
	{
		if($protocol == "sip") {
			return $this->sip->create(array());
		} else {
			return $this->sccp->create(array());
		}
	}

	private function associate_endpoint($line, $protocol, $endpoint)
	{
		if($protocol == "sip") {
			return $this->line->associate_sip($line['id'], $endpoint['id']);
		} else {
			return $this->line->associate_sccp($line['id'], $endpoint['id']);
		}
	}

	function add_device_config($line_devices)
	{
		foreach($line_devices as $association) {
			if (!$this->line->associate_device($association['line_id'], $association['device_id'])) {
				return false;
			}
		}
	}

	function edit_device_config($line_devices)
	{
		foreach($line_devices as $line_device) {
			if(!$this->update_device_association($line_device)) {
				return false;
			}
		}
	}

	function update_device_association($line_device)
	{
		$association = $this->line->get_device_association($line_device['line_id']);
		if($association === null) {
			if($line_device['device_id'] != "") {
				return $this->line->associate_device(
					$line_device['line_id'],
					$line_device['device_id']
				);
			}
		} else {
			if($line_device['device_id'] == "") {
				return $this->line->dissociate_device(
					$association['line_id'],
					$association['device_id']
				);
			} else if ($association['device_id'] != $line_device['device_id']) {
				return
					$this->line->dissociate_device(
						$association['line_id'],
						$association['device_id']
					)
					&&
					$this->line->associate_device(
						$line_device['line_id'],
						$line_device['device_id']
					);
			}
		}
		return true;
	}

	function delete_linefeatures($lines)
	{
		foreach($lines as $line) {
			$deleted = $this->delete_line($line);
			if(!$deleted) {
				return false;
			}
		}
	}

	private function delete_line($line)
	{
		return $this->dissociate_device($line) && $this->delete_endpoint($line);
	}

	private function dissociate_device($line)
	{
		$association = $this->line->get_device_association($line['id']);
		if($association !== null) {
			return  $this->line->dissociate_device($line['id'], $association['device_id']);
		}
		return true;
	}

	private function delete_endpoint($line)
	{
		$protocol = $line['protocol'];

		if($protocol == 'sip') {
			$association = $this->line->get_sip_association($line['id']);
		} else {
			$association = $this->line->get_sccp_association($line['id']);
		}

		if($association !== null) {
			if($protocol == 'sip') {
				$dissociated = $this->line->dissociate_sip($line['id'], $association['endpoint_id']);
			} else {
				$dissociated = $this->line->dissociate_sccp($line['id'], $association['endpoint_id']);
			}

			if($dissociated === true) {
				if($protocol == 'sip') {
					return $this->sip->delete($endpoint['id']);
				} else {
					return $this->sccp->delete($endpoint['id']);
				}
			}
		}

		return false;
	}

	function get_linefeatures($line_id)
	{
		$line = $this->line->get($line_id);
		$line = $this->get_legacy_line($line);
		if ($line === null) {
			return array('info' => false, 'origin' => false);
		}
		return array('info' => array($line), 'origin' => false);
	}

	private function get_legacy_line($line)
	{
		$endpoint = $this->get_endpoint($line);
		$extension = $this->get_internal_extension($line);

		if ($line === null || $endpoint === null || $extension === null) {
			dwho_report::push('error', "Could not get Line (ID ${line['id']}) and its associations");
			return null;
		}

		return $this->mapper->map_line($line, $endpoint, $extension);
	}

	private function get_endpoint($line) {
		$protocol = $line['protocol'];

		if($protocol == "sip") {
			$association = $this->line->get_sip_association($line['id']);
		} else {
			$association = $this->line->get_sccp_association($line['id']);
		}

		if($association !== null) {
			if($protocol == "sip") {
				return $this->sip->get($association['endpoint_id']);
			} else {
				return $this->sccp->get($association['endpoint_id']);
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

	function reverse_add()
	{
		$this->_line->_status = 'add';
		$this->_line->_reverse_add();
	}

	function reverse_edit()
	{
		$this->_line->_status = 'edit';
		$this->_line->_reverse_edit();
	}

}


?>
