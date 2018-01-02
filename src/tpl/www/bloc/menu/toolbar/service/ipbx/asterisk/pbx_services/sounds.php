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
$dir = $this->get_var('dir');
$search = (string) $this->get_var('search');

$param = array('act' => 'add');

if($act !== 'listdir' && $act !== 'adddir'):
	$param['dir'] = $dir;
else:
	$dir = '';
endif;

if($dir === '' && $search === ''):
	$dirjs = '';
else:
	$dirjs = $dhtml->escape($dir);
endif;

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_fm_search = \''.$dhtml->escape($search).'\';';
$toolbar_js[] = 'var xivo_toolbar_fm_dir = \''.$dirjs.'\';';
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-files-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'files[]\';';
$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

$dhtml->write_js($toolbar_js);

?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<form action="#" method="post" accept-charset="utf-8">
	<?php
		echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
					    'value'	=> DWHO_SESS_ID)),

			$form->hidden(array('name'	=> 'act',
					    'value'	=> 'list'));
	?>
	<toolbar-search display-on='list'></toolbar-search>

	<div class="form-group form-inline">
		<toolbar-buttons actions="['adddir','add']"
			actions-adv="['toolbar-advanced-menu-select-all', 'toolbar-advanced-menu-delete']"
			display-adv-on="list" page="sounds"></toolbar-buttons>

		<?php
				echo	$form->select(array('name'	=> 'dir',
							    'id'	=> 'it-toolbar-directory',
							    'empty'	=> $this->bbf('toolbar_fm_directory'),
							    'key'	=> false,
							    'paragraph'	=> false,
							    'selected'	=> $dir),
						      $this->get_var('list_dirs'),
						      'style="margin-left: 20px;"');
		?>
	</div>
</form>
