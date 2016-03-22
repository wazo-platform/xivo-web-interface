<?php

#
# XiVO Web-Interface
# Copyright (C) 2016 Avencall
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

class LineManager
{
	private $_line = null;
	private $_linefeatures = null;
	private $_userapp = null;
	private $line = null;
	private $sip = null;
	private $sccp = null;
	private $extension = null;
	private $user = null;
	private $mapper = null;
	private $context_dso = null;

	function __construct($lineapp, $linefeatures, $userapp, $line_resource, $user_resource, $sip_resource, $sccp_resource, $extension_resource, $line_mapper, $context_dso) {
		$this->_line = $lineapp;
		$this->_linefeatures = $linefeatures;
		$this->_userapp = $userapp;
		$this->line = $line_resource;
		$this->user = $user_resource;
		$this->sip = $sip_resource;
		$this->sccp = $sccp_resource;
		$this->extension = $extension_resource;
		$this->mapper = $line_mapper;
		$this->context_dso = $context_dso;
	}

	public function get_user_masterline($user_id)
	{
		$associations = $this->user->list_line_associations($user_id);
		foreach($associations as $association) {
			if ($association['main_line'] == true) {
				$line = $this->line->get($association['line_id']);
				if($line !== null && $this->entity_allowed($line)) {
					return $this->get_legacy_line($line);
				}
			}
		}
		return false;
	}

	private function entity_allowed($line)
	{
		if(defined('ENTITY_FILTER_CONTEXTS')) {
			$contexts = explode(',', ENTITY_FILTER_CONTEXTS);
			if(!in_array($line['context'], $contexts)) {
				return false;
			}
		}
		return true;
	}

	function set_linefeatures($arr)
	{
		if(is_array($arr) === false
		|| $this->_linefeatures === false)

		$reslist = $error = array();
		$errnb = 0;

		foreach($arr as $key => $list)
		{
			for($i = 0;$i < count($list);$i++)
			{
				if (isset($arr[$key][$i]) === false
				|| $arr[$key][$i] === '')
					$reslist[$i][$key] = '';
				else
					$reslist[$i][$key] = $arr[$key][$i];
			}
		}

		if(empty($reslist) === true)
		{
			$this->_userapp->_set_result('linefeatures',false);
			$this->_userapp->_set_error('linefeatures','empty');
			return(false);
		}

		array_pop($reslist);

		if (empty($reslist) === true)
			return(null);

		for($i = 0;$i < count($reslist);$i++)
		{
			$ref = &$reslist[$i];

			$ref['protocolid'] = 0;
			if (isset($ref['id']) === true
			&& (int) $ref['id'] !== 0)
				$ref['id'] = (int) $ref['id'];

			if($ref['protocol'] === XIVO_SRE_IPBX_AST_PROTO_SCCP)
				$ref['name'] = $ref['number'];
			elseif(dwho_has_len($ref, 'name') === false)
				$ref['name'] = $this->_userapp->gen_password(6,true);

			$protocolname = $ref['protocol'];
			$number = $ref['number'];

			$ref['num'] = isset($ref['num']) ? (int) $ref['num'] : 1;
			$ref['callerid'] = '"fake" <1000>';

			if($this->_linefeatures->chk_values($ref) === false)
			{
				$errnb++;
				$error[$i] = $this->_linefeatures->get_filter_error();
			}
		}

		$this->_userapp->_set_result('linefeatures',$reslist);

		if ($errnb >> 0)
		{
			$this->_userapp->_set_errors('linefeatures',$error);
			return(false);
		}

		return(true);
	}

	function edit_linefeatures()
	{
		if(($rs = $this->_userapp->get_result('linefeatures')) === null
		|| (($nbrs = count($rs)) === 0
			&& $this->_userapp->has_existing_line() === false))
			return(null);

		if(is_null($rs) === true)
			$rs = array();

		$res = array();
		if (isset($this->_userapp->_info['linefeatures']) === true
		&& ($info = $this->_userapp->_info['linefeatures']) !== false
		&& ($nbinfo = count($info)) !== 0)
		{
			for($i = 0;$i < $nbinfo;$i++)
			{
				$ref = &$info[$i];
				$res[$ref['id']] = $ref;
			}
		}

		$return = $error = array();
		$errnb = 0;
		$linefeatures = array();
		$linefeatures['list'] = $res;
		$linefeatures['slt'] = dwho_array_intersect_key($rs,$linefeatures['list'],'id');
		$linefeatures['info'] = false;

		if($linefeatures['slt'] !== false)
		{
			$linefeatures['info'] = dwho_array_copy_intersect_key($rs,$linefeatures['slt'],'id');
			$linefeatures['list'] = dwho_array_diff_key($linefeatures['list'],$linefeatures['slt']);
		}

		if(empty($linefeatures['list']) === false)
		{
			$nb = count($linefeatures['list']);
			$linefeatures_list = array_values($linefeatures['list']);
			for($i = 0;$i < $nb;$i++)
			{
				$ref = &$linefeatures_list[$i];
				if (isset($ref['id']) === false)
					continue;
				if($this->_line->get($ref['id']) === false
				|| $this->_line->delete() === false)
				{
					dwho_report::push('error','Can\'t remove line: '.$ref['id'],'linefeatures');
					$this->_userapp->_set_error('line',$this->_line->get_error());
					return(false);
				}
			}
		}

		for($i = 0;$i < $nbrs;$i++)
		{
			$line = &$rs[$i];

			$this->_line->_reset();
			$protocolname = $line['protocol'];
			$arr = array();

			if (isset($line['id']) === true
			&& (int) $line['id'] !== 0)
			{
				$methodset = 'set_edit';
				$method = 'edit';

				if (($existline = $this->_line->get($line['id'])) === false)
				{
					$errnb++;
					$error[$i] = 'line_not_exist';
					dwho_report::push('error',dwho_i18n::babelfish('Row '.($i+1).': line_not_exist'),'linefeatures');
					continue;
				}
			}
			else
			{
				$methodset = 'set_add';
				$method = 'add';
			}

			$arr['linefeatures'] =  $this->_userapp->get_result_var('linefeatures', 0);

			$arr['user_id'] = $user_id;
			$arr['protocol'] = array();

			if ($protocolname === XIVO_SRE_IPBX_AST_PROTO_SIP)
				$arr['protocol']['call-limit'] = (2 * (int) $this->_userapp->get_result_var('userfeatures', 'simultcalls'));
			$arr['protocol']['language'] = $this->_userapp->get_result_var('userfeatures', 'language');
			$arr['protocol']['name'] = $line['name'];
			$arr['protocol']['context'] = $line['context'];

			if($protocolname === XIVO_SRE_IPBX_AST_PROTO_CUSTOM)
				$arr['protocol']['interface'] = $line['name'];
			elseif($protocolname === XIVO_SRE_IPBX_AST_PROTO_SCCP)
			{
				$arr['protocol']['cid_name'] = str_replace('"','',$line['callerid']);
				$arr['protocol']['cid_num'] = $line['number'];
			}

			if(($queue = $this->_userapp->get_result('queue')) !== null
			&& ($queue_select = $this->_userapp->get_result('queue-select')) !== null)
			{
				$arr['queue'] = $queue;
				$arr['queue-select'] = $queue_select;
			}
			if(($group = $this->_userapp->get_result('group')) !== null
			&& ($group_select = $this->_userapp->get_result('group-select')) !== null)
			{
				$arr['group'] = $group;
				$arr['group-select'] = $group_select;
			}

			if (isset($line['id'])) {
				$actual_line = $this->_line->get($line['id']);
				$arr['options'] = $actual_line['options'];
			}

			if($this->_line->$methodset($arr,$protocolname) === false
			|| ($line_id = $this->_line->$method()) === false)
			{
				$errnb++;
				$error[$i] = $this->_line->get_error();
				$msgkey = array_shift(array_keys($this->_line->get_error()));
				$msg = array_shift($this->_line->get_error());
				if (empty($msgkey) === false)
					dwho_report::push('error','Line '.($i+1).': '.$msgkey.' '.$msg,'linefeatures');
				else
					dwho_report::push('error','Line '.($i+1).': unknow error during '.$method,'linefeatures');
			}
			$line['id'] = $line_id;
			$return[$i] = $line;
		}

		if ($errnb >> 0)
		{
			$this->_userapp->_set_error('linefeatures',$error);
			return(false);
		}

		return(($this->_userapp->_return['linefeatures'] = $return));
	}

	function delete_linefeatures($lines)
	{
		foreach($lines as $line) {
			$deleted = $this->delete_line($line);
			if(!$deleted) {
				return false;
			}
		}
	}

	private function delete_line($line)
	{
		return $this->dissociate_device($line) && $this->delete_endpoint($line);
	}

	private function dissociate_device($line)
	{
		$association = $this->line->get_device_association($line['id']);
		if($association !== null) {
			return  $this->line->dissociate_device($line['id'], $association['device_id']);
		}
		return true;
	}

	private function delete_endpoint($line)
	{
		$protocol = $line['protocol'];

		if($protocol == 'sip') {
			$association = $this->line->get_sip_association($line['id']);
		} else {
			$association = $this->line->get_sccp_association($line['id']);
		}

		if($association !== null) {
			if($protocol == 'sip') {
				$dissociated = $this->line->dissociate_sip($line['id'], $association['endpoint_id']);
			} else {
				$dissociated = $this->line->dissociate_sccp($line['id'], $association['endpoint_id']);
			}

			if($dissociated === true) {
				if($protocol == 'sip') {
					return $this->sip->delete($endpoint['id']);
				} else {
					return $this->sccp->delete($endpoint['id']);
				}
			}
		}

		return false;
	}

	function get_linefeatures($line_id)
	{
		$line = $this->line->get($line_id);
		$line = $this->get_legacy_line($line);
		if ($line === null) {
			return array('info' => false, 'origin' => false);
		}
		return array('info' => array($line), 'origin' => false);
	}

	private function get_legacy_line($line)
	{
		$endpoint = $this->get_endpoint($line);
		$extension = $this->get_internal_extension($line);

		if ($line === null || $endpoint === null || $extension === null) {
			dwho_report::push('error', "Could not get Line (ID $line_id) and its associations");
			return null;
		}

		return $this->mapper->map_line($line, $endpoint, $extension);
	}

	private function get_endpoint($line) {
		$protocol = $line['protocol'];

		if($protocol == "sip") {
			$association = $this->line->get_sip_association($line['id']);
		} else {
			$association = $this->line->get_sccp_association($line['id']);
		}

		if($association !== null) {
			if($protocol == "sip") {
				return $this->sip->get($association['endpoint_id']);
			} else {
				return $this->sccp->get($association['endpoint_id']);
			}
		}

		return null;
	}

	private function get_internal_extension($line) {
		$associations = $this->line->list_extension_associations($line['id']);
		foreach($associations as $association) {
			$extension = $this->extension->get($association['extension_id']);
			if($this->is_internal_extension($extension)) {
				return $extension;
			}
		}
		return null;
	}

	private function is_internal_extension($extension) {
		$contexts = $this->context_dso->get_all(null, null, null, false, 'internal');
		foreach($contexts as $context) {
			if($context['name'] == $extension['context']) {
				return true;
			}
		}
		return false;
	}

	function reverse_add()
	{
		$this->_line->_status = 'add';
		$this->_line->_reverse_add();
	}

	function reverse_edit()
	{
		$this->_line->_status = 'edit';
		$this->_line->_reverse_edit();
	}

}


?>
