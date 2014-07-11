<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2014  Avencall
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

class xivo_service_asterisk_user_line_sql extends xivo_service_asterisk_abstract_sql
{
	var $_unique	= array(array('user_id','line_id'));

	public function find_main_user_id($line_id)
	{
		$this->_dso->new_select($this->_table, 'user_id');

		$this->_dso->where(array('line_id' => $line_id));
		$this->_dso->andwhere("main_user = true");
		$this->_dso->andwhere("main_line = true");

		$user_id = $this->_dso->select_one();

		return $user_id;
	}
}

?>
