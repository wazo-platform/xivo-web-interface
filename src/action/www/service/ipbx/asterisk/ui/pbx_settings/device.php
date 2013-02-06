<?php

#
# XiVO Web-Interface
# Copyright (C) 2013 Avencall
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

include(dwho_file::joinpath(dirname(__FILE__),'..','_common.php'));

function format_devices_jquery($devices)
{
	$list = array();

	foreach ($devices as $device) {
		$value = $device['devicefeatures']['id'];

		$label = trim($device['devicefeatures']['mac']);
		if ($label === "") {
			$label = trim($device['devicefeatures']['ip']);
		}

		$formatted_device = array('label' => $label, 'value' => $value);

		$list[] = $formatted_device;
	}

	$list[] = array('value' => '', 'label' => '-----');

	return $list;
}

function format_devices_suggest($devices)
{
	$list = array();

	foreach($devices as $device) {
		$list[] = array('id' => $device['devicefeatures']['id'], 'identity' => $device['devicefeatures']['mac'], 'info' => '');
	}

	$list[] = array('id' => '', 'identity' => '-----', 'info' => '');

	return $list;
}

if(defined('XIVO_LOC_UI_ACTION') === true)
	$act = XIVO_LOC_UI_ACTION;
else
	$act = $_QRY->get('act');

switch($act)
{
	case 'search':
	default:
		$act = 'search';

		$appdevice = &$ipbx->get_application('device');
		$search = $_QRY->get('search');

		if(($devices = $appdevice->get_devices_search($search)) === false)
		{
			$http_response->set_status_line(404);
			$http_response->send(true);
		}

		switch ($_QRY->get('format'))
		{
			case 'jquery':
				$list = format_devices_jquery($devices);
				break;
			case null:
			default:
				// just to respect suggest.js data format
				$list = format_devices_suggest($devices);
				break;
		}

		$_TPL->set_var('list', $list);
}

$_TPL->display('/service/ipbx/'.$ipbx->get_name().'/pbx_settings/device');

?>
