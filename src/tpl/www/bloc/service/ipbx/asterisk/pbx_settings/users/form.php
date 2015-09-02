<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2014  Avencall
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
$error = $this->get_var('error');
$element = $this->get_var('element');

$voicemail_list = $this->get_var('voicemail_list');
$agent_list = $this->get_var('agent_list');
$profileclient_list = $this->get_var('profileclient_list');
$rightcall = $this->get_var('rightcall');
$schedules = $this->get_var('schedules');
$parking_list = $this->get_var('parking_list');
$context_list = $this->get_var('context_list');

if(($outcallerid = (string) $info['userfeatures']['outcallerid']) === ''
|| in_array($outcallerid,$element['userfeatures']['outcallerid']['value'],true) === true):
	$outcallerid_custom = false;
else:
	$outcallerid_custom = true;
endif;

$line_nb = 0;
$line_list = false;

if(dwho_issa('linefeatures',$info) === true
&&($line_nb = count($info['linefeatures'])) > 0)
	$line_list = $info['linefeatures'];

?>

<div id="sb-part-first" class="b-nodisplay">

<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_userfeatures_firstname'),
				  'name'	=> 'userfeatures[firstname]',
				  'labelid'	=> 'userfeatures-firstname',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['firstname']['default'],
				  'value'	=> $info['userfeatures']['firstname'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'firstname')) )),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_lastname'),
				  'name'	=> 'userfeatures[lastname]',
				  'labelid'	=> 'userfeatures-lastname',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['lastname']['default'],
				  'value'	=> $info['userfeatures']['lastname'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'lastname')) )),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_mobilephonenumber'),
				  'name'	=> 'userfeatures[mobilephonenumber]',
				  'labelid'	=> 'userfeatures-mobilephonenumber',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['mobilephonenumber']['default'],
				  'value'	=> $info['userfeatures']['mobilephonenumber'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'mobilephonenumber')) ));

	if($schedules === false):
		echo	'<div class="txt-center">',
			$url->href_htmln($this->bbf('create_schedules'),
					'service/ipbx/call_management/schedule',
					'act=add'),
			'</div>';
	else:
		echo $form->select(array('desc'	=> $this->bbf('fm_user_schedule'),
				    'name'	    => 'schedule_id',
				    'labelid'	  => 'schedule_id',
						'key'	      => 'name',
						'altkey'    => 'id',
						'empty'     => true,
				    'selected'	=> $this->get_var('schedule_id')),
			      $schedules);
	endif;


		echo	$form->select(array('desc'	=> $this->bbf('fm_userfeatures_ringseconds'),
				    'name'	=> 'userfeatures[ringseconds]',
				    'labelid'	=> 'userfeatures-ringseconds',
				    'key'	=> false,
				    'bbf'	=> 'fm_userfeatures_ringseconds-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['userfeatures']['ringseconds']['default'],
				    'selected'	=> $info['userfeatures']['ringseconds']),
			      $element['userfeatures']['ringseconds']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_userfeatures_simultcalls'),
				    'name'	=> 'userfeatures[simultcalls]',
				    'labelid'	=> 'userfeatures-simultcalls',
				    'key'	=> false,
				    'default'	=> $element['userfeatures']['simultcalls']['default'],
				    'selected'	=> $info['userfeatures']['simultcalls']),
			      $element['userfeatures']['simultcalls']['value']);

	if(($moh_list = $this->get_var('moh_list')) !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_userfeatures_musiconhold'),
					    'name'	=> 'userfeatures[musiconhold]',
					    'labelid'	=> 'userfeatures-musiconhold',
					    'empty'	=> true,
					    'key'	=> 'category',
					    'invalid'	=> ($this->get_var('act') === 'edit'),
					    'default'	=> ($this->get_var('act') === 'add' ? $element['userfeatures']['musiconhold']['default'] : null),
					    'selected'	=> $info['userfeatures']['musiconhold']),
				      $moh_list);
	endif;

	echo	$form->select(array('desc'	=> $this->bbf('fm_userfeatures_language'),
				    'name'	=> 'userfeatures[language]',
				    'labelid'	=> 'userfeatures-language',
				    'empty'	=> true,
				    'key'	=> false,
				    'default'	=> $element['userfeatures']['language']['default'],
				    'selected'	=> $this->get_var('info','userfeatures','language'),
				  	'error'	=> $this->bbf_args('error',
							$this->get_var('error', 'voicemail', 'locale')) ),
			      $element['userfeatures']['language']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_userfeatures_timezone'),
				    'name'	=> 'userfeatures[timezone]',
				    'labelid'	=> 'userfeatures-timezone',
				    'empty'	=> true,
				    'key'	=> false,
# no default value => take general value
#				    'default'		=> $element['userfeatures']['timezone']['default'],
				    'selected'	=> $this->get_var('info','userfeatures','timezone')),
			      array_keys(dwho_i18n::get_timezone_list())),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_callerid'),
				  'name'	=> 'userfeatures[callerid]',
				  'labelid'	=> 'userfeatures-callerid',
				  'value'	=> $this->get_var('info','userfeatures','callerid'),
				  'size'	=> 15,
				  'notag'	=> false,
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'userfeatures', 'callerid')) )),

		$form->select(array('desc'	=> $this->bbf('fm_userfeatures_outcallerid'),
				    'name'	=> 'userfeatures[outcallerid-type]',
				    'labelid'	=> 'userfeatures-outcallerid-type',
				    'key'	=> false,
				    'bbf'	=> 'fm_userfeatures_outcallerid-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'selected'	=> ($outcallerid_custom === true ? 'custom' : $outcallerid)),
			      $element['userfeatures']['outcallerid-type']['value']),

		$form->text(array('desc'	=> '&nbsp;',
				  'name'	=> 'userfeatures[outcallerid-custom]',
				  'labelid'	=> 'userfeatures-outcallerid-custom',
				  'value'	=> ($outcallerid_custom === true ? $outcallerid : ''),
				  'size'	=> 15,
				  'notag'	=> false,
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'userfeatures', 'outcallerid-custom')) )),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_preprocess-subroutine'),
				  'name'	=> 'userfeatures[preprocess_subroutine]',
				  'labelid'	=> 'userfeatures-preprocess-subroutine',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['preprocess_subroutine']['default'],
				  'value'	=> $info['userfeatures']['preprocess_subroutine'],
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'userfeatures', 'preprocess_subroutine')) )),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_userfield'),
				  'name'	=> 'userfeatures[userfield]',
				  'labelid'	=> 'userfeatures-userfield',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','userfeatures','userfield'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'userfield')) ));
?>
<fieldset id="fld-xivoclient">
	<legend><?=$this->bbf('fld-client');?></legend>
<?php echo		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enableclient'),
				      'name'	=> 'userfeatures[enableclient]',
				      'labelid'	=> 'userfeatures-enableclient',
				      'default'	=> $element['userfeatures']['enableclient']['default'],
				      'checked'	=> $info['userfeatures']['enableclient'])),

				$form->text(array('desc'	=> $this->bbf('fm_userfeatures_loginclient'),
				  'name'	=> 'userfeatures[loginclient]',
				  'labelid'	=> 'userfeatures-loginclient',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['loginclient']['default'],
				  'value'	=> $info['userfeatures']['loginclient'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'loginclient')) )),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_passwdclient'),
				  'name'	=> 'userfeatures[passwdclient]',
				  'labelid'	=> 'userfeatures-passwdclient',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['passwdclient']['default'],
				  'value'	=> $info['userfeatures']['passwdclient'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'passwdclient')) ));

	if(is_array($profileclient_list) === true && empty($profileclient_list) === false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_userfeatures_profileclient'),
						'name'	=> 'userfeatures[cti_profile_id]',
						'labelid'	=> 'userfeatures-cti_profile_id',
						'altkey'	=> 'name',
						'key'		=> 'id',
						'default'	=> $element['userfeatures']['cti_profile_id']['default'],
						'empty'	=> true,
						'selected'	=> $info['userfeatures']['cti_profile_id']),
					$profileclient_list);
	endif;
?>
</fieldset>
	<div class="fm-paragraph fm-description">
		<p>
			<label id="lb-userfeatures-description" for="it-userfeatures-description"><?=$this->bbf('fm_userfeatures_description');?></label>
		</p>
		<?=$form->textarea(array('paragraph' => false,
					 'label'	=> false,
					 'name'		=> 'userfeatures[description]',
					 'id'		=> 'it-userfeatures-description',
					 'cols'		=> 60,
					 'rows'		=> 5,
					 'default'	=> $element['userfeatures']['description']['default'],
					 'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'description')) ),
				   $info['userfeatures']['description']);?>
	</div>
</div>

<div id="sb-part-lines" class="b-nodisplay">
<?php
	if ($this->get_var('entity_list') === false):
	    echo $this->bbf('no_internal_context_for_this_entity');
	else:
	    echo	$form->select(array('desc'	=> $this->bbf('fm_userfeatures_entity'),
				    'name'		=> 'userfeatures[entityid]',
				    'labelid'	=> 'userfeatures-entityid',
				    'help'		=> $this->bbf('hlp_fm_userfeatures_entity'),
				    'key'		=> 'displayname',
				    'altkey'	=> 'id',
				    'selected'  => $info['userfeatures']['entityid'],
				    'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'entityid'))),
			      $this->get_var('entity_list'));
?>
	<div class="sb-list">
<?php
	$this->file_include('bloc/service/ipbx/asterisk/pbx_settings/users/line',
			    array('count'	=> $line_nb,
					  'list'	=> $line_list));
?>
	</div>
<?php
	endif;
?>
</div>

<div id="sb-part-voicemail" class="b-nodisplay">
<fieldset id="fld-voicemail-actions">
<legend><?= $this->bbf('user_vm_header') ?></legend>
<p id="vm-action-search" class="fm-paragraph">
	<span class="fm-desc clearboth">
		<?= $this->bbf('user_vm_search') ?>
	</span>
	<input type="text" size="15" id="user-vm-search" class="it-mblur" />
</p>
<p id="vm-action-add" class="fm-paragraph">
		<span class="fm-desc">
			<?= $this->bbf('user_vm_add') ?>
		</span>
		<?= $url->href_html(
				$url->img_html(
					'img/site/button/mini/orange/bo-add.gif',
					$this->bbf('user_vm_add')
				),
				'#',
				null,
				"id='user-vm-add'",
				$this->bbf('user_vm_add'))
				;
		?>

</p>
<p id="vm-action-delete" class="fm-paragraph">
	<span class="fm-desc">
		<?= $this->bbf('user_vm_delete') ?>
	</span>
	<?= $url->href_html(
		$url->img_html(
			'img/site/button/mini/blue/delete.gif',
			$this->bbf('user_vm_delete')
		),
		'#',
		null,
		'id="user-vm-delete"',
		$this->bbf('user_vm_delete'));
	?>
</p>
<?php
	echo $form->checkbox(array(
		'desc'    => $this->bbf('fm_userfeatures_enablevoicemail'),
		'name'    => 'userfeatures[enablevoicemail]',
		'labelid' => 'userfeatures-enablevoicemail',
		'checked' => $info['userfeatures']['enablevoicemail']));
?>
<?php
	$vm_action = '';
	$fm_voicemail_id = (int) $this->get_var('voicemail', 'id');
	if($fm_voicemail_id !== 0) {
		$vm_action = 'edit';
	}
?>
<input type="hidden" id="user-vm-action" name="user_vm_action" value="<?= $vm_action ?>" />

</fieldset>

<fieldset id='fld-voicemail-form'>
<legend><?= $this->bbf('user_vm_form') ?></legend>
<p>
<?php
		echo $form->hidden(array('name'	=> 'voicemail[id]',
				    'id'	=> 'it-voicemail-id',
				    'value'	=> $this->get_var('voicemail', 'id'))),

		$form->text(array('desc'	=> $this->bbf('fm_voicemail_fullname'),
				  'name'	=> 'voicemail[name]',
				  'labelid'	=> 'voicemail-name',
				  'size'	=> 15,
				  'value'	=> $this->get_var('voicemail','name'),
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'voicemail', 'name')) )),

		$form->text(array('desc'	=> $this->bbf('fm_voicemail_mailbox'),
				  'name'	=> 'voicemail[number]',
				  'labelid'	=> 'voicemail-number',
				  'size'	=> 10,
				  'value'	=> $this->get_var('voicemail','number'),
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'voicemail', 'number')) )),

		$form->text(array('desc'	=> $this->bbf('fm_voicemail_password'),
				  'name'	=> 'voicemail[password]',
				  'labelid'	=> 'voicemail-password',
				  'size'	=> 10,
				  'value'	=> $this->get_var('voicemail','password'),
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'voicemail', 'password')) )),

		$form->text(array('desc'	=> $this->bbf('fm_voicemail_email'),
				  'name'	=> 'voicemail[email]',
				  'labelid'	=> 'voicemail-email',
				  'size'	=> 15,
				  'value'	=> $this->get_var('voicemail','email'),
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'voicemail', 'email')) ));

	if($context_list !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_voicemail_context'),
						'name'	=> 'voicemail[context]',
						'labelid'	=> 'voicemail-context',
						'key'	=> 'identity',
						'altkey'	=> 'name',
						'selected'	=> $this->get_var('voicemail', 'context')),
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
					    'labelid'	=> 'voicemail-timezone',
					    'key'	=> 'name',
					    'selected'	=> $this->get_var('voicemail','timezone')),
				      $tz_list);
	endif;

	$lang_list = dwho_i18n::get_supported_language_list();
	echo $form->select(array('desc'	=> $this->bbf('fm_voicemail_language'),
					   'name'	=> 'voicemail[language]',
					   'labelid'	=> 'voicemail-language',
					   'empty'	=> true,
					   'key'	=> false,
					   'selected'	=> $this->get_var('voicemail', 'language')),
						$lang_list),

		$form->text(array('desc'	=> $this->bbf('fm_voicemail_maxmsg'),
				    'name'	=> 'voicemail[max_messages]',
					'labelid'	=> 'voicemail-max-messages',
					'size'		=> 10,
					'value' => $this->get_var('voicemail', 'max_messages'))),

		$form->checkbox(array('desc'	=> $this->bbf('fm_voicemail_ask-password'),
				      'name'	=> 'voicemail[ask_password]',
				      'labelid'	=> 'voicemail-ask-password',
				      'default'	=> '0',
					  'checked'	=> (int)$this->get_var('voicemail', 'ask_password'))),

		$form->checkbox(array('desc'	=> $this->bbf('fm_voicemail_attach'),
				      'name'	=> 'voicemail[attach_audio]',
				      'labelid'	=> 'voicemail-attach-audio',
				      'default'	=> '0',
				      'checked'	=> (int)$this->get_var('voicemail', 'attach_audio'))),

		$form->checkbox(array('desc'	=> $this->bbf('fm_voicemail_deletevoicemail'),
				      'name'	=> 'voicemail[delete_messages]',
				      'labelid'	=> 'voicemail-delete-messages',
				      'default'	=> '0',
				      'checked'	=> (int)$this->get_var('voicemail', 'delete_messages')));
?>
</fieldset>
</div>

<div id="sb-part-dialaction" class="b-nodisplay">
	<fieldset id="fld-dialaction-noanswer">
		<legend><?=$this->bbf('fld-dialaction-noanswer');?></legend>
<?php
		$this->file_include('bloc/service/ipbx/asterisk/dialaction/all',
				    array('event'	=> 'noanswer'));
?>
	</fieldset>

	<fieldset id="fld-dialaction-busy">
		<legend><?=$this->bbf('fld-dialaction-busy');?></legend>
<?php
		$this->file_include('bloc/service/ipbx/asterisk/dialaction/all',
				    array('event'	=> 'busy'));
?>
	</fieldset>

	<fieldset id="fld-dialaction-congestion">
		<legend><?=$this->bbf('fld-dialaction-congestion');?></legend>
<?php
		$this->file_include('bloc/service/ipbx/asterisk/dialaction/all',
				    array('event'	=> 'congestion'));
?>
	</fieldset>

	<fieldset id="fld-dialaction-chanunavail">
		<legend><?=$this->bbf('fld-dialaction-chanunavail');?></legend>
<?php
		$this->file_include('bloc/service/ipbx/asterisk/dialaction/all',
				    array('event'	=> 'chanunavail'));
?>
	</fieldset>
</div>

<div id="sb-part-service" class="b-nodisplay">

	<fieldset id="fld-services">
		<legend><?=$this->bbf('fld-services');?></legend>
<?php
	echo	$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enablehint'),
				      'name'	=> 'userfeatures[enablehint]',
				      'labelid'	=> 'userfeatures-enablehint',
				      'default'	=> $element['userfeatures']['enablehint']['default'],
				      'checked'	=> $info['userfeatures']['enablehint'])),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enablexfer'),
				      'name'	=> 'userfeatures[enablexfer]',
				      'labelid'	=> 'userfeatures-enablexfer',
				      'default'	=> $element['userfeatures']['enablexfer']['default'],
				      'checked'	=> $info['userfeatures']['enablexfer'])),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enableautomon'),
				      'name'	=> 'userfeatures[enableautomon]',
				      'labelid'	=> 'userfeatures-enableautomon',
				      'default'	=> $element['userfeatures']['enableautomon']['default'],
				      'checked'	=> $info['userfeatures']['enableautomon'])),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_callrecord'),
				      'name'	=> 'userfeatures[callrecord]',
				      'labelid'	=> 'userfeatures-callrecord',
				      'default'	=> $element['userfeatures']['callrecord']['default'],
				      'checked'	=> $info['userfeatures']['callrecord'])),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_incallfilter'),
				      'name'	=> 'userfeatures[incallfilter]',
				      'labelid'	=> 'userfeatures-incallfilter',
				      'default'	=> $element['userfeatures']['incallfilter']['default'],
				      'checked'	=> $info['userfeatures']['incallfilter'])),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enablednd'),
				      'name'	=> 'userfeatures[enablednd]',
				      'labelid'	=> 'userfeatures-enablednd',
				      'default'	=> $element['userfeatures']['enablednd']['default'],
				      'checked'	=> $info['userfeatures']['enablednd'])),

		$form->select(array('desc'	=> $this->bbf('fm_userfeatures_bsfilter'),
				    'name'	=> 'userfeatures[bsfilter]',
				    'labelid'	=> 'userfeatures-bsfilter',
				    'key'	=> false,
				    'bbf'	=> 'fm_userfeatures_bsfilter-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['userfeatures']['bsfilter']['default'],
				    'selected'	=> $info['userfeatures']['bsfilter']),
			      $element['userfeatures']['bsfilter']['value']);

	if($agent_list !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_userfeatures_agentid'),
					    'name'	=> 'userfeatures[agentid]',
					    'labelid'	=> 'userfeatures-agentid',
					    'empty'	=> true,
					    'key'	=> 'identity',
					    'altkey'	=> 'id',
					    'default'	=> $element['userfeatures']['agentid']['default'],
					    'selected'	=> $info['userfeatures']['agentid']),
				      $agent_list);
	else:
		echo	'<div id="fd-userfeatures-agentid" class="txt-center">',
			$url->href_htmln($this->bbf('create_agent'),
					'callcenter/settings/agents',
					array('act'	=> 'addagent',
					      'group'	=> 1)),
			'</div>';
	endif;
?>
	</fieldset>

	<fieldset id="fld-rightcalls">
		<legend><?=$this->bbf('fld-rightcalls');?></legend>
<?php

		echo	$form->text(array('desc'	=> $this->bbf('fm_userfeatures_rightcallcode'),
				  'name'	=> 'userfeatures[rightcallcode]',
				  'labelid'	=> 'userfeatures-rightcallcode',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['rightcallcode']['default'],
				  'value'	=> $info['userfeatures']['rightcallcode'],
				  'error'	=> $this->bbf_args('error',
				$this->get_var('error', 'userfeatures', 'rightcallcode')) ));

				if($rightcall['list'] !== false):
?>
    <div id="rightcalllist" class="fm-paragraph fm-description">
    		<?=$form->jq_select(array('paragraph'	=> false,
    					 	'label'		=> false,
                			'name'    	=> 'rightcall[]',
    						'id' 		=> 'it-rightcall',
    						'key'		=> 'identity',
    				       	'altkey'	=> 'id',
                			'selected'  => $rightcall['slt']),
    					$rightcall['list']);?>
    </div>
    <div class="clearboth"></div>
<?php
				else:
					echo	'<div class="txt-center">',
						$url->href_htmln($this->bbf('create_rightcall'),
								'service/ipbx/call_management/rightcall',
								'act=add'),
						'</div>';
				endif;
?>
	</fieldset>

	<fieldset id="fld-callforwards">
		<legend><?=$this->bbf('fld-callforwards');?></legend>
<?php

	echo	$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enablerna'),
				      'name'	=> 'userfeatures[enablerna]',
				      'labelid'	=> 'userfeatures-enablerna',
				      'default'	=> $element['userfeatures']['enablerna']['default'],
				      'checked'	=> $info['userfeatures']['enablerna'])),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_destrna'),
				  'name'	=> 'userfeatures[destrna]',
				  'labelid'	=> 'userfeatures-destrna',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['destrna']['default'],
				  'value'	=> $info['userfeatures']['destrna'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'destrna')) )),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enablebusy'),
				      'name'	=> 'userfeatures[enablebusy]',
				      'labelid'	=> 'userfeatures-enablebusy',
				      'default'	=> $element['userfeatures']['enablebusy']['default'],
				      'checked'	=> $info['userfeatures']['enablebusy'])),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_destbusy'),
				  'name'	=> 'userfeatures[destbusy]',
				  'labelid'	=> 'userfeatures-destbusy',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['destbusy']['default'],
				  'value'	=> $info['userfeatures']['destbusy'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'destbusy')) )),

		$form->checkbox(array('desc'	=> $this->bbf('fm_userfeatures_enableunc'),
				      'name'	=> 'userfeatures[enableunc]',
				      'labelid'	=> 'userfeatures-enableunc',
				      'default'	=> $element['userfeatures']['enableunc']['default'],
				      'checked'	=> $info['userfeatures']['enableunc'])),

		$form->text(array('desc'	=> $this->bbf('fm_userfeatures_destunc'),
				  'name'	=> 'userfeatures[destunc]',
				  'labelid'	=> 'userfeatures-destunc',
				  'size'	=> 15,
				  'default'	=> $element['userfeatures']['destunc']['default'],
				  'value'	=> $info['userfeatures']['destunc'],
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'userfeatures', 'destunc')) ));
?>
	</fieldset>
</div>

<div id="sb-part-groups" class="b-nodisplay">
<?php
	$this->file_include('bloc/service/ipbx/asterisk/pbx_settings/users/groups');
?>
</div>

<div id="sb-part-funckeys" class="b-nodisplay">
<?php
	$this->file_include('bloc/service/ipbx/asterisk/pbx_settings/users/phonefunckey');
?>
</div>

<div id="sb-part-rightcalls" class="b-nodisplay">
</div>

<div id="sb-part-schedule" class="b-nodisplay">
</div>
