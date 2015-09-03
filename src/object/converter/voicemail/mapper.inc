<?php
#
# XiVO Web-Interface
# Copyright (C) 2015  Avencall
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

class VoicemailMapper {

	public function map_form($form) {
		$options = array();
		if(isset($form['optionname'])) {
			for($i = 0; $i < count($form['optionname']); $i++) {
				$name = $form['optionname'][$i];
				$value = $form['optionvalue'][$i];
				if(trim($name) !== '' && trim($value) !== '') {
					$options[] = array($name, $value);
				}
			}
		}

		$voicemail =  array(
			'name' => $form['name'],
			'number' => $form['number'],
			'context' => $form['context'],
			'password' => (trim($form['password']) !== '' ? $form['password'] : null),
			'email' => (trim($form['email']) !== '' ? $form['email'] : null),
			'language' => (trim($form['language']) !== '' ? $form['language'] : null),
			'pager' => trim($form['pager']) !== '' ? $form['pager'] : null,
			'max_messages' => ((int) $form['max_messages'] > 0 ? (int) $form['max_messages'] : null),
			'ask_password' => (string) $form['ask_password'] === "1" ? true : false,
			'attach_audio' => $form['attach_audio'] === "" ? null : $form['attach_audio'] === "1",
			'delete_messages' => (string) $form['delete_messages'] === "1" ? true : false,
			'options' => $options
		);

		return $voicemail;
	}

	private function map_legacy($voicemail) 
	{
		return array(
			'number' => $voicemail['mailbox'],
			'context' => $voicemail['context'],
			'email' => 
				isset($voicemail['email']) ? $voicemail['email'] : null,
			'password' => 
				isset($voicemail['password']) ? $voicemail['password'] : null,
			'language' => 
				isset($voicemail['language']) ? $voicemail['language'] : null,
			'timezone' => 
				isset($voicemail['tz']) ? $voicemail['tz'] : null,
			'attach_audio' => 
				isset($voicemail['attach']) ? (bool) ((int) $voicemail['attach']) : null,
			'delete_messages' => 
				isset($voicemail['deletevoicemail']) ? (bool) ((int) $voicemail['deletevoicemail']) : null,
			'ask_password' => 
				isset($voicemail['skipcheckpass']) ? !((bool) ((int) $voicemail['skipcheckpass'])) : null,
			'max_messages' => 
				isset($voicemail['maxmsg']) ? ((int) $voicemail['maxmsg']) : null
		);
	}

}

?>
