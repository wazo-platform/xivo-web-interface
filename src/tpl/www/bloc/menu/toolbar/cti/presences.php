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
$dhtml = &$this->get_module('dhtml');

$act = $this->get_var('act');
$idpresences = $this->get_var('idpresences');

$toolbar_js = array();
$toolbar_js[] = 'var xivo_toolbar_form_name = \'fm-presences-list\';';
$toolbar_js[] = 'var xivo_toolbar_form_list = \'presences[]\';';
$toolbar_js[] = 'var xivo_toolbar_adv_menu_delete_confirm = \''.$dhtml->escape($this->bbf('toolbar_adv_menu_delete_confirm')).'\';';

$dhtml->write_js($toolbar_js);

?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>
<?php
if($act == 'list')
{
	$ngact = "['add']";
}

if($act == 'liststatus')
{
	$ngact = "['addstatus']";
}
?>
<toolbar-buttons actions="<?="$ngact"?>"></toolbar-buttons>
