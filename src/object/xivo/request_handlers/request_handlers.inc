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

xivo_object::required(array('abstract','abstract.inc'),true);

class xivo_object_request_handlers extends xivo_object_abstract
{
	protected $_name      = 'request_handlers';
	protected $_sysconfd  = false;

	public function __construct(&$xobj,$param=array())
	{
		if(($this->_sysconfd = &$xobj->get_module('sysconfd')) === false)
			return(false);

	}

	public function exec_cmd($cmds)
	{
		if(dwho_constant('XIVO_WEBI_CONFIGURED',true) === false)
			return(null);

		if ($this->_sysconfd->request_post('/exec_request_handlers', $cmds) === false
		|| $this->_sysconfd->last_status_code() !== 200) {
			return(false);
		}

		return(true);
	}
}

?>
