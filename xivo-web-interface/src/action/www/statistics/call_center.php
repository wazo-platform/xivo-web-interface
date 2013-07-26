<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Avencall
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

if(xivo_user::chk_acl('settings','configuration') === false) {
	if(xivo_user::chk_acl('data','stats1') === true) {
		$_QRY->go($_TPL->url('statistics/call_center/data/stats1'));
	} else if(xivo_user::chk_acl('data','stats2') === true) {
		$_QRY->go($_TPL->url('statistics/call_center/data/stats2'));
	//} else if(xivo_user::chk_acl('data','stats3') === true) {
		//$_QRY->go($_TPL->url('statistics/call_center/data/stats3'));
	} else if(xivo_user::chk_acl('data','stats4') === true) {
		$_QRY->go($_TPL->url('statistics/call_center/data/stats4'));
	}
}

$_QRY->go($_TPL->url('statistics/call_center/settings/configuration'));

?>
