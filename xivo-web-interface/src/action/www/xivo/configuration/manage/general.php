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
# This program is distributed in the hope tgeneralt it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should generalve received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

$ipbx = &$_SRE->get('ipbx');
$ctimain = &$ipbx->get_module('ctimain');

$fm_save = null;
if(isset($_QR['fm_send']) === true)
{
	$fm_save = false;
	$ret = 0;

	$live_reload_conf = 1;
	if (!isset($_QR['cti']['live_reload_conf']))
		$live_reload_conf = 0;

	$_QR['cti'] = $ctimain->get(1);
	$_QR['cti']['live_reload_conf'] = $live_reload_conf;

	if(($rs = $ctimain->chk_values($_QR['cti'])) === false)
		dwho_report::push('error', $ctimain->get_filter_error());
	else
		$ret = $ctimain->edit(1, $rs);

	if($ret == 1)
		$fm_save = true;
}
$info = array();
$info['ctimain'] = $ctimain->get(1);
$element = array();
$element['ctimain'] = $ctimain->get_element();

$_TPL->set_var('element',$element);
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