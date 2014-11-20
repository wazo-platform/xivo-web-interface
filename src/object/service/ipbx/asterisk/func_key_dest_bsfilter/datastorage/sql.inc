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

class xivo_service_asterisk_func_key_dest_bsfilter_sql extends xivo_service_asterisk_abstract_sql
{
	var $_disable		= null;

	public function get_func_key_ids_for_bsfilter($filter_id)
	{
		$this->_dso->new_select($this->_table, array('func_key_id'));

		$this->_dso->innerjoin('callfiltermember',
			'func_key_dest_bsfilter.filtermember_id = callfiltermember.id',
			null,
			array());

		$this->_dso->where(array('callfilterid' => $filter_id), 'callfiltermember');

		$rows = $this->_dso->select_all();
		if(isset($rows[0]) === false)
			return(false);

		$func_key_ids = array();
		foreach($rows as $row) {
			$func_key_ids[] = $row['func_key_id'];
		}

		return($func_key_ids);
	}

	public function get_func_key_ids_for_user($user_id)
	{
		$this->_dso->new_select($this->_table, array('func_key_id'));

		$this->_dso->innerjoin('callfiltermember',
			'func_key_dest_bsfilter.filtermember_id = callfiltermember.id',
			null,
			array());

		$this->_dso->where(array('type' => 'user',
								 'typeval' => (string) $user_id),
							'callfiltermember');

		$rows = $this->_dso->select_all();
		if($rows === false)
			return array();

		$func_key_ids = array();
		foreach($rows as $row) {
			$func_key_ids[] = $row['func_key_id'];
		}

		return($func_key_ids);
	}

	public function get_filtermember_ids_for_bsfilter($filter_id)
	{
		$this->_dso->new_select($this->_table, 'filtermember_id');

		$this->_dso->innerjoin('callfiltermember',
			'func_key_dest_bsfilter.filtermember_id = callfiltermember.id',
			null,
			array());

		$this->where(array('callfilterid' => $filter_id), 'callfiltermember');

		$rows = $this->_dso->select_all();
		if(isset($r[0]) === false)
			return(false);

		$member_ids = array();
		foreach($rows as $row) {
			$member_ids[] = $row['filtermember_id'];
		}

		return($func_key_ids);
	}

}

?>
