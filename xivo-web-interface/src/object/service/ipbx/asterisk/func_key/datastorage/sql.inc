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

class xivo_service_asterisk_func_key_sql extends xivo_service_asterisk_abstract_sql
{
	var $_disable		= null;

	public function get_func_key_v2($user_id)
	{
		$this->_dso->new_select(array('user' => 'userfeatures'), null);
		$this->_dso->where(array('id' => $user_id), 'user');

		$this->_dso->innerjoin('func_key_template',
					'func_key_template'.'."id" = "user"."func_key_private_template_id"',
					null,
					array('template_id' => 'id'));

		$this->_dso->innerjoin('func_key_mapping',
					'func_key_template'.'.id = '.'func_key_mapping'.'.template_id',
					null,
					array('position'	=> 'position',
							'blf'	=> 'blf',
							'label'	=> 'label'));

		$this->_dso->innerjoin('func_key',
					'func_key'.'.id = '.'func_key_mapping'.'.func_key_id',
					null,
					array('func_key_id'	=> 'id'));

		$this->_dso->innerjoin('func_key_destination_type',
					'func_key_destination_type'.'.id = '.'func_key'.'.type_id',
					null,
					array('destination_type_id' => 'id',
							'destination_type_name' => 'name'));

		$this->_dso->leftjoin('func_key_dest_user',
					'func_key_dest_user'.'.func_key_id = '.'func_key'.'.id',
					null,
					array('dst_user_id' => 'user_id'));

		$this->_dso->leftjoin('func_key_type',
					'func_key_type'.'.id = '.'func_key'.'.type_id',
					null,
					array('type' => 'name'));

		$this->_dso->new_order(array('position' => SORT_ASC), null, 'func_key_mapping');

		$r = $this->_dso->select_all(null, true, true);

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}
}

?>