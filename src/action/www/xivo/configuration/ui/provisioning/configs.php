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

include(dwho_file::joinpath(dirname(__FILE__),'..','_common.php'));

$ipbx = &$_SRE->get('ipbx');

if(defined('XIVO_LOC_UI_ACTION') === true)
	$act = XIVO_LOC_UI_ACTION;
else
	$act = $_QRY->get('act');

$provdconfig = &$_XOBJ->get_module('provdconfig');
$appdevice = &$ipbx->get_application('device',null,false);
$device_api = &$_RAPI->get_ressource('device');

dwho_logw($act);
switch($act)
{
	case 'get':
		if (isset($_QR['id']) === false
		|| ($res = $provdconfig->get($_QR['id'])) === false
		|| ($raw_config = $res['raw_config']) === null
		|| isset($raw_config['sip_lines']) === false)
		{
			$http_response->set_status_line(204);
			$http_response->send(true);
		}
		$sip_lines = $raw_config['sip_lines'];
		$_TPL->set_var('act',$act);
		$_TPL->set_var('list',array_keys($sip_lines));
		$_TPL->display('/struct/page/genericjson');
		break;
	case 'search':
		$list_device_line = array();
		if (isset($_QR['term']) === true)
		{
			$list_device_line = $device_api->raw_find($_QR['term'],false,[0,5]);
		}

		$list_device = array();
		$results=array();

		$nb_device = count($list_device_line);

		for($i=0; $i<$nb_device; $i++):
			$cur_device = $list_device_line[$i];
			$results[$i]['id'] = $cur_device['id'];
			$trimmed_mac = trim($cur_device['mac']);
			$trimmed_ip = trim($cur_device['ip']);
			if(empty($trimmed_mac) === false) {
				$results[$i]['text'] = 'MAC: '.$cur_device['mac'];
			} else if(empty($trimmed_ip) === false) {
				$results[$i]['text'] = 'IP: '.$cur_device['ip'];
			}
		endfor;

		$list_device['results'] = $results;
		$list_device['pagination'] = array();
		$list_device['pagination']['more'] = false;
		
		$_TPL->set_var('act',$act);
		$_TPL->set_var('list',$list_device);
		$_TPL->display('/struct/page/genericjson');
	break;

	default:
		$http_response->set_status_line(400);
		$http_response->send(true);
}

?>