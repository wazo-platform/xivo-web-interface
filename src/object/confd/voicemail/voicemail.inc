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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'abstract.inc'));

class confd_ressource_voicemail extends xivo_confd_helpers_abstract
{

	protected $_name      = 'voicemail';
	protected $_base_url  = 'voicemails';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
		$this->_httpheaders[] = dwho_json::get_header();
		$this->_httpheaders[] = 'Accept: application/json';
	}

	public function associate_user_voicemail($user_id, $voicemail_id)
	{
		$uri = $this->_build_uri(array($user_id, 'voicemail'), 'users');
		$data = array('voicemail_id' => $voicemail_id);
		$rs = $this->request_post($uri,$data);

		if($this->_status_code !== 201)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return(true);
	}

	public function dissociate_user_voicemail($user_id)
	{
		if($this->get_user_voicemail($user_id) !== false) {
			$uri = $this->_build_uri(array($user_id, 'voicemail'), 'users');
			$rs = $this->request_delete($uri);

			if($this->_status_code !== 204)
			{
				$this->_build_error_message($rs);
				return(false);
			}
		}
		return(true);
	}

	public function get_user_voicemail($user_id)
	{
		$uri = $this->_build_uri(array($user_id, 'voicemail'), 'users');
		$association = $this->request_get($uri);
		if($this->_status_code !== 200)
		{
			return false;
		}

		return $association;
	}

}

?>
