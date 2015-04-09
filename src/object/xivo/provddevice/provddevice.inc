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

xivo_object::required(array('abstract','provd.inc'),true);

class xivo_object_provddevice extends xivo_object_provd_abstract
{
	protected $_name 		    = 'provddevice';
	protected $_base_url 	    = '/provd/dev_mgr';
	protected $_provdconfig	    = false;

	public function __construct(&$xobj,$param=array())
	{
		if(($this->_provdconfig = $xobj->get_module('provdconfig')) === false)
			return(false);

		$this->_load_config(true);
		$this->_load_init();
	}

	public function get($id)
	{
		$this->reset();

		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array('devices',$id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 200
		|| empty($rs['device']) === true)
			return(false);

		return($rs['device']);
	}

	public function get_device_by_ip($ip)
	{
		$this->reset();

		$uri = $this->_build_uri(array('devices'));
		$query = array();
		array_push($query,array('q','{"ip":"'.$ip.'"}'));
		$rs = $this->request_get($uri, $query);

		if($this->_status_code !== 200
		|| empty($rs['devices']) === true) {
			return(false);
		}

		return($rs['devices'][0]);
	}

	public function get_all()
	{
		$this->reset();

		$uri = $this->_build_uri(array('devices'));
		$rs = $this->request_get($uri);

		if (is_array($rs) === false
		|| ($rs = $rs['devices']) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		return(array_values($rs));
	}

	private function _edit($id,$arr)
	{
		$uri = $this->_build_uri(array('devices',$id));
		$data['device'] = $arr;
		$rs = $this->request_put($uri,$data);

		return($this->_status_code === 204);
	}

	public function reconfigure($id)
	{
		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array('reconfigure'));

		$data['id'] = $id;

		$rs = $this->request_post($uri,$data);

		if($this->_status_code !== 204)
			return(false);

		return(true);
	}

	public function synchronize($id)
	{
		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array('synchronize'));

		$data['id'] = $id;

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 201)
			return(false);

		$decoded = $this->decode_header($this->_response['header']);

		if (isset($decoded['Location']) === false)
			return(false);

		return(urlencode($decoded['Location']));
	}

	public function rebuild_device_config($deviceid)
	{
		if (dwho_has_len($deviceid) === false)
			return(false);

		if (($device = $this->get($deviceid)) === false)
		{
			dwho_report::push('error', 'could not get device '.$deviceid);
			return(false);
		}

		$configid = isset($device['config']) === true ? $device['config'] : null;
		if (($newconfigid = $this->_provdconfig->rebuild_config($deviceid,$configid)) === false)
		{
			return(false);
		}
		elseif ($newconfigid === null)
		{
			return(true);
		}

		if ($configid !== $newconfigid)
		{
			$device['config'] = $newconfigid;
			if($this->_edit($deviceid,$device) === false)
			{
				dwho_report::push('error', 'could not edit device '.$deviceid);
				return(false);
			}
		}

		return(true);
	}
}

?>