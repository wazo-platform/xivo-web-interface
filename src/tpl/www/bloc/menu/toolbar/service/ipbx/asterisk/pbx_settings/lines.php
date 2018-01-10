<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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
$context = (string) $this->get_var('context');
$contexts = $this->get_var('contexts');
$context_js = $dhtml->escape($context);

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_fm_search = \''.$dhtml->escape($search).'\';';
$toolbar_js[] = 'var xivo_toolbar_fm_context = \''.$context_js.'\';';
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-lines-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'lines[]\';';
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

	<div class="form-group form-inline">
		<toolbar-buttons actions="['add\&proto=sip','add\&proto=custom']"
			actions-adv="['toolbar-advanced-menu-enable', 'toolbar-advanced-menu-disable', 'toolbar-advanced-menu-select-all', 'toolbar-advanced-menu-delete']"
			display-adv-on="list" page="lines"></toolbar-buttons>

		<?php
				echo $form->select(array('name'	=> 'context',
						    'id'	=> 'it-toolbar-context',
						    'paragraph'	=> false,
						    'empty'	=> $this->bbf('toolbar_fm_context'),
						    'selected'	=> $context),
					      dwho_array_same_key_val($contexts, 'name'),
					      'style="margin-left: 20px;"');
		?>
	</div>
</form>
