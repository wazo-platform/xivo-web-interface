<?php
#
# XiVO Web-Interface
# Copyright (C) 2014  Avencall
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

xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_entity_filter extends xivo_service_asterisk_abstract
{
	var $_dso			= null;
	var $_name			= 'entity_filter';
	var $_filter		= false;
	var $_origin		= false;
	var $_origin_list	= false;
	var $_identity		= false;

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_USER_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_load_config();
	}

	function _prepare($data)
	{
		$data['entity_id'] = (int) $data['entity_id'];
		$data['line_id'] = (int) $data['line_id'];
		$data['queue_id'] = (int) $data['queue_id'];
		$data['group_id'] = (int) $data['group_id'];
		$data['voicemail_id'] = (int) $data['voicemail_id'];
		$data['agent_id'] = (int) $data['agent_id'];
		$data['meetme_id'] = (int) $data['meetme_id'];
		$data['incall_id'] = (int) $data['incall_id'];
		$data['callfilter_id'] = (int) $data['callfilter_id'];
		$data['pickup_id'] = (int) $data['pickup_id'];
		$data['schedule_id'] = (int) $data['schedule_id'];

		return($data);
	}

	function add($arr,$id=0)
	{
		if(is_array($arr) === false)
			return(false);

		return(parent::add($arr,$id));
	}

	function edit($id,$arr,$disable=null)
	{
		if(is_array($arr) === false)
			return(false);

		return(parent::edit($id,$arr,$disable));
	}
}

?>
