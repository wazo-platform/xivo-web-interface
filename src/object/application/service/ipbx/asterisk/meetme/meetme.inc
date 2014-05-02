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

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);

class xivo_application_service_asterisk_meetme extends xivo_application_asterisk_abstract
{
	var $_dso		    = null;
	var $_appmeetme		= false;
	var $_meetmefeatures	= false;
	var $_meetmeroom	= false;
	var $_meetmeguest	= false;
	var $_userfeatures	= false;
	var $_extensions	= false;
	var $_contextmember	= false;
	var $_musiconhold	= array();
	var $_contextlist	= array();

	public function __construct(&$service,&$dso,$param=null,$components=true)
	{
		parent::__construct($service);

		if(is_object($dso) === false)
			trigger_error('Invalid Datastorage object',E_USER_ERROR);

		$this->_dso = &$dso;

		if(($this->_meetmefeatures = &$this->_service->get_module('meetmefeatures')) === false
		|| ($this->_appmeetme = &$this->_service->get_apprealstatic('meetme')) === false
		|| ($this->_meetmeroom = &$this->_appmeetme->get_module('room')) === false)
			return(false);
		else if((bool) $components === true)
			$this->load_components();

		return(true);
	}

	function load_components()
	{
		$this->_components = true;

		if(($this->_meetmeguest = &$this->_service->get_module('meetmeguest')) === false
		|| ($this->_userfeatures = &$this->_service->get_module('userfeatures')) === false
		|| ($this->_extensions = &$this->_service->get_module('extensions')) === false
		|| ($this->_contextmember = $this->get_abstract('contextmember',array('type' => 'meetme'))) === false
		|| ($_MGR = &dwho_gct::get('xivo_manager')) === false
		|| ($this->_func_key_manager = &$_MGR->get_ressource('func_key')) === false)
			return(false);

		return(true);
	}

	function get($id,$disable=null,$nocomponents=null)
	{
		$this->_info = $this->_origin = array();

		if(($this->_info['meetmefeatures'] = $this->_meetmefeatures->get($id)) === false
		|| ($this->_info['meetmeroom'] = $this->_meetmeroom->get_meetmeroom(
						$this->_info['meetmefeatures']['meetmeid'],
						$disable)) === false)
			return(false);

		$this->_origin['meetmefeatures'] = $this->_meetmefeatures->get_origin();
		$this->_origin['meetmeroom'] = $this->_meetmeroom->get_origin();

		if($this->_components === false)
			return($this->_info);

		if(dwho_ak('meetmeguest',$nocomponents,true) === false)
			$this->_get_meetmeguest();

		if(dwho_ak('meetmeadmininternal',$nocomponents,true) === false)
			$this->_info['meetmeadmininternal'] = $this->_get_meetmeadmininternal(
									$this->_info['meetmefeatures']['admin_internalid'],
									$this->_info['meetmefeatures']['context']);

		if(dwho_ak('exten',$nocomponents,true) === false)
			$this->_get_exten();

		if(dwho_ak('contextmember',$nocomponents,true) === false)
			$this->_get_contextmember($this->_info['meetmefeatures']['id']);

		return($this->_info);
	}

	function get_nb($arr=null,$disable=null)
	{
		return($this->_meetmeroom->get_nb($arr,$disable));
	}

	function _get_meetmeguest()
	{
		$this->_info['meetmeguest'] = false;
		$this->_origin['meetmeguest'] = false;

		$where = array();
		$where['meetmefeaturesid'] = $this->_info['meetmefeatures']['id'];

		$this->_info['meetmeguest'] = $this->_meetmeguest->get_all_where($where);
		$this->_origin['meetmeguest'] = $this->_meetmeguest->get_origin_list();

		return($this->_info['meetmeguest']);
	}

	function _get_meetmeadmininternal($internalid,$context)
	{
		if(is_object($this->_userfeatures) === false)
			return(false);

		return($this->_userfeatures->get($internalid));
	}

	function _get_exten()
	{
		$this->_info['exten'] = false;
		$this->_origin['exten'] = false;

		if(($this->_info['exten'] = $this->_extensions->get_exten($this->_info['meetmefeatures']['confno'],
										$this->_info['meetmefeatures']['context'])) !== false)
			$this->_origin['exten'] = $this->_extensions->get_origin();

		return($this->_info['exten']);
	}

	function get_elements()
	{
		$r = array();

		$r['meetmeroom'] = $this->_meetmeroom->get_element();
		$r['meetmefeatures'] = $this->_meetmefeatures->get_element();

		if(is_object($this->_meetmeguest) === true)
			$r['meetmeguest'] = $this->_meetmeguest->get_element();

		return($r);
	}

	function get_meetme_search($search,$disable=null,$order=null,$limit=null,$assoc=false)
	{
		return($this->_get_meetme('search',$search,$disable,$order,$limit,$assoc));
	}

	function get_meetme_list($disable=null,$order=null,$limit=null,$assoc=false)
	{
		return($this->_get_meetme('list',null,$disable,$order,$limit,$assoc));
	}

	function _get_meetme($action,$search='',$disable=null,$order=null,$limit=null,$assoc=false)
	{
		$search = strval($search);

		switch($action)
		{
			case 'search':
				$rs = $this->_dso->search($search,$disable,$order,$limit);
				break;
			case 'list':
			default:
				$rs = $this->_dso->get_all($disable,$order,$limit);
		}

		$this->_cnt = $this->_dso->get_cnt();

		if($rs === false || ($nb = count($rs)) === 0)
			return(false);

		$r = $identity = array();

		for($i = 0;$i < $nb;$i++)
		{
			$r[$rs[$i]['id']] = $rs[$i];

			$ref = &$r[$rs[$i]['id']];

			$identity['name'] = $ref['name'];
			$identity['confno'] = $ref['confno'];
			$identity['context'] = $ref['context'];

			$ref['identity'] = $this->_meetmefeatures->mk_identity($identity);
			$ref = xivo_service_asterisk_realstatic_staticmeetme::parse_room($ref);
			$ref['commented'] = (bool) $ref['commented'];

			unset($ref['var_val']);
		}

		if(empty($r) === true)
			return(false);
		else if((bool) $assoc === false)
			return(array_values($r));

		return($r);
	}

	function set_add($arr)
	{
		return($this->_set('add',$arr));
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
		|| dwho_issa('meetmeroom',$arr) === false
		|| dwho_issa('meetmefeatures',$arr) === false)
			return(false);

		$this->set_meetmeroom($arr['meetmeroom']);

		$confno = strtolower($this->get_result_var('meetmeroom','confno'));

		$arr['meetmefeatures']['meetmeid'] = 0;
		//$arr['meetmefeatures']['name'] = $confno;

		$this->set_meetmefeatures($arr['meetmefeatures']);

		if(dwho_issa('meetmeguest',$arr) === true)
			$this->set_meetmeguest($arr['meetmeguest']);

		$number = (string) $this->get_result_var('meetmefeatures','confno');
		$context = (string) $this->get_result_var('meetmefeatures','context');

		if($this->_errnb === 0
		&& $number !== ''
		&&($rsexten = $this->chk_exten('exten',
												$number,
												$context,
												'meetme',
												0)) !== false)
			$this->_set_result('exten',$rsexten);

		$this->set_contextmember();

		return(($this->get_errnb() < 1));
	}

	function set_meetmefeatures($arr)
	{
		if(is_array($arr) === false)
		{
			$this->_set_result('meetmefeatures',null);
			$this->_set_error('meetmefeatures','invalid data');
			return(false);
		}

		$this->get_musiconhold();

		if(isset($arr['admin_musiconhold'],
			 $this->_musiconhold[$arr['admin_musiconhold']]) === false)
			$arr['admin_musiconhold'] = '';

		if(isset($arr['user_musiconhold'],
			 $this->_musiconhold[$arr['user_musiconhold']]) === false)
			$arr['user_musiconhold'] = '';


		if(($rs = $this->_meetmefeatures->chk_values($arr)) === false)
		{
			$this->_set_result('meetmefeatures',$this->_meetmefeatures->get_filter_result());
			$this->_set_error('meetmefeatures',$this->_meetmefeatures->get_filter_error());
			return(false);
		}
		else if(($typefrom = $this->_meetmefeatures->normalize_admin_typefrom(
								$rs['admin_typefrom'],
								$rs['admin_internalid'],
								$rs['admin_externalid'])) === false)
		{
			$this->_set_result('meetmefeatures',$rs);
			$this->_set_error('meetmefeatures','invalid admin_typefrom');
			return(false);
		}
		else
			$rs = array_merge($rs,$typefrom);

		$admin_calleridnum = $rs['admin_externalid'];
		$meetmeadminternal = null;

		if($rs['admin_typefrom'] === 'undefined')
			$rs['admin_identification'] = 'pin';
		else if($rs['admin_typefrom'] === 'internal')
		{
			if(is_object($this->_userfeatures) === false)
			{
				$this->_set_result('meetmefeatures',$rs);
				$this->_set_error('meetmefeatures','userfeatures module not loaded');
				return(false);
			}
			else if(($meetmeadminternal = $this->_get_meetmeadmininternal(
						$rs['admin_internalid'],
						$rs['context'])) === false)
			{
				$this->_set_result('meetmefeatures',$rs);
				$this->_set_result('meetmeadmininternal',$meetmeadminternal);
				$this->_set_error('meetmefeatures','invalid administrator');
				return(false);
			}
			else
				$admin_calleridnum = $meetmeadminternal['number'];
		}

		if($this->_meetmefeatures->valid_admin_identification(
							$rs['admin_typefrom'],
							$rs['admin_identification'],
							$admin_calleridnum,
							$this->get_result_var('meetmeroom','pinadmin')) === false)
		{
			$this->_set_result('meetmefeatures',$rs);
			$this->_set_result('meetmeadmininternal',$meetmeadminternal);
			$this->_set_error('meetmefeatures','invalid admin_identification');
			return(false);
		}

		$this->_set_result('meetmeadmininternal',$meetmeadminternal);
		$this->_set_result('meetmefeatures',$rs);

		return(true);
	}

	function set_meetmeroom($arr)
	{
		if(is_array($arr) === false)
		{
			$this->_set_result('meetmeroom',null);
			$this->_set_error('meetmeroom','invalid data');
			return(false);
		}
		else if(($r = $this->_meetmeroom->set_meetmeroom($arr)) === false) {
			$meetmeroom_error = $this->_meetmeroom->get_error('meetmeroom');
			$this->_set_error('meetmeroom',$meetmeroom_error);
			if(isset($meetmeroom_error['confno']) === true)
				$this->_set_error('meetmefeatures',array('confno' => $meetmeroom_error['confno']));
		}

		$this->_set_result('meetmeroom',$this->_meetmeroom->get_result('meetmeroom'));

		return($r);
	}

	function set_meetmeguest($arr)
	{
		if(is_array($arr) === false)
		{
			$this->_set_result('meetmeguest',null);
			$this->_set_error('meetmeguest','invalid data');
			return(false);
		}

		$list = array_values($arr);

		if(($nb = count($list)) === 0)
			return(null);

		$reslist = $errlist = array();

		for($i = $j = 0;$i < $nb;$i++)
		{
			$ref = &$list[$i];
			$ref['meetmefeaturesid'] = 0;

			if(($rs = $this->_meetmeguest->chk_values($ref)) !== false)
				$reslist[$j++] = $rs;
			else
			{
				$errlist[$j] = $this->_meetmeguest->get_filter_error();
				$reslist[$j++] = $this->_meetmeguest->get_filter_result();
			}

			if(isset($ref['sendemail']) === true)
				$reslist[$j - 1]['sendemail'] = dwho_bool($ref['sendemail']);
			else
				$reslist[$j - 1]['sendemail'] = false;
		}

		$this->_set_result('meetmeguest',$reslist);

		if(empty($errlist) === false)
		{
			$this->_set_error('meetmeguest',$errlist);
			return(false);
		}

		return(true);
	}

	function _set_exten($action,$meetmefeaturesid)
	{
		$rs = false;

		if(($meetmefeaturesid = dwho_ulongint($meetmefeaturesid)) === 0
		|| ($rs = $this->get_result('exten')) === null
		|| $rs === false)
			return($rs);
		else if(($rsexten = $this->set_exten($action,
											'exten',
											$rs['exten'],
											$rs['context'],
											'meetme',
											$meetmefeaturesid)) === false)
			return(false);

		$this->_set_result('exten',$rsexten);

		return(true);
	}

	function set_contextmember()
	{
		$rs = array();
		$rs['meetmefeatures-admin_exitcontext']['context'] = (string) $this->get_result_var('meetmefeatures','admin_exitcontext');
		$rs['meetmefeatures-admin_exitcontext']['typeval'] = 0;
		$rs['meetmefeatures-admin_exitcontext']['varname'] = 'admin_exitcontext';

		$rs['meetmefeatures-user_exitcontext']['context'] = (string) $this->get_result_var('meetmefeatures','user_exitcontext');
		$rs['meetmefeatures-user_exitcontext']['typeval'] = 0;
		$rs['meetmefeatures-user_exitcontext']['varname'] = 'user_exitcontext';

		return(parent::set_contextmember($rs));
	}

	function add($sendguestmail=true)
	{
		$r = true;

		$this->_status = 'add';
		$this->_return = array();
		$meetmeroomid = null;

		if($this->get_errnb() > 0
		|| ($meetmeroomid = $this->add_meetmeroom()) === false
		|| ($meetmefeaturesid = $this->add_meetmefeatures($meetmeroomid)) === false
		|| $this->add_meetmeguest($meetmefeaturesid) === false
		|| $this->add_exten($meetmefeaturesid) === false
		|| $this->add_contextmember($meetmefeaturesid) === false
		|| $this->_add_func_key_dest($meetmefeaturesid) === false)
		{
			$r = false;
			if($meetmeroomid !== null)
				$this->_reverse_add();
		}

		$this->_status = '';

		if($r === true && (bool) $sendguestmail === true)
			$this->_meetmeguest_sendemail();

		return($r);
	}

	function add_meetmeroom()
	{
		if(($rs = $this->_meetmeroom->add_meetmeroom()) === null
		|| $rs === false)
			return(false);

		return(($this->_return['meetmeroom'] = $this->_meetmeroom->get_return()));
	}

	function add_meetmefeatures($meetmeid)
	{
		if(($meetmeid = dwho_ulongint($meetmeid)) === 0
		|| ($rs = $this->get_result('meetmefeatures')) === null
		|| $rs === false)
			return(false);

		$rs['meetmeid'] = $meetmeid;

		return(($this->_return['meetmefeatures'] = $this->_meetmefeatures->add($rs)));
	}

	function add_meetmeguest($meetmefeaturesid)
	{
		$rs = false;

		if(($meetmefeaturesid = dwho_ulongint($meetmefeaturesid)) === 0
		|| ($rs = $this->get_result('meetmeguest')) === null)
			return($rs);
		else if(is_array($rs) === false || empty($rs) === true)
			return(false);

		$this->_return['meetmeguest'] = array();

		foreach($rs as $key => $val)
		{
			$val['meetmefeaturesid'] = $meetmefeaturesid;
			unset($val['sendemail']);

			if(($this->_return['meetmeguest'][$key] = $this->_meetmeguest->add($val)) === false)
				return(false);
		}

		if(empty($this->_return['meetmeguest']) === true)
			$this->_return['meetmeguest'] = false;

		return(true);
	}

	function add_exten($meetmefeaturesid)
	{
		if(($rs = $this->_set_exten('add',$meetmefeaturesid)) !== true)
			return($rs);

		return(parent::add_exten('exten'));
	}

	private function _add_func_key_dest($conference_id)
	{
		if (! $this->_func_key_manager->add_dst_for_conference($conference_id))
			return(false);

		return(true);
	}

	function _reverse_add()
	{
		if($this->get_errnb() > 0
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
				case 'meetmeroom':
					$this->_meetmeroom->delete_meetmeroom($this->_status,$val);
					break;
				case 'meetmefeatures':
					$this->_meetmefeatures->delete($val);
					break;
				case 'meetmeguest':
					if(is_array($val) === false)
						continue;

					foreach($val as $meetmeguestid)
					{
						if($meetmeguestid !== false)
							$this->_meetmeguest->delete($meetmeguestid);
					}
					break;
				case 'exten':
					$this->_extensions->delete_exten($val);
					break;
				case 'contextmember':
					$this->_contextmember->reverse_add();
					break;
			}
		}

		return(true);
	}

	function edit($sendguestmail=true)
	{
		$r = true;

		$this->_status = 'edit';
		$this->_return = array();

		if($this->get_errnb() > 0
		|| $this->edit_meetmeroom() === false
		|| $this->edit_meetmefeatures() === false)
		{
			$this->_reverse_edit();
			$this->_status = '';
			return(false);
		}
		else if($this->_info['meetmefeatures']['confno'] !== '')
		{
			if($this->get_result('exten') === null)
			{
				$this->delete_exten('exten');
			}
			else if($this->edit_exten($this->_info['meetmefeatures']['id']) === false)
			{
				$this->_reverse_edit();
				$this->_status = '';
				return(false);
			}
		}
		else if($this->add_exten($this->_info['meetmefeatures']['id']) === false)
		{
			$this->_reverse_edit();
			$this->_status = '';
			return(false);
		}

		if($this->edit_meetmeguest() === false
		|| $this->edit_contextmember() === false)
		{
			$r = false;
			$this->_reverse_edit();
		}

		$this->_status = '';

		if($r === true && (bool) $sendguestmail === true)
			$this->_meetmeguest_sendemail();

		return($r);
	}

	function edit_meetmeroom()
	{
		if(dwho_issa('meetmeroom',$this->_info) === false
		|| ($rs = $this->_meetmeroom->edit_meetmeroom($this->_info['meetmeroom']['id'])) === null
		|| $rs === false)
			return(false);

		return(($this->_return['meetmeroom'] = $this->_meetmeroom->get_return()));
	}

	function edit_meetmefeatures()
	{
		if(dwho_issa('meetmefeatures',$this->_info) === false
		|| ($rs = $this->get_result('meetmefeatures')) === null
		|| $rs === false)
			return(false);

		$rs['meetmeid'] = $this->_info['meetmefeatures']['meetmeid'];

		return(($this->_return['meetmefeatures'] = $this->_meetmefeatures->edit($this->_info['meetmefeatures']['id'],$rs)));
	}

	function edit_meetmeguest()
	{
		if(dwho_issa('meetmefeatures',$this->_info) === false)
			return(false);

		$meetmefeaturesid = $this->_info['meetmefeatures']['id'];

		if(dwho_issa('meetmeguest',$this->_info) === false)
			return($this->add_meetmeguest($meetmefeaturesid));

		$this->_return['meetmeguest'] = $this->delete_meetmeguest();

		if(($rs = $this->get_result('meetmeguest')) === null)
			return($this->_return['meetmeguest']);
		else if(is_array($rs) === false || empty($rs) === true)
			return(false);

		$this->_return['meetmeguest'] = array();

		$list = array_keys($this->_info['meetmeguest']);

		$i = 0;

		foreach($rs as $key => $val)
		{
			if(isset($list[$i]) === true)
				$meetmeguestid = $this->_info['meetmeguest'][$list[$i++]]['id'];
			else
				$meetmeguestid = 0;

			$val['meetmefeaturesid'] = $meetmefeaturesid;
			unset($val['sendemail']);

			if(($this->_return['meetmeguest'][$key] = $this->_meetmeguest->add($val,$meetmeguestid)) === false)
				return(false);
		}

		if(empty($this->_return['meetmeguest']) === true)
			$this->_return['meetmeguest'] = false;

		return(true);
	}

	function edit_exten($meetmefeaturesid)
	{
		if(($rs = $this->_set_exten('edit',$meetmefeaturesid)) !== true)
			return($rs);

		return(parent::edit_exten('exten'));
	}

	function edit_contextmember()
	{
		if(dwho_issa('meetmefeatures',$this->_info) === false)
			return(false);

		return(parent::edit_contextmember($this->_info['meetmefeatures']['id']));
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
				case 'meetmeroom':
					if(isset($this->_origin['meetmeroom']) === false
					|| $this->_origin['meetmeroom'] === false)
						$this->delete_meetmeroom();
					else
						$this->_meetmeroom->edit_origin($this->_origin['meetmeroom']);
					break;
				case 'meetmefeatures':
					if(isset($this->_origin['meetmefeatures']) === false
					|| $this->_origin['meetmefeatures'] === false)
						$this->delete_meetmefeatures();
					else
						$this->_meetmefeatures->edit_origin($this->_origin['meetmefeatures']);
					break;
				case 'meetmeguest':
					$this->delete_meetmeguest();

					if(isset($this->_origin['meetmeguest']) === true
					&& $this->_origin['meetmeguest'] !== false)
						$this->_meetmeguest->add_origin_list($this->_origin['meetmeguest']);
					break;
				case 'exten':
					if(isset($this->_origin['exten']) === false
					|| $this->_origin['exten'] === false)
						$this->delete_exten('exten');
					else
						$this->_extensions->edit_origin($this->_origin['exten']);
					break;
				case 'contextmember':
					$this->_contextmember->reverse_edit($this->_status);
					break;
			}
		}

		return(true);
	}

	function _meetmeguest_sendemail()
	{
		if(($rs = $this->get_result('meetmeguest')) === false
		|| is_array($rs) === false
		|| empty($rs) === true)
			return(null);

		$variables = array();
		$variables['CONFNAME']		= (string) $this->get_result_var('meetmefeatures','name');
		$variables['CONFPIN']		= (string) $this->get_result_var('meetmeroom','pin');
		$variables['CONFPINADMIN']	= (string) $this->get_result_var('meetmeroom','pinadmin');
		$variables['CONFNUMBER']	= (string) $this->get_result_var('meetmefeatures','confno');

		$emailsubject	= (string) $this->get_result_var('meetmefeatures','emailsubject');
		$emailbody	= $this->get_result_var('meetmefeatures','emailbody');
		$emailfrom	= $this->get_result_var('meetmefeatures','emailfrom');
		$emailfromname	= $this->get_result_var('meetmefeatures','emailfromname');

		if(dwho_has_len($emailfromname) === false)
			$emailfromname = $emailfrom;

		$headers = array();
		$headers[] = 'From: '.mb_encode_mimeheader(
					'"'.$emailfromname.'" <'.$emailfrom.'>',
					'UTF-8');
		$headers[] = 'MIME-Version: 1.0';
		$headers[] = 'Content-Type: text/plain; charset=UTF-8';
		$headers[] = 'Content-Transfer-Encoding: 8bit';
		$headers[] = 'X-Mailer: XIVO';
		$headers[] = 'X-XIVO-Info: "Meetme Invitation"';
		$headers = implode("\r\n",$headers);

		foreach($rs as $value)
		{
			if(isset($value['sendemail']) === true
			&& (bool) $value['sendemail'] === true
			&& dwho_has_len($value['email']) === true)
			{
				$variables['FULLNAME'] = (string) $value['fullname'];
				mail($value['email'],
				     mb_encode_mimeheader(dwho_vksprintf($emailsubject,$variables),'UTF-8'),
				     dwho_vksprintf($emailbody,$variables),
				     $headers);
			}
		}

		return(true);
	}

	function delete()
	{
		$r = true;

		$this->_status = 'delete';
		$this->_return = array();

		if($this->_delete_func_key_dst() === false
		|| $this->delete_meetmeroom() === false
		|| $this->delete_meetmefeatures() === false
		|| $this->delete_meetmeguest() === false
		|| $this->delete_exten('exten') === false
		|| $this->delete_contextmember() === false)
		{
			$r = false;
			$this->_reverse_delete();
		}
		else
			$this->unlinks();

		$this->_status = '';

		return($r);
	}

	private function _delete_func_key_dst()
	{
		if(dwho_issa('meetmefeatures',$this->_info) === false)
			return(false);

		$this->_func_key_manager->delete_dst_for_conference($this->_info['meetmefeatures']['id']);

		return(true);
	}

	function delete_meetmeroom()
	{
		if(dwho_issa('meetmeroom',$this->_info) === false
		|| ($r = $this->_meetmeroom->delete_meetmeroom($this->_status,
							       $this->_info['meetmeroom']['id'])) === null
		|| $r === false)
			return(false);

		if($this->_status === 'delete')
			$this->_return['meetmeroom'] = $r;

		return($r);
	}

	function delete_meetmefeatures()
	{
		if(dwho_issa('meetmefeatures',$this->_info) === false)
			return(false);

		$r = $this->_meetmefeatures->delete($this->_info['meetmefeatures']['id']);

		if($this->_status === 'delete')
			$this->_return['meetmefeatures'] = $r;

		return($r);
	}

	function delete_meetmeguest()
	{
		if(dwho_issa('meetmeguest',$this->_info) === false)
			return(null);
		else if(dwho_issa('meetmefeatures',$this->_info) === false)
			return(false);

		$where = array();
		$where['meetmefeaturesid'] = $this->_info['meetmefeatures']['id'];

		$r = $this->_meetmeguest->delete_where($where);

		if($this->_status === 'delete')
			$this->_return['meetmeguest'] = $r;

		return($r);
	}

	function _reverse_delete()
	{
		if($this->_status !== 'delete'
		|| is_array($this->_return) === false
		|| empty($this->_return) === true)
			return(false);

		foreach($this->_return as $key => $val)
		{
			if($val === false)
				continue;

			switch($key)
			{
				case 'meetmeroom':
					if(isset($this->_origin['meetmeroom']) === true
					&& $this->_origin['meetmeroom'] !== false)
						$this->_meetmeroom->add_origin($this->_origin['meetmeroom']);
					break;
				case 'meetmefeatures':
					if(isset($this->_origin['meetmefeatures']) === true
					&& $this->_origin['meetmefeatures'] !== false)
						$this->_meetmefeatures->add_origin($this->_origin['meetmefeatures']);
					break;
				case 'meetmeguest':
					if(isset($this->_origin['meetmeguest']) === true
					&& $this->_origin['meetmeguest'] !== false)
						$this->_meetmeguest->add_origin_list($this->_origin['meetmeguest']);
					break;
				case 'exten':
					if(isset($this->_origin['exten']) === true
					&& $this->_origin['exten'] !== false)
						$this->_extensions->add_origin($this->_origin['exten']);
					break;
				case 'contextmember':
					$this->_contextmember->add_origin_list();
					break;
			}
		}

		return(true);
	}

	function unlinks()
	{
		if(dwho_issa('meetmefeatures',$this->_info) === false)
			return(false);
		else if(($dialaction = &$this->_service->get_module('dialaction')) !== false)
		{
			$where = array();
			$where['type'] = 'meetme';
			$where['typeval'] = $this->_info['meetmefeatures']['id'];

			$dialaction->unlinked_where($where);
		}

		return(true);
	}

	function enable()
	{
		$r = true;

		$this->_status = 'enable';
		$this->_return = array();

		if(is_object($this->_meetmeroom) === false)
			$r = false;
		else if($this->enable_meetmeroom() === false)
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

		if(is_object($this->_meetmeroom) === false)
			$r = false;
		else if($this->disable_meetmeroom() === false)
		{
			$r = false;
			$this->_reverse_enable_disable();
		}

		$this->_status = '';

		return(true);
	}

	function enable_meetmeroom()
	{
		return($this->_enable_disable_meetmeroom(false));
	}

	function disable_meetmeroom()
	{
		return($this->_enable_disable_meetmeroom(true));
	}

	function _enable_disable_meetmeroom($disable=false)
	{
		if(dwho_issa('meetmeroom',$this->_info) === false)
			return(false);
		else if((bool) $disable === false)
			$r = $this->_meetmeroom->enable($this->_info['meetmeroom']['id']);
		else
			$r = $this->_meetmeroom->disable($this->_info['meetmeroom']['id']);

		if($this->_status === 'enable' || $this->_status === 'disable')
			$this->_return['meetmeroom'] = $r;

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
				case 'meetmeroom':
					$this->_enable_disable_meetmeroom($disable);
					break;
			}
		}

		return(true);
	}

	function add_from_json()
	{
		if(($data = $this->_get_data_from_json()) === false
		|| $this->set_add($data) === false
		|| $this->add() === false)
			return(false);

		return(true);
	}

	function edit_from_json()
	{
		if(($data = $this->_get_data_from_json()) === false
		|| $this->set_edit($data) === false
		|| $this->edit() === false)
			return(false);

		return(true);
	}
}

?>
