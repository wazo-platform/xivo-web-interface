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

$act = isset($_QR['act']) === true ? $_QR['act'] : '';

$appreccampaigns = &$ipbx->get_application('recordingcampaigns');

$param = array();
$param['act'] = 'list';

switch($act)
{
	case 'add':
		$result = $fm_save = $error = null;
		
		if(isset($_QR['fm_send']) === true) //&& dwho_issa('recordcampaign',$_QR) === true)
		{
			$appreccampaigns->add($_QR['recordingcampaign_name'], $_QR['recordingcampaign_queuename']);
			$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		}
		break;

	case 'edit':
		break;
		
	case 'delete':
		break;
		
	case 'menu':
		break;
	case 'listrecordings':
		try {
			$recordings = $appreccampaigns->get_recordings($_QR['campaign']);
// 			$recordings = array();
// 			$recordings[0]["caller"] = "moi";
// 			$recordings[0]["callee"] = "lui";
// 			$recordings[0]["file_name"] = "fichier";
// 			$recordings[0]["start_time"] = "01/01/01 00:00";
			$_TPL->set_var('recordings', $recordings);
		} catch(Exception $e) {
			$_TPL->set_var('error',$e->getMessage());
		}
		break;
		
	default:
		$act = 'list';
		try
		{
			$recordingcampaigns = $appreccampaigns->get_campaigns();
			$_TPL->set_var('recordingcampaigns',$recordingcampaigns);
		}
		catch (Exception $e)
		{
			$_TPL->set_var('error',$e->getMessage());
		}
}
		

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());
$menu->set_toolbar('toolbar/service/ipbx/'.$ipbx->get_name().'/call_management/recording');

$_TPL->set_var('act',$act);
$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/call_management/recording/'.$act);
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());

$_TPL->display('index');

?>