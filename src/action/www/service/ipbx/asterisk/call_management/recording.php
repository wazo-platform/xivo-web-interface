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
		
		if(isset($_QR['fm_send']) === true) {
			$appreccampaigns->add($_QR['recordingcampaign_name'], $_QR['recordingcampaign_queueid']);
			$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		} else { 
			$queues_list = refactor_queue_list($appreccampaigns->get_queues_list());
			$_TPL->set_var('queues_list', $queues_list);
		}
		break;

	case 'edit':
		$result = $fm_save = $error = null;
		
		if(isset($_QR['fm_send']) === true) {
			$appreccampaigns->edit($_QR['original_name'], $_QR['recordingcampaign_name'], $_QR['recordingcampaign_queueid']);
			$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		} else {
			$queues_list = refactor_queue_list($appreccampaigns->get_queues_list());
			$campaign = $appreccampaigns->get_campaign_details($_QR['name']);
			$_TPL->set_var('queues_list', $queues_list);
			$_TPL->set_var('info', get_object_vars($campaign[0]));
		}
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