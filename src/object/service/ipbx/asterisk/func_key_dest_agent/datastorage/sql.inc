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

xivo_service_asterisk::required(array('abstract','datastorage','abstract','sql.inc'),true);

class xivo_service_asterisk_func_key_dest_agent_sql extends xivo_service_asterisk_abstract_sql
{
	var $_disable		= null;

	public function func_key_ids_for_agent($agent_id)
	{
		$this->_dso->new_select('func_key_dest_agent', array('func_key_id'));

		$this->_dso->where(array('agent_id' => $agent_id), 'func_key_dest_agent');

		$rows = $this->_dso->select_all(null,true,true);

		if(count($rows) === 0)
			return(false);

		$func_key_ids = array();
		foreach($rows as $row) {
			$func_key_ids[] = $row['func_key_dest_agent.func_key_id'];
		}

		return($func_key_ids);
	}

	public function get_func_key_id_by_agent_and_action($agent_id, $action)
	{
		$this->_dso->new_select('func_key_dest_agent', array('func_key_id'));

		$conditions = array('agent_id' => $agent_id, 'action' => $action);

		$this->_dso->where($conditions, 'func_key_dest_agent');

		$r = $this->_dso->select_singlecolumn();

		if(isset($r[0]) === false)
			return(false);

		return((int) array_pop($r));
	}
}

?>
