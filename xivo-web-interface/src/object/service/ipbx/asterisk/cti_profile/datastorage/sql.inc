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

xivo_service_asterisk::required(array('abstract','datastorage','abstract','sql.inc'),true);

class xivo_service_asterisk_cti_profile_sql extends xivo_service_asterisk_abstract_sql
{
	var $_unique = array('name');

	public function cti_profile_id_with_name($profile_name)
	{
		$this->_dso->new_select($this->_table,'id');

		$this->_dso->where(array('name' => $profile_name));

		$r = $this->_dso->select_singlecolumn();

		if(isset($r[0]) === false)
			return(false);

		return((int) $r[0]);
	}
}

?>