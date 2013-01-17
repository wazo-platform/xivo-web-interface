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

$url = &$this->get_module('url');
$dhtml = &$this->get_module('dhtml');
$act = $this->get_var('act');

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-recording-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'recordings[]\';';
$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

$dhtml->write_js($toolbar_js);


?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<?php
if($act == 'listrecordings')
	display_search_zone($this);
if($act == 'list') {
	echo	$url->href_html($url->img_html('img/menu/top/toolbar/bt-add.gif',
					       $this->bbf('toolbar_opt_add'),
					       'id="toolbar-bt-add"
							border="0"'),
				'service/ipbx/call_management/recording',
				'act=add',
				null,
				$this->bbf('toolbar_opt_add'));
}

function display_search_zone($this_local) {
	$form = &$this_local->get_module('form');
	$url = &$this_local->get_module('url');
	$params = $this_local->get_var('params');
	?>
	<form action="#" method="post" accept-charset="utf-8">
	<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
			'value'	=> DWHO_SESS_ID)),
			$form->hidden(array('name'	=> 'act',
					'value'	=> 'listrecordings')),
			$form->hidden(array('name'	=> 'campaign',
					'value'	=> $params['campaign']));
			?>
		<div class="fm-paragraph">
	<?php
			echo	$form->text(array('name'	=> 'search',
						  'id'		=> 'it-toolbar-search',
						  'size'	=> 20,
						  'paragraph'	=> false,
						  'default'	=> $this_local->bbf('toolbar_fm_search'))),
	
				$form->image(array('name'	=> 'submit',
						   'id'		=> 'it-toolbar-subsearch',
						   'src'	=> $url->img('img/menu/top/toolbar/bt-search.gif'),
						   'paragraph'	=> false,
						   'alt'	=> $this_local->bbf('toolbar_fm_search'),
						   'help'	=> $this_local->bbf('help_search_recordings')));
	?>
		</div>
	</form>
<?php 
}
?>