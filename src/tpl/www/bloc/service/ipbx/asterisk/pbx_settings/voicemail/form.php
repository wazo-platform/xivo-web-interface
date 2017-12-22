<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2017  Avencall
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

$context_list = $this->get_var('context_list');

$vm_options = array();
$emailsubject = null;
$emailbody = null;

if($info !== null) {
	$vm_options = $info['voicemail']['options'];
}

foreach($vm_options as $pos => $option) {
	if($option[0] === 'emailsubject' and $emailsubject === null) {
		$emailsubject = $option[1];
		unset($vm_options[$pos]);
	} else if ($option[0] === 'emailbody' and $emailbody === null) {
		$emailbody = $option[1];
		unset($vm_options[$pos]);
	}
}

function build_row($option, $form, $url, $helper) {
	$row = '<tr class="fm-paragraph"><td>';
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 30,
			'name' => 'voicemail[optionname][]',
			'value' => $option[0],
			'class' => 'vm-option-name',
		)
	);

	$row .= "</td><td>";
	$row .= $form->text(
		array(
			'paragraph' => false,
			'label' => false,
			'key' => false,
			'size' => 30,
			'name' => 'voicemail[optionvalue][]',
			'value' => $option[1],
			'class' => 'vm-option-value',
		)
	);

	$row .= '</td><td class="td-right">';
	$row .= $url->href_html(
		$url->img_html(
			'img/site/button/mini/blue/delete.gif',
			$helper->bbf('opt_voicemail-option-delete'),
			'border="0"'
		),
		'#',
		null,
		null,
		$helper->bbf('opt_voicemail-option-delete'),
		false,
		'&amp;',
		true,
		true,
		true,
		true,
		'vm-option-remove'
	);

	$row .= "</td></tr>";

	return $row;
}

?>
<uib-tabset active="active">
	<uib-tab index="0" heading="<?=$this->bbf('smenu_general');?>">
		<?php
			echo	$form->text(array('desc'	=> $this->bbf('fm_voicemail_fullname'),
						  'name'	=> 'voicemail[name]',
						  'labelid'	=> 'voicemail-fullname',
						  'size'	=> 15,
						  'value' => $info['voicemail']['name'],
						  'error'	=> $this->bbf_args('error',
							   $this->get_var('error','voicemail','fullname')))),

				$form->text(array('desc'	=> $this->bbf('fm_voicemail_mailbox'),
						  'name'	=> 'voicemail[number]',
						  'labelid'	=> 'voicemail-mailbox',
						  'size'	=> 10,
						  'value' => $info['voicemail']['number'],
						  'error'	=> $this->bbf_args('error',
							   $this->get_var('error','voicemail','mailbox')))),

				$form->text(array('desc'	=> $this->bbf('fm_voicemail_password'),
						  'name'	=> 'voicemail[password]',
						  'labelid'	=> 'voicemail-password',
						  'size'	=> 10,
						  'value' => $info['voicemail']['password'],
						  'error'	=> $this->bbf_args('error',
							   $this->get_var('error','voicemail','password')))),

				$form->text(array('desc'	=> $this->bbf('fm_voicemail_email'),
						  'name'	=> 'voicemail[email]',
						  'labelid'	=> 'voicemail-email',
						  'size'	=> 15,
						  'value' => $info['voicemail']['email'],
						  'error'	=> $this->bbf_args('error',
							   $this->get_var('error','voicemail','email'))));

				if($context_list !== false):
					echo	$form->select(array('desc'	=> $this->bbf('fm_voicemail_context'),
								    'name'	=> 'voicemail[context]',
								    'labelid'	=> 'voicemail-context',
								    'key'	=> 'identity',
								    'altkey'	=> 'name',
								    'selected'	=> $info['voicemail']['context']),
							      $context_list);
				else:
					echo	'<div id="fd-voicemail-context" class="txt-center">',
							$url->href_htmln($this->bbf('create_context'),
									'service/ipbx/system_management/context',
									'act=add'),
						'</div>';
				endif;

				if(($tz_list = $this->get_var('tz_list')) !== false):
					echo	$form->select(array('desc'	=> $this->bbf('fm_voicemail_tz'),
								    'name'	=> 'voicemail[timezone]',
								    'labelid'	=> 'voicemail-tz',
								    'key'	=> 'name',
								    'selected'	=> $info['voicemail']['timezone']),
							      $tz_list);
				endif;

			$lang_list = dwho_i18n::get_supported_language_list();

			echo	$form->select(array('desc'	=> $this->bbf('fm_voicemail_language'),
						    'name'	=> 'voicemail[language]',
						    'labelid'	=> 'voicemail-language',
						    'empty'	=> true,
						    'key'	=> false,
							'selected'	=> $info['voicemail']['language']),
							$lang_list),

			$form->text(array('desc'	=> $this->bbf('fm_voicemail_maxmsg'),
						    'name'	=> 'voicemail[max_messages]',
							'labelid'	=> 'voicemail-maxmsg',
							'size'		=> 10,
							'value' => $info['voicemail']['max_messages'])),

			$form->checkbox(array('desc'	=> $this->bbf('fm_voicemail_ask-password'),
						      'name'	=> 'voicemail[ask_password]',
						      'labelid'	=> 'voicemail-ask_password',
						      'default'	=> '1',
							'checked' => $info === null ? 1 : (int)$info['voicemail']['ask_password'])),

			$form->select(array('desc'      => $this->bbf('fm_voicemail_attach'),
								   'name'      => 'voicemail[attach_audio]',
								   'labelid'   => 'voicemail-attach_audio',
								   'empty'     => true,
								   'key'       => false,
								   'bbf'       => 'fm_bool-opt',
								   'bbfopt'    => array('argmode' => 'paramvalue'),
								   'selected'  => $info['voicemail']['attach_audio'] === null ? null : (int) $info['voicemail']['attach_audio']),
							 array(1, 0)),

			$form->checkbox(array('desc'	=> $this->bbf('fm_voicemail_deletevoicemail'),
						      'name'	=> 'voicemail[delete_messages]',
						      'labelid'	=> 'voicemail-deletevoicemail',
						      'default'	=> '0',
						      'checked'	=> (int)$info['voicemail']['delete_messages']));
		?>
	</uib-tab>
	<uib-tab index="1" heading="<?=$this->bbf('smenu_email');?>">
		<?= $form->text(array('desc'  => $this->bbf('fm_voicemail_pager'),
				'name'     => 'voicemail[pager]',
				'labelid'  => 'voicemail-pager',
				'size'     => 25,
				'required' => false,
				'value'    => $info['voicemail']['pager'],
				'error'    => $this->bbf_args('error',
				$this->get_var('error', 'pager')) ));
		?>

		<input type="hidden" name="voicemail[optionname][]", value="emailsubject" />
		<?= $form->text(array('desc'  => $this->bbf('fm_voicemail-emailsubject'),
				'name'     => 'voicemail[optionvalue][]',
				'labelid'  => 'voicemail-emailsubject',
				'size'     => 25,
				'help'     => $this->bbf('hlp_fm_voicemail-emailsubject'),
				'required' => false,
				'value'    => $emailsubject,
				'error'    => $this->bbf_args('error',
				$this->get_var('error', 'emailsubject')) ));
		?>

		<div class="col-sm-offset-2 fm-paragraph fm-description">

			<input type="hidden" name="voicemail[optionname][]", value="emailbody" />
			<?= $form->textarea(array('paragraph' => false,
								'desc'     => $this->bbf('fm_voicemail-emailbody'),
					'label'    => false,
					'name'     => 'voicemail[optionvalue][]',
					'id'       => 'it-voicemail-emailbody',
					'cols'     => 60,
					'rows'     => 5,
					'help'     => $this->bbf('hlp_fm_voicemail-emailbody'),
					'error'    => $this->bbf_args('error',
					   $this->get_var('error', 'emailbody'))),
				  $emailbody);
			  ?>
		</div>
		<br/>
	</uib-tab>

	<uib-tab index="2" heading="<?=$this->bbf('smenu_advanced');?>">
		<div class="sb-list">

			<table class="table table-condensed table-striped table-hover table-bordered">
				<thead>
					<th class="th-left">
						<?= $this->bbf('col_voicemail-option-name') ?>
					</th>
					<th class="th-center">
						<?= $this->bbf('col_voicemail-option-value') ?>
					</th>
					<th class="th-right">
						<?= $url->href_html(
								$url->img_html(
									'img/site/button/mini/orange/bo-add.gif',
									$this->bbf('col_voicemail-option-add'),
									'border="0"'),
								'#',
								null,
								'id="vm-option-add"',
								$this->bbf('col_voicemail-option-add')
							);
						?>
					</th>
				</thead>
				<tbody id="voicemail-options">
					<?php foreach($vm_options as $option_row): ?>
						<?= build_row($option_row, $form, $url, $this) ?>
					<?php endforeach ?>
				</tbody>
			</table>

			<script type='text/javascript'>

				var optionRow = <?= dwho_json::encode(build_row(array('', ''),
														$form, $url, $this)) ?>;

				var vmOptions = [
					"attachfmt",
					"backupdeleted",
					"callback",
					"dialout",
					"envelope",
					"exitcontext",
					"forcegreetings",
					"forcename",
					"hidefromdir",
					"imapfolder",
					"imappassword",
					"imapuser",
					"imapvmsharedid",
					"locale",
					"maxsecs",
					"messagewrap",
					"minsecs",
					"moveheard",
					"nextaftercmd",
					"passwordlocation",
					"review",
					"saycid",
					"sayduration",
					"saydurationm",
					"sendvoicemail",
					"serveremail",
					"tempgreetwarn",
					"volgain",
				];

				function attachEvents(row) {
					option = row.find(".vm-option-name");
				    option.autocomplete({
				      source: vmOptions,
				      minLength: 0
				    });
				    option.focus(function() {
				        $(this).autocomplete("search", "");
				    });

					remove = row.find(".vm-option-remove");
					remove.click(function(e) {
						e.preventDefault();
						row.detach();
					});
				}

				$(function() {
					$("#vm-option-add").click(function(e) {
						e.preventDefault();
						$("#voicemail-options").append(optionRow);
						row = $("#voicemail-options tr:last");
						attachEvents(row);
					});

					$("#voicemail-options tr").each(function(pos, row) {
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
	</uib-tab>
</uib-tabset>
