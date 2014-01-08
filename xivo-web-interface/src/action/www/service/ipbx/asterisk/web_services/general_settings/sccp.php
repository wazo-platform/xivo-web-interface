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

$access_category = 'general_settings';
$access_subcategory = 'sccp';

include(dwho_file::joinpath(dirname(__FILE__),'..','_common.php'));

$appsccpgeneralsettings = &$ipbx->get_application('sccpgeneralsettings');

$act = $_QRY->get('act');

switch($act)
{
	case 'edit':
		if($appsccpgeneralsettings->edit_from_json() === false)
		{
			if ($appsccpgeneralsettings->get_filter_errnb() > 0)
				print_r($appsccpgeneralsettings->get_filter_error());
			$http_response->set_status_line(400);
			$http_response->send(true);
		}

		$http_response->set_status_line(200);
		$http_response->send(true);
		break;
	case 'view':
	default:
		$info = $appsccpgeneralsettings->get_options();
		$_TPL->set_var('info', $info);
}

$_TPL->set_var('act',$act);
$_TPL->display('/service/ipbx/'.$ipbx->get_name().'/generic');

?>
