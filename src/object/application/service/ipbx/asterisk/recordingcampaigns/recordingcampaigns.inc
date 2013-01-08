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

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);
//xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

include('httpful.phar');

class xivo_application_service_asterisk_recordingcampaigns extends xivo_application_asterisk_abstract//xivo_service_asterisk_abstract
{
	private $recording_service_url = 'http://127.0.0.1:50050/rest/IPBX/recording_campaigns/';
	private $queues_service_url = 'http://127.0.0.1:50050/rest/CallCenter/queues/';
	protected $_filter = false;
	
	private function manage_errors($http_reply) {
		if(!$http_reply->hasErrors())
			return false;
		elseif($http_reply->code >= 500)
			throw new Exception("Server error: " . $http_reply->body);
		else
			throw new Exception("Client error: " . $http_reply->body);
	}
	
	public function get_campaigns()
	{
		$campaigns_reply = false;
		$campaigns_reply = \Httpful\Request::get($this->recording_service_url)->send();
		if(!$campaigns_reply->hasErrors()) {
			# TODO : Check content-type
			return dwho_json::decode($campaigns_reply->body);
		} else {
			$this->manage_errors($campaigns_reply);
		}
	}


	public function get_recordings($campaign_id) {
		$recordings_reply = false;
		$recordings_reply = \Httpful\Request::get($this->recording_service_url . $campaign_id . "/")-> send();
		if(!$recordings_reply->hasErrors()) {
			# TODO : Check content-type
			return dwho_json::decode($recordings_reply->body);
		} else {
			$this->manage_errors($recordings_reply);
		}
	}

	public function get_queues_list() {
		$queues_reply = false;
		$queues_reply = \Httpful\Request::get($this->queues_service_url)-> send();
		if(!$queues_reply->hasErrors()) {
			# TODO : Check content-type
			return dwho_json::decode($queues_reply->body);
		} else {
			$this->manage_errors($queues_reply);
		}
	}

	public function get_campaign_details($campaign_id) {
		$campaign_reply = false;
		$campaign_reply = \Httpful\Request::get($this->recording_service_url
				. $campaign_id)-> send();
		if(!$campaign_reply->hasErrors()) {
			return dwho_json::decode($campaign_reply->body);
		} else {
			$this->manage_errors($campaign_reply);
		}
	}

	public function add($campaign_name, $queue_id, $start_date, $end_date, $activated="true", $base_filename=null)
	{
		if ($base_filename == null){
			$base_filename=$campaign_name . "-file-";
		}
		$recordings_reply = false;

		$content= dwho_json::encode(array("campaign_name"=>$campaign_name, 
										  "queue_id"=>$queue_id, 
										  "start_date"=>$start_date,
										  "end_date"=>$end_date,
										  "activated"=>$activated, 
										  "base_filename"=>$base_filename) );
		$recordings_reply = \Httpful\Request::post($this->recording_service_url)
												->body($content)
												->sendsJson()
												->send();
		$this->manage_errors($recordings_reply);
	}
	
	public function edit($campaign_id, $new_campaign_name, $new_queue_id, $new_start_date, $new_end_date) {
		$editing_reply = false;
		
		$content= dwho_json::encode(array("campaign_name"=>$new_campaign_name, 
										  "queue_id"=>$new_queue_id,
										  "start_date"=>$new_start_date,
										  "end_date"=> $new_end_date) );
		$editing_reply = \Httpful\Request::put($this->recording_service_url . $campaign_id)
											->body($content)
											->sendsJson()
											->send();
		$this->manage_errors($editing_reply);
	}
	
	public function search_recordings($campaign_id, $search_pattern) {
		$recordings_reply = false;
		$params = "?key=$search_pattern";
		$recordings_reply = \Httpful\Request::get($this->recording_service_url . $campaign_id . "/search" . $params)-> send();
		if(!$recordings_reply->hasErrors()) {
			# TODO : Check content-type
			return dwho_json::decode($recordings_reply->body);
		} else {
			$this->manage_errors($recordings_reply);
		}
	}
	
	public function delete($campaign_id, $recording_id) {
		$delete_reply = false;
		$delete_reply = \Httpful\Request::delete($this->recording_service_url . $campaign_id . "/" . $recording_id)-> send();
		if(!$delete_reply->hasErrors()) {
			# TODO : Check content-type
			return dwho_json::decode($delete_reply->body);
		} else {
			$this->manage_errors($delete_reply);
		}
	}

	function add_from_json()
	{
		if(($data = $this->_get_data_from_json()) === false
				|| $this->set_add($data) === false
				|| $this->add() === false)
			return(false);

		return(true);
	}
}

?>
