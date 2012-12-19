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
	
	public function get_campaigns()
	{
		$campaigns_reply = false;
		$campaigns_list = false;
		$campaigns_reply = \Httpful\Request::get($this->recording_service_url)->send();
		if(!$campaigns_reply->hasErrors()):
		# TODO : Check content-type
		$campaigns_list = dwho_json::decode($campaigns_reply->body);
		else:
		throw new Exception("Http reply code: " + $campaigns_reply->code);
		endif;
		return $campaigns_list;
	}


	public function get_recordings($campaign_name) {
		$recordings_reply = false;
		$recordings_list = false;
		$recordings_reply = \Httpful\Request::get($this->recording_service_url . $campaign_name . "/")-> send();
		if(!$recordings_reply->hasErrors()):
		# TODO : Check content-type
		$recordings_list = dwho_json::decode($recordings_reply->body);
		else:
		throw new Exception("Http reply code: " + $recordings_reply->code);
		endif;
		return $recordings_list;
	}

	public function get_queues_list() {
		$queues_reply = false;
		$queues_reply = \Httpful\Request::get($this->queues_service_url)-> send();
		if(!$queues_reply->hasErrors()) {
			# TODO : Check content-type
			return dwho_json::decode($queues_reply->body);
		} else {
			throw new Exception("Http reply code: " + $queues_reply->code);
		}
	}

	public function get_campaign_details($campaign_name) {
		$campaign_reply = false;
		$campaign_reply = \Httpful\Request::get($this->recording_service_url
				. $campaign_name)-> send();
		if(!$campaign_reply->hasErrors()) {
			return dwho_json::decode($campaign_reply->body);
		} else {
			throw new Exception("Http reply code: " + $campaign_reply->code);
		}

	}

	public function add($campaign_name, $queue_id, $activated="true", $base_filename=null)
	{
		if ($base_filename == null){
			$base_filename=$campaign_name . "-file-";
		}
		$recordings_reply = false;
		$recordings_list = false;

		$content= dwho_json::encode(array("campaign_name"=>$campaign_name, "queue_id"=>$queue_id, "activated"=>$activated, "base_filename"=>$base_filename) );
		$recordings_reply = \Httpful\Request::post($this->recording_service_url)
												->body($content)
												->sendsJson()
												->send();
		return $recording_reply;
	}
	
	public function edit($original_campaign_name, $new_campaign_name, $new_queue_id) {
		$editing_reply = false;
		
		$content= dwho_json::encode(array("campaign_name"=>$new_campaign_name, 
										  "queue_id"=>$new_queue_id) );
		$editing_reply = \Httpful\Request::put($this->recording_service_url . $original_campaign_name)
											->body($content)
											->sendsJson()
											->send();
		return $editing_reply;
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
