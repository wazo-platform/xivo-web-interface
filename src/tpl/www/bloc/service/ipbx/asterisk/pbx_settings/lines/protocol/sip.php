<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016 Avencall
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

function build_row($option, $form, $url, $helper) {
	$row = '<tr class="fm-paragraph"><td>';
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 30,
			'name' => 'options[optionname][]',
			'value' => $option[0],
			'class' => 'sip-option-name',
		)
	);

	$row .= "</td><td>";
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 30,
			'name' => 'options[optionvalue][]',
			'value' => $option[1],
			'class' => 'sip-option-value',
		)
	);

	$row .= '</td><td class="td-right">';
	$row .= $url->href_html(
		$url->img_html(
			'img/site/button/mini/blue/delete.gif',
			$helper->bbf('opt_line-sip-option-delete'),
			'border="0"'
		),
		'#',
		null,
		null,
		$helper->bbf('opt_line-sip-option-delete'),
		false,
		'&amp;',
		true,
		true,
		true,
		true,
		'sip-option-remove'
	);

	$row .= "</td></tr>";

	return $row;
}

?>

<div id="sb-part-first" class="b-nodisplay">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_protocol_name'),
				  'name'	   => 'protocol[name]',
				  'labelid'	 => 'protocol-name',
				  'size'	   => 15,
				  'readonly' => $this->get_var('element','protocol','name','readonly'),
				  'class'    => $this->get_var('element','protocol','name','class'),
				  'default'  => $this->get_var('element','protocol','name','default'),
				  'value'	   => $info['protocol']['name'],
				  'error'	   => $this->bbf_args('error',$this->get_var('error', 'protocol', 'name')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_secret'),
				  'name'	=> 'protocol[secret]',
				  'labelid'	=> 'protocol-secret',
				  'size'	=> 15,
				  'readonly' => $this->get_var('element','protocol','secret','readonly'),
				  'class'    => $this->get_var('element','protocol','secret','class'),
				  'default'	=> $this->get_var('element', 'protocol', 'secret', 'default'),
				  'value'	=> $this->get_var('info','protocol','secret'),
				  'error'	=> $this->bbf_args('error',$this->get_var('error', 'protocol', 'secret')) ));

	if($context_list !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_context'),
					    'name'		=> 'protocol[context]',
					    'labelid'	=> 'protocol-context',
						'class'    	=> $this->get_var('act') == 'add' ? '' : 'it-disabled',
						'disabled'	=> $this->get_var('act') == 'add' ? false : true,
					    'key'		=> 'identity',
					    'altkey'	=> 'name',
					    'selected'	=> $context),
				      $context_list);
	else:
		echo	'<div id="fd-protocol-context" class="txt-center">',
			$url->href_htmln($this->bbf('create_context'),
					'service/ipbx/system_management/context',
					'act=add'),
			'</div>';
	endif;

	echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_language'),
				    'name'	=> 'protocol[language]',
				    'labelid'	=> 'protocol-language',
				    'empty'	=> true,
				    'key'	=> false,
				    'default'	=> $element['protocol']['sip']['language']['default'],
				    'selected'	=> $this->get_var('info','protocol','language')),
			      $element['protocol']['sip']['language']['value']),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_callerid'),
				    'name'	=> 'protocol[callerid]',
				    'labelid'	=> 'protocol-callerid',
				    'value'	=> $this->get_var('info','protocol','callerid'),
				    'size'	=> 15,
				    'notag'	=> false,
				    'error'	=> $this->bbf_args('error', $this->get_var('error', 'protocol', 'callerid')) )),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_nat'),
				    'name'	=> 'protocol[nat]',
				    'labelid'	=> 'sip-protocol-nat',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_nat-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['nat']['default'],
				    'selected'	=> $this->get_var('info','protocol','nat')),
			      $element['protocol']['sip']['nat']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_dtmfmode'),
				    'name'		=> 'protocol[dtmfmode]',
				    'labelid'	=> 'sip-protocol-dtmfmode',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_protocol_dtmfmode-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['dtmfmode']['default'],
				    'selected'	=> $this->get_var('info','protocol','dtmfmode')),
			      $element['protocol']['sip']['dtmfmode']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_qualify'),
				    'name'		=> 'protocol[qualify]',
				    'labelid'	=> 'sip-protocol-qualify',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_protocol_qualify-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['qualify']['default'],
				    'selected'	=> $qualify),
			      $element['protocol']['sip']['qualify']['value']);

?>

<fieldset id="fld-codeclist">
	<legend><?=$this->bbf('fld-codeclist');?></legend>
<?php
	echo	$form->checkbox(array('desc'	=> $this->bbf('fm_codec-custom'),
							'name'	=> 'codec-active',
							'labelid'	=> 'codec-active',
							'checked'	=> $codec_active));
?>
<div id="codeclist">
<?php
	echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_codec-disallow'),
							'name'		=> 'protocol[disallow]',
							'labelid'	=> 'protocol-disallow',
							'key'		=> false,
							'bbf'		=> 'fm_protocol_codec-disallow-opt',
							'bbfopt'	=> array('argmode' => 'paramvalue')),
					$element['protocol']['sip']['disallow']['value']);
?>
	<div class="fm-paragraph fm-description">
		<?=$form->jq_select(array('paragraph'	=> false,
							'label'		=> false,
							'name'		=> 'protocol[allow][]',
							'id' 		=> 'it-protocol-allow',
							'key'		=> false,
							'bbf'		=> 'ast_codec_name_type',
							'bbfopt'	=> array('argmode' => 'paramvalue'),
							'selected'  => $allow),
					$element['protocol']['sip']['allow']['value']);?>
	<div class="clearboth"></div>
	</div>
</div>
</fieldset>

</div>

<div id="sb-part-advanced" class="b-nodisplay">
	<div class="sb-list">
		<table>
			<thead>
				<th class="th-left">
					<?= $this->bbf('col_line-sip-option-name') ?>
				</th>
				<th class="th-center">
					<?= $this->bbf('col_line-sip-option-value') ?>
				</th>
				<th class="th-right">
					<?= $url->href_html(
							$url->img_html(
								'img/site/button/mini/orange/bo-add.gif',
								$this->bbf('col_line-sip-option-add'),
								'border="0"'),
							'#',
							null,
							'id="sip-option-add"',
							$this->bbf('col_line-sip-option-add')
						);
					?>
				</th>
			</thead>
			<tbody id="sip-options">
				<?php foreach($sip_options as $option_row): ?>
					<?= build_row($option_row, $form, $url, $this) ?>
				<?php endforeach ?>
			</tbody>
		</table>

		<script type='text/javascript'>
		var optionRow = <?= dwho_json::encode(build_row(
			array('', ''),
			$form, $url, $this))
		?>;

		function attachEvents(row) {
			remove = row.find(".sip-option-remove");
			remove.click(function(e) {
				e.preventDefault();
				row.detach();
			});
		}

		$(function() {
			$("#sip-option-add").click(function(e) {
				e.preventDefault();
				$("#sip-options").append(optionRow);
				row = $("#sip-options tr:last");
				attachEvents(row);
			});

			$("#sip-options tr").each(function(pos, row) {
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
</div>
