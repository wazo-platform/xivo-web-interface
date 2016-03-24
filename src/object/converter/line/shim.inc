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

	private $_line = null;
	private $manager = null;

	function __construct($lineapp, $manager)
	{
		$this->_line = $lineapp;
		$this->manager = $manager;
	}

	public function get_user_masterline($user_id)
	{
		try { 
			return $this->manager->get_main_line($user_id);
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
	}

	public function edit_linefeatures($user_id, $lines, $action)
	{
		try {
			if($action == "add") {
				return $this->manager->create($user_id, $lines);
			} else {
				return $this->manager->update($user_id, $lines);
			}
		} catch (ClientException $e) {
			$e->report_errors();
			return false;
		}
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
			$line = $this->manager->get($line_id);
			return array('info' => array($line), 'origin' => false);
		} catch (ClientException $e) {
			$e->report_errors();
			return array('info' => false, 'origin' => false);
		}
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
