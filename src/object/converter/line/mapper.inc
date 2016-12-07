<?php
#
# XiVO Web-Interface
# Copyright 2016 The Wazo Authors  (see the AUTHORS file)
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


class LineMapper {

	function map_form($form)
	{
		$line = array(
			'protocol' => $form['protocol'],
			'context' => $form['context'],
			'registrar' => $form['configregistrar'],
			'position' => $form['num']
		);
		if ($form['protocol'] === 'sip' || $form['protocol'] === 'sccp') {
			if (isset($form['caller_id_name'])) {
				$line['caller_id_name'] = $form['caller_id_name'];
			}
			if (isset($form['caller_id_num'])) {
				$line['caller_id_num'] = $form['caller_id_num'];
			}
		}

		if($form['id'] != "0") {
			$line['id'] = $form['id'];
		}

		return $line;
	}

	function map_line($line, $endpoint=null, $extension=null)
	{
		return array(
			'id' => $line['id'],
			'protocol' => $line['protocol'],
			'protocolid' => is_null($endpoint) ? null : $endpoint['id'],
			'device' => $line['device_id'],
			'name' => $this->determine_line_name($line, $endpoint, $extension),
			'number' => is_null($extension) ? null : $extension['exten'],
			'configregistrar' => $line['registrar'],
			'context' => $line['context'],
			'provisioningid' => (int) $line['provisioning_code'],
			'num' => $line['device_slot']
		);
	}

	private function determine_line_name($line, $endpoint, $extension)
	{
		if($line['protocol'] == 'sip' and !is_null($endpoint)) {
			return $endpoint['username'];
		} else if ($line['protocol'] == 'sccp' and !is_null($extension)) {
			return $extension['exten'];
		} else if ($line['protocol'] == 'custom') {
			return $endpoint['interface'];
		}
	}

}

?>
