<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016 Avencall
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

if(xivo_user::chk_acl('switchboard', 'data') === false)
	$_QRY->go($_TPL->url('statistics/switchboard'));

$_I18N->load_file('tpl/www/bloc/statistics/switchboard');

$switchboards_api = &$_RAPI->get_ressource('switchboards');

$result = false;

if(isset($_QR['fm_send']) === true || isset($_QR['search']) === true)
{
	$switchboard = $_QR['switchboard'];
	$query = array();
	if(isset($_QR['dbeg']) === true && $_QR['dbeg'])
	{
		$start_date = date("Y-m-d", strtotime($_QR['dbeg']));
		$start_time = date("Y-m-d\TH:i:s", strtotime($_QR['dbeg']));
		array_push($query, array('start_date', $start_time));
	} else {
		$start = '';
	}
	if(isset($_QR['dend']) === true && $_QR['dend'])
	{
		$end_date = date("Y-m-d", strtotime($_QR['dend']));
		$end_time = date("Y-m-d\T23:59:59", strtotime($_QR['dend']));
	} else {
		$end_date = date("Y-m-d");
		$end_time = date("Y-m-d\TH:i:s");
	}
	array_push($query, array('end_date', $end_time));

	if(isset($_QR['switchboard']) === false
	|| ! $_QR['switchboard']
	|| ($result = $switchboards_api->stats($switchboard, $query)) === false)
	{
		dwho_report::push('error', 'Could not fetch switchboard statistics: switchboard_id='.$switchboard);
	} else {
		$_TPL->set_var('result', $result);
		$_TPL->set_var('dbeg', $start_date);
		$_TPL->set_var('dend', $end_date);
		$_TPL->set_var('switchboard', $switchboard);
		$_TPL->display('/bloc/statistics/switchboard/exportcsv');
		die();
	}
}

if(($switchboards_api = $switchboards_api->list_()) === false)
{
	dwho_report::push('error', 'Could not fetch switchboard list');
	$switchboards_display = array();
} else {
	$switchboards_display = $switchboards_api['items'];
}

$_TPL->set_var('switchboards', $switchboards_display);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/statistics/statistics');
$menu->set_toolbar('toolbar/statistics/switchboard/data');

$dhtml = &$_TPL->get_module('dhtml');
$dhtml->add_js('/struct/js/date.js.php');
$dhtml->set_js('js/dwho/submenu.js');
$dhtml->set_js('js/statistics/switchboard/data.js');

$dhtml->set_css('extra-libs/timepicker/jquery-ui-timepicker-addon.css',true);
$dhtml->set_js('extra-libs/timepicker/jquery-ui-timepicker-addon.js',true);

$_TPL->set_bloc('main','statistics/switchboard/data');
$_TPL->set_struct('statistics/index');
$_TPL->display('index');

?>
