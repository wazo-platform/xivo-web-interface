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
$dhtml = &$this->get_module('dhtml');

$info = $this->get_var('info');
$element = $this->get_var('element');
$error = $this->get_var('error');

$allow = $info['sccpgeneralsettings']['allow'];
$codec_active = empty($allow) === false;

?>

<div class="b-infos b-form">
	<h3 class="sb-top xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center"><?=$this->bbf('title_content_name');?></span>
		<span class="span-right">&nbsp;</span>
	</h3>

	<div class="sb-menu">
	</div>

	<div class="sb-content">
		<form action="#" method="post" accept-charset="utf-8">

		<?php
			echo $form->checkbox(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_directmedia'),
				'name'      => 'sccpgeneralsettings[directmedia]',
				'labelid'   => 'sccpgeneralsettings-directmedia',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_directmedia'),
				'checked'   => $info['sccpgeneralsettings']['directmedia'],
				'error'		=> $this->bbf_args('error',
							$this->get_var('error', 'sccpgeneralsettings', 'directmedia')) )),

			$form->text(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_dialtimeout'),
				'name'      => 'sccpgeneralsettings[dialtimeout]',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_dialtimeout'),
				'labelid'   => 'sccpgeneralsettings-dialtimeout',
				'size'      => 4,
				'default'   => $element['sccpgeneralsettings']['dialtimeout']['default'],
				'value'     => $info['sccpgeneralsettings']['dialtimeout'],
				'error'		=> $this->bbf_args('error',
							$this->get_var('error', 'sccpgeneralsettings', 'dialtimeout')))),

			$form->text(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_keepalive'),
				'name'      => 'sccpgeneralsettings[keepalive]',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_keepalive'),
				'labelid'   => 'sccpgeneralsettings-keepalive',
				'size'      => 4,
				'default'   => $element['sccpgeneralsettings']['keepalive']['default'],
				'value'     => $info['sccpgeneralsettings']['keepalive'],
				'error'		=> $this->bbf_args('error',
						$this->get_var('error', 'sccpgeneralsettings', 'keepalive')))),

			$form->select(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_language'),
				'name'      => 'sccpgeneralsettings[language]',
				'labelid'   => 'sccpgeneralsettings-language',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_language'),
				'key'       => false,
				'default'   => $element['sccpgeneralsettings']['language']['default'],
				'selected'  => $info['sccpgeneralsettings']['language']),
				$this->get_var('language_list')),

			$form->checkbox(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_guest'),
				'name'      => 'sccpgeneralsettings[guest]',
				'labelid'   => 'sccpgeneralsettings-guest',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_guest'),
				'checked'   => $info['sccpgeneralsettings']['guest'],
				'error'		=> $this->bbf_args('error',
							$this->get_var('error', 'sccpgeneralsettings', 'guest')) )),

			$form->text(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_max_guests'),
				'name'      => 'sccpgeneralsettings[max_guests]',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_max_guests'),
				'labelid'   => 'sccpgeneralsettings-max_guests',
				'size'      => 4,
				'default'   => $element['sccpgeneralsettings']['max_guests']['default'],
				'value'     => $info['sccpgeneralsettings']['max_guests'],
				'error'		=> $this->bbf_args('error',
							$this->get_var('error', 'sccpgeneralsettings', 'max_guests'))));

		?>

		<fieldset id="fld-codeclist">
			<legend><?=$this->bbf('fld-codeclist');?></legend>
		<?php
			echo $form->checkbox(array(
				'desc'		=> $this->bbf('fm_codec-custom'),
				'name'		=> 'codec-active',
				'labelid'	=> 'codec-active',
				'checked'	=> $codec_active));
		?>
		<div id="codeclist">
		<?php
			echo $form->select(array(
				'desc'		=> $this->bbf('fm_codec-disallow'),
				'name'		=> 'disallow',
				'labelid'	=> 'disallow',
				'key'		=> false,
				'bbf'		=> 'fm_codec-disallow-opt',
				'bbfopt'	=> array('argmode' => 'paramvalue')),
					$element['sccpgeneralsettings']['disallow']['value']);
		?>
			<div class="fm-paragraph fm-description">
				<?=$form->jq_select(array(
						'paragraph'	=> false,
						'label'		=> false,
						'name'		=> 'allow[]',
						'id' 		=> 'it-allow',
						'key'		=> false,
						'bbf'		=> 'ast_codec_name_type',
						'bbfopt'	=> array('argmode' => 'paramvalue'),
						'selected'  => $allow),
							$element['sccpgeneralsettings']['allow']['value']);?>
			<div class="clearboth"></div>
			</div>
		</div>
		</fieldset>

		<?php
				echo $form->hidden(array(
					'name'	=> DWHO_SESS_NAME,
					'value'	=> DWHO_SESS_ID)),

				$form->hidden(array(
					'name'	=> 'fm_send',
					'value'	=> 1)),

				$form->submit(array(
					'name'	=> 'submit',
					'id'	=> 'it-submit',
					'value'	=> $this->bbf('fm_bt-save')));
		?>

		</form>
	</div>

	<div class="sb-foot xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center">&nbsp;</span>
		<span class="span-right">&nbsp;</span>
	</div>

</div>
