<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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
$file_version_lts = '/usr/share/xivo/XIVO-VERSION-LTS';
$xivo_version = 'Unknown';

if (file_exists($file_version))
	$xivo_version = trim(file_get_contents($file_version));

$xivo_version_num = $xivo_version;

if (file_exists($file_version_lts)) {
	$xivo_version_lts = trim(file_get_contents($file_version_lts));
	if (!empty($xivo_version_lts))
		$xivo_version = $xivo_version_lts." (".$xivo_version.")";
}

define('XIVO_OPERATOR_SIP_CONFIG_DIR','/etc/xivo/operator-configuration');
define('XIVO_OPERATOR_SIP_CONFIG_MAX_BYTES','200000');
define('DWHO_LABEL_CUSTOM','XIVO-WEBI');

define('XIVO_SOFT_NAME','xivo');
define('XIVO_SOFT_LABEL','XIVO');
define('XIVO_SOFT_VERSION',$xivo_version);
define('XIVO_SOFT_CODENAME','');
define('XIVO_SOFT_URL','www.xivo.solutions');
define('XIVO_DOWNLOAD_URL','mirror.xivo.solutions/iso/archives/xivo-'.$xivo_version_num);
define('XIVO_DOC_URL','documentation.xivo.solutions');
define('XIVO_DOC_API_URL','documentation.xivo.solutions');
define('XIVO_BLOG_URL','www.xivo.solutions');
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

define('CONFD_LINE_PROTO_CUSTOM', 'custom');
define('CONFD_LINE_PROTO_SCCP', 'sccp');
define('CONFD_LINE_PROTO_SIP', 'sip');

?>
