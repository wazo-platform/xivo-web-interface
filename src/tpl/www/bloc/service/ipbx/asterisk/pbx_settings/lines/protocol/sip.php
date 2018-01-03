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

$free_options = $this->get_var('info', 'free_options');
if ($free_options === null) {
	$free_options = array();
}

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

<div role="tabpanel" class="tab-pane active" id="general">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_protocol_name'),
				  'name'	   => 'protocol[name]',
				  'labelid'	 => 'protocol-name',
				  'size'	   => 15,
				  'readonly' => $this->get_var('element','protocol','name','readonly'),
				  'class'    => $this->get_var('element','protocol','name','class'),
				  'default'  => $this->get_var('element','protocol','name','default'),
				  'value'	   => $info['endpoint']['username'])),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_secret'),
				  'name'	=> 'protocol[secret]',
				  'labelid'	=> 'protocol-secret',
				  'size'	=> 15,
				  'readonly' => $this->get_var('element','protocol','secret','readonly'),
				  'class'    => $this->get_var('element','protocol','secret','class'),
				  'default'	=> $this->get_var('element', 'protocol', 'secret', 'default'),
				  'value'	=> $this->get_var('info','endpoint','secret')));

	if($context_list !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_context'),
					    'name'		=> 'protocol[context]',
					    'labelid'	=> 'protocol-context',
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
				    'selected'	=> $this->get_var('info','extra','language')),
			      $element['protocol']['sip']['language']['value']),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_callerid'),
				    'name'	=> 'protocol[callerid]',
				    'labelid'	=> 'protocol-callerid',
				    'value'	=> $this->get_var('info','extra','callerid'),
				    'size'	=> 35,
				    'notag'	=> false)),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_nat'),
				    'name'	=> 'protocol[nat]',
				    'labelid'	=> 'sip-protocol-nat',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_nat-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['nat']['default'],
				    'selected'	=> $this->get_var('info','extra','nat')),
			      $element['protocol']['sip']['nat']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_dtmfmode'),
				    'name'		=> 'protocol[dtmfmode]',
				    'labelid'	=> 'sip-protocol-dtmfmode',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_protocol_dtmfmode-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['dtmfmode']['default'],
				    'selected'	=> $this->get_var('info','extra','dtmfmode')),
			      $element['protocol']['sip']['dtmfmode']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_qualify'),
				    'name'		=> 'protocol[qualify]',
				    'labelid'	=> 'sip-protocol-qualify',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_protocol_qualify-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['qualify']['default'],
				    'selected'	=> $this->get_var('info','extra','qualify')),
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

<div role="tabpanel" class="tab-pane" id="advanced">
	<div class="sb-list">
		<table class="table">
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
				<?php foreach($free_options as $option_row): ?>
					<?= build_row($option_row, $form, $url, $this) ?>
				<?php endforeach ?>
			</tbody>
		</table>

		<script type='text/javascript'>
		var optionRow = <?= dwho_json::encode(build_row(
			array('', ''),
			$form, $url, $this))
		?>;

		var sipOptions = [
			"accountcode",
			"acl",
			"allow",
			"allowoverlap",
			"allowsubscribe",
			"allowtransfer",
			"amaflags",
			"autoframing",
			"avpf",
			"buggymwi",
			"busylevel",
			"call-limit",
			"callbackextension",
			"callcounter",
			"callerid",
			"callgroup",
			"callingpres",
			"cc_agent_dialstring",
			"cc_agent_policy",
			"cc_callback_macro",
			"cc_callback_sub",
			"cc_max_agents",
			"cc_max_monitors",
			"cc_monitor_policy",
			"cc_offer_timer",
			"cc_recall_timer",
			"ccbs_available_timer",
			"ccnr_available_timer",
			"cid_number",
			"cid_tag",
			"contactacl",
			"contactdeny",
			"contactpermit",
			"context",
			"defaultip",
			"defaultuser",
			"deny",
			"description",
			"directmedia",
			"directmediaacl",
			"directmediadeny",
			"directmediapermit",
			"disallow",
			"disallowed_methods",
			"discard_remote_hold_retrieval",
			"dtlscafile",
			"dtlscapath",
			"dtlscertfile",
			"dtlscipher",
			"dtlsenable",
			"dtlsfingerprint",
			"dtlsprivatekey",
			"dtlsrekey",
			"dtlssetup",
			"dtlsverify",
			"dtmfmode",
			"encryption",
			"encryption_taglen",
			"faxdetect",
			"force_avp",
			"fromdomain",
			"fromuser",
			"fullname",
			"g726nonstandard",
			"hasvoicemail",
			"header",
			"host",
			"icesupport",
			"ignore_requested_pref",
			"ignoresdpversion",
			"insecure",
			"keepalive",
			"language",
			"mailbox",
			"maxcallbitrate",
			"maxforwards",
			"md5secret",
			"mohinterpret",
			"mohsuggest",
			"mwi_from",
			"namedcallgroup",
			"namedpickupgroup",
			"nat",
			"outboundproxy",
			"outofcall_message_context",
			"parkinglot",
			"permit",
			"pickupgroup",
			"port",
			"preferred_codec_only",
			"progressinband",
			"promiscredir",
			"qualify",
			"qualifyfreq",
			"recordofffeature",
			"recordonfeature",
			"regexten",
			"remotesecret",
			"rfc2833compensate",
			"rpid_immediate",
			"rpid_update",
			"rtp_engine",
			"rtpholdtimeout",
			"rtpkeepalive",
			"rtptimeout",
			"secret",
			"sendrpid",
			"session-expires",
			"session-minse",
			"session-refresher",
			"session-timers",
			"setvar",
			"snom_aoc_enabled",
			"subscribecontext",
			"subscribemwi",
			"supportpath",
			"t38pt_udptl",
			"t38pt_usertpsource",
			"textsupport",
			"timerb",
			"timert1",
			"tonezone",
			"transport",
			"trunkname",
			"trust_id_outbound",
			"trustrpid",
			"type",
			"unsolicited_mailbox",
			"use_q850_reason",
			"useclientcode",
			"usereqphone",
			"videosupport",
			"vmexten",
			"webrtc"
		];

		function attachEvents(row) {
			option = row.find(".sip-option-name");
			option.autocomplete({
				source: sipOptions,
				minLength: 0
			});
			option.focus(function() {
				$(this).autocomplete("search", "");
			});

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
