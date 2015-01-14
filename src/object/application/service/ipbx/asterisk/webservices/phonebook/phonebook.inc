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
dwho::load_class('dwho_json');

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk','webservices','abstract.inc'),true);

class xivo_application_service_asterisk_webservices_phonebook extends xivo_application_service_asterisk_webservices_abstract
{
	var $_dso		= null;
	var $_accessfeatures	= false;
	var $_appldapfilter	= null;
	var $_curl		= null;
	var $_uri		= null;

	public function __construct(&$service,&$dso)
	{
		if(is_object($dso) === false)
		{
			trigger_error('Invalid Datastorage object',E_USER_ERROR);
			return(false);
		}

		$this->_dso = &$dso;

		if(parent::_init($service) === false
		|| ($this->_accessfeatures = &$this->_service->get_module('accessfeatures')) === false)
			return(false);

		return(true);
	}

	function _load_config()
	{
		return(parent::_load_config(dirname(__FILE__)));
	}

	function _load_ldapfilter()
	{
		if(is_object($this->_appldapfilter) === false
		&& ($this->_appldapfilter = $this->_service->get_application('serverfeatures',
									     array('feature'	=> 'phonebook',
										   'type'	=> 'ldap'))) === false)
			return(false);

		return(true);
	}

	function chk_host_access($host)
	{
		if(($rs = $this->_accessfeatures->get_all_where(array('feature' => 'phonebook'),false)) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		for($i = 0;$i < $nb;$i++)
		{
			if(dwho_network::ipv4_in_lhost($host,$rs[$i]['host']) === true)
				return($rs[$i]);
		}

		return(false);
	}

	function get_vendor_from_useragent()
	{
		if(isset($_SERVER['HTTP_USER_AGENT']) === false
		|| is_array($this->_conf['useragent_to_vendor']) === false
		|| empty($this->_conf['useragent_to_vendor']) === true)
			return(false);

		$useragent = strtolower($_SERVER['HTTP_USER_AGENT']);

		foreach($this->_conf['useragent_to_vendor'] as $useragent_substring => $vendor)
		{
			if(strpos($useragent,$useragent_substring) !== false)
				return($vendor);
		}

		return(false);
	}

	function chk_vendor($vendor)
	{
		$vendor_str = strval($vendor);
		if (! in_array($vendor_str, $this->_conf['supported_vendors'])) {
			return false;
		} else {
			return $vendor_str;
		}
	}

	function get_phonebook_search($search)
	{
		$order = array();
		$order['displayname'] = SORT_ASC;

		if(($rs = $this->_dso->search($search, $order)) === false || ($nb = count($rs)) === 0)
			return(false);

		for($i = 0;$i < $nb;$i++)
			$rs[$i]['identity'] = $rs[$i]['name'].' ('.$rs[$i]['type'].')';

		return($rs);
	}

	function get_user_search($search,$disable=null)
	{
		$appuser = &$this->_service->get_application('user');

		$order = array();
		$order['firstname'] = SORT_ASC;
		$order['lastname'] = SORT_ASC;

		if(($rs = $appuser->get_users_search_number($search,$disable,$order)) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		$r = array();

		for($i = 0;$i < $nb;$i++)
		{
			if((dwho_issa('masterline', $rs[$i]) === true)
			&& (empty($rs[$i]['masterline']['number']) === false)) {
				$info['phone'] = $rs[$i]['masterline']['number'];

				if(($info['name'] = $rs[$i]['fullname']) === '')
					$info['name'] = $info['phone'];

				$info['type'] = 'local';
				$info['identity'] = $info['name'];

				$r[] = $info;
			}

		}

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}

	function get_phonebook_search_from_ldapfilter($query,$disable=null)
	{
		if(dwho_has_len($query) === false
		|| $this->_load_ldapfilter() === false
		|| ($rs = $this->_appldapfilter->get_servers_list($disable)) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		$r = array();

		for($i = 0;$i < $nb;$i++)
		{
			$ldapfilter = &$rs[$i]['server']['ldapfilter'];
			$ldapserver = &$rs[$i]['server']['ldapserver'];

			if($ldapserver === false
			|| isset($ldapfilter['attrphonenumber'][0]) === false)
				continue;

			$ldapfilter['attrdisplayname'] = dwho_array_change_value_case($ldapfilter['attrdisplayname'],CASE_LOWER);
			$ldapfilter['attrphonenumber'] = dwho_array_change_value_case($ldapfilter['attrphonenumber'],CASE_LOWER);

			if(dwho_has_len($ldapfilter['filter']) === true)
			{
				$ldapserver['user'] = $ldapfilter['user'];
				$ldapserver['passwd'] = $ldapfilter['passwd'];

				$dso = dwho_datastorage::factory('ldap',$ldapserver);

				if(($filter = dwho_replacef('Q',$dso->escape_string($query),$ldapfilter['filter'])) === false)
				{
					$dso->close();
					continue;
				}
				else if($filter{0} !== '(' || substr($filter,-1) !== ')')
					$filter = '('.$filter.')';

				$search = array();
				$search['from'] = $ldapfilter['basedn'];
				$search['column'] = array_merge($ldapfilter['attrdisplayname'],$ldapfilter['attrphonenumber']);
				$search['where'] = '(&'.$filter.'(|('.implode('=*)(',$ldapfilter['attrdisplayname']).'=*))'.
						   '(|('.implode('=*)(',$ldapfilter['attrphonenumber']).'=*)))';

				if(($res = $dso->select_all($search,true,true)) !== false
				&& ($recv = $this->_prepare_ldapfilter_recv($res,
									    $ldapfilter['attrdisplayname'],
									    $ldapfilter['attrphonenumber'],
									    $ldapfilter['additionaltype'],
									    $ldapfilter['additionaltext'])) !== false)
					$r = array_merge($r,$recv);

				$dso->close();
			}
			else
			{
				$columns = array_merge($ldapfilter['attrdisplayname'],$ldapfilter['attrphonenumber']);

				$ldapserver['user'] = $ldapfilter['user'];
				$ldapserver['passwd'] = $ldapfilter['passwd'];

				$dso = dwho_datastorage::factory('ldap',$ldapserver);

				$dso->new_select($ldapfilter['basedn'],$columns);

				$cnt = count($columns);

				for($j = 0;$j < $cnt;$j++)
					$dso->orwherecontain($columns[$j],$query);

				$dso->aggregate_where();

				$cnt = count($ldapfilter['attrdisplayname']);

				$dso->andwherenotnull($ldapfilter['attrdisplayname'][0]);

				for($j = 1;$j < $cnt;$j++)
					$dso->orwherenotnull($ldapfilter['attrdisplayname'][$j]);

				$dso->aggregate_where();

				$cnt = count($ldapfilter['attrphonenumber']);

				$dso->andwherenotnull($ldapfilter['attrphonenumber'][0]);

				for($j = 1;$j < $cnt;$j++)
					$dso->orwherenotnull($ldapfilter['attrphonenumber'][$j]);

				if(($res = $dso->select_all(null,true,true)) !== false
				&& ($recv = $this->_prepare_ldapfilter_recv($res,
									    $ldapfilter['attrdisplayname'],
									    $ldapfilter['attrphonenumber'],
									    $ldapfilter['additionaltype'],
									    $ldapfilter['additionaltext'])) !== false)
					$r = array_merge($r,$recv);

				$dso->close();
			}
		}

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}

	function _prepare_ldapfilter_recv($recv,$attrdisplayname,$attrphonenumber,$additionaltype,$additionaltext)
	{
		if(is_array($recv) === false
		|| is_array($attrdisplayname) === false
		|| is_array($attrphonenumber) === false
		|| ($nb = count($recv)) === 0
		|| ($namecnt = count($attrdisplayname)) === 0
		|| ($phonecnt = count($attrphonenumber)) === 0)
			return(false);

		$r = $info = array();

		if($additionaltype !== 'custom')
			$info['type'] = $additionaltype;
		else
			$info['type'] = $additionaltext;

		$info['additionaltype'] = $additionaltype;
		$info['additionaltext'] = $additionaltext;

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$recv[$i];

			for($j = 0;$j < $namecnt;$j++)
			{
				if(isset($ref[$attrdisplayname[$j]]) === true)
				{
					$info['name'] = $ref[$attrdisplayname[$j]][0];
					break;
				}
			}

			for($j = 0;$j < $phonecnt;$j++)
			{
				if(isset($ref[$attrphonenumber[$j]]) === true)
				{
					$info['phone'] = $ref[$attrphonenumber[$j]][0];
					break;
				}
			}

			if(isset($info['name'],$info['phone']) === false
			|| preg_match_all('/[\+\(\)\d]+/',$info['phone'],$match) === 0)
				continue;

			// Remove all chars not in +()0-9
			$info['phone'] = implode('',$match[0]);

			// Convert numbers +33(0)123456789 to 0033123456789
			if(preg_match('/^\+(\d+)\(\d+\)(\d+)$/',$info['phone'],$match) === 1)
				$info['phone'] = '00'.$match[1].$match[2];

			if(preg_match_all('/^[0-9\*#+]+$/',$info['phone'],$match) === 0)
				continue;

			// Remove all chars not in +*#0-9
			$info['phone'] = implode('',$match[0]);

			$info['identity'] = $info['name'];

			if($info['type'] !== '')
				$info['identity'] .= ' ('.$info['type'].')';

			$r[] = $info;
		}

		if(isset($r[0]) === false)
			$r = false;

		return($r);
	}
}

?>
