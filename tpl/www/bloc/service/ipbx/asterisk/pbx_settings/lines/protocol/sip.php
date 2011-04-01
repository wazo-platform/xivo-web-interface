<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Proformatique <technique@proformatique.com>
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
				  'error'	=> $this->bbf_args('error',$this->get_var('error', 'protocol', 'secret')) )),

		$form->text(array('desc'	=> $this->bbf('fm_linefeatures_number'),
				  'name'	=> 'linefeatures[number]',
				  'labelid'	=> 'linefeatures-number',
				  'size'	=> 15,
				  'disabled'	=> true,
				'readonly' => $this->get_var('element','linefeatures','number','readonly'),
				'class'    => $this->get_var('element','linefeatures','number','class'),
				  'value'	=> $this->get_var('info','linefeatures','number'),
				  'error'	=> $this->bbf_args('error',$this->get_var('error', 'linefeatures', 'number')) ));

	if($context_list !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_context'),
					    'name'		=> 'protocol[context]',
					    'labelid'	=> 'protocol-context',
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

     $form->select(array('desc'  => $this->bbf('fm_protocol-encryption'),
            'name'      => 'protocol[encryption]',
            'labelid'   => 'sip-protocol-encryption',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-encryption'),
            'selected'  => $info['protocol']['encryption'],
            'default'   => $element['protocol']['sip']['encryption']['default']),
         $element['protocol']['sip']['encryption']['value']),

		$form->checkbox(array('desc'	=> $this->bbf('fm_protocol_subscribemwi'),
				      'name'	=> 'protocol[subscribemwi]',
				      'labelid'	=> 'protocol-subscribemwi',
				      'default'	=> $element['protocol']['sip']['subscribemwi']['default'],
				      'checked'	=> $this->get_var('info','protocol','subscribemwi'))),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_buggymwi'),
				    'name'	=> 'protocol[buggymwi]',
				    'labelid'	=> 'protocol-buggymwi',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['buggymwi']['default'],
				    'selected'	=> $this->get_var('info','protocol','buggymwi')),
			      $element['protocol']['sip']['buggymwi']['value']);
?>
</div>

<div id="sb-part-signalling" class="b-nodisplay">
<?php
	echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_progressinband'),
				    'name'	=> 'protocol[progressinband]',
				    'labelid'	=> 'protocol-progressinband',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_progressinband-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['progressinband']['default'],
				    'selected'	=> $this->get_var('info','protocol','progressinband')),
			      $element['protocol']['sip']['progressinband']['value']),

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

		$form->select(array('desc'	=> $this->bbf('fm_protocol_rfc2833compensate'),
				    'name'	=> 'protocol[rfc2833compensate]',
				    'labelid'	=> 'protocol-rfc2833compensate',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['rfc2833compensate']['default'],
				    'selected'	=> $this->get_var('info','protocol','rfc2833compensate')),
			      $element['protocol']['sip']['rfc2833compensate']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_qualify'),
				    'name'		=> 'protocol[qualify]',
				    'labelid'	=> 'sip-protocol-qualify',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_protocol_qualify-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['qualify']['default'],
				    'selected'	=> $qualify),
			      $element['protocol']['sip']['qualify']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_rtptimeout'),
				    'name'	=> 'protocol[rtptimeout]',
				    'labelid'	=> 'protocol-rtptimeout',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_rtptimeout-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['rtptimeout']['default'],
				    'selected'	=> $this->get_var('info','protocol','rtptimeout')),
			      $element['protocol']['sip']['rtptimeout']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_rtpholdtimeout'),
				    'name'	=> 'protocol[rtpholdtimeout]',
				    'labelid'	=> 'protocol-rtpholdtimeout',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_rtpholdtimeout-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['rtpholdtimeout']['default'],
				    'selected'	=> $this->get_var('info','protocol','rtpholdtimeout')),
			      $element['protocol']['sip']['rtpholdtimeout']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_rtpkeepalive'),
				    'name'		=> 'protocol[rtpkeepalive]',
				    'labelid'	=> 'protocol-rtpkeepalive',
				    'empty'		=> true,
				    'key'		=> false,
				    'bbf'		=> 'fm_protocol_rtpkeepalive-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['rtpkeepalive']['default'],
				    'selected'	=> $this->get_var('info','protocol','rtpkeepalive')),
			      $element['protocol']['sip']['rtpkeepalive']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_allowtransfer'),
				    'name'	=> 'protocol[allowtransfer]',
				    'labelid'	=> 'protocol-allowtransfer',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['allowtransfer']['default'],
				    'selected'	=> $this->get_var('info','protocol','allowtransfer')),
			      $element['protocol']['sip']['allowtransfer']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_pickupcontext'),
					    'name'	=> 'protocol[pickupcontext]',
					    'labelid'	=> 'protocol-pickupcontext',
					    'key'	=> 'identity',
					    'altkey'	=> 'name',
					    'help'		=> $this->bbf('hlp_fm_pickupcontext'),
					    'selected'	=> $this->get_var('info', 'protocol', 'pickupcontext')),
				      $context_list),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_autoframing'),
				    'name'	=> 'protocol[autoframing]',
				    'labelid'	=> 'protocol-autoframing',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['autoframing']['default'],
				    'selected'	=> $this->get_var('info','protocol','autoframing')),
			      $element['protocol']['sip']['autoframing']['value']),

    $form->select(array('desc'  => $this->bbf('fm_protocol-textsupport'),
            'name'      => 'protocol[textsupport]',
            'labelid'   => 'protocol-textsupport',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-textsupport'),
            'selected'  => $info['protocol']['textsupport'],
            'default'   => $element['protocol']['sip']['textsupport']['default']),
         $element['protocol']['sip']['textsupport']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_videosupport'),
				    'name'	=> 'protocol[videosupport]',
				    'labelid'	=> 'protocol-videosupport',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['videosupport']['default'],
				    'selected'	=> $this->get_var('info','protocol','videosupport')),
			      $element['protocol']['sip']['videosupport']['value']),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_maxcallbitrate'),
				  'name'	=> 'protocol[maxcallbitrate]',
				  'labelid'	=> 'protocol-maxcallbitrate',
				  'size'	=> 10,
				  'default'	=> $element['protocol']['sip']['maxcallbitrate']['default'],
				  'value'	=> $this->get_var('info','protocol','maxcallbitrate'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'maxcallbitrate')) )),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_g726nonstandard'),
				    'name'	=> 'protocol[g726nonstandard]',
				    'labelid'	=> 'protocol-g726nonstandard',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['g726nonstandard']['default'],
				    'selected'	=> $this->get_var('info','protocol','g726nonstandard')),
			      $element['protocol']['sip']['g726nonstandard']['value']),

    $form->text(array('desc'  => $this->bbf('fm_protocol-timert1'),
            'name'     => 'protocol[timert1]',
            'labelid'  => 'protocol-timert1',
            'size'     => 5,
            'help'     => $this->bbf('hlp_fm_protocol-timert1'),
            'required' => false,
            'value'    => $info['protocol']['timert1'],
            'default'  => $element['protocol']['sip']['timert1']['default'],
            'error'    => $this->bbf_args('error',
        $this->get_var('error', 'timert1')) )),

    $form->text(array('desc'  => $this->bbf('fm_protocol-timerb'),
            'name'     => 'protocol[timerb]',
            'labelid'  => 'protocol-timerb',
            'size'     => 5,
            'help'     => $this->bbf('hlp_fm_protocol-timerb'),
            'required' => false,
            'value'    => $info['protocol']['timerb'],
            'default'  => $element['protocol']['sip']['timerb']['default'],
            'error'    => $this->bbf_args('error',
        $this->get_var('error', 'timerb')) )),

     $form->select(array('desc'  => $this->bbf('fm_protocol-registertrying'),
            'name'      => 'protocol[registertrying]',
            'labelid'   => 'protocol-registertrying',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-registertrying'),
            'selected'  => $info['protocol']['registertrying'],
            'default'   => $element['protocol']['sip']['registertrying']['default']),
         $element['protocol']['sip']['registertrying']['value']),

     $form->select(array('desc'  => $this->bbf('fm_protocol-ignoresdpversion'),
            'name'      => 'protocol[ignoresdpversion]',
            'labelid'   => 'protocol-ignoresdpversion',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-ignoresdpversion'),
            'selected'  => $info['protocol']['ignoresdpversion'],
            'default'   => $element['protocol']['sip']['ignoresdpversion']['default']),
         $element['protocol']['sip']['ignoresdpversion']['value']),

     $form->select(array('desc'  => $this->bbf('fm_protocol-session-timers'),
            'name'    => 'protocol[session-timers]',
            'labelid' => 'protocol-session-timers',
            'key'   => false,
            'empty' => true,
            'bbf'   => 'fm_protocol-session-timers-opt',
            'bbfopt'  => array('argmode' => 'paramvalue'),
            'help'    => $this->bbf('hlp_fm_protocol-session-timers'),
            'selected'  => $info['protocol']['session-timers'],
            'default' => $element['protocol']['sip']['session-timers']['default']),
         $element['protocol']['sip']['session-timers']['value']),

   $form->select(array('desc'  => $this->bbf('fm_protocol-session-expires'),
            'name'     => 'protocol[session-expires]',
            'labelid'  => 'protocol-session-expires',
            'key'      => false,
            'empty'    => true,
            'help'     => $this->bbf('hlp_fm_protocol-session-expires'),
            'selected' => $info['protocol']['session-expires'],
            'default'  => $element['protocol']['sip']['session-expires']['default']),
        $element['protocol']['sip']['session-expires']['value']),

    $form->select(array('desc'  => $this->bbf('fm_protocol-session-minse'),
            'name'     => 'protocol[session-minse]',
            'labelid'  => 'protocol-session-minse',
            'key'      => false,
            'empty'    => true,
            'help'     => $this->bbf('hlp_fm_protocol-session-minse'),
            'selected' => $info['protocol']['session-minse'],
            'default'  => $element['protocol']['sip']['session-minse']['default']),
        $element['protocol']['sip']['session-minse']['value']),

     $form->select(array('desc'  => $this->bbf('fm_protocol-session-refresher'),
            'name'    => 'protocol[session-refresher]',
            'labelid' => 'protocol-session-refresher',
            'key'   => false,
            'empty' => true,
            'bbf'   => 'fm_protocol-session-refresher-opt',
            'bbfopt'  => array('argmode' => 'paramvalue'),
            'help'    => $this->bbf('hlp_fm_protocol-session-refresher'),
            'selected'  => $info['protocol']['session-refresher'],
            'default' => $element['protocol']['sip']['session-refresher']['default']),
         $element['protocol']['sip']['session-refresher']['value']),

     $form->select(array('desc'  => $this->bbf('fm_protocol-use_q850_reason'),
            'name'      => 'protocol[use_q850_reason]',
            'labelid'   => 'protocol-use_q850_reason',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-use_q850_reason'),
            'selected'  => $info['protocol']['use_q850_reason'],
            'default'   => $element['protocol']['sip']['use_q850_reason']['default']),
         $element['protocol']['sip']['use_q850_reason']['value']),

     $form->select(array('desc'  => $this->bbf('fm_protocol-snom_aoc_enabled'),
            'name'      => 'protocol[snom_aoc_enabled]',
            'labelid'   => 'protocol-snom_aoc_enabled',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-snom_aoc_enabled'),
            'selected'  => $info['protocol']['snom_aoc_enabled'],
            'default'   => $element['protocol']['sip']['snom_aoc_enabled']['default']),
         $element['protocol']['sip']['snom_aoc_enabled']['value']),

    $form->text(array('desc'  => $this->bbf('fm_protocol-disallowed_methods'),
            'name'     => 'protocol[disallowed_methods]',
            'labelid'  => 'protocol-disallowed_methods',
            'size'     => 35,
            'help'     => $this->bbf('hlp_fm_protocol-disallowed_methods'),
            'required' => false,
            'value'    => $info['protocol']['disallowed_methods'],
            'error'    => $this->bbf_args('error',
        $this->get_var('error', 'disallowed_methods')) )),

    $form->select(array('desc'  => $this->bbf('fm_protocol-maxforwards'),
            'name'     => 'protocol[maxforwards]',
            'labelid'  => 'protocol-maxforwards',
            'key'      => false,
						'empty'    => true,
            'help'     => $this->bbf('hlp_fm_protocol-maxforwards'),
            'selected' => $info['protocol']['maxforwards'],
            'default'  => $element['protocol']['sip']['maxforwards']['default']),
        $element['protocol']['sip']['maxforwards']['value']),

		$form->checkbox(array('desc'	=> $this->bbf('fm_codec-custom'),
				      'name'	=> 'codec-active',
				      'labelid'	=> 'codec-active',
				      'checked'	=> $codec_active),
				'onclick="xivo_chg_attrib(\'ast_fm_user_codec\',
							  \'it-protocol-disallow\',
							  Number((this.checked === false)));"'),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_codec-disallow'),
				    'name'	=> 'protocol[disallow]',
				    'labelid'	=> 'protocol-disallow',
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_codec-disallow-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue')),
			      $element['protocol']['sip']['disallow']['value']);
?>

<div id="codeclist" class="fm-paragraph fm-multilist">
	<p>
		<label id="lb-codeclist" for="it-codeclist" onclick="dwho_eid('it-codeclist').focus();">
			<?=$this->bbf('fm_protocol_codec-allow');?>
		</label>
		<?=$form->input_for_ms('codeclist',$this->bbf('ms_seek'))?>
	</p>
	<div class="slt-outlist">
<?php
		echo	$form->select(array('name'	=> 'codeclist',
					    'label'	=> false,
					    'id'	=> 'it-codeclist',
					    'multiple'	=> true,
					    'size'	=> 5,
					    'paragraph'	=> false,
					    'key'	=> false,
					    'bbf'	=> 'ast_codec_name_type',
					    'bbfopt'	=> array('argmode' => 'paramvalue')),
				      $element['protocol']['sip']['allow']['value']);
?>
	</div>

	<div class="inout-list">
		<a href="#"
		   onclick="dwho.form.move_selected('it-codeclist',
						  'it-codec');
			    return(dwho.dom.free_focus());"
		   title="<?=$this->bbf('bt_incodec');?>">
			<?=$url->img_html('img/site/button/arrow-left.gif',
					  $this->bbf('bt_incodec'),
					  'class="bt-inlist" id="bt-incodec" border="0"');?></a><br />
		<a href="#"
		   onclick="dwho.form.move_selected('it-codec',
						  'it-codeclist');
			    return(dwho.dom.free_focus());"
		   title="<?=$this->bbf('bt_outcodec');?>">
			<?=$url->img_html('img/site/button/arrow-right.gif',
					  $this->bbf('bt_outcodec'),
					  'class="bt-outlist" id="bt-outcodec" border="0"');?></a>
	</div>

	<div class="slt-inlist">
<?php
		echo	$form->select(array('name'	=> 'protocol[allow][]',
					    'label'	=> false,
					    'id'	=> 'it-codec',
					    'multiple'	=> true,
					    'size'	=> 5,
					    'paragraph'	=> false,
					    'key'	=> false,
					    'bbf'	=> 'ast_codec_name_type',
					    'bbfopt'	=> array('argmode' => 'paramvalue')),
				      $allow);
?>
		<div class="bt-updown">
			<a href="#"
			   onclick="dwho.form.order_selected('it-codec',1);
				    return(dwho.dom.free_focus());"
			   title="<?=$this->bbf('bt_upcodec');?>">
				<?=$url->img_html('img/site/button/arrow-up.gif',
						  $this->bbf('bt_upcodec'),
						  'class="bt-uplist" id="bt-upcodec" border="0"');?></a><br />
			<a href="#"
			   onclick="dwho.form.order_selected('it-codec',-1);
				    return(dwho.dom.free_focus());"
			   title="<?=$this->bbf('bt_downcodec');?>">
				<?=$url->img_html('img/site/button/arrow-down.gif',
						  $this->bbf('bt_downcodec'),
						  'class="bt-downlist" id="bt-downcodec" border="0"');?></a>
		</div>
	</div>
</div>
<div class="clearboth"></div>

</div>

<div id="sb-part-t38" class="b-nodisplay">
<?php
	echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_t38pt-udptl'),
				    'name'	=> 'protocol[t38pt_udptl]',
				    'labelid'	=> 'protocol-t38pt-udptl',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['t38pt_udptl']['default'],
				    'selected'	=> $this->get_var('info','protocol','t38pt_udptl')),
			      $element['protocol']['sip']['t38pt_udptl']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_t38pt-usertpsource'),
				    'name'	=> 'protocol[t38pt_usertpsource]',
				    'labelid'	=> 'protocol-t38pt-usertpsource',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['t38pt_usertpsource']['default'],
				    'selected'	=> $this->get_var('info','protocol','t38pt_usertpsource')),
			      $element['protocol']['sip']['t38pt_usertpsource']['value']);
?>
</div>

<div id="sb-part-advanced" class="b-nodisplay">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_protocol_callerid'),
				  'name'	=> 'protocol[callerid]',
				  'labelid'	=> 'protocol-callerid',
				  'value'	=> $this->get_var('info','protocol','callerid'),
				  'size'	=> 15,
				  'notag'	=> false,
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'callerid')) )),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_insecure'),
				    'name'	=> 'protocol[insecure]',
				    'labelid'	=> 'protocol-insecure',
				    'empty'	=> true,
				    'bbf'	=> 'fm_protocol_insecure-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['insecure']['default'],
				    'selected'	=> $this->get_var('info','protocol','insecure')),
			      $element['protocol']['sip']['insecure']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_host-type'),
				    'name'	=> 'protocol[host-type]',
				    'labelid'	=> 'protocol-host-type',
				    'key'	=> false,
				    'bbf'	=> 'fm_protocol_host-type-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'selected'	=> ($host_static === true ? 'static' : $host)),
			      $element['protocol']['sip']['host-type']['value']),

		$form->text(array('desc'	=> '&nbsp;',
				  'name'	=> 'protocol[host-static]',
				  'labelid'	=> 'protocol-host-static',
				  'size'	=> 15,
				  'value'	=> ($host_static === true ? $host : ''),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'host-static')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_permit'),
				  'name'	=> 'protocol[permit]',
				  'labelid'	=> 'protocol-permit',
				  'size'	=> 20,
				  'value'	=> $this->get_var('info','protocol','permit'),
				  'error'   => $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'permit')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_deny'),
				  'name'	=> 'protocol[deny]',
				  'labelid'	=> 'protocol-deny',
				  'size'	=> 20,
				  'value'	=> $this->get_var('info','protocol','deny'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'deny')) )),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_trustrpid'),
				    'name'	=> 'protocol[trustrpid]',
				    'labelid'	=> 'protocol-trustrpid',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['trustrpid']['default'],
				    'selected'	=> $this->get_var('info','protocol','trustrpid')),
			      $element['protocol']['sip']['trustrpid']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_sendrpid'),
				    'name'	=> 'protocol[sendrpid]',
				    'labelid'	=> 'protocol-sendrpid',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['sendrpid']['default'],
				    'selected'	=> $this->get_var('info','protocol','sendrpid')),
			      $element['protocol']['sip']['sendrpid']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_allowsubscribe'),
				    'name'	=> 'protocol[allowsubscribe]',
				    'labelid'	=> 'protocol-allowsubscribe',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['allowsubscribe']['default'],
				    'selected'	=> $this->get_var('info','protocol','allowsubscribe')),
			      $element['protocol']['sip']['allowsubscribe']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_allowoverlap'),
				    'name'	=> 'protocol[allowoverlap]',
				    'labelid'	=> 'protocol-allowoverlap',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['allowoverlap']['default'],
				    'selected'	=> $this->get_var('info','protocol','allowoverlap')),
			      $element['protocol']['sip']['allowoverlap']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_promiscredir'),
				    'name'	=> 'protocol[promiscredir]',
				    'labelid'	=> 'protocol-promiscredir',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['promiscredir']['default'],
				    'selected'	=> $this->get_var('info','protocol','promiscredir')),
			      $element['protocol']['sip']['promiscredir']['value']),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_usereqphone'),
				    'name'	=> 'protocol[usereqphone]',
				    'labelid'	=> 'protocol-usereqphone',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['usereqphone']['default'],
				    'selected'	=> $this->get_var('info','protocol','usereqphone')),
			      $element['protocol']['sip']['usereqphone']['value']),

     $form->select(array('desc'  => $this->bbf('fm_protocol-directmedia'),
            'name'    => 'protocol[directmedia]',
            'labelid' => 'protocol-directmedia',
            'key'   => false,
            'empty' => true,
            'bbf'   => 'fm_protocol-directmedia-opt',
            'bbfopt'  => array('argmode' => 'paramvalue'),
            'help'    => $this->bbf('hlp_fm_protocol-directmedia'),
            'selected'  => $info['protocol']['directmedia'],
            'default' => $element['protocol']['sip']['directmedia']['default']),
         $element['protocol']['sip']['directmedia']['value']),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_fromuser'),
				  'name'	=> 'protocol[fromuser]',
				  'labelid'	=> 'protocol-fromuser',
				  'size'	=> 15,
				  'default'	=> $element['protocol']['sip']['fromuser']['default'],
				  'value'	=> $this->get_var('info','protocol','fromuser'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'fromuser')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_fromdomain'),
				  'name'	=> 'protocol[fromdomain]',
				  'labelid'	=> 'protocol-fromdomain',
				  'size'	=> 15,
				  'default'	=> $element['protocol']['sip']['fromdomain']['default'],
				  'value'	=> $this->get_var('info','protocol','fromdomain'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'fromdomain')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_keepalive'),
				  'name'	=> 'protocol[keepalive]',
				  'labelid'	=> 'protocol-keepalive',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','keepalive'),
				  'help'		=> $this->bbf('hlp_fm_keepalive'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'keepalive')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_imageversion'),
				  'name'	=> 'protocol[imageversion]',
				  'labelid'	=> 'protocol-imageversion',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','imageversion'),
				  'help'		=> $this->bbf('hlp_fm_imageversion'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'imageversion')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_secondary_dialtone_tone'),
				  'name'	=> 'protocol[secondary_dialtone_tone]',
				  'labelid'	=> 'protocol-secondary_dialtone_tone',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','secondary_dialtone_tone'),
				  'help'		=> $this->bbf('hlp_fm_dialtone_tone'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'secondary_dialtone_tone')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_audio_tos'),
				  'name'	=> 'protocol[audio_tos]',
				  'labelid'	=> 'protocol-audio_tos',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','audio_tos'),
				  'help'		=> $this->bbf('hlp_fm_audio_tos'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'audio_tos')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_audio_cos'),
				  'name'	=> 'protocol[audio_cos]',
				  'labelid'	=> 'protocol-audio_cos',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','audio_cos'),
				  'help'		=> $this->bbf('hlp_fm_audio_cos'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'audio_cos')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_video_tos'),
				  'name'	=> 'protocol[video_tos]',
				  'labelid'	=> 'protocol-video_tos',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','video_tos'),
				  'help'		=> $this->bbf('hlp_fm_video_tos'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'video_tos')) )),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_video_cos'),
				  'name'	=> 'protocol[video_cos]',
				  'labelid'	=> 'protocol-video_cos',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','video_cos'),
				  'help'		=> $this->bbf('hlp_fm_video_cos'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'video_cos')) )),

		$form->select(array('desc'	=> $this->bbf('fm_protocol_useclientcode'),
				    'name'	=> 'protocol[useclientcode]',
				    'labelid'	=> 'protocol-useclientcode',
				    'empty'	=> true,
				    'key'	=> false,
				    'bbf'	=> 'fm_bool-opt',
				    'bbfopt'	=> array('argmode' => 'paramvalue'),
				    'default'	=> $element['protocol']['sip']['useclientcode']['default'],
				    'selected'	=> $this->get_var('info','protocol','useclientcode')),
			      $element['protocol']['sip']['useclientcode']['value']),

		$form->text(array('desc'	=> $this->bbf('fm_protocol_adhocnumber'),
				  'name'	=> 'protocol[adhocnumber]',
				  'labelid'	=> 'protocol-adhocnumber',
				  'size'	=> 15,
				  'value'	=> $this->get_var('info','protocol','adhocnumber'),
				  'help'		=> $this->bbf('hlp_fm_adhocnumber'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'protocol', 'adhocnumber')) )),

	// asterisk 1.8 fields
    $form->select(array('desc'  => $this->bbf('fm_protocol-transport'),
            		'name'      => 'protocol[transport]',
            		'labelid'   => 'protocol-transport',
					'key'       => false,
					'empty'     => true,
            		'help'      => $this->bbf('hlp_fm_protocol-transport'),
            		'selected'  => $info['protocol']['transport'],
					'default'   => $element['protocol']['sip']['transport']['default'],
					'error'     => $this->bbf_args('error', $this->get_var('error','protocol','transport'))),
         $element['protocol']['sip']['transport']['value']),

    $form->select(array('desc'  => $this->bbf('fm_protocol-callcounter'),
            'name'      => 'protocol[callcounter]',
            'labelid'   => 'protocol-callcounter',
            'key'       => false,
            'empty'     => true,
            'bbf'       => 'fm_bool-opt',
            'bbfopt'    => array('argmode' => 'paramvalue'),
            'help'      => $this->bbf('hlp_fm_protocol-callcounter'),
            'selected'  => $info['protocol']['callcounter'],
            'default'   => $element['protocol']['sip']['callcounter']['default']),
         $element['protocol']['sip']['callcounter']['value']),

    $form->select(array('desc'  => $this->bbf('fm_protocol-busylevel'),
            'name'     => 'protocol[busylevel]',
            'labelid'  => 'protocol-busylevel',
            'key'      => false,
						'empty'    => true,
            'help'     => $this->bbf('hlp_fm_protocol-busylevel'),
            'selected' => $info['protocol']['busylevel'],
            'default'  => $element['protocol']['sip']['busylevel']['default']),
        $element['protocol']['sip']['busylevel']['value']),

    $form->text(array('desc'  => $this->bbf('fm_protocol-contactpermit'),
            'name'     => 'protocol[contactpermit]',
            'labelid'  => 'protocol-contactpermit',
            'size'     => 25,
            'help'     => $this->bbf('hlp_fm_protocol-contactpermit'),
            'required' => false,
            'value'    => $info['protocol']['contactpermit'],
            'default'  => $element['protocol']['sip']['contactpermit']['default'],
            'error'    => $this->bbf_args('error',
        $this->get_var('error', 'contactpermit')) )),

    $form->text(array('desc'  => $this->bbf('fm_protocol-contactdeny'),
            'name'     => 'protocol[contactdeny]',
            'labelid'  => 'protocol-contactdeny',
            'size'     => 25,
            'help'     => $this->bbf('hlp_fm_protocol-contactdeny'),
            'required' => false,
            'value'    => $info['protocol']['contactdeny'],
            'default'  => $element['protocol']['sip']['contactdeny']['default'],
            'error'    => $this->bbf_args('error',$this->get_var('error', 'contactdeny')) )),

	    $form->text(array('desc'  => $this->bbf('fm_protocol-unsolicited_mailbox'),
            'name'     => 'protocol[unsolicited_mailbox]',
            'labelid'  => 'protocol-unsolicited_mailbox',
            'size'     => 25,
            'help'     => $this->bbf('hlp_fm_protocol-unsolicited_mailbox'),
            'required' => false,
            'value'    => $info['protocol']['unsolicited_mailbox'],
            'error'    => $this->bbf_args('error',$this->get_var('error', 'unsolicited_mailbox')) ));

?>
</div>