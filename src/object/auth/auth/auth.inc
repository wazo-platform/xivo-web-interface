<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2015  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'auth', 'abstract.inc'));

class auth_ressource_auth extends xivo_auth_helpers_abstract
{

	protected $_name      = 'auth';
	protected $_base_url  = 'token';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
		$this->_httpheaders[] = dwho_json::get_header();
		$this->_httpheaders[] = 'Accept: application/json';
	}

	public function create_token($login, $password, $backend=null)
	{
		$uri = $this->_build_uri('token');
		$data = array('backend' => $backend, 'expiration' => 3600);
		$tk = $this->request_post($login,$password,$uri,$data);

		if($this->_status_code !== 200)
		{
			$this->_build_error_message($tk);
			return(false);
		}

		return($tk);
	}

	public function delete_token($token)
	{
		$uri = $this->_build_uri(array($token));
		$tk = $this->request_delete($uri);

		if($this->_status_code !== 200)
		{
			$this->_build_error_message($tk);
			return(false);
		}
		return(true);
	}

	public function get_token($token)
	{
		$uri = $this->_build_uri(array($token));
		$tk = $this->request_get($uri);
		if($this->_status_code !== 200)
		{
			return false;
		}

		return $tk;
	}

}

?>
