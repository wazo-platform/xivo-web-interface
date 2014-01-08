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

dwho::load_class('dwho_network');
dwho::load_class('dwho_uri');
dwho::load_class('dwho_filter');
dwho::load_class('dwho_http');
dwho::load_class('dwho::file::csv');

dwho_file::required(array(XIVO_PATH_OBJECT,'service','ipbx','asterisk','config','config.inc'),true);
dwho_file::required(array(XIVO_PATH_OBJECT,'service','ipbx','asterisk.inc',),true);
dwho_file::required(array(XIVO_PATH_OBJECT,'application','xivo.inc'),true);

class xivo_application_xivo_object_wizard extends xivo_application_xivo_object_abstract
{
	var $_conf             = array();
	var $_sysconfd         = false;
	var $_filter           = false;
	var $_uri              = false;
	var $_http_response    = false;
	var $_wizsess          = null;
	var $_ipbx             = null;
	var $_savetmpfile      = '/tmp/wizard';

	public function __construct(&$xobj,$param=null,$components=true)
	{
		parent::__construct($xobj);

		if(($this->_sysconfd = &$this->_xobj->get_module('sysconfd')) === false)
			trigger_error('Unable to load sysconfd module',E_USER_ERROR);

		$this->_uri = new dwho_uri();
		$this->_filter = new dwho_filter();
		$this->_http_response = dwho_http::factory('response');

		if(dwho_issa('_wizard',$_SESSION) === false)
			$_SESSION['_wizard'] = array();

		$this->_load_config(dirname(__FILE__));

		$this->_wizsess = &$_SESSION['_wizard'];

		$this->_load_datastorage();
	}

	private function _load_datastorage()
	{
		$var = dwho_gat::get('_CF');
		$this->_load_ipbx_service($var['datastorage']['general']['asterisk']);
		$this->_load_xivo_dso($var['datastorage']['general']['xivo']);
	}

	private function _load_xivo_dso($uri)
	{
		$_XIVO = dwho_gct::get('xivo');
		$_XIVO->load_datastorage($uri);
	}

	private function _load_ipbx_service($uri)
	{
		$_SRE = dwho_gct::get('xivo_service');
		$engines = $_SRE->get_engines('ipbx');

		if(defined('XIVO_SRE_IPBX_LABEL') === false)
			define('XIVO_SRE_IPBX_LABEL', $engines['asterisk']['label']);

		$config['general'] = array();
		$config['general']['datastorage'] = $uri;

		$_SRE->set_engine_config('ipbx','asterisk',$config);

		$this->_ipbx = &$_SRE->get('ipbx','asterisk',true);
	}

	private function _get_save_data_tmp()
	{
		if (!($data = file_get_contents($this->_savetmpfile))
		|| !($wizsess = dwho_json::decode($data,true))) {
			dwho_report::push('error', 'Can\'t get data for wizard');
			return false;
		}
		return(($this->_wizsess = $wizsess));
	}

	function reset_wizard()
	{
		$_SESSION['_wizard'] = array();
	}

	function get_first_step()
	{
		reset($this->_conf['step']);
		return(key($this->_conf['step']));
	}

	function get_last_step()
	{
		end($this->_conf['step']);
		return(key($this->_conf['step']));
	}

	function get_current_step()
	{
		if(isset($this->_wizsess['step']) === false)
			$this->_wizsess['step'] = $this->get_first_step();

		return($this->_wizsess['step']);
	}

	function get_previous_sess_step()
	{
		if(isset($this->_wizsess['prevstep']) === false)
			return(false);

		return($this->_wizsess['prevstep']);
	}

	function get_previous_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];

		if(isset($this->_conf['step'][$step]) === false)
			return(false);

		if($step === $this->get_last_step())
		{
			end($this->_conf['step']);
			prev($this->_conf['step']);
		}
		else
		{
			reset($this->_conf['step']);
			while((list($key) = each($this->_conf['step']))
			&& $key !== $step);
			prev($this->_conf['step']);
			prev($this->_conf['step']);
		}

		return(key($this->_conf['step']));
	}

	function can_previous_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];

		if($step === $this->get_first_step())
			return(false);

		$prevstep = $this->get_previous_step($step);

		if(dwho_issa('commit',$this->_wizsess) === true
		&& dwho_issa('step',$this->_wizsess['commit']) === true
		&& isset($this->_wizsess['commit']['step'][$prevstep]) === true)
			return(false);
		else if($step === $this->get_last_step())
			return($this->get_previous_step($step));
		else if(isset($this->_conf['step'][$prevstep]) === false)
			return(false);

		return($prevstep);
	}

	function set_previous_step()
	{
		$step = $this->get_current_step();

		if(($step = $this->can_previous_step($step)) === false)
			return(false);

		$this->_wizsess['step'] = $step;
	}

	function get_next_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
		$step = $this->_wizsess['step'];

		if(isset($this->_conf['step'][$step]) === false)
			return(false);

		reset($this->_conf['step']);
		while((list($key) = each($this->_conf['step']))
		&& $key !== $step);

		return(key($this->_conf['step']));
	}

	function can_next_step($step=null)
	{
		if($step === null && isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];

		if($step === $this->get_last_step())
			return(false);

		return($step);
	}

	function get_step_element()
	{
		if(isset($this->_wizsess['step']) === true)
			$step = $this->_wizsess['step'];
		else
			$step = 'welcome';

		switch($step)
		{
			case 'mainconfig':
				exec('hostname -s', $hostname);
				exec('hostname -d', $dnsdomainname);
				exec('sed -e \'/^$/d\' /etc/resolv.conf | awk \'{if (tolower($1)=="nameserver") print $2}\'', $nameserver);

				$mainconfig = array();
				$mainconfig['hostname'] = $hostname[0];
				$mainconfig['domain'] = $dnsdomainname[0];
				$resolvconf = array();
				$resolvconf['nameserver1'] = $nameserver[0];
				$resolvconf['nameserver2'] = isset($nameserver[1])?$nameserver[1]:'';

				return(array('mainconfig'	=> $mainconfig,
							'resolvconf'	=> $resolvconf));
			case 'entitycontext':
				return(array('entity'	=> $this->_conf['element']['entity'],
							'context'	=> $this->_conf['element']['context']));
			default:
				return(null);
		}
	}

	function discover_finish_uri()
	{
		if (($wizsess = $this->_get_save_data_tmp()) === false)
		{
			dwho_report::push('error', 'Can\'t get data wizard');
			return false;
		}
		elseif(isset($_SERVER['SERVER_ADDR'],$_SERVER['SERVER_PORT']) === false)
		{
			dwho_report::push('error', 'Can\'t get SERVER_ADDR or SERVER_PORT');
			return false;
		}
		else if($_SERVER['SERVER_ADDR'] !== '127.0.0.1'
		&& $_SERVER['REMOTE_ADDR'] !== '::1'
		&& dwho_issa('netiface',$wizsess) === true
		&& isset($wizsess['netiface']['address']) === true)
			$uri = $wizsess['netiface']['address'];
		else
			$uri = $_SERVER['SERVER_ADDR'];

		$uri .= ':'.$_SERVER['SERVER_PORT'];
		// append lang
		if(isset($wizsess['welcome']['language']))
			$uri .= '?hl='.$wizsess['welcome']['language'];

		$scheme = 'http';

		if(isset($_SERVER['HTTPS']) === true
		&& $_SERVER['HTTPS'] === 'on')
			$scheme .= 's';

		return($scheme.'://'.$uri);
	}

	function get_eth_iface()
	{
		$interface = 'eth0';
		exec('ifconfig "'.$interface.'" | sed -rn \'2s/ .*:(.*)/\1/p\'', $netmask);
		exec('ifconfig "'.$interface.'" | sed -rn \'2s/.*addr:(.*) Bcast.*/\1/p\'', $addr);
		exec('ip route | awk \'/default/ { print $3 }\'', $gateway);

		$r = array();
		$r['name'] = $interface;
		$r['address'] = $addr[0];
		$r['netmask'] = $netmask[0];
		$r['gateway'] = $gateway[0];

		return($r);
	}

	function step_mainconfig()
	{
		$r = array();

		if(dwho_issa('mainconfig',$this->_wizsess) === true)
			$r['mainconfig'] = $this->_wizsess['mainconfig'];
		else
			$r['mainconfig'] = null;

		if(dwho_issa('netiface',$this->_wizsess) === true)
			$r['netiface'] = $this->_wizsess['netiface'];
		else
			$r['netiface'] = $this->get_eth_iface();

		if(dwho_issa('resolvconf',$this->_wizsess) === true)
			$r['resolvconf'] = $this->_wizsess['resolvconf'];
		else
			$r['resolvconf'] = null;

		return($r);
	}

	function step_entitycontext()
	{
		$r = array();

		if(dwho_issa('entity',$this->_wizsess) === true)
			$r['entity'] = $this->_wizsess['entity'];
		else
			$r['entity'] = null;

		if(dwho_issa('context_internal',$this->_wizsess) === true)
			$r['context_internal'] = $this->_wizsess['context_internal'];
		else
			$r['context_internal'] = null;

		if(dwho_issa('context_incall',$this->_wizsess) === true)
			$r['context_incall'] = $this->_wizsess['context_incall'];
		else
			$r['context_incall'] = null;

		if(dwho_issa('context_outcall',$this->_wizsess) === true)
			$r['context_outcall'] = $this->_wizsess['context_outcall'];
		else
			$r['context_outcall'] = null;

		return($r);
	}

	function step_validate()
	{
		$data = dwho_json::encode($this->_wizsess);
		if(($fh = fopen($this->_savetmpfile, 'w+')) !== false)
		{
			fputs($fh, $data, strlen($data));
			fclose($fh);
		}

		$this->_get_save_data_tmp();

		return($this->_wizsess);
	}

	function save_step_welcome($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		if(is_array($data) === false
		|| isset($data['hl']) === false)
		{
			$this->_set_error('welcome','language','missing');
			return(false);
		}

		$this->_wizsess['welcome'] = array();

		$_LANG = &dwho_gat::load_get('language',XIVO_PATH_OBJECTCONF);
		$language_available = dwho_array_intersect_key($_LANG,dwho_i18n::get_language_translated_list());

		if(isset($language_available[$data['hl']]) === false)
		{
			$this->_set_error('welcome','language','invalid');
			return(false);
		}

		$_SESSION['_i18n']['user'] = array();
		$_SESSION['_i18n']['user']['language'] = $data['hl'];

		$this->_wizsess['welcome']['language'] = $data['hl'];
		$this->_wizsess['step'] = $this->get_next_step('welcome');

		return(true);
	}

	function save_step_license($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		if(is_array($data) === false
		|| isset($data['license-agree']) === false
		|| (bool) $data['license-agree'] === false)
		{
			$this->_set_error('license','disagree');
			return(false);
		}

		$this->_wizsess['license'] = array();
		$this->_wizsess['license']['agree'] = true;

		$this->_wizsess['step'] = $this->get_next_step('license');

		return(true);
	}

	function save_mainconfig($data)
	{
		if(dwho_issa('mainconfig',$data) === false)
		{
			$this->_set_error('mainconfig','hostname','missing');
			return(false);
		}

		$this->_filter->set_validator($this->_conf['filter']['mainconfig']);

		if(($r = (bool) $this->_filter->chk($data['mainconfig'])) === false)
			$this->_set_error('mainconfig',$this->_filter->get_error());

		$this->_set_result('mainconfig',$this->_filter->get_result());

		return($r);
	}

	function save_netiface($data)
	{
		if(($iface = $this->get_eth_iface()) === null)
			return(null);
		else if(dwho_issa('netiface',$data) === false)
		{
			$this->_set_error('netiface','address','missing');
			return(false);
		}

		$this->_filter->set_validator($this->_conf['filter']['netiface']);

		if(($r = (bool) $this->_filter->chk($data['netiface'])) === false)
			$this->_set_error('netiface',$this->_filter->get_error());

		$resnetiface = $this->_filter->get_result();
		$resnetiface['name'] = $iface['name'];
		$this->_set_result('netiface',$resnetiface);

		if($r === true
		&& dwho_has_len($resnetiface,'gateway') === true
		&& dwho_network::ipv4_in_subnet($resnetiface['gateway'],
			$resnetiface['address'].'/'.$resnetiface['netmask']) === false)
		{
			$r = false;
			$this->_set_error('netiface','gateway','invalid');
		}

		return($r);
	}

	function save_resolvconf($data)
	{
		if(dwho_issa('resolvconf',$data) === false)
		{
			$this->_set_error('resolvconf','nameserver1','invalid');
			return(false);
		}

		$this->_filter->set_validator($this->_conf['filter']['resolvconf']);

		if(($r = (bool) $this->_filter->chk($data['resolvconf'])) === false)
			$this->_set_error('resolvconf',$this->_filter->get_error());

		$result = $this->_filter->get_result();

		if($r === true)
		{
			$result = dwho_array_combine(array_keys($result),
			explode(' ',trim(dwho_implode(' ',$result))),
			true,
			'');
		}

		$this->_set_result('resolvconf',$result);

		return($r);
	}

	function save_step_mainconfig($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		$this->save_mainconfig($data);
		$this->save_netiface($data);
		$this->save_resolvconf($data);

		if($this->get_errnb() > 0)
			return(false);

		$this->_wizsess['mainconfig'] = $this->get_result('mainconfig');
		$this->_wizsess['netiface'] = $this->get_result('netiface');
		$this->_wizsess['resolvconf'] = $this->get_result('resolvconf');
		$this->_wizsess['step'] = $this->get_next_step('mainconfig');

		return(true);
	}

	function save_entity($data)
	{
		if(dwho_issa('entity',$data) === false)
		{
			$this->_set_error('entity','name','missing');
			return(false);
		}

		$appentity = &$this->_xobj->get_application('entity');

		if(($r = (bool) $appentity->set_add($data['entity'])) === false)
			$this->_set_error('entity',$appentity->get_error('entity'));

		$this->_set_result('entity',$appentity->get_result('entity'));

		return($r);
	}

	function _prepare_context($data,$type=null,$entity=null)
	{
		$r = array();
		$r['context']['name'] = dwho_ak('name',$data,true);
		$r['context']['displayname'] = dwho_ak('displayname',$data,true);
		$r['context']['entity'] = strval($entity);
		$r['context']['contexttype'] = dwho_ak('contexttype',$data,true);

		if($type === null)
			return($r);

		$r['contextnumbers'] = array();
		$r['contextnumbers'][$type] = array();
		$r['contextnumbers'][$type][0] = array();
		$r['contextnumbers'][$type][0]['numberbeg'] = dwho_ak('numberbeg',$data,true);
		$r['contextnumbers'][$type][0]['numberend'] = dwho_ak('numberend',$data,true);

		if($type === 'incall')
			$r['contextnumbers'][$type][0]['didlength'] = dwho_ak('didlength',$data,true);

		return($r);
	}

	function save_context($data)
	{
		if(dwho_issa('context',$data) === false)
		{
			$this->_set_error('context_internal','name','missing');
			$this->_set_error('context_incall','name','missing');
			$this->_set_error('context_outcall','name','missing');
			return(false);
		}

		$entity = $this->get_result_var('entity','name');

		$appcontext = &$this->_ipbx->get_application('context');

		$outcall = null;
		$resoutcall = false;

		if(dwho_issa('outcall',$data['context']) === false)
			$this->_set_error('context_outcall','missing_context');
		else
		{
			$data['context']['outcall']['contexttype'] = 'outcall';
			$outcall = $this->_prepare_context($data['context']['outcall'],null,$entity);

			if(($resoutcall = (bool) $appcontext->set_add($outcall)) === false)
			$this->_set_error('context_outcall',$appcontext->get_error());

			$this->_set_result('context_outcall',$appcontext->get_result());
		}

		$internal = null;
		$resinternal = false;

		if(dwho_issa('internal',$data['context']) === false)
			$this->_set_error('context_internal','missing_context');
		else
		{
			$data['context']['internal']['contexttype'] = 'internal';
			$internal = $this->_prepare_context($data['context']['internal'],'user',$entity);

			if(isset($outcall['context']['name']) === true)
				$internal['contextinclude'] = array($outcall['context']['name']);

			if(($resinternal = (bool) $appcontext->set_add($internal)) === false)
				$this->_set_error('context_internal',$appcontext->get_error());
			else if(dwho_has_len($data['context']['internal'],'numberbeg') === false)
			{
				$resinternal = false;
				$this->_set_error('context_internal','missing_numberbeg');
			}

			$this->_set_result('context_internal',$appcontext->get_result());
		}

		$incall = null;
		$resincall = false;

		if(dwho_issa('incall',$data['context']) === false)
			$this->_set_error('context_incall','name','missing');
		else
		{
			$data['context']['incall']['contexttype'] = 'incall';
			$incall = $this->_prepare_context($data['context']['incall'],'incall',$entity);

			if(dwho_has_len($incall['contextnumbers']['incall'][0]['numberbeg']) === false
			&& dwho_has_len($incall['contextnumbers']['incall'][0]['numberend']) === false)
				unset($incall['contextnumbers']);

			if(($resincall = (bool) $appcontext->set_add($incall)) === false)
				$this->_set_error('context_incall',$appcontext->get_error());

			$this->_set_result('context_incall',$appcontext->get_result());
		}

		if($resoutcall === false
		|| $resinternal === false
		|| $resincall === false)
			return(false);

		return(array('outcall'	=> $outcall,
					'internal'	=> $internal,
					'incall'	=> $incall));
	}

	function save_step_entitycontext($data)
	{
		$this->_wizsess['prevstep'] = $this->get_current_step();

		$this->save_entity($data);
		$rescontext = $this->save_context($data);

		if($this->get_errnb() > 0 || is_array($rescontext) === false)
			return(false);

		$this->_wizsess['entity'] = $this->get_result('entity');
		$this->_wizsess['context'] = $rescontext;
		$this->_wizsess['context_internal'] = $this->get_result('context_internal');
		$this->_wizsess['context_incall'] = $this->get_result('context_incall');
		$this->_wizsess['context_outcall'] = $this->get_result('context_outcall');

		$this->_wizsess['step'] = $this->get_next_step('entitycontext');

		return(true);
	}

	private function _commit_adminpasswd()
	{
		if(dwho_issa('mainconfig',$this->_wizsess) === false
		|| isset($this->_wizsess['mainconfig']['adminpasswd']) === false)
		return(false);

		xivo::load_class('xivo_user',XIVO_PATH_OBJECT);
		xivo::load_class('xivo_user_root',array(XIVO_PATH_OBJECT,'user'),'root');

		$root = new xivo_user_root(false);

		if(($info = $root->get_where(array('meta' => 'root'))) === false
		|| $root->edit('root',
		array('id'		=> $info['id'],
			'passwd'	=> $this->_wizsess['mainconfig']['adminpasswd'])) === false)
		{
			$this->_set_error('mainconfig','adminpasswd','save');
			return(false);
		}

		return(true);
	}

	private function _commit_hosts()
	{
		if(dwho_issa('mainconfig',$this->_wizsess) === false
		|| isset($this->_wizsess['mainconfig']['hostname'],
		$this->_wizsess['mainconfig']['domain']) === false)
			return(false);

		$hosts = array();
		$hosts['hostname'] = $this->_wizsess['mainconfig']['hostname'];
		$hosts['domain'] = $this->_wizsess['mainconfig']['domain'];

		if($this->_sysconfd->request_post('/hosts',$hosts) === false)
		{
			$this->_set_error('mainconfig','hosts','save');
			return(false);
		}

		return(true);
	}

	private function _commit_resolvconf()
	{
		if(dwho_issa('resolvconf',$this->_wizsess) === false)
			return(null);

		$resolvconf = array();
		$resolvconf['nameservers'] = array_values(
		array_filter($this->_wizsess['resolvconf'],'dwho_has_len'));

		if(empty($resolvconf['nameservers']) === true)
			return(null);

		if(dwho_issa('mainconfig',$this->_wizsess) === true
		&& isset($this->_wizsess['mainconfig']['domain']) === true)
			$resolvconf['search'] = $this->_wizsess['mainconfig']['domain'];

		if($this->_sysconfd->request_post('/resolv_conf',$resolvconf) === false)
		{
			$this->_set_error('resolvconf','nameservers','save');
			return(false);
		}

		$module = &$this->_xobj->get_module('resolvconf');

		$data = array(
			'hostname'    => $this->_wizsess['mainconfig']['hostname'],
			'domain'      => $this->_wizsess['mainconfig']['domain'],
			'nameserver1' => $resolvconf['nameservers'][0],
			'nameserver2' => $resolvconf['nameservers'][1],
			'nameserver3' => '',
			'search'      => $resolvconf['search'],
			'description' => 'wizard configuration'
		);

		if($module->save($data) === false)
			return false;

		return(true);
	}

	private function _commit_entity()
	{
		if(dwho_issa('entity',$this->_wizsess) === false)
			return(false);

		$name = $this->_wizsess['entity']['name'];

		$appentity = &$this->_xobj->get_application('entity');

		// allow to reexecute the process several times
		$appentity->delete_all();

		if($appentity->set_add($this->_wizsess['entity']) === false
		|| $appentity->add() === false)
		{
			$this->_set_error('entity','save');
			return(false);
		}

		return(true);
	}

	private function _commit_context()
	{
		if(dwho_issa('context',$this->_wizsess) === false
		|| dwho_issa('internal',$this->_wizsess['context']) === false
		|| dwho_issa('incall',$this->_wizsess['context']) === false
		|| dwho_issa('outcall',$this->_wizsess['context']) === false)
			return(false);

		$context_internal = true;
		$context_incall = true;
		$context_outcall = true;
		$context_switchboard = true;

		$listcontext = $this->_wizsess['context'];

		$appcontext = &$this->_ipbx->get_application('context');

		// allow to reexecute the process several times
		foreach($appcontext->get_contexts_list() as $context)
		{
			if($appcontext->get($context['context']['name']) !== false)
			{
				$appcontext->_info['deletable']       = true;
				$appcontext->_info['deletablenumber'] = true;
				$appcontext->_info['deletablemember'] = true;

				$appcontext->delete();
			}
		}

		$switchboard_directory_context = array(
			'context' => array(
				'name' => '__switchboard_directory',
				'displayname' => 'Switchboard',
				'entity' => $this->_wizsess['entity']['name'],
				'contexttype' => 'others'
			)
		);

		if($appcontext->set_add($switchboard_directory_context) === false
		|| $appcontext->add() === false)
		{
			$context_switchboard = false;
			$this->_set_error('context_switchboard','save');
		}

		if($appcontext->set_add($listcontext['internal']) === false
		|| $appcontext->add() === false)
		{
			$context_internal = false;
			$this->_set_error('context_internal','save');
		}

		if($appcontext->set_add($listcontext['incall']) === false
		|| $appcontext->add() === false)
		{
			$context_incall = false;
			$this->_set_error('context_incall','save');
		}

		if($appcontext->set_add($listcontext['outcall']) === false
		|| $appcontext->add() === false)
		{
			$context_outcall = false;
			$this->_set_error('context_outcall','save');
		}

		if($context_internal === false
		|| $context_incall === false
		|| $context_outcall === false
		|| $context_switchboard === false)
			return(false);

		return(true);
	}

	private function _commit_netiface()
	{
		$modnetiface = &$this->_xobj->get_module('netiface');
		if (($voip_iface = $modnetiface->get_voip_iface()) === false)
		{
			$this->_set_error('netiface','get_voip_iface');
			return(false);
		}

		$args = array();
		$args['address'] = $voip_iface['address'];
		$args['netmask'] = $voip_iface['netmask'];
		$args['gateway'] = $voip_iface['gateway'];

		if($this->_sysconfd->request_post('/modify_eth_ipv4',
		$args,
		array(array('ifname',$voip_iface['ifname']))) === false)
		{
			$this->_set_error('netiface','save');
			return(false);
		}

		return true;
	}

	private function _prepare_netiface()
	{
		if(dwho_issa('netiface',$this->_wizsess) === false
		|| isset($this->_wizsess['netiface']['name']) === false)
		{
			dwho_report::push('error', 'Can\'t get netiface in session');
			return(false);
		}

		$netiface = $this->_wizsess['netiface'];
		$name = $netiface['name'];

		$module = &$this->_xobj->get_module('netiface');

		//computing broadcast address
		$ip = split('\.', $netiface['address']);
		$nm = split('\.', $netiface['netmask']);

		$bcast  = array();
		for($i = 0; $i < 4; $i++)
		{
			$bcast[]  = intval($ip[$i]) | (intval($nm[$i]) ^ 255);
		}

		$bcast  = implode('.', $bcast);

		$data = array(
			'name'        => $name,
			'ifname'      => $name,
			'hwtypeid'    => 1,
			'networktype' => 'voip',
			'type'        => 'iface',
			'method'      => 'static',
			'address'     => $netiface['address'],
			'netmask'     => $netiface['netmask'],
			'broadcast'   => $bcast,
			'gateway'     => $netiface['gateway'],
			'mtu'         => 1500,
			'description' => 'wizard configuration');

		if(($data = $module->chk_values($data)) === false)
		{
			dwho_report::push('error', $module->get_filter_error());
			return false;
		}

		if($module->save($data) === false)
		{
			dwho_report::push('error', 'Can\'t save network data');
			return false;
		}

		return(true);
	}

	private function _commit_language($lang)
	{
		$this->_get_save_data_tmp();

		if($lang == 'fr')
			$lang = 'fr_FR';
		else
			$lang = 'en_US';

		exec('cat /etc/timezone', $timezone);
		$module = &$this->_ipbx->get_module('general');
		$module->edit(1, array('timezone' => $timezone[0]));

		foreach(array('sip', 'iax') as $name)
		{
			$app = &$this->_ipbx->get_apprealstatic($name);
			$module = &$app->get_module('general');

			if(!$module->set(array('var_name' => 'language', 'var_val' => $lang))
			|| !$module->save())
			{
				dwho_report::push('error', 'Can\'t edit '.$name.' module');
				return false;
			}
		}

		$sccpgeneralmodule = &$this->_ipbx->get_module('sccpgeneralsettings');
		if($sccpgeneralmodule->save_sccp_general_option('language', $lang) === false)
		{
			dwho_report::push('error', 'Can\'t save language in SCCP general settings');
			return(false);
		}

		return(true);
	}

	function save_step_validate($data, $tpl)
	{
		return(true);
	}

	public function start_xivo_services()
	{
		$data = array();
		$data['xivo-service'] = 'enable';
		if ($this->_sysconfd->request_post('/xivoctl', $data))
			$data['xivo-service'] = 'start';
			return($this->_sysconfd->request_post('/xivoctl', $data));
		return(false);
	}

	public function commit_db_data()
	{
		$this->_get_save_data_tmp();
		$this->_commit_adminpasswd();
		$this->_commit_hosts();
		$this->_commit_resolvconf();
		$this->_commit_entity();
		$this->_commit_context();
		$this->_prepare_netiface();
		return(true);
	}

	public function commit_syslanguage()
	{
		$this->_commit_language($this->_wizsess['welcome']['language']);
		return(true);
	}

	public function set_default_provisioning_values()
	{
		$wizsess = $this->_get_save_data_tmp();
		$netiface = $wizsess['netiface'];

		$rs = array();
		$rs['commented'] = 0;
		$rs['var_name'] = 'autocreate_prefix';
		$rs['var_val'] = 'ap'.dwho_generate_code();

		$appsip = &$this->_ipbx->get_apprealstatic('sip');
		$appgeneralsip = &$appsip->get_module('general');

		if($appgeneralsip->set($rs) === false
		|| $appgeneralsip->save() === false)
		{
			dwho_report::push('error', 'Can\'t create autocreate_prefix in staticsip table');
			return(false);
		}

		$provdconfig = &$this->_xobj->get_module('provdconfig');
		if ($provdconfig->eval_required_config($netiface['address'],true) === false)
			return(false);

		return(true);
	}

	public function commit_netinfos()
	{
		return($this->_commit_netiface());
	}

	public function commit_commonconf()
	{
		if(($commonconf = &$this->_xobj->get_module('commonconf')) === false)
			trigger_error('Unable to load commonconf module',E_USER_ERROR);

		$content = '';
		if($commonconf->generate($content) !== false)
		{
			$content = str_replace("\n", "<br/>\n", $content);
			$content = $commonconf->apply();

			$this->_sysconfd->request_get('/dhcpd_update');

			unlink($this->_savetmpfile);
			session_destroy();
			touch(XIVO_LIB.'/configured');
			file_put_contents(XIVO_LIB.'/configured', date('Y-m-d H:i:s')."\n");
			return(true);
		}

		dwho_report::push('error', $content);
	}

	function save($data, $tpl)
	{
		$step = $this->get_current_step();

		if(isset($this->_conf['step'][$step]) === false)
			return(null);

		$save_step_function_name = 'save_step_'.$step;

		return($this->$save_step_function_name($data, $tpl));
	}
}

?>
