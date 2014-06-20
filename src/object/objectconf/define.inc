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


$file_version = '/usr/share/xivo/XIVO-VERSION';
$xivo_version = 'Unknown';
if (file_exists($file_version))
	$xivo_version = file_get_contents($file_version);

define('DWHO_LABEL_CUSTOM','XIVO-WEBI');

define('XIVO_SOFT_NAME','xivo');
define('XIVO_SOFT_LABEL','XIVO');
define('XIVO_SOFT_VERSION',trim($xivo_version));
define('XIVO_SOFT_CODENAME','Five');
define('XIVO_SOFT_URL','xivo.fr');
define('XIVO_DOWNLOAD_URL','mirror.xivo.fr/iso/archives/xivo-'.XIVO_SOFT_VERSION);
define('XIVO_DOC_URL','documentation.xivo.fr');
define('XIVO_BLOG_URL','blog.xivo.fr');
define('XIVO_CORP_LABEL','Avencall');
define('XIVO_PF_FR_CORP_LABEL','Avencall');
define('XIVO_CORP_URL','avencall.com');

define('XIVO_AV_FR_CORP_NAME'		,'Avencall');
define('XIVO_AV_FR_CORP_EMAIL'		,'contact@avencall.com');
define('XIVO_AV_FR_CORP_URL'		,'avencall.com');
define('XIVO_AV_FR_CORP_ADDRESS'	,'3 allée des Séquoïas <br> Le Chinook');
define('XIVO_AV_FR_CORP_ZIPCODE'	,'69760');
define('XIVO_AV_FR_CORP_CITY'		,'Limonest');
define('XIVO_AV_FR_CORP_COUNTRY'	,'France');
define('XIVO_AV_FR_CORP_PHONE'		,'+33 (0) 4 37 49 78 10');
define('XIVO_AV_FR_CORP_FAX'		,'+33 (0) 4 27 46 63 30');

define('XIVO_WS_HEADER_NAME_VERSION','X-XIVO-WS-VERSION');
define('XIVO_WS_VERSION','1.0');

?>
