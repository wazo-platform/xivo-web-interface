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
$error = array();

$appsccpgeneralsettings = &$ipbx->get_application('sccpgeneralsettings');
$info = $appsccpgeneralsettings->get_options();
$element = $appsccpgeneralsettings->get_elements();

$fm_save = null;
if(isset($_QR['fm_send']) === true)
{
	$saved = $appsccpgeneralsettings->save_sccp_general_settings($_QR['sccpgeneralsettings']);
	$info = $appsccpgeneralsettings->get_result();

	$fm_save = true;

	if($saved === false)
	{
		$error = $appsccpgeneralsettings->get_error();
		$fm_save = false;
	}
}

$_TPL->set_var('info', $info);
$_TPL->set_var('element', $element);
$_TPL->set_var('fm_save', $fm_save);
$_TPL->set_var('error', $error);
$_TPL->set_var('language_list', dwho_i18n::get_supported_language_list());

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());

$_TPL->set_bloc('main', 'service/ipbx/'.$ipbx->get_name().'/general_settings/sccp');
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
