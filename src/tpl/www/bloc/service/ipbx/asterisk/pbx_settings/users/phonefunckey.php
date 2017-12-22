<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2015  Avencall
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
dwho::load_class('dwho_json');

$url = &$this->get_module('url');
$dhtml = &$this->get_module('dhtml');

$funckeys = $this->get_var('fkidentity_list');
$fktypes = $this->get_var('fktype_list');

$supervisable = array();
foreach($fktypes as $fktype => $info) {
	if(array_key_exists('supervisable', $info) && $info['supervisable'] === true) {
		$supervisable[] = $fktype;
	}
}

function build_bsfilter($helper, $funckey) {
	$bsfilters = $helper->get_var('bsfilter_list');
	$html = 'class="fkbsfilter form-control" style="display: none;"';

	if(is_array($bsfilters) && count($bsfilters) >= 1) {
		$form = &$helper->get_module('form');
		$options = array(
			'paragraph' => false,
			'label' => false,
			'key' => 'callfilteridentity',
			'altkey' => 'id',
			'selected' => $funckey['typeval'],
			'name' => 'phonefunckey[fkbsfilter][]'
		);
		return $form->select($options, $bsfilters, $html);
	}

	$url = &$helper->get_module('url');
	return $url->href_htmln($helper->bbf('create_callfilter'),
		'service/ipbx/call_management/callfilter',
		'act=add',
		$html
	);
}

function build_row($helper, $funckey) {
	$row = "";

	$form = &$helper->get_module('form');
	$url = &$helper->get_module('url');
	$fktype_options = $helper->get_var('fktype_list');

	$defaults = array(
		'paragraph' => false,
		'label' => false,
		'key' => false
	);

	$fknum = array_merge($defaults, array(
		'name' => 'phonefunckey[fknum][]',
		'default' => '1',
		'selected' => $funckey['fknum']
	));
	$fknum_options = array_combine(range(1, 250), range(1, 250));

	$fktype = array_merge($defaults, array(
		'name' => 'phonefunckey[type][]',
		'key' => 'name',
		'altkey' => 'name',
		'selected' => $funckey['type'],
		'bbf' => 'fm_phonefunckey_type-opt',
		'bbfopt' => array('argmode' => 'paramvalue'),
		'optgroup' => array(
			'key'	=> 'category',
			'unique'	=> true,
			'bbf'	=> 'fm_phonefunckey_type-optgroup',
			'bbfopt'	=> array('argmode' => 'paramvalue')
		)));

	$fkidentity = array_merge($defaults, array(
		'name' => 'phonefunckey[typevalidentity][]',
		'size' => 20,
		'value' => $funckey['identity']['identity']
	));

	$fktypeval = array(
		'name' => 'phonefunckey[typeval][]',
		'value' => $funckey['typeval']
	);

	$fklabel = array_merge($defaults, array(
		'name' => 'phonefunckey[label][]',
		'size' => 10,
		'default' => $funckey['label']
	));

	$fksupervision = array_merge($defaults, array(
		'name' => 'phonefunckey[supervision][]',
		'class' => 'it-enabled form-control',
		'default' => '1',
		'selected' => $funckey['supervision'],
		'bbf' => 'fm_phonefunckey_supervision-opt',
		'bbfopt' => array('argmode' => 'paramvalue')
	));
	$fksupervision_options = array('Enabled' => '1', 'Disabled' => '0');

	$row .= '<tr class="fm-paragraph"><td>';
	$row .= $form->select($fknum, $fknum_options);

	$row .= "</td><td>";
	$row .= $form->select($fktype, $fktype_options);

	$row .= "</td><td>";
	$row .= $form->text($fkidentity);
	$row .= $form->hidden($fktypeval);
	$row .= build_bsfilter($helper, $funckey);

	$row .= "</td><td>";
	$row .= $form->text($fklabel);

	$row .= "</td><td>";
	$row .= $form->select($fksupervision, $fksupervision_options);

	$row .= '</td><td class="td-right">';
	$row .= $url->href_html(
		$url->img_html('img/site/button/mini/blue/delete.gif',
		$helper->bbf('opt_phonefunckey-delete'),
		'border="0"'),
		'#',
		null,
		null,
		$helper->bbf('opt_phonefunckey-delete'),
		false,
		'&amp;',
		true,
		true,
		true,
		true,
		'fkdelete');

	$row .= "</td></tr>";

	return $row;
}

$funckey_row = build_row($this, array('fknum' => 1,
									  'type' => 'user',
									  'typeval' => '',
									  'label' => '',
									  'identity' => array('identity' => ''),
									  'supervision' => '1'));

$dhtml->write_js('var xivo_fk_row = ' . dwho_json::encode($funckey_row) . ';');
$dhtml->write_js('var xivo_fk_supervision = '. dwho_json::encode($supervisable) . ';');

?>
<div class="sb-list">
<table class="table">
	<thead>
	<tr class="sb-top">
		<th class="th-left"><?=$this->bbf('col_phonefunckey-fknum');?></th>
		<th class="th-center"><?=$this->bbf('col_phonefunckey-type');?></th>
		<th class="th-center"><?=$this->bbf('col_phonefunckey-typeval');?></th>
		<th class="th-center"><?=$this->bbf('col_phonefunckey-label');?></th>
		<th class="th-center"><?=$this->bbf('col_phonefunckey-supervision');?></th>
		<th class="th-right"><?=$url->href_html($url->img_html('img/site/button/mini/orange/bo-add.gif',
								       $this->bbf('col_phonefunckey-add'),
								       'border="0"'),
							'#',
							null,
							'id="add_funckey_button"',
							$this->bbf('col_phonefunckey-add'));?></th>
	</tr>
	</thead>
	<tbody id="phonefunckey">
	<?php
		foreach($funckeys as $funckey) {
			echo build_row($this, $funckey);
		}
	?>
	</tbody>
	<?php if(count($funckeys) == 0): ?>
       <tfoot>
       <tr id="no-phonefunckey">
               <td colspan="6" class="td-single"><?=$this->bbf('no_phonefunckey');?></td>
       </tr>
	   </tfoot>
	<?php endif ?>
</table>
</div>
