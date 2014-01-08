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

xivo_object::required(array('abstract','abstract.inc'),true);

class xivo_object_commonconf extends xivo_object_abstract
{
	var $_name       = 'commonconf';
	var $_sysconfd	 = false;
	var $_dhcp       = false;
	var $_monitoring = false;
	var $_mail       = false;
	var $_netiface   = false;
	var $_resolvconf = false;
	var $_provisioning = false;

	public function __construct(&$xobj)
	{
		parent::__construct($xobj);

		if(($this->_sysconfd = &$this->_xobj->get_module('sysconfd')) === false
		|| ($this->_dhcp = &$this->_xobj->get_module('dhcp')) === false
		|| ($this->_monitoring = &$this->_xobj->get_module('monitoring')) === false
		|| ($this->_mail = &$this->_xobj->get_module('mail')) === false
		|| ($this->_provisioning = &$this->_xobj->get_module('provisioning')) === false
		|| ($this->_netiface = &$this->_xobj->get_module('netiface') === false))
			return(false);

		return(true);
	}

	function _ip2subnet($ip, $netmask)
	{
		$dotted_ip = split('\.', $ip);
		$dotted_nm = split('\.', $netmask);

		$subnet = sprintf("%d.%d.%d.%d",
				(((int)$dotted_ip[0]) & ((int)$dotted_nm[0])),
				(((int)$dotted_ip[1]) & ((int)$dotted_nm[1])),
				(((int)$dotted_ip[2]) & ((int)$dotted_nm[2])),
				(((int)$dotted_ip[3]) & ((int)$dotted_nm[3]))
		);

		return($subnet);
	}


	function generate($content, $onlystatic=true)
	{
		$conf = array();
		$rawdata = $this->_dhcp->get(1);
		$pool_start = strlen($rawdata['pool_start'])==0?'':$rawdata['pool_start'];

		$conf['dhcp_pool'] = $pool_start.' '.$rawdata['pool_end'];
		$conf['dhcp_extra_ifaces'] = $rawdata['extra_ifaces'];
		$conf['dhcp_active'] = (int) $rawdata['active'];

		$rawdata = $this->_mail->get(1);
		$mydomain = strlen($rawdata['mydomain'])== 0 ? null : $rawdata['mydomain'];
		$conf['smtp_mydomain'] = $mydomain;
		$conf['smtp_origin'] = $rawdata['origin'];
		$conf['smtp_relayhost'] = $rawdata['relayhost'];
		$conf['smtp_fallback_relayhost'] = $rawdata['fallback_relayhost'];
		$conf['smtp_canonical'] = $rawdata['canonical'];

		$rawdata = $this->_provisioning->get(1);
		$conf['provd_net4_ip'] = $rawdata['net4_ip'];
		$conf['provd_http_port'] = $rawdata['http_port'];
		$conf['provd_username'] = $rawdata['username'];
		$conf['provd_password'] = $rawdata['password'];
		$conf['provd_rest_port'] = $rawdata['rest_port'];
		$conf['provd_rest_net4_ip'] = $rawdata['net4_ip_rest'];
		$conf['provd_rest_authentication'] = (int) $rawdata['private'];
		$conf['provd_rest_ssl'] = (int) $rawdata['secure'];
		$conf['provd_dhcp_integration'] = (int) $rawdata['dhcp_integration'];

		$rawdata = $this->_monitoring->get(1);
		$maintenance = $rawdata['maintenance']==0?false:true;
		$alert_emails = strlen($rawdata['alert_emails']) == 0?null:str_replace("\r\n", " ", $rawdata['alert_emails']);
		$dahdi_monitor_ports = strlen($rawdata['dahdi_monitor_ports'])==0?null:$rawdata['dahdi_monitor_ports'];
		$max_call_duration = strlen($rawdata['max_call_duration'])==0?null:$rawdata['max_call_duration'];
		$conf['maintenance'] = $maintenance;
		$conf['alert_emails'] = $alert_emails;
		$conf['dahdi_monitor_ports'] = $dahdi_monitor_ports;
		$conf['max_call_duration'] = $max_call_duration;

		// we can't load application at start, or we'll have an infinite loop
		if(($this->_resolvconf  = &$this->_xobj->get_application('resolvconf')) == false)
		{
			$content = 'cannot load *resolvconf* application';
			return false;
		}

		$rawdata = $this->_resolvconf->get(1);
		$ns = '';
		if(strlen($rawdata['resolvconf']['nameserver1']) > 0)
			$ns .= $rawdata['resolvconf']['nameserver1'] . " ";
		if(strlen($rawdata['resolvconf']['nameserver2']) > 0)
			$ns .= $rawdata['resolvconf']['nameserver2'] . " ";
		if(strlen($rawdata['resolvconf']['nameserver3']) > 0)
			$ns .= $rawdata['resolvconf']['nameserver3'];
		$ns = trim($ns);

		$hostname = is_null($rawdata['resolvconf']['hostname'])?'':$rawdata['resolvconf']['hostname'];
		$resolvconf = is_null($rawdata['resolvconf']['domain'])?'':$rawdata['resolvconf']['domain'];
		$conf['hostname'] = $hostname;
		$conf['domain'] = $resolvconf;
		$conf['extra_dns_search'] = '';
		$conf['nameservers'] = $ns;

		if (($voip_iface = $this->_netiface->get_voip_iface()) == false)
		{
			$content = 'no VOIP interface configured';
			return false;
		}

		$conf['voip_ifaces'] = $voip_iface['ifname'];
		$conf['net4_ip'] = $voip_iface['address'];
		$conf['net4_netmask'] = $voip_iface['netmask'];
		$conf['net4_subnet'] = $this->_ip2subnet($voip_iface['address'], $voip_iface['netmask']);

		$_cf = dwho_gat::get('_CF');
		$conf['astdb'] = $_cf['datastorage']['general']['asterisk'];
		$conf['xivodb'] = $_cf['datastorage']['general']['xivo'];


		return $this->_sysconfd->request_post('/commonconf_generate', $conf);
	}

	function apply()
	{
		$ret = $this->_sysconfd->request_get('/commonconf_apply');

		if (dwho_file::is_f('/tmp/commonconf.reload'))
			unlink('/tmp/commonconf.reload');

		return $ret;
	}

	function last_status_code()
	{
		return $this->_sysconfd->last_status_code();
	}

	function notify_changes()
	{
		touch('/tmp/commonconf.reload');
		return(true);
	}
}

?>
