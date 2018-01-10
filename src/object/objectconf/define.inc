<?php

#
# XiVO Web-Interface
# Copyright 2006-2018 The Wazo Authors  (see the AUTHORS file)
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

define('XIVO_SOFT_NAME','wazo');
define('XIVO_SOFT_LABEL','Wazo');
define('XIVO_SOFT_VERSION',trim($xivo_version));
define('XIVO_SOFT_CODENAME','phoenix-stretch');
define('XIVO_SOFT_URL','wazo.community');
define('XIVO_DOWNLOAD_URL','mirror.wazo.community/iso/archives/wazo-'.XIVO_SOFT_VERSION);
define('XIVO_DOC_URL','documentation.wazo.community');
define('XIVO_DOC_API_URL','api.wazo.community');
define('XIVO_BLOG_URL','blog.wazo.community');

define('XIVO_WS_HEADER_NAME_VERSION','X-XIVO-WS-VERSION');
define('XIVO_WS_VERSION','1.0');

define('CONFD_LINE_PROTO_CUSTOM', 'custom');
define('CONFD_LINE_PROTO_SCCP', 'sccp');
define('CONFD_LINE_PROTO_SIP', 'sip');

?>
