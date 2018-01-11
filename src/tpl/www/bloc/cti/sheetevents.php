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

$url = &$this->get_module('url');
$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');

$info = $this->get_var('info');
$element = $this->get_var('element');

$error = $this->get_var('error');
$sheetactionslist = $this->get_var('sheetactionslist');

if(isset($error_js[0]) === true)
	$dhtml->write_js($error_js);

?>
<div class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>
<div class="sb-content">
<form action="#" method="post" accept-charset="utf-8" class="form-horizontal">
<?php
	echo
		$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),
		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1)),

		$form->select(array('desc'	=> $this->bbf('fm_sheetevents_dial'),
				    'name'	=> 'ctisheetevents[dial]',
				    'labelid'	=> 'dial',
				    'key'	=> false,
				    'default'	=> $element['ctisheetevents']['dial']['default'],
			      	'selected' => $info['ctisheetevents']['dial']),
				$sheetactionslist),

		$form->select(array('desc'	=> $this->bbf('fm_sheetevents_link'),
				    'name'	=> 'ctisheetevents[link]',
				    'labelid'	=> 'link',
				    'key'	=> false,
				    'default'	=> $element['ctisheetevents']['link']['default'],
			      	'selected' => $info['ctisheetevents']['link']),
				$sheetactionslist),

		$form->select(array('desc'	=> $this->bbf('fm_sheetevents_unlink'),
				    'name'	=> 'ctisheetevents[unlink]',
				    'labelid'	=> 'unlink',
				    'key'	=> false,
				    'default'	=> $element['ctisheetevents']['unlink']['default'],
			      	'selected' => $info['ctisheetevents']['unlink']),
                                $sheetactionslist),

		$form->select(array('desc'	=> $this->bbf('fm_sheetevents_incomingdid'),
				    'name'	=> 'ctisheetevents[incomingdid]',
				    'labelid'	=> 'incomingdid',
				    'key'	=> false,
				    'default'	=> $element['ctisheetevents']['incomingdid']['default'],
			      	'selected' => $info['ctisheetevents']['incomingdid']),
				$sheetactionslist),

        $form->select(array('desc'  => $this->bbf('fm_sheetevents_hangup'),
                    'name'  => 'ctisheetevents[hangup]',
                    'labelid'   => 'hangup',
                    'key'   => false,
                    'default'   => $element['ctisheetevents']['hangup']['default'],
                    'selected' => $info['ctisheetevents']['hangup']),
                $sheetactionslist);

	echo	$form->submit(array('name'	=> 'submit',
				    'id'	=> 'it-submit',
				    'value'	=> $this->bbf('fm_bt-save')));
?>
</form>

	</div>
	<div class="sb-foot xspan"></div>
</div>
