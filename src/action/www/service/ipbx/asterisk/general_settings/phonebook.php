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

$appaccessfeatures = &$ipbx->get_application('accessfeatures',array('feature' => 'phonebook'));

$info = array();

dwho::load_class('dwho_sort');

$accessfeaturessort = new dwho_sort(array('key' => 'host'));

if(($info['accessfeatures'] = $appaccessfeatures->get()) !== false)
	uasort($info['accessfeatures'],array($accessfeaturessort,'str_usort'));

$error = array();
$error['accessfeatures'] = array();

$fm_save = null;

if(isset($_QR['fm_send']) === true)
{
	if(isset($_QR['accessfeatures']) === false)
		$_QR['accessfeatures'] = array();

	if($appaccessfeatures->set($_QR['accessfeatures']) !== false)
		$appaccessfeatures->save();

	$info['accessfeatures'] = $appaccessfeatures->get_result();

	if(is_array($info['accessfeatures']) === true)
		uasort($info['accessfeatures'],array($accessfeaturessort,'str_usort'));

	$error['accessfeatures'] = $appaccessfeatures->get_error();

	if(isset($error['accessfeatures'][0]) === true)
		$fm_save = false;
	else if($fm_save !== false)
		$fm_save = true;
}

$_TPL->set_var('fm_save',$fm_save);
$_TPL->set_var('info',$info);

$dhtml = &$_TPL->get_module('dhtml');

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/general_settings/phonebook');
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
