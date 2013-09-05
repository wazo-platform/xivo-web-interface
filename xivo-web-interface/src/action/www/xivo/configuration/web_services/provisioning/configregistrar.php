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

dwho::load_class('dwho_http');
$http_response = dwho_http::factory('response');

if(isset($_SERVER['REMOTE_ADDR']) === false
|| ($_SERVER['REMOTE_ADDR'] !== '127.0.0.1'
	&& $_SERVER['REMOTE_ADDR'] !== '::1'))
{
	$http_response->set_status_line(403);
	$http_response->send(true);
}

$act = $_QRY->get('act');
$ipbx = &$_SRE->get('ipbx');

switch($act)
{
	case 'rebuild_required_config':
		$provdconfig = &$_XOBJ->get_module('provdconfig');
		$linefeatures = &$ipbx->get_module('linefeatures');

		$status = 204;
		if (($list = $linefeatures->get_list_device_associated()) !== false
		&& ($nb = count($list)) > 0)
		{
			$status = 200;
			for($i=0; $i<$nb; $i++)
			{
				$ref = &$list[$i];
				$provdconfig->rebuild_device_config($ref['device']);
			}
		}
		$provdconfig->rebuild_required_config();

		$http_response->set_status_line($status);
		$http_response->send(true);
		break;
	case 'sync_bsfilter_devices':
		$appdevice = &$ipbx->get_application('device');
		$provddevice = &$_XOBJ->get_module('provddevice');

		$status = 204;
		if (($list = $appdevice->get_devices_has_bsfilter()) !== false
		&& ($nb = count($list)) > 0)
		{
			$status = 200;
			for($i=0; $i<$nb; $i++)
			{
				$ref = &$list[$i];
				$provddevice->synchronize($ref['id']);
			}
		}

		$http_response->set_status_line($status);
		$http_response->send(true);
		break;
	case 'sync_devices':
		$provddevice = &$_XOBJ->get_module('provddevice');

		$status = 204;
		if (($list = $provddevice->get_all()) !== false
		&& ($nb = count($list)) > 0)
		{
			$status = 200;
			for($i=0; $i<$nb; $i++)
			{
				$ref = &$list[$i];
				$provddevice->synchronize($ref['id']);
			}
		}

		$http_response->set_status_line($status);
		$http_response->send(true);
		break;
	default:
		$http_response->set_status_line(400);
		$http_response->send(true);
}

?>
