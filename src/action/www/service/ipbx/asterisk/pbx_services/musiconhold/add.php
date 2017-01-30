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

$fm_save = null;

if(isset($_QR['fm_send']) === true)
{
	if($appmoh->set_add($_QR) === false
	|| $appmoh->add() === false)
	{
		$fm_save = false;
	}
	else
	{
		$_QRY->go($_TPL->url('service/ipbx/pbx_services/musiconhold'),$param);
	}
}

$_TPL->set_var('fm_save',$fm_save);

$element = $appmoh->get_elements();

?>
