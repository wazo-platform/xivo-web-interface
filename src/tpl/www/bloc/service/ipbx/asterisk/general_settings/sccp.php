<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Avencall
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
$error_nb = count($error['sccpgeneralsettings']);

foreach($error['sccpgeneralsettings'] as $field):
	$error_js[] = "dwho.form.error['it-sccpgeneralsettings-".$field."'] = true;";
endforeach;

if(isset($error_js[0]) === true)
	$dhtml->write_js($error_js);

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
				'checked'   => $info['sccpgeneralsettings']['directmedia'])),

			$form->text(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_dialtimeout'),
				'name'      => 'sccpgeneralsettings[dialtimeout]',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_dialtimeout'),
				'labelid'   => 'sccpgeneralsettings-dialtimeout',
				'size'      => 4,
				'default'   => $element['sccpgeneralsettings']['dialtimeout']['default'],
				'value'     => $info['sccpgeneralsettings']['dialtimeout'])),

			$form->select(array(
				'desc'      => $this->bbf('fm_sccpgeneralsettings_language'),
				'name'      => 'sccpgeneralsettings[language]',
				'labelid'   => 'sccpgeneralsettings-language',
				'help'      => $this->bbf('hlp_fm_sccpgeneralsettings_language'),
				'key'       => false,
				'default'   => $element['sccpgeneralsettings']['language']['default'],
				'selected'  => $info['sccpgeneralsettings']['language']),
				$this->get_var('language_list'));

		?>

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
