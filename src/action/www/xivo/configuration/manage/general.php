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
# This program is distributed in the hope tgeneralt it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should generalve received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

$RAPI = &dwho_gct::get('xivo_confd');
$config_api = $RAPI->get_ressource('configuration');

$fm_save = null;
if(isset($_QR['fm_send']) === true)
{
	$fm_save = false;
	$ret = 0;

	$live_reload_conf = true;
	if (!isset($_QR['live_reload']))
		$live_reload_conf = false;

	$ret = $config_api->edit('live_reload', array('enabled' => $live_reload_conf));

	if($ret == 1)
		$fm_save = true;
}
$info = array();
$info['live_reload'] = $config_api->get('live_reload');

$_TPL->set_var('fm_save', $fm_save);
$_TPL->set_var('info', $info);

$dhtml = &$_TPL->get_module('dhtml');
$dhtml->set_js('js/xivo/configuration/manage/general.js');

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/xivo/configuration');
$menu->set_toolbar('toolbar/xivo/configuration/manage/general');

$_TPL->set_bloc('main','xivo/configuration/manage/general');
$_TPL->set_struct('xivo/configuration');
$_TPL->display('index');

?>