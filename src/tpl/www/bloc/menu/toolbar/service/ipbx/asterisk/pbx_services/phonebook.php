<?php

#
# Copyright 2006-2017 The Wazo Authors  (see the AUTHORS file)
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

$search = (string) $this->get_var('search');
$act = $this->get_var('act');

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_fm_search = \''.$dhtml->escape($search).'\';';
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-phonebook-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'phonebook[]\';';
$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

$dhtml->write_js($toolbar_js);

?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<form action="#" method="post" accept-charset="utf-8">
<?php
	if($act === 'list_contacts'
	&& ($entity = $this->get_var('entity'))
	&& ($phonebook_id = (int)$this->get_var('phonebook_id'))) {
		echo $form->hidden(array('name' => 'entity', 'value' => $entity));
		echo $form->hidden(array('name' => 'phonebook', 'value' => $phonebook_id));
		$list_action = 'list_contacts';
		$add_params = array('act' => 'add_contact', 'entity' => $entity, 'phonebook' => $phonebook_id);
		$import_params = array('act' => 'import', 'entity' => $entity, 'phonebook' => $phonebook_id);
	} else {
		$list_action = 'list';
		$add_params = array('act' => 'add');
	}
	echo 	$form->hidden(array('name'	=> DWHO_SESS_NAME, 'value' => DWHO_SESS_ID)),
			$form->hidden(array('name' => 'act', 'value' => $list_action));
?>
	<div class="fm-paragraph">
<?php
		echo	$form->text(array('name'	=> 'search',
					  'id'		=> 'it-toolbar-search',
					  'size'	=> 20,
					  'paragraph'	=> false,
					  'value'	=> $search,
					  'default'	=> $this->bbf('toolbar_fm_search'))),

			$form->image(array('name'	=> 'submit',
					   'id'		=> 'it-subsearch',
					   'src'	=> $url->img('img/menu/top/toolbar/bt-search.gif'),
					   'paragraph'	=> false,
					   'alt'	=> $this->bbf('toolbar_fm_search')));
?>
	</div>
</form>
<?php
	echo    $url->href_html($url->img_html('img/menu/top/toolbar/bt-add.gif',
							$this->bbf('toolbar_opt_add'),
							'id="toolbar-bt-add"
							border="0"'),
							'service/ipbx/pbx_services/phonebook',
							$add_params,
							$this->bbf('toolbar_opt_add'));
	if ($act === 'list_contacts') {
		echo	$url->img_html('img/menu/top/toolbar/bt-more.gif',
				       $this->bbf('toolbar_opt_advanced'),
				       'id="toolbar-bt-advanced"
					border="0"');
?>
<div class="sb-advanced-menu">
	<ul id="toolbar-advanced-menu">
		<li><?=$url->href_html(
			$this->bbf('toolbar_add_menu_import-file'),
			'service/ipbx/pbx_services/phonebook',
			$import_params)?></li>
	</ul>
</div>
<?php
	}
?>
