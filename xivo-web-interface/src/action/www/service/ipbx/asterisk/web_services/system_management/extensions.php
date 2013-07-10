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

$access_category = 'system_management';
$access_subcategory = 'extensions';

include(dwho_file::joinpath(dirname(__FILE__),'..','_common.php'));

if(defined('XIVO_LOC_UI_ACTION') === true)
	$act = XIVO_LOC_UI_ACTION;
else
	$act = $_QRY->get('act');

switch($act)
{
	case 'search':
	default:
		$act = 'search';
		$appcontext = &$ipbx->get_application('context');

		$context_id = $_QRY->get('context');

		if(($context = $appcontext->get($context_id)) === false) {
			$http_response->set_status_line(404);
			$http_response->send(true);
		}

		$obj = $_QRY->get('obj');

		if (($numbers_unavailable = $appcontext->get_extens_for_context_and_object($context_id, $obj)) === false) {
			$http_response->set_status_line(204);
			$http_response->send(true);
		}

		$number  = $_QRY->get('number');
		if(strlen($number) > 0 && !is_numeric($number)) {
			$http_response->set_status_line(500);
			$http_response->send(true);
		}

		$numbers_available = array();
		foreach($context['contextnumbers'][$obj] as $numb) {
			$start = intval($numb['numberbeg']);
			if(strlen($numb['numberend']) === 0) {
				array_push($numbers_available, $start);
				continue;
			}

			$end = intval($numb['numberend']);
			$numbers_available = array_merge($numbers_available, range($start, $end));
		}

		if(strlen($number) > 0) {
			function match($val) {
				global $number;
				return (strpos(strval($val), $number) !== false);
			}
			$numbers_available = array_filter($numbers_available, "match");
		}

		foreach($numbers_unavailable as $number_unavailable) {
			if(($idx = array_search($number_unavailable, $numbers_available)) !== false)
				unset($numbers_available[$idx]);
		}

		$_TPL->set_var('list', array_values($numbers_available));
		break;
}

$_TPL->set_var('act',$act);
$_TPL->display('/service/ipbx/'.$ipbx->get_name().'/generic');

?>
