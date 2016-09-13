<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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

$form = &$this->get_module('form');
$url = &$this->get_module('url');

$info = $this->get_var('info');
$element = $this->get_var('element');

?>
<div id="sb-part-first" class="b-nodisplay">
<?php
	function format_text($tpl, $form, $info, $field) {
		$value = isset($info['phonebook'][$field]) ? $info['phonebook'][$field] : '';
		$params = array('desc' => $tpl->bbf('fm_phonebook_'.$field),
						'name' => 'phonebook['.$field.']',
						'labelid' => 'phonebook-'.$field,
						'size' => 15,
						'value' => $value);
		return($form->text($params));
	}

	function format_number_text($tpl, $form, $type) {
		$params = array('desc' => $tpl->bbf('fm_phonebooknumber_'.$type),
						'name' => 'phonebooknumber['.$type.']',
						'labelid' => 'phonebooknumber-'.$type,
						'size' => 15,
						'value' => $tpl->get_var('phonebooknumber',$type));
		return($form->text($params));
	}

	function format_address_text($tpl, $form, $type, $field) {
		$params = array('desc' => $tpl->bbf('fm_phonebookaddress_'.$field),
						'name' => 'phonebookaddress['.$type.']['.$field.']',
						'labelid' => 'phonebookaddress-'.$type.'-'.$field,
						'size' => 15,
						'value' => $tpl->get_var('phonebookaddress',$type,$field));
		return($form->text($params));
	}

	$predefined_fields = array('title', 'firstname', 'lastname', 'displayname', 'society', 'email', 'url');
	foreach($predefined_fields as $field) {
		echo format_text($this, $form, $info, $field);
	}
	echo format_number_text($this, $form, 'mobile');

?>
</div>

<div id="sb-part-office" class="b-nodisplay">
<?php
	$this->file_include('bloc/service/ipbx/asterisk/pbx_services/phonebook/type',
						array('type' => 'office'));
?>
</div>

<div id="sb-part-home" class="b-nodisplay">
<?php
	$this->file_include('bloc/service/ipbx/asterisk/pbx_services/phonebook/type',
						array('type' => 'home'));
?>
</div>

<div id="sb-part-last" class="b-nodisplay">
<?php
	$this->file_include('bloc/service/ipbx/asterisk/pbx_services/phonebook/type',
						array('type' => 'other'));
?>
</div>
