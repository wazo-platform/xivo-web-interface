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
$element = $this->get_var('element');

?>
<div class="b-infos b-form">
	<breadcrumb
		parent="<?=$this->bbf('title_parent_name');?>"
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>

	<div class="sb-content">
		<form class="form-horizontal" action="#" method="post" accept-charset="utf-8">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1)),

		$form->hidden(array('name'	=> 'act',
				    'value'	=> 'add')),

		$form->text(array('desc'	=> $this->bbf('fm_category'),
				  'name'	=> 'category',
				  'labelid'	=> 'category',
				  'help'		=> $this->bbf('hlp_fm_category'),
				  'size'	=> 15,
				  'default'	=> $element['category']['default'],
				  'value'	=> $this->get_var('info','category'))),

		$form->select(array('desc'	=> $this->bbf('fm_mode'),
				    'name'	=> 'mode',
				    'labelid'	=> 'mode',
				    'key'	=> false,
				    'default'	=> $element['mode']['default'],
				    'selected'	=> $this->get_var('info','mode')),
			      $element['mode']['value'],
			      'onchange="xivo_chg_attrib(\'fm_musiconhold\',
							 \'fd-application\',
							 Number(this.value === \'custom\'));"'),

		$form->text(array('desc'	=> $this->bbf('fm_application'),
				  'name'	=> 'application',
				  'labelid'	=> 'application',
				  'size'	=> 15,
				  'default'	=> $element['application']['default'],
				  'value'	=> $this->get_var('info','application'))),

		$form->checkbox(array('desc'	=> $this->bbf('fm_random'),
				      'name'	=> 'random',
				      'labelid'	=> 'random',
				      'default'	=> $element['random']['default'],
				      'checked' => $this->get_var('info','random'))),

		$form->submit(array('name'	=> 'submit',
				    'id'	=> 'it-submit',
				    'value'	=> $this->bbf('fm_bt-save')));
?>
		</form>
	</div>
	<div class="sb-foot xspan"></div>
</div>
