<?php

#
# XiVO Web-Interface
# Copyright (C) 2012-2014  Avencall
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

class xivo_service_asterisk_stat_agent_periodic extends xivo_service_asterisk_abstract
{
	protected	$_dso			= null;
	protected	$_name			= 'stat_agent_periodic';
	protected	$_filter		= false;
	protected	$_agentfeatures	= null;
	protected	$_stat_agent	= null;

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_USER_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_agentfeatures = $this->_sre->get_module('agentfeatures');
		$this->_stat_agent = $this->_sre->get_module('stat_agent');
	}

	public function get_min_and_max_time()
	{
		if(($min = $this->_dso->get_min_time()) === false
		|| ($max = $this->_dso->get_max_time()) === false)
			return(false);

		$rs = array();
		$rs['min'] = $min;
		$rs['max'] = $max;

		return($rs);
	}

	public function get_productivity($interval, $agent_id)
	{
		return($this->_dso->agent_productivity($interval, $agent_id));
	}

	public function get_pause_time($interval, $agent_id)
	{
		$pause_time = $this->_dso->agent_time_interval('pause', $interval, $agent_id);
		return($this->_interval_value($pause_time));
	}

	public function get_wrapup_time($interval, $agent_id)
	{
		$wrapup_time = $this->_dso->agent_time_interval('wrapup', $interval, $agent_id);
		return($this->_interval_value($wrapup_time));
	}

	public function get_login_time($interval, $agent_id)
	{
		$login_time = $this->_dso->agent_time_interval('login', $interval, $agent_id);
		return($this->_interval_value($login_time));
	}
}

?>
