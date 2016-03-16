<?php

#
# XiVO Web-Interface
# Copyright (C) 2010-2016 Avencall
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

if(xivo_user::chk_acl('settings', 'queues', 'service/callcenter') === false) {
	if(xivo_user::chk_acl('settings', 'agents', 'service/callcenter') === true) {
		$_QRY->go($_TPL->url('callcenter/settings/agents'));
	} else if(xivo_user::chk_acl('settings', 'queuepenalty', 'service/callcenter') === true) {
		$_QRY->go($_TPL->url('callcenter/settings/queuepenalty'));
	} else if(xivo_user::chk_acl('settings', 'queueskills', 'service/callcenter') === true) {
		$_QRY->go($_TPL->url('callcenter/settings/queueskills'));
	} else if(xivo_user::chk_acl('settings', 'queueskillrules', 'service/callcenter') === true) {
		$_QRY->go($_TPL->url('callcenter/settings/queueskillrules'));
	} else {
		$_QRY->go($_TPL->url('xivo'));
	}
}

$_QRY->go($_TPL->url('callcenter/settings/queues'));

?>
