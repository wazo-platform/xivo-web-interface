<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016 Avencall
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


dwho::load_class('dwho_network');
dwho::load_class('dwho_http');

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'bus','publisher.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'funckey', 'mapper.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'funckey', 'funckey.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'voicemail', 'mapper.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'line', 'shim.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'line', 'manager.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'line', 'mapper.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'converter', 'extension', 'shim.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'client.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'user', 'user.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'line', 'line.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'extension', 'extension.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'facade', 'line.inc'));

class xivo_application_service_asterisk_user extends xivo_application_asterisk_abstract
{
	protected $_dso				= null;
	protected $_userfeatures 	= false;
	protected $_rightcall		= false;
	protected $_dialaction		= false;
	protected $_queuemember		= null;
	protected $_agentfeatures	= null;
	protected $_linefeatures	= false;
	protected $_line			= false;
	protected $_cti_profile     = false;
	protected $_appentity		= false;
	protected $_entity			= false;
	protected $_provdconfig		= false;
	protected $_provddevice		= false;
	protected $_device		 	= false;
	protected $_sccpdevice		= false;
	protected $_musiconhold		= array();
	protected $_profileclientlist	= array();
	protected $_conf			= array();
	protected $_param			= array();
	protected $_destination		= null;
	protected $_schedule_path 	= false;
	protected $_func_key_manager	= false;
	protected $_callfilter = false;
	protected $_callfiltermember = false;
	protected $_func_key_agent = null;
	protected $_func_keys = null;
	protected $_vm_mapper = null;
	protected $_voicemail = null;
	protected $_confd_user = null;
	protected $_line_manager = null;
	protected $_bus_msg_factory = null;

	public function __construct(&$service,&$dso,$param=null,$components=true)
	{
		parent::__construct($service);

		if(is_object($dso) === false)
			trigger_error('Invalid Datastorage object',E_USER_ERROR);

		$this->_dso = &$dso;

		if(($this->_userfeatures = &$this->_service->get_module('userfeatures')) === false
		|| $this->load_init($param) === false)
			return(false);
		else if((bool) $components === true)
			$this->load_components();

		$this->_load_config(dirname(__FILE__));

		if(($size = $this->_verify_can_upload_file()) === false)
			die();

		$this->_conf['import']['file']['size'] = $size;
		$this->_bus_msg_queue = new MessageQueue();
		$this->_bus_msg_factory = new MessageFactory();

		// init random generator
		mt_srand(time()+mt_rand());

		return(true);
	}

	function load_init($param=array())
	{
		$this->_reset_info();
		$this->_reset_params();
		$this->_reset();

		return(true);
	}

	function load_components()
	{
		$this->_components = true;

		if(($this->_sccpdevice = &$this->_service->get_module('sccpdevice')) === false
		|| ($this->_device = &$this->_service->get_application('device')) === false
		|| ($this->_rightcall = $this->get_abstract('rightcall',array('type' => 'user'))) === false
		|| ($this->_dialaction = $this->get_abstract('dialaction',array('category' => 'user'))) === false
		|| ($this->_schedule_path = &$this->_service->get_module('schedule_path')) === false
		|| ($this->_linefeatures = $this->_service->get_module('linefeatures')) === false
		|| ($_XOBJ = dwho_gct::get('xivo_object')) === false
		|| ($this->_appentity = &$_XOBJ->get_application('entity')) === false
		|| ($this->_entity = &$_XOBJ->get_module('entity')) === false
		|| ($this->_provdconfig = &$_XOBJ->get_module('provdconfig')) === false
		|| ($_RAPI = dwho_gct::get('xivo_confd')) === false
		|| ($this->_cti_profile = $_RAPI->get_ressource('cti_profile')) === false
		|| ($this->_provddevice = &$_XOBJ->get_module('provddevice')) === false
		|| ($_MGR = dwho_gct::get('xivo_manager')) === false
		|| ($this->_func_key_manager = &$_MGR->get_ressource('func_key')) === false
		|| ($this->_func_key_agent = $this->_service->get_module('func_key_dest_agent')) === false
		|| ($this->_callfilter = $this->_service->get_application('callfilter', array('type' => 'bosssecretary'))) === false
		|| ($this->_callfiltermember = $this->_service->get_module('callfiltermember')) === false
		|| ($func_keys_confd = $_RAPI->get_ressource('user_func_keys')) === false
		|| ($this->_voicemail = $this->_service->get_application('voicemail')) === false)
			return(false);

		if(
			($groupfeatures = &$this->_service->get_module('groupfeatures')) === false
			|| ($meetmefeatures = &$this->_service->get_module('meetmefeatures')) === false
			|| ($queuefeatures = &$this->_service->get_module('queuefeatures')) === false
			|| ($paging_dso = &$this->_service->get_module('paging')) === false
		) {
			dwho_report::push('error', 'could not load DSOs for func key mapper');
			return(false);
		}

		$this->_queuemember = &$this->_service->get_module('queuemember');
		$this->_agentfeatures = &$this->_service->get_module('agentfeatures');

		$func_key_mapper = new FuncKeyMapper(
			$this->_userfeatures,
			$groupfeatures,
			$meetmefeatures,
			$queuefeatures,
			$this->_agentfeatures,
			$paging_dso,
			$this->_callfilter,
			$this->_callfiltermember);

		$this->_func_keys = new FuncKeyConverter($func_keys_confd, $func_key_mapper);
		$this->_vm_mapper = new VoicemailMapper();

		$nonraising_client = Client::default_config();
		$this->_confd_user = new UserResource($nonraising_client);

		$context_dso = &$this->_service->get_module('context');

		$client = Client::default_config(true);
		$manager = new LineManager(
			new LineFacade($nonraising_client),
			new LineResource($client),
			new UserResource($client),
			new ExtensionResource($client),
			new LineMapper(),
			$context_dso
		);

		$this->_line_manager = new LineShim($manager);

		$extension_resource = New ExtensionResource($client);
		$this->_extensions = New ExtensionShim($extension_resource);

		return(true);
	}

	function get($id,$disable=null,$nocomponents=null)
	{
		$this->_info = $this->_origin = array();

		$this->_info['userfeatures'] = $this->_userfeatures->get($id);

		if ($this->_info['userfeatures'] === false)
			return(false);

		$this->_origin['userfeatures'] = $this->_userfeatures->get_origin();

		if($this->_components === false)
			return($this->_info);

		if(dwho_ak('entity',$nocomponents,true) === false)
			$this->_get_entity();

		if(dwho_ak('linefeatures',$nocomponents,true) === false)
			$this->_get_linefeatures();

		if(dwho_ak('exten',$nocomponents,true) === false)
			$this->_get_exten();

		if(dwho_ak('groupmember',$nocomponents,true) === false)
			$this->_get_groupmember();

		if(dwho_ak('queuemember',$nocomponents,true) === false)
			$this->_get_queuemember();

		if(dwho_ak('rightcall',$nocomponents,true) === false)
			$this->_get_rightcall($this->_info['userfeatures']['id']);

		if(dwho_ak('dialaction',$nocomponents,true) === false)
			$this->_get_dialaction($this->_info['userfeatures']['id']);

		if(dwho_ak('voicemail',$nocomponents,true) === false)
			$this->_get_voicemail();

		$this->_get_phonefunckey($id);

		// schedule
		$this->_info['schedule_id'] = 0;
		$where = array('path' => 'user', 'pathid' => $this->_info['userfeatures']['id']);
		if(($schedule = $this->_schedule_path->get_where($where)) !== false)
			$this->_info['schedule_id'] = intval($schedule['schedule_id']);

		return($this->_info);
	}

	function _get_linefeatures()
	{
		$this->_info['linefeatures'] = false;
		$this->_origin['linefeatures'] = false;

		if(($user_lines = $this->_get_user_lines($this->_info['userfeatures']['id'])) === false) {
			return false;
		}

		$features =  array('info' => array(), 'origin' => false);
		foreach ($user_lines as $user_line) {
			$line = $this->_line_manager->get_linefeatures($user_line['line_id']);
			array_push($features['info'], $line);
		}

		$this->_info['linefeatures'] = $features['info'];
		$this->_origin['linefeatures'] = $features['origin'];
		return($this->_info['linefeatures']);
	}

	function _get_user_lines($user_id)
	{
		$user_lines = $this->_confd_user->list_line_associations($user_id);
		if (empty($user_lines)){
			return false;
		}

		// put the main_line at the head of the array
		foreach($user_lines as $position=>$user_line) {
			if ($user_line['main_line'] === true) {
				unset($user_lines[$position]);
				array_unshift($user_lines, $user_line);
			}
		}
		return $user_lines;
	}

	function _get_exten()
	{
		$this->_info['exten'] = false;
		$this->_origin['exten'] = false;

		if(dwho_issa('linefeatures', $this->_info) === false) {
			return false;
		}

		$lines = $this->_info['linefeatures'];
		$extensions = array();
		foreach($lines as $line) {
			$line_extensions = $this->_line_manager->list_extension_associations($line['id']);
			if (empty($line_extensions)){
				return false;
			}

			// When line_extension will has the attribute 'main_extension',
			// we should use it to find the right line_extension instead to take first
			$line_extension = $line_extensions[0];

			$extensions[] = $this->_extensions->get($line_extension['extension_id']);
		}
		$this->_info['exten'] = $extensions;

		return $this->_info['exten'];
	}

	function _get_entity()
	{
		$this->_info['entity'] = false;
		$this->_origin['entity'] = false;

		if(is_object($this->_appentity) === false)
			return(false);

		$this->_info['entity']  = $this->_appentity->get($this->_info['userfeatures']['entityid'],null,'internal');

		return($this->_info['entity']);
	}

	function _get_groupmember()
	{
		return($this->_get_member_type('group'));
	}

	function _get_queuemember()
	{
		return($this->_get_member_type('queue'));
	}

	function _get_member_type($type)
	{
		if($type !== 'group' && $type !== 'queue')
			return(false);

		$membertype = $type.'member';

		$this->_info[$membertype] = false;
		$this->_origin[$membertype] = false;

		$where = array();
		$where['category'] = $type;
		$where['usertype'] = 'user';
		$where['userid'] = $this->_info['userfeatures']['id'];

		if(is_object($this->_queuemember) === false
		|| ($module = &$this->_service->get_module($type.'features')) === false
		|| ($this->_info[$membertype] = $this->_queuemember->get_all_where($where)) === false
		|| ($nb = count($this->_info[$membertype])) === 0)
		{
			$this->_info[$membertype] = false;
			return(false);
		}

		$ref = &$this->_info[$membertype];

		for($i = 0;$i < $nb;$i++)
		{
			if(($id = $module->get_primary(array('name' => $ref[$i]['queue_name']))) === false)
				continue;
			else if($type === 'queue')
				$ref[$i]['queuefeaturesid'] = $id;
			else
				$ref[$i]['groupfeaturesid'] = $id;
		}

		$this->_origin[$membertype] = $this->_queuemember->get_origin_list();

		return($this->_info[$membertype]);
	}

	function _get_voicemail()
	{
		$this->_info['voicemail'] = false;
		$this->_origin['voicemail'] = false;

		$user_id = $this->_info['userfeatures']['id'];
		$voicemail_id = $this->_voicemail->get_voicemail_id_for_user($user_id);

		if($voicemail_id === false)
			return false;

		$voicemail = $this->_voicemail->get($voicemail_id);
		if($voicemail === false)
			return false;

		$this->_info['voicemail'] = $voicemail;
		$this->_origin['voicemail'] = $voicemail;
		return($this->_info['voicemail']);
	}

	function _get_agentfeatures()
	{
		$this->_info['agentfeatures'] = false;
		$this->_origin['agentfeatures'] = false;

		if(is_object($this->_agentfeatures) === false
		|| $this->_agentfeatures->get($this->_info['userfeatures']['agentid']) === false)
			return(false);

		$this->_origin['agentfeatures'] = $this->_agentfeatures->get_origin();

		return($this->_info['agentfeatures']);
	}

	function _get_phonefunckey($user_id)
	{
		$phonefunckeys = $this->_func_keys->get_user_func_keys($user_id);
		$this->_info['phonefunckey'] = $phonefunckeys;
		$this->_origin['phonefunckey'] = $phonefunckeys;
		return($phonefunckeys);
	}

	function get_timezones()
	{
		if(is_object($this->_voicemail) === false)
			return(false);

		return($this->_voicemail->get_timezones());
	}

	function get_phonefunckey_type()
	{
		return($this->_func_keys->get_legacy_types());
	}

	function get_bsfilter_list()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$rs = $this->_dso->get_bsfilter($this->_info['userfeatures']['id'],
						$this->_info['userfeatures']['bsfilter']);

		if($rs === false || ($nb = count($rs)) === 0)
			return(false);

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];
			$ref = $this->_userfeatures->_prepare($ref);
			$ref['callfilteridentity'] = $ref['callfiltername'].' / '.$ref['identity'];
		}

		if(empty($rs) === true)
			$rs = false;

		return($rs);
	}

	function get_config_import_file()
	{
		return($this->_chk_exists_cfg('import','file',true));
	}

	function _get_config_file_maxsize()
	{
		if(($import_file = $this->_chk_exists_cfg('import','file',true)) !== false
		&& isset($import_file['size']) === true)
			return(dwho_get_memory($import_file['size'],false));

		return(false);
	}

	function get_voicemail_list()
	{
		if(is_object($this->_voicemail) === false)
			return(false);

		return($this->_voicemail->get_voicemail_search());
	}

	function get_agent_list()
	{
		if(is_object($this->_agentfeatures) === false)
			return(false);

		$order = array();
		$order['firstname'] = SORT_ASC;
		$order['lastname'] = SORT_ASC;
		$order['number'] = SORT_ASC;

		return($this->_agentfeatures->get_all(null,true,$order));
	}

	function get_profileclient_list()
	{
		if(empty($this->_profileclientlist) === false)
			return($this->_profileclientlist);
		elseif($this->_cti_profile === false
		|| ($list = $this->_cti_profile->find_all()) === false
		|| ($nb = count($list)) === 0)
			return(false);

		$this->_profileclientlist = array();

		for($i=0;$i<$nb;$i++)
		{
			$ref = &$list[$i];
			$this->_profileclientlist[$ref['id']] = $ref['name'];
		}

		return($this->_profileclientlist);
	}

	function get_elements()
	{
		$r = array();

		$r['userfeatures'] = $this->_userfeatures->get_element();
		$r['dialaction'] = $this->_dialaction->get_element();

		if(is_object($this->_queuemember) === true)
			$r['qmember'] = $this->_queuemember->get_element();
		else
			$r['qmember'] = false;

		$r['voicemail'] = false;

		return($r);
	}

	function get_users_search($search,$disable=null,$order=null,$limit=null,$assoc=false)
	{
		return($this->_get_users('search',$disable,$search,$order,$limit,$assoc));
	}

	function get_users_list($disable=null,$order=null,$limit=null,$assoc=false,$initialized=null)
	{
		return($this->_get_users('list',$disable,null,$order,$limit,$assoc,$initialized));
	}

	function _get_users($action,$disable=null,$search='',$order=null,$limit=null,$assoc=false,$initialized=null)
	{
		$search = strval($search);

		switch($action)
		{
			case 'search':
				$rs = $this->_dso->search($search,$disable,null,$order,$limit);
				break;
			case 'search_number':
				$rs = $this->_dso->search($search,$disable,true,$order,$limit);
				break;
			case 'list':
			default:
				$rs = $this->_dso->get_all($disable,$initialized,$order,$limit);
		}

		$this->_cnt = $this->_dso->get_cnt();

		if($rs === false || ($nb = count($rs)) === 0)
			return(false);

		$r = array();
		$summaries = $this->get_summaries($search, $order, $limit);

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];

			$userfeatures = dwho_array_startswith_key($ref,'userfeatures.',true);

			if($this->_components === false) {
				$r[$userfeatures['id']] = $this->_userfeatures->_prepare($userfeatures);
				continue;
			} else {
				$summary = $summaries[$userfeatures['id']];
				$entity = dwho_array_startswith_key($ref,'entity.',true);
				$r[$userfeatures['id']] = $this->_userfeatures->_prepare($userfeatures, $summary);
				$r[$userfeatures['id']]['entity'] = $this->_entity->_prepare($entity);
				$r[$userfeatures['id']]['masterline'] = $this->map_line_from_summary($summary);
			}
		}

		if(empty($r) === true)
			return(false);
		else if((bool) $assoc === false)
			return(array_values($r));

		return($r);
	}

	private function get_summaries($search, $order, $limit) {
		if($this->_components === false) {
			return array();
		}

		$response = $this->_confd_user->search(array('view' => 'summary'));
		if($response === false) {
			return array();
		}

		$summaries = array();
		foreach($response['items'] as $summary) {
			$summaries[$summary['id']] = $summary;
		}

		return $summaries;
	}

	private function map_line_from_summary($summary) {
		return array(
			'number' => $summary['extension'],
			'context' => $summary['context'],
			'provisioningid' => $summary['provisioning_code'],
			'protocol' => $summary['protocol']
		);
	}

	function get_dialaction_result()
	{
		return($this->_dialaction->get_result_for_display());
	}

	function get_phonefunckey_result()
	{
		$result = $this->get_result('phonefunckey');

		if(is_array($result) === false
		|| ($nb = count($result)) === 0)
		return(array());

		return($result);
	}

	function get_destination_list()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			$id = 0;
		else
			$id = $this->_info['userfeatures']['id'];

		$this->_get_destination();

		$r = $this->_destination;
		$r['users'] = $this->get_destination_users($id);
		$r['groups'] = $this->get_destination_groups();
		$r['queues'] = $this->get_destination_queues();
		$r['meetme'] = $this->get_destination_meetme();
		$r['schedule'] = $this->get_destination_schedule();
		$r['sounds'] = $this->_dialaction->get_sound();
		$r['outcalls']  = $this->get_destination_outcall();
		$r['voicemail'] = $this->get_destination_voicemail();
		//$r['trunk'] = $this->get_destination_trunk();

		return($r);
	}

	function get_phonefunckey_identity($result=true)
	{
		if (dwho_issa('userfeatures',$this->_info) === false)
			return array();

		if(is_array($this->_info['phonefunckey'])) {
			return $this->_info['phonefunckey'];
		}

		return $this->_func_keys->get_user_func_keys($this->_info['userfeatures']['id']);
	}

	function _get_destination()
	{
		if(is_array($this->_destination) === true)
			return(true);

		$this->_destination = array();

		return(true);
	}

	function set_add($arr)
	{
		$r = $this->_set('add', $arr);
		if($r === false)
			$this->_set_result_var('voicemail', 'id', '');
		return $r;
	}

	function set_edit($arr)
	{
		if(empty($this->_info) === true)
			return(false);

		return($this->_set('edit',$arr));
	}

	function _set($action,$arr)
	{
		$this->_reset();

		if(($action !== 'add' && $action !== 'edit') === true
		|| dwho_issa('userfeatures',$arr) === false)
			return(false);

		$arr['userfeatures']['callerid'] = $this->_service->mk_callerid($arr['userfeatures']['callerid']);

		$this->set_userfeatures($action,$arr['userfeatures']);

		$vmaction = $action;

		if(dwho_issa('linefeatures',$arr) === true
		&& $this->set_linefeatures($arr['linefeatures']) !== false
		&& isset($this->_result['linefeatures'][0]))
		{
			// Should be done for each linefeatures not only on the main line ([0])
			// Only if we want support the following scenario:
			// user1 ___ line1
			// user2 _\_ line2
			$protocolname = $this->_result['linefeatures'][0]['protocol'];
			$name = $this->_result['linefeatures'][0]['name'];
			$number = $this->_result['linefeatures'][0]['number'];
			$context = $this->_result['linefeatures'][0]['context'];

			if(dwho_issa('group',$arr) === true
			&& dwho_issa('group-select',$arr) === true)
			{
				$this->set_groupmember($arr['group'],
						$arr['group-select'],
						$name,
						$context,
						$number,
						$protocolname);
			}

			if(dwho_issa('queue',$arr) === true
			&& dwho_issa('queue-select',$arr) === true)
			{
				$this->set_queuemember($arr['queue'],
							$arr['queue-select'],
							$name,
							$context,
							$number,
							$protocolname);
			}
		}

		if(dwho_issa('linefeatures', $this->_result) === true) {
			$this->set_exten($this->_result['linefeatures']);
		}

		if(dwho_issa('dialaction',$arr) === true)
			$this->set_dialaction($arr['dialaction']);

		if(dwho_issa('rightcall',$arr) === true)
			$this->set_rightcall($arr['rightcall']);

		if(dwho_issa('phonefunckey',$arr) === true)
			$this->set_phonefunckey($arr['phonefunckey']);

		if(dwho_issa('voicemail', $arr) === true)
			$this->set_voicemail($arr, $action);

		if(array_key_exists('schedule_id', $arr))
			$this->_set_result('schedule_id', intval($arr['schedule_id']));

		if($this->get_errnb() > 0)
			$r = false;
		else
			$r = true;

		return($r);
	}

	function set_userfeatures($action,$arr)
	{
		if(($action !== 'add' && $action !== 'edit') === true
		|| ($action === 'edit' && dwho_issa('userfeatures',$this->_origin) === false) === true)
			return(false);
		else if(is_array($arr) === false)
		{
			$this->_set_result('userfeatures',null);
			$this->_set_error('userfeatures','invalid data');
			return(false);
		}

		if (isset($arr['enableclient'])
		&& (bool) $arr['enableclient'] === true
		&& (dwho_has_len($arr['loginclient']) === false
			|| dwho_has_len($arr['passwdclient']) === false))
		{
			dwho_report::push('error', 'cant_activate_xivoclient_without_login_and_password');
			$err = array('loginclient' => 'empty', 'passwdclient' => 'empty');
			$this->_set_error('userfeatures', $err);
		}

		$this->get_musiconhold();

		if(isset($arr['musiconhold']) === true
		&& isset($this->_musiconhold[$arr['musiconhold']]) === false)
			$arr['musiconhold'] = '';

		if($action === 'edit'
		&& isset($arr['cti_profile_id']) === false
		&& isset($arr['enableclient']) === false)
			$arr['cti_profile_id'] = $this->_origin['userfeatures']['cti_profile_id'];

		$this->get_profileclient_list();

		if(isset($arr['cti_profile_id']) === true
		&& isset($this->_profileclientlist[$arr['cti_profile_id']]) === false)
			$arr['cti_profile_id'] = '';

		if(isset($arr['destunc']) === true
		&& dwho_has_len($arr['destunc']) === false)
			$arr['enableunc'] = false;

		if(isset($arr['destrna']) === true
		&& dwho_has_len($arr['destrna']) === false)
			$arr['enablerna'] = false;

		if(isset($arr['destbusy']) === true
		&& dwho_has_len($arr['destbusy']) === false)
			$arr['enablebusy'] = false;

		if(isset($arr['outcallerid-type']) === true)
			$arr['outcallerid'] = $arr['outcallerid-type'];
		else if(isset($arr['outcallerid']) === false)
			$arr['outcallerid'] = '';

		if(isset($arr['outcallerid-custom']) === true
		&& $arr['outcallerid'] === 'custom')
			$arr['outcallerid'] = $arr['outcallerid-custom'];

		if($action === 'add')
		{
			$arr['func_key_template_id'] = null;
			$arr['func_key_private_template_id'] = 0;
		}
		else
		{
			$arr['func_key_private_template_id'] = $this->_info['userfeatures']['func_key_private_template_id'];
			$func_key_template_id = $this->_info['userfeatures']['func_key_template_id'];
			$arr['func_key_template_id'] = ($func_key_template_id == 0)?null:(int)$func_key_template_id;
		}

		if(($rs = $this->_userfeatures->chk_values($arr)) === false)
		{
			$this->_set_result('userfeatures',$this->_userfeatures->get_filter_result());
			$this->_set_error('userfeatures',$this->_userfeatures->get_filter_error());
			return(false);
		}
		else
		{
			if($action === 'edit')
				$id = $this->_origin['userfeatures']['id'];
			else
				$id = null;

			if($this->_userfeatures->loginclient_exists($rs['loginclient'],$id) === true)
			{
				$this->_set_result('userfeatures',$rs);
				$this->_set_error('userfeatures',array('loginclient' => 'client login exists'));
				return(false);
			}
		}

		if(dwho_has_len($rs['agentid']) === false
		|| is_object($this->_agentfeatures) === false
		|| $this->_agentfeatures->get_primary($rs['agentid']) === false)
			$rs['agentid'] = null;

		$this->_set_result('userfeatures',$rs);

		return(true);
	}

	function set_linefeatures($arr)
	{
		if(is_array($arr)) {
			$lines = array();
			foreach($arr as $key => $values) {
				array_pop($values);  # remove bogus empty value
				foreach($values as $pos => $value) {
					if(!array_key_exists($pos, $lines)) {
						$lines[$pos] = array();
					}

					if($pos == 0) {
						$lines[$pos][$key] = $value;
					}
					else {
						$lines[$pos] = $this->_info['linefeatures'][$pos];
					}
				}
			}

			if(empty($lines)) {
				return null;
			}

			$this->_set_result('linefeatures', $lines);
			return true;
		}

		$this->_set_result('linefeatures', false);
		$this->_set_error('linefeatures', 'empty');
		return false;
	}

	function set_voicemail($form, $action)
	{
		$voicemail = $form['voicemail'];
		$this->_set_result('voicemail', $voicemail);
		$this->_set_result('user_vm_action', $form['user_vm_action']);
		return(true);
	}

	function set_phonefunckey($list)
	{
		if(($list = dwho_group_array('fknum',$list)) === false) {
			$list = array();
		}
		$this->_set_result('phonefunckey',$list);

		return(true);
	}

	function set_exten($lines) {
		$extensions = array();
		foreach ($lines as $position=>$line) {
			$extension = array('exten' => $line['number'],
							   'context' => $line['context']);
			if(isset($this->_info['exten'][$position]['id'])) {
				$extension['id'] = $this->_info['exten'][$position]['id'];
			}
			array_push($extensions, $extension);
		}
		$this->_set_result('exten',$extensions);
	}

	function set_groupmember($group,$select,$name,$context='',$number='',$protocolname='')
	{
		return($this->_set_member_type('group',$group,$select,$name,$context,$number,$protocolname));
	}

	function set_queuemember($queue,$select,$name,$context='',$number='',$protocolname='')
	{
		return($this->_set_member_type('queue',$queue,$select,$name,$context,$number,$protocolname));
	}

	function _set_member_type($type,$list,$select,$name,$context='',$number='',$protocolname='')
	{
		if(($type !== 'group' && $type !== 'queue') === true
		|| is_array($list) === false
		|| is_array($select) === false
		|| ($module = &$this->_service->get_module($type.'features')) === false
		|| is_object($this->_queuemember) === false)
			return(false);

		$slt = array_values($select);

		if(($nb = count($slt)) === 0)
			return(null);

		$info = array();
		$info['usertype'] = 'user';
		$info['userid'] = 0;
		$info['position'] = 0;
		$info['category'] = $type;

		$key = $type.'member';

		$tmp = $reslist = array();

		if($type === 'group')
			$info['penalty'] = 0;

		for($i = 0;$i < $nb;$i++)
		{
			$qname = &$slt[$i];

			if(isset($tmp[$qname]) === true
			|| isset($list[$qname]) === false
			|| ($chantype = dwho_ak('chantype',$list[$qname],true)) === false
			|| ($interface = $this->mk_interface($name,
							     $chantype,
							     $context,
							     $number,
							     $protocolname)) === false
			|| ($typeid = $module->get_primary(array('name' => $qname))) === false) {
				continue;
			}

			$info['queue_name'] = $qname;
			$info['interface'] = $interface['interface'];
			$info['channel'] = $interface['channel'];

			$values = array_merge($list[$qname],$info);

			if(($rs = $this->_queuemember->chk_values($values)) === false)
				continue;
			else if($type === 'queue')
				$rs['queuefeaturesid'] = $typeid;
			else
				$rs['groupfeaturesid'] = $typeid;

			$tmp[$qname] = 1;
			$reslist[] = $rs;
		}

		if(isset($reslist[0]) === false)
		{
			$this->_set_result($key,false);
			$this->_set_error($key,'empty');

			return(false);
		}

		$this->_set_result($key,$reslist);

		return(true);
	}

	function add()
	{
		$r = true;

		$this->_status = 'add';
		$this->_return = array();
		$user_id = null;
		$line_id = null;
		$extension_id = null;

		if($this->get_errnb() > 0
		|| $this->add_func_key_private_tpl() === false
		|| ($user_id = $this->add_userfeatures()) === false
		|| $this->add_func_key_dest_user($user_id) === false
		|| ($lines = $this->edit_linefeatures($user_id)) === false
		|| ($extensions = $this->add_exten()) === false
		|| $this->add_rightcall($user_id) === false
		|| $this->add_dialaction($user_id) === false
		|| $this->add_schedule_path($user_id) === false
		|| $this->add_phonefunckey($user_id) === false)
		{
			$r = false;
			if($user_id !== null)
				$this->_reverse_add();
		}

		if($r === true && $lines && $extensions)
		{
			$line_id = $lines[0]['id'];
			$extension_id = $extensions[0]['id'];
			if($this->add_groupmember($user_id) === false
			|| $this->add_queuemember($user_id) === false
			|| $this->add_line_extension($line_id, $extension_id) === false
			|| $this->add_user_line($user_id, $line_id) === false)
				$r = false;
		}

		if($r === true && $user_id !== null)
			$r = $this->add_voicemail($user_id);

		if($r === true && $line_id !== null)
			$r = $this->add_device_config($line_id);

		if($r === false && $user_id !== null)
			$this->_reverse_add();

		$this->_status = '';

		return($r);
	}

	function add_func_key_private_tpl()
	{
		if(($rs = $this->get_result('userfeatures')) === null
		|| $rs === false)
			return(false);

		if(($func_key_template_id = $this->_func_key_manager->add_private_tpl($rs['firstname'].' '.$rs['lastname'])) === false)
			return(false);

		$this->_return['func_key_private_template_id'] = $func_key_template_id;
		$this->_set_result_var('userfeatures','func_key_private_template_id', $func_key_template_id);
		return(true);
	}

	function add_userfeatures()
	{
		if(($rs = $this->get_result('userfeatures')) === null
		|| $rs === false)
			return(false);

		$user_id = $this->_return['userfeatures'] = $this->_userfeatures->add($rs);
		$user_uuid = $rs['uuid'];
		$bus_msg = $this->_bus_msg_factory->new_user_created_msg($user_id, $user_uuid);
		$this->_bus_msg_queue->add_msg($bus_msg);

		return($this->_return['userfeatures']);
	}

	function add_func_key_dest_user($user_id)
	{
		if (!($func_key_id = $this->_func_key_manager->add_dst_for_user($user_id)))
			return(false);

		$this->_return['func_key_dest_user'] = $user_id;
		return(true);
	}

	function add_line_extension($line_id, $extension_id)
	{
		if(($line_id = (int) $line_id) === 0
		|| ($extension_id = (int) $extension_id) === 0)
			return(false);

		$return = $this->_line_manager->associate_extension($line_id, $extension_id);
		return $return;
	}

	function add_user_line($user_id, $line_id)
	{
		if(($user_id = (int) $user_id) === 0
		|| ($line_id = (int) $line_id) === 0)
			return(false);

		$return = $this->_line_manager->associate_user($line_id, $user_id);
		return $return;
	}

	function add_device_config($line_id)
	{
		if(is_array($this->_return['line_device'])) {
			return $this->_line_manager->add_device_config($this->_return['line_device']);
		}
		return true;
	}

	function add_voicemail($user_id)
	{
		$this->_return['voicemail'] = false;

		$action = $this->_result['user_vm_action'];
		if(!($action === 'add' || $action === 'search'))
			return true;

		if(is_object($this->_voicemail) === false) {
			dwho_report::push('error', 'voicemail app not loaded. cannot add voicemail');
			return false;
		}

		if ($action == 'add') {
			$voicemail_id = $this->_voicemail->add($this->_result);
		} else if ($action == 'search') {
			$voicemail_id = (int) $this->get_result_var('voicemail', 'id');
		}

		if($voicemail_id === false)
			return false;

		if($this->_voicemail->associate($voicemail_id, $user_id) === false)
			return false;

		$this->_return['voicemail'] = $voicemail_id;
		return true;
	}

	function add_phonefunckey($user_id)
	{
		if(
			($user_id = dwho_ulongint($user_id)) === 0
			|| ($phonefunckeys = $this->get_result('phonefunckey')) === false
		) {
			return(false);
		}

		if($phonefunckeys === null) {
			$phonefunckeys = array();
		}

		$updated_phonefunckeys = $this->_func_keys->add_user_func_keys($user_id, $phonefunckeys);
		$this->_return['phonefunckey'] = $updated_phonefunckeys;
		return (count($phonefunckeys) === count($updated_phonefunckeys));
	}

	function add_schedule_path($user_id)
	{
		return $this->_schedule_path->schedule_add(
			$this->get_result('schedule_id'),
			'user',
			$user_id);
	}

	function add_exten()
	{
		$results = array();
		$extensions = $this->get_result('exten');
		if($extensions === false || count($extensions) === 0) {
			return null;
		}
		foreach ($extensions as $extension) {
			if(($result = $this->_extensions->add($extension)) === false) {
				return false;
			}
			array_push($results, $result);
		}
		$this->_return['exten'] = $results;
		return $this->_return['exten'];
	}

	function add_groupmember($user_id)
	{
		return($this->_add_member_type('group',$user_id));
	}

	function add_queuemember($user_id)
	{
		return($this->_add_member_type('queue',$user_id));
	}

	function _add_member_type($type,$user_id)
	{
		if(($type !== 'group' && $type !== 'queue') === true
		|| ($user_id = dwho_ulongint($user_id)) === 0)
			return(false);

		$key = $type.'member';

		if(($rs = $this->get_result($key)) === null || $rs === false)
			return($rs);

		$ret = $this->_return[$key] = array();

		$nb = count($rs);
		for($i = 0;$i < $nb;$i++)
		{
			$rs[$i]['userid'] = $user_id;

			unset($rs[$i]['groupfeaturesid'],$rs[$i]['queuefeaturesid']);

			if($this->_queuemember->add($rs[$i]) === false)
				return(false);

			$ret['queue_name'] = $rs[$i]['queue_name'];
			$ret['interface'] = $rs[$i]['interface'];

			$this->_return[$key][] = $ret;
		}

		if(isset($this->_return[$key][0]) === false)
			$this->_return[$key] = false;

		return(true);
	}

	function _reverse_add()
	{
		$this->_set_result_var('voicemail', 'id', '');

		if($this->_status !== 'add'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		if(array_key_exists('func_key_dest_user', $this->_return)) {
			$this->_func_key_manager->delete_dst_for_user($this->_return['func_key_dest_user']);
		}

		if(array_key_exists('userfeatures', $this->_return)) {
			$this->_func_keys->delete_user_func_keys($this->_return['userfeatures']);
		}

		if(isset($this->_return['voicemail'])
			&& isset($this->_return['userfeatures'])
			&& $this->_return['userfeatures'] !== false
			&& $this->_return['voicemail'] !== false)
		{
			$user_id = $this->_return['userfeatures'];
			$voicemail_id = $this->_return['voicemail'];

			$this->_voicemail->dissociate($user_id);
			if($this->get_result('user_vm_action') === 'add') {
				$this->_voicemail->delete($voicemail_id);
			}
		}

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'userfeatures':
					$this->_userfeatures->delete($val);
					break;
				case 'rightcall':
					$this->_rightcall->reverse_add();
					break;
				case 'dialaction':
					$this->_dialaction->reverse_add();
					break;
				case 'groupmember':
				case 'queuemember':
					if(is_array($val) === false)
						continue;

					foreach($val as $memberval)
						$this->_queuemember->delete($memberval);
					break;
			}
		}

		if(array_key_exists('func_key_private_template_id', $this->_return)) {
			$this->_func_key_manager->delete_template($this->_return['func_key_private_template_id']);
		}

		if(array_key_exists('linefeatures', $this->_return)) {
			$this->_line_manager->reverse_add($this->_return['linefeatures']);
		}

		return(true);
	}

	function edit()
	{
		$this->_status = 'edit';
		$this->_return = array();
		$user_id = (int) $this->_info['userfeatures']['id'];

		if(is_array($this->_info) === false
		|| empty($this->_info) === true
		|| $this->get_errnb() > 0
		|| $this->edit_userfeatures() === false
		|| $this->edit_rightcall() === false
		|| $this->edit_dialaction() === false
		|| $this->edit_schedule_path() === false
		|| $this->edit_phonefunckey() === false)
		{
			$this->_reverse_edit();
			$this->_status = '';
			return(false);
		}

		if(($line = $this->get_result('linefeatures')) !== null
		&& dwho_issa(0, $line) === true
		&& ($this->edit_linefeatures($user_id)) !== false)
		{
			if(isset($this->_info['linefeatures'][0])
			&& $this->_info['linefeatures'][0]['number'] !== null)
			{
				if($this->edit_exten() === false
				|| $this->edit_device_config() === false
				|| $this->edit_groupmember($user_id) === false
				|| $this->edit_queuemember($user_id) === false)
				{
					$this->_reverse_edit();
					$this->_status = '';
					return(false);
				}
			}
			else if($this->add_exten() === false
			|| ($line_id = $this->_return['linefeatures'][0]['id']) === 0
			|| ($extension_id = $this->_return['exten'][0]['id']) === 0
			|| $this->add_line_extension($line_id, $extension_id) === false
			|| $this->add_user_line($user_id, $line_id) === false
			|| $this->add_device_config($line_id) === false
			|| $this->add_groupmember($user_id) === false
			|| $this->add_queuemember($user_id) === false)
			{
				$this->_reverse_edit();
				$this->_status = '';
				return(false);
			}
		}
		elseif($this->get_result('linefeatures') === null
		&& $this->_info['linefeatures'] !== false)
		{
			if ($this->delete_linefeatures() === false)
			{
				$this->_reverse_edit();
				$this->_status = '';
				return(false);
			}
		}

		if($this->edit_voicemail() === false) {
			$this->_reverse_edit();
			$this->_status = '';
			return(false);
		}

		$this->_status = '';
		return(true);
	}

	function edit_userfeatures()
	{
		if(dwho_issa('userfeatures',$this->_info) === false
		|| ($rs = $this->get_result('userfeatures')) === null
		|| $rs === false)
			return(false);

		$return = $this->_userfeatures->edit($this->_info['userfeatures']['id'],$rs);

		$user_uuid = $this->_info['userfeatures']['uuid'];
		$user_id = $this->_info['userfeatures']['id'];
		$bus_msg = $this->_bus_msg_factory->new_user_edited_msg($user_id, $user_uuid);
		$this->_bus_msg_queue->add_msg($bus_msg);

		return(($this->_return['userfeatures'] = $return));
	}

	function edit_linefeatures($user_id)
	{
		$lines = $this->get_result('linefeatures');
		if($lines === false || count($lines) === 0) {
			return null;
		}

		$result = $this->_line_manager->edit_linefeatures(
			$lines,
			$this->_status
		);

		if($result !== false) {
			$this->_return = array_merge($this->_return, $result);
		}

		return $this->_return['linefeatures'];
	}

	function edit_rightcall()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		return(parent::edit_rightcall($this->_info['userfeatures']['id']));
	}

	function edit_dialaction()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		return(parent::edit_dialaction($this->_info['userfeatures']['id']));
	}

	function edit_voicemail()
	{
		$this->_return['voicemail'] = false;

		if(dwho_issa('userfeatures', $this->_info) === false)
			return false;

		$user_id = dwho_ulongint($this->_info['userfeatures']['id']);
		if($user_id === 0) {
			dwho_report::push('error', 'cannot find user id needed for editing voicemail');
			return false;
		}

		$action = $this->get_result('user_vm_action');
		if($action === false) {
			dwho_report::push('error', 'cannot find voicemail action needed editing voicemail');
			return false;
		}

		if($action === 'add' || $action === 'search')
		{
			return $this->add_voicemail($user_id);
		}
		else if ($action === 'edit' || $action === 'delete')
		{
			return $this->update_voicemail($user_id, $action);
		}

	}

	private function update_voicemail($user_id, $action)
	{
		$voicemail_id = dwho_ulongint($this->_info['voicemail']['id']);
		if($voicemail_id === 0) {
			dwho_report::push('error', 'cannot find voicemail id needed for voicemail update');
			return false;
		}

		if ($action === 'delete') {
			if($this->_voicemail->dissociate($user_id) === false)
				return false;
		}

		if($action === 'edit') {
			if($this->_voicemail->edit($voicemail_id, $this->_result) === false)
				return false;
		}

		return true;
	}

	function edit_phonefunckey()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$user_id = $this->_info['userfeatures']['id'];
		$updated = $this->add_phonefunckey($user_id);
		return $updated;
	}

	function edit_schedule_path()
	{
		$this->_schedule_path->delete_where(array('path' => 'user', 'pathid' => intval($this->_info['userfeatures']['id'])));

		return $this->_schedule_path->schedule_add(
			$this->get_result('schedule_id'),
			'user',
			$this->_info['userfeatures']['id']);
	}

	function edit_exten()
	{
		$extensions = $this->get_result('exten');
		if($extensions === false || count($extensions) === 0) {
			return null;
		}
		foreach ($extensions as $extension) {
			if(($this->_extensions->edit($extension)) === false) {
				return false;
			}
		}
		return true;
	}

	function edit_device_config()
	{
		if(is_array($this->_return['line_device'])) {
			return $this->_line_manager->edit_device_config($this->_return['line_device']);
		}
		return(true);
	}

	function edit_groupmember($user_id)
	{
		return($this->_edit_member_type('group', $user_id));
	}

	function edit_queuemember($user_id)
	{
		return($this->_edit_member_type('queue', $user_id));
	}

	function _edit_member_type($type, $user_id)
	{
		if(($type !== 'group' && $type !== 'queue') === true
		|| dwho_issa('linefeatures',$this->_info) === false)
			return(false);

		$key = $type.'member';
		$keyid = $type.'featuresid';

		$old_qm =  array();
		$new_qm = dwho_array_set_value_to_key($this->get_result($key), $keyid);

		if(dwho_issa($key,$this->_info) === true)
		{
			$old_qm = dwho_array_set_value_to_key($this->_info[$key], $keyid);

			$to_delete = array_diff_key($old_qm, $new_qm);
			$to_edit = array_intersect_key($new_qm, $old_qm);

			foreach ($to_delete as $element)
			{
				$where = array('usertype' => $element['usertype'],
							'userid' => $element['userid'],
							'queue_name' => $element['queue_name']);
				if($this->_queuemember->delete_where($where) === false)
					return(false);

				$this->_return[$key][] = $element;
			}

			foreach ($to_edit as $element)
			{
				$old_element = $old_qm[$element[$keyid]];

				$element['userid'] = $old_element['userid'];
				$element['position'] = $old_element['position'];
				$element['commented'] = $old_element['commented'];

				unset($element['groupfeaturesid'],$element['queuefeaturesid']);

				$id = array('queue_name' => $old_element['queue_name'],
							'interface' => $old_element['interface']);
				if($this->_queuemember->edit($id, $element) === false)
					return(false);

				$this->_return[$key][] = $element;
			}
		}

		$to_add = array_diff_key($new_qm, $old_qm);

		foreach ($to_add as $element)
		{
			$element['userid'] = $user_id;

			unset($element['groupfeaturesid'],$element['queuefeaturesid']);

			if($this->_queuemember->add($element) === false)
				return(false);

			$this->_return[$key][] = $element;
		}

		if(isset($this->_return[$key][0]) === false)
			$this->_return[$key] = false;

		return(true);
	}

	function _reverse_edit()
	{
		if($this->get_errnb() > 0
		|| $this->_status !== 'edit'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'userfeatures':
					if(isset($this->_origin['userfeatures']) === false
					|| $this->_origin['userfeatures'] === false)
						$this->delete_userfeatures();
					else
						$this->_userfeatures->edit_origin($this->_origin['userfeatures']);
					break;
				case 'rightcall':
					$this->_rightcall->reverse_edit($this->_status);
					break;
				case 'dialaction':
					$this->_dialaction->reverse_edit($this->_status);
					break;
				case 'groupmember':
				case 'queuemember':
					if($key === 'groupmember')
						$this->delete_groupmember();
					else
						$this->delete_queuemember();

					if(isset($this->_origin[$key]) === true
					&& $this->_origin[$key] !== false)
						$this->_queuemember->add_origin_list($this->_origin[$key]);
					break;
			}
		}

		$this->_line_manager->reverse_edit();

		return(true);
	}

	function delete()
	{
		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if($this->delete_phonefunckey() === false
		|| $this->delete_voicemail() === false
		|| $this->delete_funckey_dst() === false
		|| $this->delete_linefeatures() === false
		|| $this->delete_userfeatures() === false
		|| $this->delete_rightcall() === false
		|| $this->delete_dialaction() === false
		|| $this->delete_callfiltermember() === false
		|| $this->delete_groupmember() === false
		|| $this->delete_queuemember() === false
		|| $this->delete_schedule_path() === false)
		{
			$r = false;
			$this->_reverse_delete();
		}
		else
			$this->unlinks();

		$this->_status = '';

		return($r);
	}

	private function delete_voicemail()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$user_id = $this->_info['userfeatures']['id'];
		$this->_voicemail->dissociate($user_id);
	}

	private function delete_funckey_dst()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$this->_func_key_manager->delete_dst_for_user($this->_info['userfeatures']['id']);

		return(true);
	}

	function delete_userfeatures()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$user_id = (int) $this->_info['userfeatures']['id'];
		$user_uuid = $this->_info['userfeatures']['uuid'];
		$func_key_private_template_id = (int) $this->_info['userfeatures']['func_key_private_template_id'];

		$r = $this->_userfeatures->delete($user_id);

		if($this->_status === 'delete')
			$this->_return['userfeatures'] = $r;

		if ($this->_func_key_manager->delete_template($func_key_private_template_id) === false)
			return(false);

		$bus_msg = $this->_bus_msg_factory->new_user_deleted_msg($user_id, $user_uuid);
		$this->_bus_msg_queue->add_msg($bus_msg);

		return($r);
	}

	function delete_linefeatures()
	{
		if(dwho_issa('linefeatures',$this->_info) === false)
			return(null);

		$result = $this->_line_manager->delete_linefeatures($this->_info['linefeatures']);

		if($this->_status === 'delete')
			$this->_return['linefeatures'] = $result;
	}

	function delete_phonefunckey()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$deleted = $this->_func_keys->delete_user_func_keys($this->_info['userfeatures']['id']);
		return($deleted);
	}

	function delete_callfiltermember()
	{
		if(($callfiltermember = &$this->_service->get_module('callfiltermember')) === false)
			return(true);

		return($callfiltermember->delete_member($this->_info['userfeatures']['id']));
	}

	function delete_schedule_path()
	{
		return $this->_schedule_path->delete_where(array('path' => 'user', 'pathid' =>	$this->_info['userfeatures']['id']));
	}

	function delete_groupmember()
	{
		return($this->_delete_member_type('group'));
	}

	function delete_queuemember()
	{
		return($this->_delete_member_type('queue'));
	}

	function _delete_member_type($type)
	{
		if(($type !== 'group' && $type !== 'queue') === true)
			return(false);

		$key = $type.'member';

		if(dwho_issa($key,$this->_info) === false)
			return(null);
		else if(dwho_issa('userfeatures',$this->_info) === false
		|| is_object($this->_queuemember) === false)
			return(false);

		$where = array();
		$where['usertype'] = 'user';
		$where['category'] = $type;
		$where['userid'] = $this->_info['userfeatures']['id'];

		$r = $this->_queuemember->delete_where($where);

		if($this->_status === 'delete')
			$this->_return[$key] = $r;

		return($r);
	}

	function _reverse_delete()
	{
		if($this->get_errnb() > 0
		|| $this->_status !== 'delete'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'userfeatures':
					if(isset($this->_origin['userfeatures']) === true
					&& $this->_origin['userfeatures'] !== false)
						$this->_userfeatures->add_origin($this->_origin['userfeatures']);
					break;
				case 'rightcall':
					$this->_rightcall->add_origin_list();
					break;
				case 'dialaction':
					$this->_dialaction->add_origin_list();
					break;
				case 'groupmember':
				case 'queuemember':
					if(isset($this->_origin[$key]) === true
					&& $this->_origin[$key] !== false)
						$this->_queuemember->add_origin_list($this->_origin[$key]);
					break;
			}
		}

		if(isset($this->_origin['phonefunckey']) && is_array($this->_origin['phonefunckey']))
		{
			$user_id = $this->_origin['userfeatures']['id'];
			$phonefunckeys = $this->_origin['phonefunckey'];
			$this->_func_keys->add_user_func_keys($user_id, $phonefunckeys);
		}

		return(true);
	}

	function unlinks()
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);

		$this->_dialaction->unlinked('user',$this->_info['userfeatures']['id']);

		return(true);
	}

	function enable()
	{
		$r = true;

		$this->_status = 'enable';
		$this->_return = array();

		if($this->enable_userfeatures() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return($r);
	}

	function disable()
	{
		$r = true;

		$this->_status = 'disable';
		$this->_return = array();

		if($this->disable_userfeatures() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return(true);
	}

	function enable_userfeatures()
	{
		return($this->_enable_disable_userfeatures(false));
	}

	function disable_userfeatures()
	{
		return($this->_enable_disable_userfeatures(true));
	}

	function _enable_disable_userfeatures($disable=false)
	{
		if(dwho_issa('userfeatures',$this->_info) === false)
			return(false);
		else if((bool) $disable === false)
			$r = $this->_userfeatures->enable($this->_info['userfeatures']['id']);
		else
			$r = $this->_userfeatures->disable($this->_info['userfeatures']['id']);

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['userfeatures'] = $r;

		return($r);
	}

	function _reverse_enable_disable()
	{
		if(($this->_status !== 'enable' && $this->_status !== 'disable') === true
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		$disable = $this->_status === 'enable';

		$this->_status = '';

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'userfeatures':
					$this->_enable_disable_userfeatures($disable);
					break;
			}
		}

		return(true);
	}

	function _prepare_import($arr, $partial=false)
	{
		if (isset($arr['userfeatures']['enableclient']) === true
		&& isset($arr['userfeatures']['profileclient']) === true
		&& (dwho_has_len($arr['userfeatures']['profileclient']) === true))
		{
			$profile_name = $arr['userfeatures']['profileclient'];
			if (($profile_id = $this->_cti_profile->get_cti_profile_id_with_name($profile_name)) === false)
				return('Wrong CTI profile name');
			$arr['userfeatures']['cti_profile_id'] = $profile_id;
		}

		$arr['user_vm_action'] = '';

		if(dwho_issa('voicemail',$arr) === true
		&& empty($arr['voicemail']) === false
		&& dwho_has_len($arr['voicemail'], 'number') === true
		&& dwho_has_len($arr['voicemail'], 'context') === true
		) {
			if(dwho_has_len($arr['voicemail'], 'name') !== true) {
				$arr['voicemail']['name'] = trim($arr['userfeatures']['firstname'] . " " . $arr['userfeatures']['lastname']);
			}
			$arr['userfeatures']['enablevoicemail'] = true;
			$arr['user_vm_action'] = 'add';
		} else {
			unset($arr['voicemail']);
		}

		if (isset($arr['userfeatures']['entityid']) === false)
			$arr['userfeatures']['entityid'] = 1;

		if (isset($arr['userfeatures']['enablexfer']) === false
		|| dwho_has_len($arr['userfeatures']['enablexfer']) === false)
			$arr['userfeatures']['enablexfer'] = 1;

		if (isset($arr['userfeatures']['dtmf_hangup']) === false
		|| dwho_has_len($arr['userfeatures']['dtmf_hangup']) === false)
			$arr['userfeatures']['dtmf_hangup'] = 0;

		if(dwho_has_len($arr['userfeatures'],'firstname') === true)
			$arr['userfeatures']['callerid'] = $arr['userfeatures']['firstname'].' ';

		if(dwho_has_len($arr['userfeatures'],'lastname') === true)
			$arr['userfeatures']['callerid'] .= $arr['userfeatures']['lastname'];

		if(dwho_has_len($arr['userfeatures'],'agentnumber') === true
		&& is_object($this->_agentfeatures) === true
		&& ($agentid = $this->_agentfeatures->get_primary(
					array('number' => $arr['userfeatures']['agentnumber']))) !== false) {
			$arr['userfeatures']['agentid'] = $agentid;
		}

		unset($arr['userfeatures']['agentnumber']);

		if(dwho_issa('linefeatures',$arr) === true) {
			if (!dwho_issa('id', $arr['linefeatures'])) {
				$arr['linefeatures']['id'] = array();
				array_push($arr['linefeatures']['id'], 0);
			}
			array_push($arr['linefeatures']['id'], 0);
		}

		return($arr);
	}

	private function upload_csv_file()
	{
		$http_response = dwho_http::factory('response');
		if($http_response === false) {
			dwho_report::push('error', 'could not parse HTTP request');
			return(false);
		}

		$fileinfo = $http_response->upload_file('import', $this->_conf['import']['file']);
		if($fileinfo === false) {
			dwho_report::push('error', 'could not upload file');
			return(false);
		}

		$csvdata = file_get_contents($fileinfo['tmp_name']);
		if($csvdata === false) {
			dwho_report::push('error', 'could not read CSV file');
			dwho_file::rm($fileinfo['tmp_name']);
			return(false);
		}

		$fileinfo['data'] = $csvdata;
		return $fileinfo;
	}

	function import_csv()
	{
		$fileinfo = $this->upload_csv_file();
		if($fileinfo === false)
			return(false);

		$result = $this->_confd_user->import_csv($fileinfo['data']);
		if($result === true) {
			dwho_file::rm($fileinfo['tmp_name']);
			return(true);
		}

		foreach($result as $error) {
			error_log($error);
			dwho_report::push('error', $error);
		}

		dwho_file::rm($fileinfo['tmp_name']);
		return(false);
	}

	function update_csv()
	{
		$fileinfo = $this->upload_csv_file();
		if($fileinfo === false)
			return(false);

		$result = $this->_confd_user->update_csv($fileinfo['data']);
		if($result === true) {
			dwho_file::rm($fileinfo['tmp_name']);
			return(true);
		}

		foreach($result as $error) {
			error_log($error);
			dwho_report::push('error', $error);
		}

		dwho_file::rm($fileinfo['tmp_name']);
		return(false);
	}

	function export_csv()
	{
		return $this->_confd_user->export_csv();
	}

	function add_from_json()
	{
		if(($data = $this->_get_data_from_json()) === false
		|| ($arr = $this->_prepare_import($data)) === false
		|| $this->set_add($arr) === false
		|| $this->add() === false)
			return(false);

		return($this->_return['userfeatures']);
	}

	function array_merge_recursive($arr1, $arr2)
	{
		foreach($arr2 as $k => $v)
		{
			if(array_key_exists($k, $arr1) && is_array($arr1[$k]) && is_array($v))
				$arr1[$k] = $this->array_merge_recursive($arr1[$k], $arr2[$k]);
			else
				$arr1[$k] = $v;
		}

		return $arr1;
	}

	function edit_from_json($mergewith=null)
	{
		if(($data = $this->_get_data_from_json()) === false
		|| ($arr = $this->_prepare_import($data, !is_null($mergewith))) === false)
			return(false);

		if(!is_null($mergewith))
		{
			unset($mergewith['linefeatures']);
			$arr = $this->array_merge_recursive($mergewith,$arr);
			if(is_array($arr['rightcall'])) {
			    $rightcall = array();
			    $rightcall[0] = strval($arr['rightcall'][0]['rightcallid']);
			    $arr['rightcall'] = $rightcall;
			}
		}

		if($this->set_edit($arr) === false
		|| $this->edit() === false)
			return(false);

		return(true);
	}

	function gen_password($len, $lower=false)
	{
		$pwd = '';
		for($i = 0; $i < $len; $i++)
		{
			$r = mt_rand(0, 35);
			$pwd .= $r < 10?$r:chr($r-10+65);
		}

		if($lower)
			$pwd = strtolower($pwd);

		return $pwd;
	}

	public function has_existing_line()
	{
		if(isset($this->_info['linefeatures'])
		&& $this->_info['linefeatures'] !== false
		&& count($this->_info['linefeatures']) > 0)
			return(true);
		return(false);
	}
}

?>
