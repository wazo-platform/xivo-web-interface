<?php
#
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

class xivo_service_asterisk_func_key_dest_agent extends xivo_service_asterisk_abstract
{
	var $_dso			= null;
	var $_name			= 'func_key_dest_agent';
	var $_filter		= false;
	var $_origin		= false;
	var $_origin_list	= false;
	var $_type_list		= false;

	var $_wonky_actions = array(
		'agentstaticlogtoggle' => 'toggle',
		'agentstaticlogin' => 'login',
		'agentstaticlogoff' => 'logoff'
	);

	var $_actions = array(
		'toggle' => 'agentstaticlogtoggle',
		'login' => 'agentstaticlogin',
		'logoff' => 'agentstaticlogoff',
	);

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
		$data['extension_id'] = (int) $data['extension_id'];
		$data['agent_id'] = (int) $data['agent_id'];
		$data['func_key_id'] = (int) $data['func_key_id'];
		$data['destination_type_id'] = (int) $data['destination_type_id'];
		return($data);
	}


	public function func_key_ids_for_agent($agent_id)
	{
		return($this->_dso->func_key_ids_for_agent($agent_id));
	}

	public function get_func_key_id_by_agent_and_action($agent_id, $action)
	{
		return($this->_dso->get_func_key_id_by_agent_and_action($agent_id, $action));
	}

	public function translate_wonky_action($wonky_action)
	{
		if (!array_key_exists($wonky_action, $this->_wonky_actions))
			return(false);

		return($this->_wonky_actions[$wonky_action]);
	}

	public function translate_action($action)
	{
		if (!array_key_exists($action, $this->_actions))
			return(false);

		return($this->_actions[$action]);
	}

	public function wonky_action_exists($wonky_action)
	{
		return array_key_exists($wonky_action, $this->_wonky_actions);
	}

}

?>
