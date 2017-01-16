<?php

#
# XiVO Web-Interface
# Copyright 2006-2017 The Wazo Authors  (see the AUTHORS file)
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

class xivo_service_asterisk_schedule_identity
{
	function _prepare_identity(&$data)
	{
		# name can be null, and thus identity can be null, and this can cause
		# some issues later on
		if ($data['name'] === null) {
			$data['identity'] = '-';
		} else {
			$data['identity'] = $data['name'];
		}

		return($data);
	}

	function mk_identity($arr)
	{
		if(is_array($arr) === false
		|| isset($arr['name']) === false)
			return(false);

		$this->_prepare_identity($arr);

		return($arr['identity']);
	}
}

?>
