<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2014  Avencall
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

class confd_ressource_call_logs extends xivo_confd_helpers_abstract
{

	protected $_name      = 'call_logs';
	protected $_base_url  = 'call_logs';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
	}

	protected function fetch_response($response)
	{
		$this->_status_code = $this->_curl->get_info('http_code');
		$this->_curl->close();

		return($response);
	}

	public function find_all($query)
	{
		$httpheaders = array('Accept: text/csv; charset=utf-8');
		$uri = $this->_build_uri();
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
