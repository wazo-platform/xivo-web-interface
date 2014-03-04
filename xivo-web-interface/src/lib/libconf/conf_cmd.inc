<?php


$array = array();

$array['userfeatures'] = array();
$array['userfeatures']['ctibus'] = array('xivo[user,%s,%s]');
$array['userfeatures']['ipbx'] = array('dialplan reload', 'module reload app_queue.so', 'sip reload');

$array['groupfeatures'] = array();
$array['groupfeatures']['ctibus'] = array('xivo[group,%s,%s]');
$array['groupfeatures']['ipbx'] = array('dialplan reload', 'module reload app_queue.so', 'sip reload');

$array['linefeatures'] = array();
$array['linefeatures']['ctibus'] = array('xivo[phone,%s,%s]');
$array['linefeatures']['ipbx'] = array('module reload app_queue.so', 'dialplan reload', 'sip reload');

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
$array['general']['ipbx'] = array('sip reload');

$array['queue'] = array();
$array['queue']['ipbx'] = array('dialplan reload', 'module reload app_queue.so', 'sip reload');

$array['queuefeatures'] = array();
$array['queuefeatures']['ctibus'] = array('xivo[queue,%s,%s]', 'xivo[queuemember,update]');
$array['queuefeatures']['agentbus'] = array('queue.%s.%s');
$array['queuefeatures']['ipbx'] = array('module reload app_queue.so', 'dialplan reload', 'sip reload');

$array['queueskill'] = array();
$array['queueskill']['ipbx'] = array('module reload app_queue.so');

$array['queueskillrule'] = array();
$array['queueskillrule']['ipbx'] = array('module reload app_queue.so');

$array['cti*'] = array();
$array['cti*']['ctibus'] = array('xivo[cticonfig,update]');

$array['ctidirectories'] = array();
$array['ctidirectories']['dird'] = array('update-config');

$array['ctireversedirectories'] = array();
$array['ctireversedirectories']['dird'] = array('update-config');

$array['callfilter'] = array();
$array['callfilter']['ipbx'] = array('dialplan reload');

$array['callfiltermember'] = array();
$array['callfiltermember']['ipbx'] = array('dialplan reload');

$array['incall'] = array();
$array['incall']['ipbx'] = array('dialplan reload');

$array['outcall'] = array();
$array['outcall']['ipbx'] = array('dialplan reload');

$array['voicemenu'] = array();
$array['voicemenu']['ipbx'] = array('dialplan reload');

$array['context'] = array();
$array['context']['ipbx'] = array('dialplan reload');

$array['pickup'] = array();
$array['pickup']['ipbx'] = array('sip reload');

$array['staticsip'] = array();
$array['staticsip']['ipbx'] = array('sip reload');

$array['trunksip'] = array();
$array['trunksip']['ipbx'] = array('dialplan reload', 'sip reload');

$array['staticiax'] = array();
$array['staticiax']['ipbx'] = array('iax2 reload');

$array['trunkiax'] = array();
$array['trunkiax']['ipbx'] = array('dialplan reload', 'iax2 reload');

$array['extensions'] = array();
$array['extensions']['ipbx'] = array('dialplan reload', 'features reload');

$array['musiconhold'] = array();
$array['musiconhold']['ipbx'] = array('moh reload');

$array['parkinglot'] = array();
$array['parkinglot']['ipbx'] = array('features reload');

$array['phonebook'] = array();
$array['phonebook']['dird'] = array('update-phonebook');
$array['phonebook']['ctibus'] = array('xivo[phonebook,%s,%s]');

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
$array['sip.conf']['ipbx'] = array('sip reload');

$array['features.conf'] = array();
$array['features.conf']['ipbx'] = array('dialplan reload', 'features reload');

?>
