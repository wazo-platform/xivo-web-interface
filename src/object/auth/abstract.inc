<?php

#
# XiVO Web-Interface
# Copyright (C) 2015 Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'auth','abstract','abstract.inc'));

abstract class xivo_auth_helpers_abstract extends xivo_auth_ressource_abstract
{
	protected $_cnt = 0;

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
	}

	protected function fetch_response($response)
	{
		$decoded = dwho_json::decode(rtrim($response),true);

		if(is_null($decoded) === true)
			$this->_build_error_message($response);

		$this->_status_code = $this->_curl->get_info('http_code');
		$this->_curl->close();

		return($decoded);
	}

	public function get($id)
	{
		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array($id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 200
		|| empty($rs) === true)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return($rs);
	}

	public function add($data)
	{
		if (is_array($data) === false)
			return(false);

		$uri = $this->_build_uri();
		$rs = $this->request_post($uri,$data);

		if($this->_status_code !== 201)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return($rs['id']);
	}

	public function delete($id)
	{
		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array($id));
		$rs = $this->request_delete($uri);

		if($this->_status_code !== 204)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return(true);
	}

	protected function _build_error_message($msg)
	{
		dwho_report::push('error', $msg);
	}

}

?>
