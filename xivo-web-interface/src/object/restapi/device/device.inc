<?php
#
# XiVO Web-Interface
# Copyright (C) 2013  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'restapi', 'abstract.inc'));

class restapi_ressource_device extends xivo_restapi_helpers_abstract
{

	protected $_name      = 'device';
	protected $_base_url  = 'devices';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
	}

	public function get_device_by_ip($ip)
	{
		$uri = $this->_build_uri();
		$query = array();
		array_push($query, array('search',$ip));
		$rs = $this->request_get($uri, $query);

		if($this->_status_code !== 200
		|| empty($rs) === true
		|| (int) $rs['total'] === 0)
		{
			dwho_report::push('error', 'An Error occured while retrieving the device. '.$this->_get_error_msg($rs));
			return(false);
		}

		return($rs['items'][0]);
	}

	public function reset_to_autoprov($id)
	{
		$uri = $this->_build_uri(array($id, 'autoprov'));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
			return(false);

		return(true);
	}

	public function synchronize($id)
	{
		$uri = $this->_build_uri(array($id, 'synchronize'));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			dwho_report::push('error', 'An Error occured while synchronizing the device: '.$this->_get_error_msg($rs));
			return(false);
		}

		return(true);
	}

	public function associate_line($device_id, $line_id)
	{
		$uri = $this->_build_uri(array($device_id, 'associate_line', $line_id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			dwho_report::push('error', 'An Error occured while associating line on the device: '.$this->_get_error_msg($rs));
			return(false);
		}

		return(true);
	}

	public function remove_line($device_id, $line_id)
	{
		$uri = $this->_build_uri(array($device_id, 'remove_line', $line_id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			dwho_report::push('error', 'An Error occured while removing line on the device: '.$this->_get_error_msg($rs));
			return(false);
		}

		return(true);
	}
}

?>
