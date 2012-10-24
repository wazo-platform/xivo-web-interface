<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Avencall
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

class stats_ressource_agent extends xivo_stats_ressource_abstract
{

	protected	$_type = 'agent';

	public function __construct(&$_XS)
	{
		parent::__construct(&$_XS);
	}

	protected function _build_stat($obj_process, $key, $interval)
	{
		$agent_ids = $this->_stat_agent->id_from_numbers($obj_process['keydata']);
		if (! $agent_ids)
			return;

		$conversation_time = $this->_stat_call_on_queue->get_agent_conversation_time($interval, $agent_ids);
		$answered = $this->_stat_call_on_queue->get_count_agent_answered($interval, $agent_ids);
		$login_time = $this->_stat_agent_periodic->get_login_time($interval, $agent_ids);
		$pause_time = $this->_stat_agent_periodic->get_pause_time($interval, $agent_ids);
		$wrapup_time = $this->_stat_agent_periodic->get_wrapup_time($interval, $agent_ids);
		#$productivity = $this->_stat_agent_periodic->get_productivity($interval, $agent_ids);

		#$this->_result[$key]['productivity']['result'] = $productivity;
		$this->_result[$key]['connect']['result'] = $answered;

		$this->_result[$key]['login_time']['result'] = $login_time;
		$this->_result[$key]['conversationtime']['result'] = $conversation_time;
		$this->_result[$key]['pause_time']['result'] = $pause_time;
		$this->_result[$key]['wrapup_time']['result'] = $wrapup_time;

		return($this->_result);
	}

}

?>