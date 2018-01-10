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
$dhtml = &$this->get_module('dhtml');

$act = $this->get_var('act');

$search = (string) $this->get_var('search');
$linked = (string) $this->get_var('linked');
$linked_js = $dhtml->escape($linked);

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_fm_search = \''.$dhtml->escape($search).'\';';
$toolbar_js[] = 'var xivo_toolbar_fm_linked = \''.$linked_js.'\';';
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-devices-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'devices[]\';';
$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

$dhtml->write_js($toolbar_js);

?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<form action="#" method="post" accept-charset="utf-8" class="form-horizontal">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'act',
				    'value'	=> 'list'));
?>

	<toolbar-search></toolbar-search>
	<div class="pull-right">
	<?php

				echo $form->select(array('name'	=> 'search_column',
						    'id'		=> 'it-toolbar-column',
						    'paragraph'	=> false,
						    'selected'	=> $this->get_var('search_column')),
						array('all'	=> $this->bbf('search_all_fields_except_number'),
							  'number'	=> $this->bbf('only_number_field')),
						'style="margin-left: 10px;"');
	?>
	</div>
</form>
<toolbar-buttons actions="['add']"
	actions-adv="['toolbar-advanced-menu-autoprov', 'toolbar-advanced-menu-select-all', 'toolbar-advanced-menu-delete']"
	display-adv-on="list" page="devices"></toolbar-buttons>

<div id="autoprov_dialog" class="well">
	<form action="#">
		<b><?=$this->bbf('opt_synchronize_devices_confirm');?></b>
		<hr/>
		<div>
			<input type="submit" class='btn btn-primary' value="<?=$this->bbf('autoprov_validate');?>" onclick="return autoprov_validate();" />
			<input type="submit" class='btn btn-default' value="<?=$this->bbf('autoprov_cancel');?>" onclick="return autoprov_cancel();" />
		</div>
	</form>
</div>
