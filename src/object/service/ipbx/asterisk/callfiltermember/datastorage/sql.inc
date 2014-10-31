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

xivo_service_asterisk::required(array('abstract','datastorage','abstract','sql.inc'),true);

class xivo_service_asterisk_callfiltermember_sql extends xivo_service_asterisk_abstract_sql
{
	var $_unique	= array(array('callfilterid','type','typeval'));
	var $_disable	= null;

	public function get_by_boss_secretary($boss_id, $secreatary_id)
	{
		$this->_dso->new_select($this->_table);

		$this->_dso->where(array('type' => 'user'));
		$subquery = '(SELECT "callfilterid" FROM "callfiltermember" WHERE "type" = \'user\' AND "bstype" = \'boss\'  AND "typeval" = \''.$boss_id.'\')';
		$this->_dso->andwhere(array('callfilterid' => (object) $subquery));
		$this->_dso->andwhere(array('typeval' => $secreatary_id));
		$this->_dso->andwhere(array('bstype' => 'secretary'));

		$r = $this->_dso->select_single();

		return($r);
	}

	public function get_secretary_member_ids_for_bsfilter($filter_id)
	{
		$this->_dso->new_select($this->_table, array('id'));

		$this->_dso->where(array('type' => 'user',
								 'bstype' => 'secretary',
								 'callfilterid' => $filter_id));

		$rows = $this->_dso->select_all();
		if(isset($rows[0]) === false)
			return(false);

		$member_ids = array();
		foreach($rows as $row) {
			$member_ids[] = $row['id'];
		}

		return($member_ids);
	}
}

?>
