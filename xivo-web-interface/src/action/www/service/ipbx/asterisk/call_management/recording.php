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
$page = isset($_QR['page']) === true ? intval($_QR['page']) : 1;
$pagesize = 50;

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
			} catch(WebServiceException $e) {
				$errors_list = array_concatenate($errors_list, $e->getErrorsList());
			}
			if(empty($errors_list)) {
				$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
			} else {
				$info = array('start_date' => $_QR["recordingcampaign_start_date"],
							  'end_date' => $_QR["recordingcampaign_end_date"],
							  'campaign_name' => $_QR["recordingcampaign_name"],
							  'queue_id' => $_QR['recordingcampaign_queueid']);
				$_TPL->set_var('info', $info);
			}
		}
		//on définit queues_list dans tous les cas, on en a besoin en cas d'erreur
		$queues_list = refactor_queue_list($appreccampaigns->get_queues_list());
		$_TPL->set_var('queues_list', $queues_list);
		
		$_TPL->set_var('errors_list', $errors_list);
		break;

	case 'edit':
		$errors_list = array();
		//fm_send is set ==> the edition form has been submited
		if(isset($_QR['fm_send'])) {
			try {
				$appreccampaigns->edit($_QR['id'], $_QR['recordingcampaign_name'], $_QR['recordingcampaign_queueid'],
						$_QR["recordingcampaign_start_date"], $_QR["recordingcampaign_end_date"]);
			} catch(WebServiceException $e) {
				$errors_list = array_concatenate($errors_list, $e->getErrorsList());
			}
			//if there is no error, we go back to the list of campaigns
			//else, we keep in memory the data which was sent and continue
			if(empty($errors_list)){
				$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
			} else {
				$info = array('start_date' => $_QR["recordingcampaign_start_date"],
						'end_date' => $_QR["recordingcampaign_end_date"],
						'campaign_name' => $_QR["recordingcampaign_name"],
						'queue_id' => $_QR['recordingcampaign_queueid'],
						'id' => $_QR['id']);
				$_TPL->set_var('info', $info);
			}
		} else {
			try {
				//we display the current state of the campaign
				$campaign = $appreccampaigns->get_campaign_details($_QR['id'])->items;
				$_TPL->set_var('info', get_object_vars($campaign[0]));
			} catch(Exception $e) {
				$errors_list = array_concatenate($errors_list, $e->getMessage());
			}
		}
		try {
			$queues_list = refactor_queue_list($appreccampaigns->get_queues_list());
			$_TPL->set_var('queues_list', $queues_list);
		} catch(Exception $e) {
			$errors_list = array_concatenate($errors_list, $e->getMessage());
		}
		
		$_TPL->set_var('errors_list', $errors_list);
		break;
		
	case 'delete_recording':
		$errors_list = array();
		try {
			$appreccampaigns->delete_recording($_QR['campaign'], $_QR['id']);
		} catch(Exception $e) {
			array_push($errors_list, $e->getMessage());
		}
		$param['act'] = 'listrecordings';
		$param['campaign'] = $_QR['campaign'];
		$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		break;
	
	case 'menu':
		break;
	case 'listrecordings':
		$errors_list = array();
		$total = 0;
		if(isset($_QR['search'])) {
			try {
				$result = $appreccampaigns->search_recordings($_QR['campaign'], $_QR['search'], $page, $pagesize);
				$recordings = $result->items;
				$total = $result->total;
				$_TPL->set_var('recordings', $recordings);
			} catch(Exception $e) {
				array_push($errors_list, $e->getMessage());
			}
		} else {
			try {
				$result = $appreccampaigns->get_recordings($_QR['campaign'], $page, $pagesize);
				$recordings = $result->items;
				$total = $result->total;
				$_TPL->set_var('recordings', $recordings);
			} catch(Exception $e) {
				array_push($errors_list, $e->getMessage());
			}
		}
		
		$params = array('campaign' => $_QR['campaign']);
		if(isset($_QR['search']))
				$params['search'] = $_QR['search'];
		$pages = get_nb_pages($total, $pagesize);
		$pager = array('pages' => $pages,
					   'page' => $page,
					   'prev' => $page - 1,
					   'next' => $page == $pages ? 0 : $page + 1);
		$_TPL->set_var('pager', $pager);
		$_TPL->set_var('params', $params);
		$_TPL->set_var('errors_list', $errors_list);
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
		
	case 'delete_campaign':
		$errors_list = array();
		try {
			$appreccampaigns->delete_campaign($_QR['campaign']);
		} catch(WebServiceException $e) {
			$errors_list = array_concatenate($errors_list, $e->getErrorsList());
			foreach($errors_list as $error) {
				dwho_report::push('error', dwho_i18n::babelfish($error));
			}
		}
		$_QRY->go($_TPL->url('service/ipbx/call_management/recording'),$param);
		break;
		
	default:
		$act = 'list';
		if($errors_list == null) $errors_list = array();
		try	{
			$recordingcampaigns = $appreccampaigns->get_campaigns()->items;
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
$dhtml->add_js('/struct/js/date.js.php');
$dhtml->set_js('js/service/ipbx/'.$ipbx->get_name().'/recording.js');
$dhtml->set_js('extra-libs/timepicker/jquery-ui-timepicker-addon.js', true);
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

function get_nb_pages($total, $pagesize) {
	return ceil($total/$pagesize);
}

function array_concatenate($modified_array, $new_values) {
	foreach($new_values as $value) {
		array_push($modified_array, $value);
	}
	return $modified_array;
}
?>