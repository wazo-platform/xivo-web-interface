<?php

#
# XiVO Web-Interface
# Copyright (C) 2012  Avencall
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

xivo_service_asterisk::required(array('abstract','datastorage','abstract','sql.inc'),true);

class xivo_service_asterisk_stat_agent_periodic_sql extends xivo_service_asterisk_abstract_sql
{

	public function get_min_time()
	{
		$field = array((object) ('MIN(time)'));
		$this->_dso->new_select($this->_table,$field);

		$r = $this->_dso->select_singlecolumn();

		if(isset($r[0]) === false)
			return(false);

		return($r[0]);
	}

	public function get_max_time()
	{
		$field = array((object) ('MAX(time)'));
		$this->_dso->new_select($this->_table,$field);

		$r = $this->_dso->select_singlecolumn();

		if(isset($r[0]) === false)
			return(false);

		return($r[0]);
	}

	public function agent_productivity($interval, $agent_ids)
	{
		$field = array((object) ("(SUM(stat_call_on_queue.talktime)::float / extract('epoch' from SUM(stat_queue_periodic.login_time))) * 100 AS productivity"));

		$this->_stat_common_where($interval);

		$this->_dso->new_select($this->_table, $field);

		$this->_dso->innerjoin('stat_call_on_queue',
				       'stat_call_on_queue.agent_id = '.$this->_table.'.agent_id',
				       null,
				       '');

		$this->_dso->wherein('agent_id',$agent_ids);
		$this->_stat_common_where($interval);

		$r = $this->_dso->select_singlecolumn();
	}

	public function agent_time_interval($type, $interval, $agent_ids)
	{
		switch($type)
		{
			case 'login':
				$field = array((object) ('SUM("login_time")'));
				break;
			case 'pause':
				$field = array((object) ('SUM("pause_time")'));
				break;
			case 'wrapup':
				$field = array((object) ('SUM("wrapup_time")'));
				break;
			default;
				return(false);
		}

		$this->_dso->new_select($this->_table, $field);

		$this->_dso->wherein('agent_id',$agent_ids);
		$this->_stat_common_where($interval);

		$r = $this->_dso->select_singlecolumn();

		if(isset($r[0]) === false)
			return(false);

		return($r[0]);
	}
}

?>
