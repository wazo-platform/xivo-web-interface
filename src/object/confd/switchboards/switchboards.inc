<?php
#
# XiVO Web-Interface
# Copyright (C) 2016  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'abstract.inc'));

class confd_ressource_switchboards extends xivo_confd_helpers_abstract
{

	protected $_name      = 'switchboards';
	protected $_base_url  = 'switchboards';


	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
		/* $this->_httpheaders[] = dwho_json::get_header(); */
		$this->_httpheaders[] = 'Accept: application/json';
	}

	protected function fetch_response($response)
	{
		if ($this->_curl->get_info('content_type') == 'application/json') {
			$decoded = dwho_json::decode(rtrim($response),true);

			if(is_null($decoded) === true)
				$this->_build_error_message($response);
		} else {
			$decoded = $response;
		}

		$this->_status_code = $this->_curl->get_info('http_code');
		$this->_curl->close();

		return($decoded);
	}

	public function list_()
	{
		$uri = $this->_build_uri();
		$rs = $this->request_get($uri);

		if($this->_status_code !== 200
		|| empty($rs) === true)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return($rs);
	}

	public function stats($switchboard, $query)
	{
		$uri = $this->_build_uri(array($switchboard, 'stats'));
		$httpheaders[] = 'Accept: text/csv; charset=utf-8';
		$rs = $this->request_get($uri, $query, $httpheaders);

		if($this->_status_code !== 200
		|| empty($rs) === true)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return($rs);
	}

}

?>
