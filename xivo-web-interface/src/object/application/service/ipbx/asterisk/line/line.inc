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


dwho::load_class('dwho_network');
dwho::load_class('dwho_http');

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);

class xivo_application_service_asterisk_line extends xivo_application_asterisk_abstract
{
	protected $_dso				= null;
	protected $_contextlist		= false;
	protected $_linefeatures 	= false;
	protected $_protocolname	= false;
	protected $_protocol		= false;
	protected $_user_line		= false;
	protected $_extensions		= false;
	protected $_provdconfig		= false;
	protected $_devicefeatures 	= false;
	protected $_conf			= array();

	public function __construct(&$service,&$dso,$param=null,$components=true)
	{
		parent::__construct($service);

		if(is_object($dso) === false)
			trigger_error('Invalid Datastorage object',E_USER_ERROR);

		$this->_dso = &$dso;

		if(($this->_linefeatures = &$this->_service->get_module('linefeatures')) === false
		|| $this->load_init($param) === false)
			return(false);
		else if((bool) $components === true)
			$this->load_components();

		return(true);
	}

	public function load_init($param=array())
	{
		$this->_reset_info();
		$this->_reset_params();
		$this->_reset();

		return(true);
	}

	public function load_components()
	{
		$this->_components = true;

		if(($this->_user_line = &$this->_service->get_module('user_line')) === false
		|| ($this->_extensions = &$this->_service->get_module('extensions')) === false
		|| ($this->_devicefeatures = &$this->_service->get_module('devicefeatures')) === false
		|| ($_XOBJ = &dwho_gct::get('xivo_object')) === false
		|| ($this->_provdconfig = &$_XOBJ->get_module('provdconfig')) === false)
			return(false);

		return(true);
	}

	public  function _reset()
	{
		$this->_protocolname = false;

		return(parent::_reset());
	}

	public function get_nb($arr=null,$disable=null,$initialized=null)
	{
		return($this->_dso->get_nb($arr,$disable,$initialized));
	}

	public function get($id,$disable=null,$nocomponents=null,$ipbxinfos=false)
	{
		$this->_info = $this->_origin = array();

		if(($this->_info['linefeatures'] = $this->_linefeatures->get($id)) === false)
			return(false);

		if($this->_info['linefeatures'] === false
		|| ($this->_protocol = &$this->_service->get_protocol_module($this->_info['linefeatures']['protocol'])) === false
		|| ($this->_info['protocol'] = $this->_protocol->get($this->_info['linefeatures']['protocolid'],$disable)) === false)
			return(false);

		$this->_origin['protocol'] = $this->_protocol->get_origin();
		$this->_origin['linefeatures'] = $this->_linefeatures->get_origin();

		if($this->_components === false)
			return($this->_info);

		if(dwho_ak('exten',$nocomponents,true) === false)
			$this->_get_exten();

		if ($ipbxinfos === true)
			$this->_get_ipbx_infos();

		return($this->_info);
	}

	function _get_exten()
	{
		$this->_info['exten'] = false;
		$this->_origin['exten'] = false;

		if(($this->_info['exten'] = $this->_extensions->get_exten(
										$this->_info['linefeatures']['number'],
										$this->_info['linefeatures']['context'])) !== false)
			$this->_origin['exten'] = $this->_extensions->get_origin();

		return($this->_info['exten']);
	}

	function _get_ipbx_infos()
	{
		$show_peer_command = array(
			'sip'  => 'sip show peer',
		);

		$this->_info['ipbx'] = false;

		$line_protocol = $this->_info['linefeatures']['protocol'];
		if (!array_key_exists($line_protocol, $show_peer_command))
			return false;

		$peer_name = $this->_info['protocol']['name'];
		$ipbx_query = sprintf("%s %s load",
			$show_peer_command[$line_protocol],
			$peer_name
		);

		$ipbx_response = $this->_service->discuss_ipbx($ipbx_query, true);
		$ipbx_infos = $this->_parse_ipbx_infos($ipbx_response[0]);

		$this->_info['ipbx'] = $ipbx_infos;
		return true;
	}

	function _parse_ipbx_infos($ipbx_response)
	{
		$ipbx_response_lines = explode("\n", $ipbx_response);

		$ipbx_infos = array();
		foreach($ipbx_response_lines as $line)
		{
			if (strstr($line, 'XIVO_USERID = '))
			{
				$item = explode('=',$line);
				$ipbx_infos['xivo_userid'] = trim($item[1]);
			} else {
				if (strpos($line,':'))
				{
					$item = explode(':',$line,2);

					// Remove bad characters and normalize the key values
					$item[0] = str_replace(array('.'),'',$item[0]);
					$item[0] = trim(str_replace(array('*','=','->'),' ',$item[0]));
					$item[0] = strtolower(str_replace(array(' ','-'),'_',$item[0]));

					$info_key = $item[0];
					$info_value = trim($item[1]);

					if (empty($info_value)) {
						$info_value = '&nbsp;';
					} else {
						$info_value = htmlspecialchars($info_value);
					}

					$ipbx_infos[$info_key] = $info_value;
				}
			}
		}
		return $ipbx_infos;
	}

	public function get_elements()
	{
		$r = array();

		$r['protocol'] = $this->_service->get_protocol_element();

		return($r);
	}

	public function get_all_context()
	{
		if(($r = $this->_linefeatures->get_all_context()) !== false)
			ksort($r);

		return($r);
	}

	public function get_lines_search($search,$where=array(),$disable=null,$order=null,$limit=null,$assoc=false)
	{
		return($this->_get_lines('search',$disable,$search,$where,$order,$limit,$assoc));
	}

	public function get_lines_list($where=array(),$disable=null,$order=null,$limit=null,$assoc=false)
	{
		return($this->_get_lines('list',$disable,'',$where,$order,$limit,$assoc));
	}

	private function _get_lines(
			$action,
			$disable=null,
			$search='',
			$where=array(),
			$order=null,
			$limit=null,
			$assoc=false)
	{
		$search = strval($search);

		switch($action)
		{
			case 'search':
				$rs = $this->_dso->search($search,$where,$disable,$order,$limit);
				break;
			case 'list':
			default:
				$rs = $this->_dso->get_all($where,$disable,$order,$limit);
		}

		$this->_cnt = $this->_dso->get_cnt();

		if($rs === false || ($nb = count($rs)) === 0)
			return(false);

		$r = array();

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];

			$linefeatures = dwho_array_startswith_key($ref,'linefeatures.',true);
			$linefeatures = $this->_linefeatures->_prepare($linefeatures);

			$r[$linefeatures['id']] = array();

			$userfeatures = dwho_array_startswith_key($ref,'userfeatures.',true);

			if (is_array($userfeatures) === true
			&& isset($userfeatures['firstname'], $userfeatures['lastname']))
				$linefeatures['useridentity'] = $userfeatures['firstname'].' '.$userfeatures['lastname'];
			else
				$linefeatures['useridentity'] = '-';

			switch($linefeatures['protocol'])
			{
				case XIVO_SRE_IPBX_AST_PROTO_SIP:
					$sip = dwho_array_startswith_key($ref,'usersip.',true);
					$sip['initialized'] = dwho_has_len($sip['fullcontact']);
					unset($sip['fullcontact']);
					$sip['identity'] = strtoupper($sip['protocol']).'/'.$sip['name'];
					$r[$linefeatures['id']] = array_merge($sip,$linefeatures);
					break;
				case XIVO_SRE_IPBX_AST_PROTO_SCCP:
					$sccp = dwho_array_startswith_key($ref,'sccpline.',true);
					$sccp['protocol'] = 'SCCP';
					$sccp['initialized'] = true;
					$sccp['identity'] = strtoupper($sccp['protocol']).'/'.$sccp['name'];
					$r[$linefeatures['id']] = array_merge($sccp,$linefeatures);
					break;
				case XIVO_SRE_IPBX_AST_PROTO_CUSTOM:
					$custom = dwho_array_startswith_key($ref,'usercustom.',true);
					$custom['initialized'] = true;
					#$custom['identity'] = strtoupper(XIVO_SRE_IPBX_AST_PROTO_CUSTOM).'/'.$custom['interface'];
					$custom['identity'] = $custom['interface'];
					$r[$linefeatures['id']] = array_merge($custom,$linefeatures);
					break;
				default:
					continue;
			}
		}

		if(empty($r) === true)
			return(false);
		else if((bool) $assoc === false)
			return(array_values($r));

		return($r);
	}

	public function set_add($arr,$protocolname)
	{
		return($this->_set('add',$arr,$protocolname));
	}

	public function set_edit($arr,$protocolname)
	{
		if(empty($this->_info) === true)
			return(false);

		return($this->_set('edit',$arr,$protocolname));
	}

	private function _set($action,$arr,$protocolname)
	{
		$this->_reset();

		if(($action !== 'add' && $action !== 'edit') === true
		|| dwho_issa('protocol',$arr) === false)
			return(false);

		$this->_protocolname = $protocolname;

		if (isset($arr['user_id']))
			$this->_set_result('user_id', $arr['user_id']);

		if ($action === 'add'
		&& dwho_has_len($arr['protocol'], 'secret') === false)
			$arr['protocol']['secret'] = $this->gen_password(6);

		$callerid = '';
		if(isset($arr['protocol']['callerid']) === true)
			$callerid =$arr['protocol']['callerid'];
		elseif(isset($arr['linefeatures']['callerid']) === true)
			$callerid = $arr['linefeatures']['callerid'];

		$arr['protocol']['callerid'] = $this->_service->mk_callerid($callerid, '', $protocolname);

		if (isset($arr['protocol']['allow']) === true
		&& dwho_issa('allow', $arr['protocol']) === false)
			$arr['protocol']['allow'] = explode(',', $arr['protocol']['allow']);
		elseif(dwho_issa('allow', $arr['protocol']) === false)
			$arr['protocol']['allow'] = null;

		$this->set_protocol($arr['protocol'],$protocolname);

		if($protocolname === XIVO_SRE_IPBX_AST_PROTO_CUSTOM)
			$name = $arr['protocol']['interface'];
		else
			$name = $arr['protocol']['name'];

		$arr['linefeatures']['protocol'] = $protocolname;
		$arr['linefeatures']['protocolid'] = 0;
		$arr['linefeatures']['name'] = $name;
		$arr['linefeatures']['context'] = (string) $arr['protocol']['context'];

		$this->set_linefeatures($action,$arr['linefeatures']);

		if($this->get_errnb() > 0)
			$this->_protocolname = $r = false;
		else
			$r = true;

		return($r);
	}

	function set_protocol($arr,$protocolname)
	{
		if(is_array($arr) === false
		|| ($protocol = &$this->_service->get_protocol_module($protocolname)) === false)
		{
			$this->_set_result('protocol',null);
			$this->_set_error('protocol','invalid module');
			return(false);
		}

		if(isset($arr['host-type']) === true)
			$arr['host'] = $arr['host-type'];
		else if(isset($arr['host']) === false)
			$arr['host'] = '';

		if(isset($arr['host-static']) === true
		&& $arr['host'] === 'static')
			$arr['host'] = $arr['host-static'];

		if (isset($this->_info['protocol']['id']) === true
		&& ($protocolret = $protocol->get($this->_info['protocol']['id'])) !== false
		&& $this->get_result('user_id') !== null)
		{
			if($arr['allow'] === null)
				unset($arr['allow']);
			$arr = array_merge($protocolret, $arr);

			if(empty($arr['allow']) === false
			&& is_array($arr['allow']) === false)
				$arr['allow'] = explode(',',$arr['allow']);
		}

		if(($rs = $protocol->chk_values($arr)) === false)
		{
			$this->_set_result('protocol',$protocol->get_filter_result());
			$this->_set_error('protocol',$protocol->get_filter_error());
			return(false);
		}

		if(dwho_issa('allow',$rs) === true)
			$rs['allow'] = implode(',',$rs['allow']);

		$this->_set_result('protocol',$rs);

		return(true);
	}

	function set_linefeatures($action,$arr)
	{
		if(($action !== 'add' && $action !== 'edit') === true
		|| ($action === 'edit' && dwho_issa('linefeatures',$this->_origin) === false) === true)
			return(false);
		else if(is_array($arr) === false)
		{
			$this->_set_result('linefeatures',null);
			$this->_set_error('linefeatures','invalid data');
			return(false);
		}
		elseif(($rs = $this->_linefeatures->chk_values($arr)) === false)
		{
			$this->_set_result('linefeatures',$this->_linefeatures->get_filter_result());
			$this->_set_error('linefeatures',$this->_linefeatures->get_filter_error());
			return(false);
		}

		$this->_set_result('linefeatures',$rs);

		return(true);
	}

	function add()
	{
		$r = true;

		$this->_status = 'add';
		$this->_return = array();
		$line_id = null;
		$user_id = (int) $this->get_result('user_id');

		if($this->get_errnb() > 0
		|| $this->_protocolname === false
		|| ($protocol_id = $this->add_protocol()) === false
		|| ($line_id = $this->add_linefeatures($protocol_id)) === false)
		{
			if($protocol_id !== null)
				$this->_reverse_add();
			$r = false;
		}

		if ($user_id !== 0)
			$this->update_protocol_setvar($this->_protocolname, $protocol_id, $user_id);

		$this->_status = '';

		return($line_id);
	}

	function add_protocol()
	{
		if(($rs = $this->get_result('protocol')) === null
		|| ($protocol = &$this->_service->get_protocol_module($this->_protocolname)) === false)
			return(false);

		return(($this->_return['protocol'] = $protocol->add($rs)));
	}

	function add_linefeatures($protocol_id)
	{
		if(($protocol_id = dwho_ulongint($protocol_id)) === 0
		|| ($rs = $this->get_result('linefeatures')) === null
		|| $rs === false)
			return(false);

		$rs['protocolid'] = (int) $protocol_id;

		$this->_return['linefeatures'] = $this->_linefeatures->add($rs);

		return($this->_return['linefeatures']);
	}

	function _reverse_add()
	{
		if($this->get_errnb() > 0
		|| $this->_protocolname === false
		|| $this->_status !== 'add'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'protocol':
					$protocol = &$this->_service->get_protocol_module($this->_protocolname);
					$protocol->delete($val);
					break;
				case 'linefeatures':
					$this->_linefeatures->delete($val);
					break;
			}
		}

		return(true);
	}

	function edit()
	{
		$this->_status = 'edit';
		$this->_return = array();
		$user_id = (int) $this->get_result('user_id');

		if(is_array($this->_info) === false
		|| empty($this->_info) === true
		|| $this->get_errnb() > 0
		|| $this->_protocolname === false
		|| ($protocol_id = $this->edit_protocol()) === false)
		{
			$this->_status = '';
			return(false);
		}
		elseif($this->edit_linefeatures($protocol_id) === false)
		{
			$this->_reverse_edit($protocol_id);
			$this->_status = '';
			return(false);
		}

		if ($user_id !== 0)
			$this->update_protocol_setvar($this->_protocolname, $protocol_id, $user_id);

		$this->_status = '';
		return($this->_info['linefeatures']['id']);
	}

	function edit_protocol()
	{
		if(dwho_issa('protocol',$this->_info) === false
		|| dwho_issa('linefeatures',$this->_info) === false
		|| ($protocol = &$this->_service->get_protocol_module($this->_protocolname)) === false)
			return(false);
		else if($this->_info['linefeatures']['protocol'] === $this->_protocolname)
		{
			if(($rs = $this->get_result('protocol')) === null)
				return(false);

			if(($ret = $protocol->edit($this->_info['protocol']['id'],$rs)) === false)
			{
				$this->_return['protocol'] = $ret;
				return(false);
			}

			return(($this->_return['protocol'] = $this->_info['protocol']['id']));
		}
		else if(($protocol_id = $this->add_protocol()) === false)
			return(false);
		else if($this->delete_protocol() === false)
		{
			$protocol->delete($protocol_id);
			return(false);
		}
		return(($this->_return['protocol'] = $protocol_id));
	}

	function edit_origin_protocol($protocol_id=0)
	{
		if(dwho_issa('protocol',$this->_origin) === false
		|| dwho_issa('linefeatures',$this->_info) === false
		|| ($protocol = &$this->_service->get_protocol_module($this->_info['linefeatures']['protocol'])) === false)
			return(false);
		else if($this->_info['linefeatures']['protocol'] === $this->_protocolname)
			return($protocol->edit_origin($this->_origin['protocol']));
		else if(($protocol_id = dwho_ulongint($protocol_id)) === 0
		|| ($proto = &$this->_service->get_protocol_module($this->_protocolname)) === false
		|| $proto->delete($protocol_id) === false)
			return(false);

		return($protocol->add_origin($this->_origin['protocol']));
	}

	function edit_linefeatures($protocol_id)
	{
		if(dwho_issa('linefeatures',$this->_info) === false
		|| dwho_issa('protocol',$this->_info) === false
		|| ($rs = $this->get_result('linefeatures')) === null
		|| $rs === false)
			return(false);

		if($this->_info['linefeatures']['protocol'] === $this->_protocolname)
		{
			$rs['protocolid'] = $this->_info['protocol']['id'];
			$provisioning = false;
		}
		else if(($protocol_id = dwho_ulongint($protocol_id)) !== 0)
		{
			$rs['protocolid'] = $protocol_id;
			$provisioning = true;
		}
		else
			return(false);

		if(empty($this->_info['linefeatures']['device']) === false)
		{
			if(($device = $this->_devicefeatures->get($this->_info['linefeatures']['device'])) === false)
			{
				dwho_report::push('error', 'device doesn\'t exist: '.$device['device']);
				return(false);
			}
			elseif($this->_provdconfig->rebuild_device_config($device['id']) === false)
			{
				dwho_report::push('error', 'Can\'t rebuild config device');
				return(false);
			}
		}

		return(($this->_return['linefeatures'] = $this->_linefeatures->edit(
											$this->_info['linefeatures']['id'],
											$rs,
											$provisioning)));
	}

	function _reverse_edit($protocol_id=0)
	{
		if($this->get_errnb() > 0
		|| $this->_protocolname === false
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
				case 'protocol':
					$this->edit_origin_protocol($protocol_id);
					break;
				case 'linefeatures':
					if(isset($this->_origin['linefeatures']) === false
					|| $this->_origin['linefeatures'] === false)
						$this->delete_linefeatures();
					else
						$this->_linefeatures->edit_origin($this->_origin['linefeatures']);
					break;
			}
		}

		return(true);
	}

	function delete()
	{
		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if(isset($this->_info['protocol']['mailbox'])
		&& empty($this->_info['protocol']['mailbox']) === false)
		{
			$this->_set_error('voicemail', dwho_i18n::babelfish('user_no_remove_lines_when_voicemail'));
			dwho_report::push('error', dwho_i18n::babelfish('user_no_remove_lines_when_voicemail'));
			return(false);
		}

		if($this->delete_protocol() === false
		|| $this->delete_linefeatures() === false
		|| $this->delete_exten('exten') === false)
		{
			$r = false;
			$this->_reverse_delete();
		}

		$this->_status = '';

		return($r);
	}

	function delete_protocol()
	{
		if(dwho_issa('protocol',$this->_info) === false
		|| dwho_issa('linefeatures',$this->_info) === false
		|| ($protocol = &$this->_service->get_protocol_module($this->_info['linefeatures']['protocol'])) === false)
			return(false);

		$r = $protocol->delete($this->_info['protocol']['id']);

		if($this->_status === 'delete')
			$this->_return['protocol'] = $r;

		return($r);
	}

	function delete_linefeatures()
	{
		if(dwho_issa('linefeatures',$this->_info) === false)
			return(false);

		$where = array('line_id' => $this->_info['linefeatures']['id']);
		$this->_user_line->delete_where($where);
		$r = $this->_linefeatures->delete($this->_info['linefeatures']['id']);

		if($this->_status === 'delete')
			$this->_return['linefeatures'] = $r;

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
				case 'protocol':
					if(isset($this->_origin['protocol']) === false
					|| $this->_origin['protocol'] === false)
						continue;

					$protocol = &$this->_service->get_protocol_module($this->_info['linefeatures']['protocol']);
					$protocol->add_origin($this->_origin['protocol']);
					break;
				case 'linefeatures':
					if(isset($this->_origin['linefeatures']) === true
					&& $this->_origin['linefeatures'] !== false)
						$this->_linefeatures->add_origin($this->_origin['linefeatures']);
					break;
			}
		}

		return(true);
	}

	function enable()
	{
		$r = true;

		$this->_status = 'enable';
		$this->_return = array();

		if(is_object($this->_protocol) === false)
			$r = false;
		else if($this->enable_protocol() === false
		|| $this->enable_linefeatures() === false)
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

		if(is_object($this->_protocol) === false)
			$r = false;
		else if($this->disable_protocol() === false
		|| $this->disable_linefeatures() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return(true);
	}

	function enable_protocol()
	{
		return($this->_enable_disable_protocol(false));
	}

	function disable_protocol()
	{
		return($this->_enable_disable_protocol(true));
	}

	function _enable_disable_protocol($disable=false)
	{
		if(dwho_issa('protocol',$this->_info) === false)
			return(false);
		else if((bool) $disable === false)
			$r = $this->_protocol->enable($this->_info['protocol']['id']);
		else
			$r = $this->_protocol->disable($this->_info['protocol']['id']);

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['protocol'] = $r;

		return($r);
	}

	function enable_linefeatures()
	{
		return($this->_enable_disable_linefeatures(false));
	}

	function disable_linefeatures()
	{
		return($this->_enable_disable_linefeatures(true));
	}

	function _enable_disable_linefeatures($disable=false)
	{
		if(dwho_issa('linefeatures',$this->_info) === false)
			return(false);
		else if((bool) $disable === false)
			$r = $this->_linefeatures->enable($this->_info['linefeatures']['id']);
		else
			$r = $this->_linefeatures->disable($this->_info['linefeatures']['id']);

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['linefeatures'] = $r;

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
				case 'protocol':
					$this->_enable_disable_protocol($disable);
					break;
				case 'linefeatures':
					$this->_enable_disable_linefeatures($disable);
					break;
			}
		}

		return(true);
	}

	function _prepare_import($arr)
	{
		if(dwho_issa('protocol',$arr) === false)
			return(false);

		return($arr);
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

	function add_from_json()
	{
		if(($data = $this->_get_data_from_json()) === false
		|| ($arr = $this->_prepare_import($data)) === false
		|| $this->set_add($arr,$arr['protocol']['protocol']) === false
		|| $this->add() === false)
			return(false);

		return(true);
	}

	function edit_from_json($mergewith=null)
	{
		if(($data = $this->_get_data_from_json()) === false
		|| ($arr = $this->_prepare_import($data)) === false)
			return false;

		if(!is_null($mergewith))
			$arr = $this->array_merge_recursive($mergewith,$arr);

		if($this->set_edit($arr,$arr['protocol']['protocol']) === false
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
}

?>
