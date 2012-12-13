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

include('httpful.phar');

class xivo_application_service_asterisk_recordingcampaigns extends xivo_application_asterisk_abstract
{

	private $service_url = 'http://127.0.0.1:50050/rest/record/';
	private function get_recordings()
	{
		$recordings_reply = false;
		$recordings_list = false;
		$recordings_reply = \Httpful\Request::get($this->service_url)->send();
		if(!$recordings_reply->hasErrors()):
			# TODO : Check content-type
			$recordings_list = dwho_json::decode($recordings_reply->body);
		else:
			throw new Exception("Http reply code: " + $recordings_reply->code);
		endif;
		return $recordings_list;
	}
	
	function get($id=null)
	{
		if ($id == null):
			return $this->get_recordings();
		else:
		
		endif;
		return null;
	}


	function add($campaign_name, $queue_id, $activated="true", $base_filename=null)
	{
		if ($base_filename == null){
			$base_filename=$campaign_name+"-file-";
		}
		$recordings_reply = false;
		$recordings_list = false;
		
		$content= dwho_json::encode(array("campaign_name"=>$campaign_name, "queue_id"=>$queue_id, "activated"=>$activated, "base_filename"=>$base_filename) );
		$recordings_reply = \Httpful\Request::post($this->service_url)
								->body($content)
								->sendsJson()
								->send();
		
// 		if(!$recordings_reply->hasErrors()):
// 			# TODO : Check content-type
// 			$recording_reply_body = dwho_json::decode();
// 		else:
// 			throw new Exception("Http reply code: " + $recordings_reply->code);
// 		endif;
// 		return $recordings_list;
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
