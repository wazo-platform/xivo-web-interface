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
$dhtml = &$this->get_module('dhtml');

$info = $this->get_var('info');
$element = $this->get_var('element');
$error = $this->get_var('error');

$error_js = array();
$error_nb = count($error['generalmeetme']);

for($i = 0;$i < $error_nb;$i++):
	$error_js[] = 'dwho.form.error[\'it-generalmeetme-'.$error['generalmeetme'][$i].'\'] = true;';
endfor;

if(isset($error_js[0]) === true)
	$dhtml->write_js($error_js);

?>
<div class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>

<div class="sb-content">
<form action="#" method="post" accept-charset="utf-8">

<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),
		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1));
?>
<uib-tabset active="active">
<uib-tab index="0" heading="<?=$this->bbf('smenu_agents');?>">
	<?php
			echo $form->checkbox(array('desc'	=> $this->bbf('fm_agentoptions_autologoffunavail'),
						'name'	=> 'agentglobalparams[autologoffunavail]',
						'labelid'	=> 'agentglobalparams-autologoffunavail',
						'help'	=> $this->bbf('hlp_fm_agentoptions_autologoffunavail'),
						'default'	=> $element['agentglobalparams']['autologoffunavail']['default'],
						'checked' => $info['agentglobalparams']['autologoffunavail']));
	?>
</uib-tab>

<uib-tab index="1" heading="<?=$this->bbf('smenu_queues');?>">
	<?php
		echo	$form->checkbox(array('desc'	=> $this->bbf('fm_generalqueues_persistentmembers'),
								'name'	=> 'generalqueues[persistentmembers]',
								'labelid'	=> 'generalqueues-persistentmembers',
								'default'	=> $element['generalqueues']['persistentmembers']['default'],
								'help'	=> $this->bbf('hlp_fm_generalqueues_persistentmembers'),
								'checked'	=> $this->get_var('generalqueues','persistentmembers','var_val'))),

			$form->checkbox(array('desc'	=> $this->bbf('fm_generalqueues_autofill'),
								'name'	=> 'generalqueues[autofill]',
								'labelid'	=> 'generalqueues-autofill',
								'default'	=> $element['generalqueues']['autofill']['default'],
								'help'	=> $this->bbf('hlp_fm_generalqueues_autofill'),
								'checked'	=> $this->get_var('generalqueues','autofill','var_val'))),

			$form->checkbox(array('desc'	=> $this->bbf('fm_generalqueues_monitor-type'),
								'name'	=> 'generalqueues[monitor-type]',
								'labelid'	=> 'generalqueues-monitor-type',
								'default'	=> $element['generalqueues']['monitor-type']['default'],
								'help'	=> $this->bbf('hlp_fm_generalqueues_monitor-type'),
								'checked'	=> $this->get_var('generalqueues','monitor-type','var_val'))),

			$form->checkbox(array('desc'	=> $this->bbf('fm_generalqueues_updatecdr'),
								'name'	=> 'generalqueues[updatecdr]',
								'labelid'	=> 'generalqueues-updatecdr',
								'default'	=> $element['generalqueues']['updatecdr']['default'],
								'help'	=> $this->bbf('hlp_fm_generalqueues_updatecdr'),
								'checked'	=> $this->get_var('generalqueues','updatecdr','var_val'))),

			$form->checkbox(array('desc'	=> $this->bbf('fm_generalqueues_shared_lastcall'),
								'name'	=> 'generalqueues[shared_lastcall]',
								'labelid'	=> 'generalqueues-shared_lastcall',
								'default'	=> $element['generalqueues']['shared_lastcall']['default'],
								'help'	=> $this->bbf('hlp_fm_generalqueues_shared_lastcall'),
								'checked'	=> $this->get_var('generalqueues','shared_lastcall','var_val')));

	?>
</uib-tab>
<uib-tab index="2" heading="<?=$this->bbf('smenu_meetme');?>">
	<?php
		echo	$form->select(array('desc'	=> $this->bbf('fm_generalmeetme_audiobuffers'),
							'name'	=> 'generalmeetme[audiobuffers]',
							'labelid'	=> 'generalmeetme-audiobuffers',
							'key'	=> false,
							'default'	=> $element['generalmeetme']['audiobuffers']['default'],
							'help'	=> $this->bbf('hlp_fm_generalmeetme_audiobuffers'),
							'selected'	=> $this->get_var('generalmeetme','audiobuffers','var_val')),
							$element['generalmeetme']['audiobuffers']['value']),

			$form->checkbox(array('desc'  => $this->bbf('fm_generalmeetme_schedule'),
								'name'    => 'generalmeetme[schedule]',
								'labelid' => 'generalmeetme-schedule',
								'help'    => $this->bbf('hlp_fm_generalmeetme_schedule'),
								'checked' => $this->get_var('generalmeetme','schedule','var_val'),
								'default' => $element['generalmeetme']['schedule']['default'])),

			$form->checkbox(array('desc'  => $this->bbf('fm_generalmeetme_logmembercount'),
								'name'    => 'generalmeetme[logmembercount]',
								'labelid' => 'generalmeetme-logmembercount',
								'help'    => $this->bbf('hlp_fm_generalmeetme_logmembercount'),
								'checked' => $this->get_var('generalmeetme','logmembercount','var_val'),
								'default' => $element['generalmeetme']['logmembercount']['default'])),

			$form->select(array('desc'  => $this->bbf('fm_generalmeetme_fuzzystart'),
							'name'    => 'generalmeetme[fuzzystart]',
							'labelid' => 'generalmeetme-fuzzystart',
							'key'     => false,
							'bbf'     => 'time-opt',
							'bbfopt'  => array('argmode' => 'paramvalue',
									'time' => array('from'=>'second', 'format'=>'%M%s')),
							'help'    => $this->bbf('hlp_fm_generalmeetme_fuzzystart'),
							'selected'  => $this->get_var('generalmeetme','fuzzystart','var_val'),
							'default' => $element['generalmeetme']['fuzzystart']['default']),
					$element['generalmeetme']['fuzzystart']['value']),

			$form->select(array('desc'  => $this->bbf('fm_generalmeetme_earlyalert'),
							'name'    => 'generalmeetme[earlyalert]',
							'labelid' => 'generalmeetme-earlyalert',
							'key'     => false,
							'bbf'     => 'time-opt',
							'bbfopt'  => array('argmode' => 'paramvalue',
									'time' => array('from'=>'second', 'format'=>'%M%s')),
							'help'    => $this->bbf('hlp_fm_generalmeetme_earlyalert'),
							'selected'  => $this->get_var('generalmeetme','earlyalert','var_val'),
							'default' => $element['generalmeetme']['earlyalert']['default']),
					$element['generalmeetme']['earlyalert']['value']),

			$form->select(array('desc'  => $this->bbf('fm_generalmeetme_endalert'),
							'name'    => 'generalmeetme[endalert]',
							'labelid' => 'generalmeetme-endalert',
							'key'     => false,
							'bbf'     => 'time-opt',
							'bbfopt'  => array('argmode' => 'paramvalue',
									'time' => array('from'=>'second', 'format'=>'%M%s')),
							'help'    => $this->bbf('hlp_fm_generalmeetme_endalert'),
							'selected'  => $this->get_var('generalmeetme','endalert','var_val'),
							'default' => $element['generalmeetme']['endalert']['default']),
					$element['generalmeetme']['endalert']['value']);
	?>
</uib-tab>
<uib-tab index="3" heading="<?=$this->bbf('smenu_timezone');?>">
	<?php
		echo	$form->select(array('desc'	=> $this->bbf('fm_general_timezone'),
							'name'     => 'general[timezone]',
							'labelid'  => 'general-timezone',
							'key'      => false,
							//'default'  => $element['general']['timezone']['default'],
							'selected' => $this->get_var('general','timezone')),
							$element['general']);
	?>
</uib-tab>
</uib-tabset>
<?php
	echo	$form->submit(array('name'	=> 'submit',
				    'id'	=> 'it-submit',
				    'value'	=> $this->bbf('fm_bt-save')));
?>
</form>

	</div>
	<div class="sb-foot xspan"></div>
</div>
