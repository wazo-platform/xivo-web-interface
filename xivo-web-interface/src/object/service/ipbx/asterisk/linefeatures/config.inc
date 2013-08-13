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

$array = array();

$array['provisioning_id'] = array();

$array['provisioning_id']['sip'] = array();
$array['provisioning_id']['sip']['prefix'] = 1;
$array['provisioning_id']['sip']['range'] = 100000;

$array['element'] = array();

$array['element']['protocol'] = array();
$array['element']['protocol']['value'] = array(XIVO_SRE_IPBX_AST_PROTO_SIP,
												XIVO_SRE_IPBX_AST_PROTO_SCCP,
												XIVO_SRE_IPBX_AST_PROTO_CUSTOM);
$array['element']['protocol']['default'] = XIVO_SRE_IPBX_AST_PROTO_SIP;

$array['element']['protocolid'] = array();
$array['element']['protocolid']['default'] = 0;

$array['element']['provisioningid'] = array();
$array['element']['provisioningid']['default'] = 0;

$array['element']['name'] = array();
$array['element']['name']['default'] = '';

$array['element']['context'] = array();
$array['element']['context']['default'] = 'default';

$array['element']['number'] = array();
$array['element']['number']['default'] = '';

$array['element']['configregistrar'] = array();
$array['element']['configregistrar']['default'] = 'default';
$array['element']['configregistrar']['set'] = true;

$array['element']['device'] = array();
$array['element']['device']['default'] = '';

$array['element']['num'] = array();
$array['element']['num']['value'] = array(1,2,3,4,5,6,7,8,9,10,11,12);
$array['element']['num']['default'] = 1;

$array['element']['description'] = array();
$array['element']['description']['default'] = '';


$array['filter'] = array();
$array['filter']['protocol'] = array('key' => $array['element']['protocol']['value']);
$array['filter']['protocolid'] = array('cast' => 'ulongint');
$array['filter']['num'] = array('set' => false,'cast' => 'ulongint');
$array['filter']['name'] = array('minlen' => 1,'maxlen' => 128,'regexp' => '/^[a-z0-9#\*\.\/_@:-]+$/i');
$array['filter']['configregistrar'] = array('set' => false,'chk' => 2,'maxlen' => 128,'regexp' => '/^[a-z0-9#\*\.\/_@:-]+$/i');
$array['filter']['device'] =  array('set' => false,'chk' => 2,'regexp' => '/^[a-z0-9]+$/','maxlen' => 32);
$array['filter']['context'] = array('callback' => 'xivo_service_asterisk::chk_context',
				    'callbackarg' => array(array(
							XIVO_SRE_IPBX_AST_CONTEXT_AUTHENTICATION,
							XIVO_SRE_IPBX_AST_CONTEXT_ZONEMESSAGES)));
$array['filter']['number'] = array('set' => false,'chk' => 2,'regexp' => '/^[0-9\*#]+$/','maxlen' => 40);
$array['filter']['description'] = array('set' => false,'chk' => 2,'maxlen' => 1000,'cast' => 'sgraph_crlf_tab','eol' => true);

?>
