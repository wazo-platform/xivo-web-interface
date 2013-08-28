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

// force loading translation file
$_TPL->load_i18n_file('tpl/www/bloc/statistics/statistics', 'global');
$_TPL->load_i18n_file('tpl/www/bloc/service/ipbx/'.$ipbx->get_name().'/call_management/cel/index.i18n', 'global');

$act = isset($_QR['act']) === false || $_QR['act'] !== 'exportcsv' ? 'advanced_search' : 'exportcsv';

$_STS->load_ressource('cel');



$info = null;
$result = false;

if(isset($_QR['fm_send']) === true || isset($_QR['search']) === true)
{
	$query = "";
	if(isset($_QR['dbeg']) === true && isset($_QR['dend']) === true)
	{
		$start = date("Y-m-d", strtotime($_QR['dbeg'])) . 'T00:00:00';
		$end = date("Y-m-d", strtotime($_QR['dend'])) . 'T23:59:59';
		$query = "?start_date=" . $start . "&end_date=" . $end;
	}

	$restapi = &$_XOBJ->get_module('restapi');
	$restapi_uri_csv = $restapi->get_uri('call_logs');

	$result = file_get_contents($restapi_uri_csv . $query);
	$_TPL->set_var('result',$result);
	$_TPL->display('/bloc/service/ipbx/'.$ipbx->get_name().'/call_management/cel/exportcsv');
	die();
}

$_TPL->set_var('result',$result);
$_TPL->set_var('info',$info);
$_TPL->set_var('act',$act);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());
$menu->set_toolbar('toolbar/service/ipbx/'.$ipbx->get_name().'/call_management/cel');

$dhtml = &$_TPL->get_module('dhtml');
$dhtml->set_js('js/dwho/submenu.js');
$dhtml->set_css('/css/statistics/statistics.css');
$dhtml->add_js('/struct/js/date.js.php');
$dhtml->set_js('js/service/ipbx/'.$ipbx->get_name().'/cel.js');

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/call_management/cel/'.$act);
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
