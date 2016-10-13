<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016 Avencall
# Copyright (C) 2016 Proformatique
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

function format_extrafield_text($form, $name, $value) {
	$params = array('paragraph' => false,
					'label' => false,
					'key' => false,
					'size' => 30,
					'name' => 'extrafields['.$name.'][]',
					'value' => $value);
	return($form->text($params));
}

function build_row($name, $value, $form, $url, $helper) {
	$row = '<tr class="fm-paragraph"><td>';
	$row .= format_extrafield_text($form, 'name', $name);
	$row .= "</td><td>";
	$row .= format_extrafield_text($form, 'value', $value);
	$row .= '</td><td class="td-right">';
	$row .= $url->href_html(
		$url->img_html(
			'img/site/button/mini/blue/delete.gif',
			$helper->bbf('opt_phonebook-option-delete'),
			'border="0"'
		),
		'#', null, null, $helper->bbf('opt_phonebook-option-delete'), false, '&amp;', true,
		true, true,	true, 'phonebook-option-remove');
	$row .= "</td></tr>";

	return($row);
}

$predefined_fields = array('title', 'firstname', 'lastname', 'displayname', 'society', 'email', 'url');
$hidden_fields = array('id');
$extra_fields = array();
if(isset($info['phonebook']) === true) {
	foreach($info['phonebook'] as $name => $value) {
		if(empty($name) === false
			&& in_array($name, $predefined_fields) === false
			&& in_array($name, $hidden_fields) === false
			&& empty($value) === false) {
				$extra_fields[$name] = $value;
		}
	}
}
?>

<script type='text/javascript'>
var optionRow = <?=dwho_json::encode(build_row('', '', $form, $url, $this))?>;

function attachEvents(row) {
	remove = row.find(".phonebook-option-remove");
	remove.click(function(e) {
		e.preventDefault();
		row.detach();
	});
}

$(function() {
	$("#phonebook-option-add").click(function(e) {
		e.preventDefault();
		$("#phonebook-options").append(optionRow);
		row = $("#phonebook-options tr:last");
		attachEvents(row);
	});

	$("#phonebook-options tr").each(function(pos, row) {
		attachEvents($(row));
	});
});
</script>

<div id="sb-part-first" class="b-nodisplay">
<?php
	foreach($predefined_fields as $field) {
		echo format_text($this, $form, $info, $field);
	}
	echo format_number_text($this, $form, 'mobile');
?>

	<div class="sb-list">
	<fieldset>
	<legend><?= $this->bbf('phonebook-extra-fields') ?></legend>
		<table>
			<thead>
				<th class="th-left">
					<?= $this->bbf('col_phonebook-name') ?>
				</th>
				<th class="th-center">
					<?= $this->bbf('col_phonebook-value') ?>
				</th>
				<th class="th-right">
					<?= $url->href_html(
							$url->img_html(
								'img/site/button/mini/orange/bo-add.gif',
								$this->bbf('col_phonebook-option-add'),
								'border="0"'),
							'#',
							null,
							'id="phonebook-option-add"',
							$this->bbf('col_phonebook-option-add')
						);
					?>
				</th>
			</thead>
			<tbody id="phonebook-options">
<?php
	foreach($extra_fields as $name => $value) {
		echo build_row($name, $value, $form, $url, $this);
	}
?>
			</tbody>
		</table>
		</fieldset>
	</div>
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
