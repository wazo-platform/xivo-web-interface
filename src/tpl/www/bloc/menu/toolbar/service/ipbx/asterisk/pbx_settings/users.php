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

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_fm_search = \''.$dhtml->escape($search).'\';';
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-users-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'users[]\';';
$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

$dhtml->write_js($toolbar_js);

?>

<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<div class="toolbar">
	<form action="#" method="post" accept-charset="utf-8">
	<?php
		echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
					    'value'	=> DWHO_SESS_ID)),

			$form->hidden(array('name'	=> 'act',
					    'value'	=> 'list'));
	?>
		<div class="fm-paragraph">
			<div class="form-group form-inline pull-right">
				 <div class="input-group">
				 			 <!-- TODO move in directive and allow pre filtering with dropdown as suggested below
							 <div class="input-group-btn">

						 <div class="btn-group">
								 <button class="btn btn-default btn-dropdown dropdown-toggle" type="button" data-toggle="dropdown">
										 <span data-bind="label" id="searchLabel">Search By</span><span class="caret"></span>
								 </button>
								 <ul class="dropdown-menu" role="menu">
										 <li><a href="#">1</a></li>
										 <li><a href="#">2</a></li>
										 <li><a href="#">3</a></li>
								 </ul>
						 </div>
				 </div> -->
				 <input type="search" name="search" id="it-toolbar-search" class="form-control input-search" size=24 placeholder="<?= $this->bbf('toolbar_fm_search') ?>"/>
				 <span class="input-group-btn">
						 <button id="'it-toolbar-subsearch" name="submit" class="btn btn-default btn-search btn-block" title="<?= $this->bbf('toolbar_fm_search') ?>">
								 <span class="glyphicon glyphicon-search"></span>
						 </button>
				 </span>
				 </div>
			</div>
	</form>

	<div class="form-group">
		<div class="btn-group sb-advanced-menu">
			<button type="button" class="glyphicon glyphicon-plus btn btn-default btn-action dropdown-toggle" data-toggle="dropdown"
			aria-haspopup="true" aria-expanded="false" id="toolbar-bt-add">
			</button>
			<ul id="toolbar-add-menu" class="dropdown-menu">
				<li><?=$url->href_html($this->bbf('toolbar_add_menu_add'),
						       'service/ipbx/pbx_settings/users',
						       'act=add');?></li>
				<li><?=$url->href_html($this->bbf('toolbar_add_menu_import-file'),
						       'service/ipbx/pbx_settings/users',
						       'act=import');?></li>
				<li><?=$url->href_html($this->bbf('toolbar_add_menu_update_import'),
						       'service/ipbx/pbx_settings/users',
						       'act=update_import');?></li>
				<li><?=$url->href_html($this->bbf('toolbar_add_menu_export'),
						       'service/ipbx/pbx_settings/users',
						       'act=export');?></li>
			</ul>
		</div>
	<?php
	/* TODO disable on edit
	if($act === 'list'):
		echo	$url->img_html('img/menu/top/toolbar/bt-more.gif',
				       $this->bbf('toolbar_opt_advanced'),
				       'id="toolbar-bt-advanced"
					border="0"');
					*/
	?>
		<div class="btn-group sb-advanced-menu">
			<button type="button" class="glyphicon glyphicon-chevron-down btn btn-default btn-action dropdown-toggle" data-toggle="dropdown"
			aria-haspopup="true" aria-expanded="false" id="toolbar-bt-advanced">
			</button>
			<ul id="toolbar-advanced-menu" class="dropdown-menu">
				<li>
					<a href="#" id="toolbar-advanced-menu-enable"><?=$this->bbf('toolbar_adv_menu_enable');?></a>
				</li>
				<li>
					<a href="#" id="toolbar-advanced-menu-disable"><?=$this->bbf('toolbar_adv_menu_disable');?></a>
				</li>
				<li>
					<a href="#" id="toolbar-advanced-menu-select-all"><?=$this->bbf('toolbar_adv_menu_select-all');?></a>
				</li>
				<li>
					<a href="#" id="toolbar-advanced-menu-delete"><?=$this->bbf('toolbar_adv_menu_delete');?></a>
				</li>
			</ul>
		</div>
	</div>
</div>
<?php
//endif;
?>
