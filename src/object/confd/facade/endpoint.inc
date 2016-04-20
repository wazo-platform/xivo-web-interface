<?php

#
# XiVO Web-Interface
# Copyright (C) 2016 Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'custom', 'custom.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'sccp', 'sccp.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'sip', 'sip.inc'));

class EndpointFacade {

	private $_custom = null;
	private $_sip = null;
	private $_sccp = null;

	function __construct($client) {
		$this->_custom = new CustomResource($client);
		$this->_sip = new SipResource($client);
		$this->_sccp = new SccpResource($client);
	}

	function get($id, $protocol) {
		switch ($protocol) {
			case CONFD_LINE_PROTO_CUSTOM:
				return $this->_custom->get($id);
			case CONFD_LINE_PROTO_SCCP:
				return $this->_sccp->get($id);
			case CONFD_LINE_PROTO_SIP:
				return $this->_sip->get($id);
			default:
				trigger_error('invalid endpoint protocol '.$protocol, E_USER_ERROR);
				return null;
		}
	}

	function create($endpoint, $protocol)
	{
		switch ($protocol) {
			case CONFD_LINE_PROTO_CUSTOM:
				return $this->_custom->create($endpoint);
			case CONFD_LINE_PROTO_SCCP:
				return $this->_sccp->create($endpoint);
			case CONFD_LINE_PROTO_SIP:
				return $this->_sip->create($endpoint);
			default:
				trigger_error('invalid endpoint protocol '.$protocol, E_USER_ERROR);
				return null;
		}
	}

	function delete($id, $protocol) {
		switch ($protocol) {
			case CONFD_LINE_PROTO_CUSTOM:
				return $this->_custom->delete($id);
			case CONFD_LINE_PROTO_SCCP:
				return $this->_sccp->delete($id);
			case CONFD_LINE_PROTO_SIP:
				return $this->_sip->delete($id);
			default:
				trigger_error('invalid endpoint protocol '.$protocol, E_USER_ERROR);
				return false;
		}
	}

	function update($id, $endpoint, $protocol)
	{
		switch ($protocol) {
			case CONFD_LINE_PROTO_CUSTOM:
				return $this->_custom->update($id, $endpoint);
			case CONFD_LINE_PROTO_SCCP:
				return $this->_sccp->update($id, $endpoint);
			case CONFD_LINE_PROTO_SIP:
				return $this->_sip->update($id, $endpoint);
			default:
				trigger_error('invalid endpoint protocol '.$protocol, E_USER_ERROR);
				return false;
		}
	}
}
