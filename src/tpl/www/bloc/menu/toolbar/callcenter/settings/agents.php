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
$group = $this->get_var('group');

$search = (string) $this->get_var('search');

$param = array('act' => 'addagent');

if($act !== 'list' && $act !== 'add'):
	$param['group'] = $group;
else:
	$group = '';
endif;

if($act === 'list'):
	$toolbar_js = array();
	$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-agentgroups-list\';';
	$toolbar_js[] = 'var xivo_toolbar_form_list = \'agentgroups[]\';';
	$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

	$dhtml->write_js($toolbar_js);
elseif($act === 'listagent'):
	$toolbar_js = array();
	$toolbar_js[] = 'var xivo_toolbar_fm_search = \''.$dhtml->escape($search).'\';';
	$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-agents-list\';';
	$toolbar_js[] = 'var xivo_toolbar_form_list = \'agents[]\';';
	$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

	$dhtml->write_js($toolbar_js);
endif;

?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<form action="#" method="post" accept-charset="utf-8">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'act',
				    'value'	=> $act));
?>
<toolbar-search display-on='listagent'></toolbar-search>
</form>

<div class="form-group form-inline">
	<toolbar-buttons actions="['add', 'addagent']" page='agents'></toolbar-buttons>

	<toolbar-buttons actions-adv="['toolbar-advanced-menu-select-all', 'toolbar-advanced-menu-delete']"
		display-adv-on="list" page='agents'></toolbar-buttons>

	<toolbar-buttons actions-adv="['toolbar-advanced-menu-select-all', 'toolbar-advanced-menu-delete-agents']"
		display-adv-on="listagent" page='agents'></toolbar-buttons>
</div>

<script type="text/javascript">
dwho.dom.set_onload(function()
{
	dwho.dom.add_event('change',
			   dwho_eid('it-toolbar-linked'),
			   function(e)
			   {
				if(xivo_toolbar_fm_search === ''
				&& dwho_has_len(dwho.form.text_helper['it-toolbar-search']) === false)
					this.form['search'].value = '';

				this.form.submit();
			   });
});
</script>
