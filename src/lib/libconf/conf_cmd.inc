<?php

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


$array = array();

$array['accesswebservice'] = array();
$array['accesswebservice']['update_keys'] = array('reload');

$array['userfeatures'] = array();
$array['userfeatures']['ctibus'] = array('xivo[user,%s,%s]');
$array['userfeatures']['ipbx'] = array('dialplan reload', 'module reload app_queue.so', 'module reload res_pjsip.so');

$array['groupfeatures'] = array();
$array['groupfeatures']['ipbx'] = array('dialplan reload', 'module reload app_queue.so', 'module reload res_pjsip.so', 'module reload chan_sccp.so');

$array['linefeatures'] = array();
$array['linefeatures']['ctibus'] = array('xivo[phone,%s,%s]');
$array['linefeatures']['ipbx'] = array('module reload app_queue.so', 'dialplan reload', 'module reload res_pjsip.so');

$array['meetmefeatures'] = array();
$array['meetmefeatures']['ctibus'] = array('xivo[meetme,%s,%s]');
$array['meetmefeatures']['ipbx'] = array('dialplan reload', 'module reload app_queue.so');

$array['voicemail'] = array();
$array['voicemail']['ctibus'] = array('xivo[voicemail,%s,%s]');
$array['voicemail']['ipbx'] = array('voicemail reload');

$array['agentfeatures'] = array();
$array['agentfeatures']['ctibus'] = array('xivo[agent,%s,%s]', 'xivo[queuemember,update]');
$array['agentfeatures']['agentbus'] = array('agent.%s.%s');
$array['agentfeatures']['ipbx'] = array('module reload app_queue.so');

$array['agentglobalparams'] = array();
$array['agentglobalparams']['ctibus'] = array('xivo[cticonfig,update]');

$array['general'] = array();
$array['general']['ipbx'] = array('module reload res_pjsip.so');

$array['queue'] = array();
$array['queue']['ipbx'] = array('dialplan reload', 'module reload app_queue.so', 'module reload res_pjsip.so');

$array['queuefeatures'] = array();
$array['queuefeatures']['ctibus'] = array('xivo[queue,%s,%s]', 'xivo[queuemember,update]');
$array['queuefeatures']['agentbus'] = array('queue.%s.%s');
$array['queuefeatures']['ipbx'] = array('module reload app_queue.so', 'dialplan reload', 'module reload res_pjsip.so', 'module reload chan_sccp.so');

$array['queueskill'] = array();
$array['queueskill']['ipbx'] = array('module reload app_queue.so');

$array['queueskillrule'] = array();
$array['queueskillrule']['ipbx'] = array('module reload app_queue.so');

$array['cti*'] = array();
$array['cti*']['ctibus'] = array('xivo[cticonfig,update]');

$array['callfilter'] = array();
$array['callfilter']['ipbx'] = array('dialplan reload');

$array['callfiltermember'] = array();
$array['callfiltermember']['ipbx'] = array('dialplan reload');

$array['incall'] = array();
$array['incall']['ipbx'] = array('dialplan reload');

$array['outcall'] = array();
$array['outcall']['ipbx'] = array('dialplan reload');

$array['context'] = array();
$array['context']['ipbx'] = array('dialplan reload');

$array['pickup'] = array();
$array['pickup']['ipbx'] = array('module reload res_pjsip.so', 'module reload chan_sccp.so');

$array['staticsip'] = array();
$array['staticsip']['ipbx'] = array('module reload res_pjsip.so');

$array['trunksip'] = array();
$array['trunksip']['ipbx'] = array('dialplan reload', 'module reload res_pjsip.so');

$array['staticiax'] = array();
$array['staticiax']['ipbx'] = array('iax2 reload');

$array['trunkiax'] = array();
$array['trunkiax']['ipbx'] = array('dialplan reload', 'iax2 reload');

$array['extensions'] = array();
$array['extensions']['ipbx'] = array('dialplan reload', 'module reload features');

$array['sccp*'] = array();
$array['sccp*']['ipbx'] = array('module reload chan_sccp.so');

$array['configfiles'] = array();
$array['configfiles']['ipbx'] = array('dialplan reload');

# REALSTATIC FILE

$array['queues.conf'] = array();
$array['queues.conf']['ctibus'] = array('xivo[cticonfig,update]');
$array['queues.conf']['ipbx'] = array('module reload app_queue.so');

$array['meetme.conf'] = array();
$array['meetme.conf']['ctibus'] = array('xivo[cticonfig,update]');
$array['meetme.conf']['ipbx'] = array('module reload app_meetme.so');

$array['voicemail.conf'] = array();
$array['voicemail.conf']['ipbx'] = array('voicemail reload');

$array['iax.conf'] = array();
$array['iax.conf']['ipbx'] = array('iax2 reload');

$array['sip.conf'] = array();
$array['sip.conf']['ipbx'] = array('module reload res_pjsip.so');

$array['features.conf'] = array();
$array['features.conf']['ipbx'] = array('dialplan reload', 'module reload features', 'module reload res_parking.so');

?>
