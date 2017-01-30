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

$param['page'] = $page;

if(isset($_QR['uuid']) === false || $appmoh->get($_QR['uuid']) === false)
	$_QRY->go($_TPL->url('service/ipbx/pbx_services/musiconhold'),'act=list');

if(isset($_QR['filename']) === false
|| ($data = $appmoh->download_file($_QR['filename'])) === false)
	$_QRY->go($_TPL->url('service/ipbx/pbx_services/musiconhold'),$param);

$currenttime = mktime();

header('Pragma: no-cache');
header('Cache-Control: private, must-revalidate');
header('Last-Modified: '.
	date('D, d M Y H:i:s',$currenttime).' '.
	dwho_i18n::strftime_l('%Z',null));
header('Content-Disposition: attachment; filename='.$_QR['filename']);
header('Content-Type: application/octet-stream');

ob_start();

echo $data;

header('Content-Length: '.ob_get_length());
ob_end_flush();
die();

?>
