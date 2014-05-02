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

require_once('define.inc');

$array = array();

$array['appcustom'] = array();

$array['appcustom']['webservices'] = array();
$array['appcustom']['webservices']['phonebook'] = true;

$array['realstatic'] = array();

$array['realstatic']['features'] = array();
$array['realstatic']['features']['datastorage'] = true;

$array['realstatic']['staticiax'] = array();
$array['realstatic']['staticiax']['datastorage'] = true;

$array['realstatic']['staticmeetme'] = array();
$array['realstatic']['staticmeetme']['datastorage'] = true;

$array['realstatic']['staticqueue'] = array();
$array['realstatic']['staticqueue']['datastorage'] = true;

$array['realstatic']['staticsip'] = array();
$array['realstatic']['staticsip']['datastorage'] = true;

$array['realstatic']['staticvoicemail'] = array();
$array['realstatic']['staticvoicemail']['datastorage'] = true;

$array['module'] = array();

$array['module']['accessfeatures'] = array();
$array['module']['accessfeatures']['datastorage'] = true;

$array['module']['agentfeatures'] = array();
$array['module']['agentfeatures']['datastorage'] = true;

$array['module']['agentgroup'] = array();
$array['module']['agentgroup']['datastorage'] = true;

$array['module']['agentglobalparams'] = array();
$array['module']['agentglobalparams']['datastorage'] = true;

$array['module']['agentqueueskill'] = array();
$array['module']['agentqueueskill']['datastorage'] = true;

$array['module']['attachment'] = array();
$array['module']['attachment']['datastorage'] = true;

$array['module']['autoprov'] = array();
$array['module']['autoprov']['datastorage'] = true;

$array['module']['backupfiles'] = array();
$array['module']['backupfiles']['datastorage'] = false;

$array['module']['callerid'] = array();
$array['module']['callerid']['datastorage'] = true;

$array['module']['callfilter'] = array();
$array['module']['callfilter']['datastorage'] = true;

$array['module']['callfiltermember'] = array();
$array['module']['callfiltermember']['datastorage'] = true;

$array['module']['cel'] = array();
$array['module']['cel']['datastorage'] = true;

$array['module']['configfiles'] = array();
$array['module']['configfiles']['datastorage'] = false;

$array['module']['context'] = array();
$array['module']['context']['datastorage'] = true;

$array['module']['contextinclude'] = array();
$array['module']['contextinclude']['datastorage'] = true;

$array['module']['contextmember'] = array();
$array['module']['contextmember']['datastorage'] = true;

$array['module']['contextnumbers'] = array();
$array['module']['contextnumbers']['datastorage'] = true;

$array['module']['contexttype'] = array();
$array['module']['contexttype']['datastorage'] = true;

$array['module']['cti_preference'] = array();
$array['module']['cti_preference']['datastorage'] = true;

$array['module']['cti_profile'] = array();
$array['module']['cti_profile']['datastorage'] = true;

$array['module']['cti_profile_preference'] = array();
$array['module']['cti_profile_preference']['datastorage'] = true;

$array['module']['cti_profile_service'] = array();
$array['module']['cti_profile_service']['datastorage'] = true;

$array['module']['cti_profile_xlet'] = array();
$array['module']['cti_profile_xlet']['datastorage'] = true;

$array['module']['cti_service'] = array();
$array['module']['cti_service']['datastorage'] = true;

$array['module']['cti_xlet'] = array();
$array['module']['cti_xlet']['datastorage'] = true;

$array['module']['cti_xlet_layout'] = array();
$array['module']['cti_xlet_layout']['datastorage'] = true;

$array['module']['cticontexts'] = array();
$array['module']['cticontexts']['datastorage'] = true;

$array['module']['ctidirectories'] = array();
$array['module']['ctidirectories']['datastorage'] = true;

$array['module']['ctidirectoryfields'] = array();
$array['module']['ctidirectoryfields']['datastorage'] = true;

$array['module']['ctidisplays'] = array();
$array['module']['ctidisplays']['datastorage'] = true;

$array['module']['ctimain'] = array();
$array['module']['ctimain']['datastorage'] = true;

$array['module']['ctiphonehints'] = array();
$array['module']['ctiphonehints']['datastorage'] = true;

$array['module']['ctiphonehintsgroup'] = array();
$array['module']['ctiphonehintsgroup']['datastorage'] = true;

$array['module']['ctipresences'] = array();
$array['module']['ctipresences']['datastorage'] = true;

$array['module']['ctistatus'] = array();
$array['module']['ctistatus']['datastorage'] = true;

$array['module']['ctireversedirectories'] = array();
$array['module']['ctireversedirectories']['datastorage'] = true;

$array['module']['ctisheets'] = array();
$array['module']['ctisheets']['datastorage'] = true;

$array['module']['ctisheetactions'] = array();
$array['module']['ctisheetactions']['datastorage'] = true;

$array['module']['ctisheetevents'] = array();
$array['module']['ctisheetevents']['datastorage'] = true;

$array['module']['dialaction'] = array();
$array['module']['dialaction']['datastorage'] = true;

$array['module']['dialpattern'] = array();
$array['module']['dialpattern']['datastorage'] = true;

$array['module']['extensions'] = array();
$array['module']['extensions']['datastorage'] = true;

$array['module']['func_key'] = array();
$array['module']['func_key']['datastorage'] = true;

$array['module']['func_key_destination_type'] = array();
$array['module']['func_key_destination_type']['datastorage'] = true;

$array['module']['func_key_dest_user'] = array();
$array['module']['func_key_dest_user']['datastorage'] = true;

$array['module']['func_key_dest_group'] = array();
$array['module']['func_key_dest_group']['datastorage'] = true;

$array['module']['func_key_dest_queue'] = array();
$array['module']['func_key_dest_queue']['datastorage'] = true;

$array['module']['func_key_dest_conference'] = array();
$array['module']['func_key_dest_conference']['datastorage'] = true;

$array['module']['func_key_mapping'] = array();
$array['module']['func_key_mapping']['datastorage'] = true;

$array['module']['func_key_template'] = array();
$array['module']['func_key_template']['datastorage'] = true;

$array['module']['func_key_type'] = array();
$array['module']['func_key_type']['datastorage'] = true;

$array['module']['general'] = array();
$array['module']['general']['datastorage'] = true;

$array['module']['groupfeatures'] = array();
$array['module']['groupfeatures']['datastorage'] = true;

$array['module']['iaxcallnumberlimits'] = array();
$array['module']['iaxcallnumberlimits']['datastorage'] = true;

$array['module']['incall'] = array();
$array['module']['incall']['datastorage'] = true;

$array['module']['ldapfilter'] = array();
$array['module']['ldapfilter']['datastorage'] = true;

$array['module']['linefeatures'] = array();
$array['module']['linefeatures']['datastorage'] = true;

$array['module']['logfiles'] = array();
$array['module']['logfiles']['datastorage'] = false;

$array['module']['meetmefeatures'] = array();
$array['module']['meetmefeatures']['datastorage'] = true;

$array['module']['meetmeguest'] = array();
$array['module']['meetmeguest']['datastorage'] = true;

$array['module']['musiconhold'] = array();
$array['module']['musiconhold']['datastorage'] = true;

$array['module']['operator'] = array();
$array['module']['operator']['datastorage'] = true;

$array['module']['operator_destination'] = array();
$array['module']['operator_destination']['datastorage'] = true;

$array['module']['operator_trunk'] = array();
$array['module']['operator_trunk']['datastorage'] = true;

$array['module']['outcall'] = array();
$array['module']['outcall']['datastorage'] = true;

$array['module']['outcalltrunk'] = array();
$array['module']['outcalltrunk']['datastorage'] = true;

$array['module']['paging'] = array();
$array['module']['paging']['datastorage'] = true;

$array['module']['paginguser'] = array();
$array['module']['paginguser']['datastorage'] = true;

$array['module']['parkinglot'] = array();
$array['module']['parkinglot']['datastorage'] = true;

$array['module']['phonebook'] = array();
$array['module']['phonebook']['datastorage'] = true;

$array['module']['phonebookaddress'] = array();
$array['module']['phonebookaddress']['datastorage'] = true;

$array['module']['phonebooknumber'] = array();
$array['module']['phonebooknumber']['datastorage'] = true;

$array['module']['phonefunckey'] = array();
$array['module']['phonefunckey']['datastorage'] = true;

$array['module']['pickup'] = array();
$array['module']['pickup']['datastorage'] = true;

$array['module']['pickupmember'] = array();
$array['module']['pickupmember']['datastorage'] = true;

$array['module']['queue'] = array();
$array['module']['queue']['datastorage'] = true;

$array['module']['queuefeatures'] = array();
$array['module']['queuefeatures']['datastorage'] = true;

$array['module']['queuemember'] = array();
$array['module']['queuemember']['datastorage'] = true;

$array['module']['queuepenalty'] = array();
$array['module']['queuepenalty']['datastorage'] = true;

$array['module']['queuepenaltychange'] = array();
$array['module']['queuepenaltychange']['datastorage'] = true;

$array['module']['queueskill'] = array();
$array['module']['queueskill']['datastorage'] = true;

$array['module']['queueskillcat'] = array();
$array['module']['queueskillcat']['datastorage'] = true;

$array['module']['queueskillrule'] = array();
$array['module']['queueskillrule']['datastorage'] = true;

$array['module']['rightcall'] = array();
$array['module']['rightcall']['datastorage'] = true;

$array['module']['rightcallexten'] = array();
$array['module']['rightcallexten']['datastorage'] = true;

$array['module']['rightcallmember'] = array();
$array['module']['rightcallmember']['datastorage'] = true;

$array['module']['sccpline'] = array();
$array['module']['sccpline']['datastorage'] = true;

$array['module']['sccpdevice'] = array();
$array['module']['sccpdevice']['datastorage'] = true;

$array['module']['sccpgeneralsettings'] = array();
$array['module']['sccpgeneralsettings']['datastorage'] = true;

$array['module']['schedule'] = array();
$array['module']['schedule']['datastorage'] = true;

$array['module']['schedule_path'] = array();
$array['module']['schedule_path']['datastorage'] = true;

$array['module']['schedule_time'] = array();
$array['module']['schedule_time']['datastorage'] = true;

$array['module']['serverfeatures'] = array();
$array['module']['serverfeatures']['datastorage'] = true;

$array['module']['sipauthentication'] = array();
$array['module']['sipauthentication']['datastorage'] = true;

$array['module']['stat_agent'] = array();
$array['module']['stat_agent']['datastorage'] = true;

$array['module']['stat_agent_periodic'] = array();
$array['module']['stat_agent_periodic']['datastorage'] = true;

$array['module']['stat_call_on_queue'] = array();
$array['module']['stat_call_on_queue']['datastorage'] = true;

$array['module']['stat_queue'] = array();
$array['module']['stat_queue']['datastorage'] = true;

$array['module']['stat_queue_periodic'] = array();
$array['module']['stat_queue_periodic']['datastorage'] = true;

$array['module']['sounds'] = array();
$array['module']['sounds']['datastorage'] = false;

$array['module']['trunkcustom'] = array();
$array['module']['trunkcustom']['datastorage'] = true;

$array['module']['trunkfeatures'] = array();
$array['module']['trunkfeatures']['datastorage'] = true;

$array['module']['trunkiax'] = array();
$array['module']['trunkiax']['datastorage'] = true;

$array['module']['trunksip'] = array();
$array['module']['trunksip']['datastorage'] = true;

$array['module']['user_line'] = array();
$array['module']['user_line']['datastorage'] = true;

$array['module']['usercustom'] = array();
$array['module']['usercustom']['datastorage'] = true;

$array['module']['userfeatures'] = array();
$array['module']['userfeatures']['datastorage'] = true;

$array['module']['usersip'] = array();
$array['module']['usersip']['datastorage'] = true;

$array['module']['voicemail'] = array();
$array['module']['voicemail']['datastorage'] = true;

$array['module']['voicemenu'] = array();
$array['module']['voicemenu']['datastorage'] = true;


$array['protocol'] = array();
$array['protocol'][XIVO_SRE_IPBX_AST_PROTO_SIP]    = array('module' => 'usersip');
$array['protocol'][XIVO_SRE_IPBX_AST_PROTO_SCCP]    = array('module' => 'sccpline');
$array['protocol'][XIVO_SRE_IPBX_AST_PROTO_CUSTOM] = array('module' => 'usercustom');

$array['channel'] = array();
$array['channel']['sip'] = XIVO_SRE_IPBX_AST_CHAN_SIP;
$array['channel']['sccp'] = XIVO_SRE_IPBX_AST_CHAN_SCCP;
$array['channel']['iax'] = XIVO_SRE_IPBX_AST_CHAN_IAX;
$array['channel']['local'] = XIVO_SRE_IPBX_AST_CHAN_LOCAL;
$array['channel']['agent'] = XIVO_SRE_IPBX_AST_CHAN_AGENT;

$array['format'] = array();
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_WAV] = array(
				'type'		=> 'audio',
				'extension'	=> 'wav',
				'mime'		=> array('audio/wav',
							 'audio/x-wav',
							 'application/octet-stream'));
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_WAV49] = array(
				'type'		=> 'audio',
				'extension'	=> array('WAV','wav49'),
				'mime'		=> array('audio/wav',
							 'audio/x-wav',
							 'application/octet-stream'));
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_GSM] = array(
				'type'		=> 'audio',
				'extension'	=> 'gsm',
				'mime'		=> array('audio/gsm',
							 'audio/x-gsm',
							 'application/octet-stream'));
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_OGG] = array(
				'type'		=> 'audio',
				'extension'	=> 'ogg',
				'mime'		=> array('audio/ogg',
							 'audio/x-ogg',
							 'application/octet-stream',
							 'application/ogg',
							 'application/x-ogg'));
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_ULAW] = array(
				'type'		=> 'audio',
				'extension'	=> array('ul','ulaw','um','pcm'),
				'mime'		=> 'application/octet-stream');
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_ALAW] = array(
				'type'		=> 'audio',
				'extension'	=> array('al','alaw'),
				'mime'		=> 'application/octet-stream');
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_AU] = array(
				'type'		=> 'audio',
				'extension'	=> 'au',
				'mime'		=> 'audio/basic');
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_VOX] = array(
				'type'		=> 'audio',
				'extension'	=> 'vox',
				'mime'		=> 'application/octet-stream');
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_SLIN] = array(
				'type'		=> 'audio',
				'extension'	=> array('sln','raw'),
				'mime'		=> 'application/octet-stream');
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_MP3] = array(
				'type'		=> 'audio',
				'extension'	=> 'mp3',
				'mime'		=> array('audio/mp3',
							 'audio/x-mp3',
							 'audio/mpeg',
							 'audio/x-mpeg',
							 'audio/mpeg3',
							 'audio/x-mpeg3'));
$array['format'][XIVO_SRE_IPBX_AST_FORMAT_AUDIO_G722] = array(
				'type'		=> 'audio',
				'extension'	=> array('g722'),
				'mime'		=> 'application/octet-stream');

?>
