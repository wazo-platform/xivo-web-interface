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

class LineShim {

	private $manager = null;

	function __construct($manager)
	{
		$this->manager = $manager;
	}

	public function edit_linefeatures($lines, $action)
	{
		try {
			$action = $this->determine_action($action, $lines);
			if($action == "add") {
				return $this->manager->create($lines);
			} else {
				return $this->manager->update($lines);
			}
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	private function determine_action($action, $lines)
	{
		if($action == "edit") {
			foreach($lines as $line) {
				if($line['id'] == "0") {
					$action = "add";
				}
			}
		}
		return $action;
	}

	public function delete_linefeatures($lines)
	{
		try {
			return $this->manager->delete($lines);
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	public function add_device_config($line_devices)
	{
		return $this->edit_device_config($line_devices);
	}

	public function edit_device_config($line_devices)
	{
		try { 
			return $this->manager->associate_devices($line_devices);
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	public function get_linefeatures($line_id)
	{
		try { 
			return $this->manager->get($line_id);
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	function reverse_add($lines)
	{
		try {
			$this->manager->delete($lines);
		} catch (ClientException $e) {}
	}

	function reverse_edit()
	{
	}

	function associate_extension($line_id, $extension_id)
	{
		try {
			return $this->manager->associate_extension($line_id, $extension_id);
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	function associate_user($line_id, $user_id)
	{
		try {
			return $this->manager->associate_user($line_id, $user_id);
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	function list_extension_associations($line_id)
	{
		try {
			return $this->manager->list_extension_associations($line_id);
		} catch (ClientException $e) {
			$e->report_errors();
			return array();
		}
	}

}
