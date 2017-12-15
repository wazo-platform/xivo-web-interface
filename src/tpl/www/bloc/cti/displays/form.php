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

$element = $this->get_var('element');
$info = $this->get_var('info');

$data = $this->get_var('data');
$urilist = $this->get_var('urilist');
$presence = $this->get_var('displays');

function build_row($data, $form, $url, $helper) {
	$row = '<tr class="fm-paragraph"><td>';
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 15,
			'name' => 'dispcol1[]',
			'value' => $data[0],
		)
	);

	$row .= "</td><td>";
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 15,
			'name' => 'dispcol2[]',
			'value' => $data[1],
			'class' => 'field-type-name',
		)
	);

	$row .= "</td><td>";
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 15,
			'name' => 'dispcol3[]',
			'value' => $data[2],
		)
	);

	$row .= "</td><td>";
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 15,
			'name' => 'dispcol4[]',
			'value' => $data[3],
		)
	);
	$row .= '</td><td class="td-right">';
	$row .= $url->href_html(
		$url->img_html(
			'img/site/button/mini/blue/delete.gif',
			$helper->bbf('opt_disp-delete'),
			'border="0"'
		),
		'#',
		null,
		null,
		$helper->bbf('opt_disp-delete'),
		false,
		'&amp;',
		true,
		true,
		true,
		true,
		'display-filter-remove'
	);

	$row .= "</td></tr>";

	return $row;
}

?>

<div id="sb-part-first">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_displays_name'),
				  'name'	=> 'displays[name]',
				  'labelid'	=> 'displays-name',
				  'size'	=> 15,
				  'default'	=> $element['displays']['name']['default'],
				  'value'	=> $info['displays']['name']));

?>
	<p>&nbsp;</p>
	<div class="sb-list">
		<table class="table">
			<thead>
			<tr class="sb-top">

				<th class="th-left"><?=$this->bbf('col_1');?></th>
				<th class="th-center"><?=$this->bbf('col_2');?></th>
				<th class="th-center"><?=$this->bbf('col_3');?></th>
				<th class="th-center"><?=$this->bbf('col_4');?></th>
				<th class="th-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/orange/bo-add.gif',
									  $this->bbf('col_add'),
									  'border="0"'),
							   '#',
							   null,
							   'id="display-filter-add"',
							   $this->bbf('col_add'));?>
				</th>
			</tr>
			</thead>
			<tbody id="disp">
					<?php foreach($data as $data_row): ?>
						<?= build_row($data_row, $form, $url, $this) ?>
					<?php endforeach ?>
			</tbody>
		</table>

<script type='text/javascript'>

var displayFilterRow = <?= dwho_json::encode(build_row(array('', '', '', ''),
												$form, $url, $this)) ?>;

var fieldTypes = [
		"agent",
		"callable",
		"email",
		"favorite",
		"name",
		"number",
		"personal",
		"voicemail",
];

function attachEvents(row) {
	option = row.find(".field-type-name");
    option.autocomplete({
      source: fieldTypes,
      minLength: 0
    });
    option.focus(function() {
        $(this).autocomplete("search", "");
    });

	remove = row.find(".display-filter-remove");
	remove.click(function(e) {
		e.preventDefault();
		row.detach();
	});
}

$(function() {
	$("#display-filter-add").click(function(e) {
		e.preventDefault();
		$("#disp").append(displayFilterRow);
		row = $("#disp tr:last");
		attachEvents(row);
	});

	$("#disp tr").each(function(pos, row) {
		attachEvents($(row));
	});
});

</script>
<style>
.ui-autocomplete {
	max-height: 200px;
	overflow-y: auto;
	overflow-x: hidden;
}
</style>
	</div>
<br />
<div class="col-sm-offset-2 fm-paragraph fm-description">
	<p>
		<label id="lb-description" for="it-description"><?=$this->bbf('fm_description');?></label>
	</p>
	<?=$form->textarea(array('paragraph'    => false,
				 'label'    => false,
				 'name'     => 'displays[description]',
				 'id'       => 'it-description',
				 'cols'     => 60,
				 'rows'     => 5,
				 'default'  => $element['displays']['description']['default']),
			   $info['displays']['description']);?>
</div>

</div>

<div class="col-sm-offset-2 fm-paragraph fm-description"><p><?=$this->bbf('need-xivo-dird-restart');?></p></div>
