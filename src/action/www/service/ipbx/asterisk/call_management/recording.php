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
		$errors_list = array();
		
		if(isset($_QR['fm_send']) === true) {
			try {	
				$appreccampaigns->add($_QR['recordingcampaign_name'], $_QR['recordingcampaign_queueid'], 
								$_QR["recordingcampaign_start_date"], $_QR["recordingcampaign_end_date"]);
			} catch(Exception $e) {
				array_push($errors_list, $e->getMessage());
			}
			if(empty($errors_list))
				$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		} else { 
			$queues_list = refactor_queue_list($appreccampaigns->get_queues_list());
			$_TPL->set_var('queues_list', $queues_list);
		}
		$_TPL->set_var('errors_list', $errors_list);
		break;

	case 'edit':
		$errors_list = array();
		if(isset($_QR['fm_send']) === true) {
			try {
				$appreccampaigns->edit($_QR['campaign_id'], $_QR['recordingcampaign_name'], $_QR['recordingcampaign_queueid'],
						$_QR["recordingcampaign_start_date"], $_QR["recordingcampaign_end_date"]);
			} catch(Exception $e) {
				array_push($errors_list, $e->getMessage());
			}
			if(empty($errors_list))
				$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		} else {
			try {
				$queues_list = refactor_queue_list($appreccampaigns->get_queues_list());
				$_TPL->set_var('queues_list', $queues_list);
			} catch(Exception $e) {
				array_push($errors_list, $e->getMessage());
			}
			try {
				$campaign = $appreccampaigns->get_campaign_details($_QR['id']);
				$_TPL->set_var('info', get_object_vars($campaign[0]));
			} catch(Exception $e) {
				array_push($errors_list, $e->getMessage());
			}
		}
		$_TPL->set_var('errors_list', $errors_list);
		break;
		
	case 'delete':
		break;
		
	case 'menu':
		break;
	case 'listrecordings':
		try {
			$recordings = $appreccampaigns->get_recordings($_QR['campaign']);
			$_TPL->set_var('recordings', $recordings);
		} catch(Exception $e) {
			$_TPL->set_var('error',$e->getMessage());
		}
		break;
	
	case 'download':
		$file = basename($_QR['file']);
		$filepath = "/var/lib/pf-xivo/sounds/campagnes/" . $file;
		if ($file != "" && file_exists($filepath)) {
			$_TPL->set_var('file', $file);
			$_TPL->set_var('filepath', $filepath);
			$_TPL->display('/bloc/service/ipbx/'.$ipbx->get_name().'/call_management/recording/download');
			die();
		} else {
			die();
		}
		break;
	default:
		$act = 'list';
		$errors_list = $_TPL->get_var('errors_list');
		if($errors_list == null) $errors_list = array();
		try	{
			$recordingcampaigns = $appreccampaigns->get_campaigns();
			$_TPL->set_var('recordingcampaigns',$recordingcampaigns);
		} catch (Exception $e) {
			array_push($errors_list, $e->getMessage());
			$_TPL->set_var('errors_list',$errors_list);
		}
}
		

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());
$menu->set_toolbar('toolbar/service/ipbx/'.$ipbx->get_name().'/call_management/recording');

$_TPL->set_var('act',$act);
$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/call_management/recording/'.$act);
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());

$dhtml = &$_TPL->get_module('dhtml');
$dhtml->set_js('js/dwho/submenu.js');
$dhtml->set_css('/css/statistics/statistics.css');
$dhtml->add_js('/struct/js/date.js.php');
$dhtml->set_js('js/service/ipbx/'.$ipbx->get_name().'/recording.js');

$_TPL->display('index');

function refactor_queue_list($queues_list) {
	$queues_list_refactored = array();
	for($i = 0; $i < count($queues_list); $i++) {
		$new_item = array();
		$item = get_object_vars(&$queues_list[$i]);
		$new_item['id'] = $item['id'];
		$new_item['ext_name'] = $item['number'] . ': ' . $item['displayname'];
		$queues_list_refactored[$i] = $new_item;
	}
	return $queues_list_refactored;
}
?>