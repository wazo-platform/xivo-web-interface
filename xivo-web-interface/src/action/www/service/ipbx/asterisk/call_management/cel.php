<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2013  Avencall
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

$result = false;

if(isset($_QR['fm_send']) === true || isset($_QR['search']) === true)
{
	$query = null;
	if(isset($_QR['dbeg']) === true && $_QR['dbeg'])
	{
		$start = date("Y-m-d\TH:i:s", strtotime($_QR['dbeg']));
		if(isset($_QR['dend']) === true && $_QR['dend'])
		{
			$end = date("Y-m-d\TH:i:s", strtotime($_QR['dend']));
		} else
		{
			$end = date("Y-m-d\TH:i:s");
		}
		$query= array(array('start_date', $start), array('end_date', $end));
	}
	$restapi = &$_XOBJ->get_module('restapi');
	$csv_uri = $restapi->get_uri('call_logs', $query);
	$result = file_get_contents($csv_uri);
	$_TPL->set_var('result', $result);
	$_TPL->display('/bloc/service/ipbx/'.$ipbx->get_name().'/call_management/cel/exportcsv');
	die();
}

$_TPL->set_var('result',$result);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());
$menu->set_toolbar('toolbar/service/ipbx/'.$ipbx->get_name().'/call_management/cel');

$dhtml = &$_TPL->get_module('dhtml');
$dhtml->add_js('/struct/js/date.js.php');
$dhtml->set_js('js/service/ipbx/'.$ipbx->get_name().'/cel.js');

$dhtml->set_css('extra-libs/timepicker/jquery-ui-timepicker-addon.css',true);
$dhtml->set_js('extra-libs/timepicker/jquery-ui-timepicker-addon.js',true);

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/call_management/cel/advanced_search');
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
