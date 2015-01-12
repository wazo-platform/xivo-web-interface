<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2014  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'abstract.inc'));

class confd_ressource_device extends xivo_confd_helpers_abstract
{

	protected $_name      = 'device';
	protected $_base_url  = 'devices';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
		$this->_httpheaders[] = dwho_json::get_header();
		$this->_httpheaders[] = 'Accept: application/json';
	}

	public function find_all($search='',$order=false,$limit=false,$search_column=null)
	{
		$_SRE = dwho_gct::get('xivo_service');
		$line_module = $_SRE->get_module('ipbx', 'linefeatures');

		$result = array();
		switch($search_column)
		{
			case 'number':
				if (($lines = $line_module->search_number($search)) === false)
					return(false);
				foreach ($lines as $line)
				{
					if (dwho_has_len($line['device']))
					{
						$device = $this->get($line['device']);
						$this->_device_entity_filtering($device, $result);
					}
				}
				break;
			default:
				if (($devices = parent::find_all($search,$order)) === false)
					return(false);
				foreach ($devices as $device)
				{
					$this->_device_entity_filtering($device, $result);
				}
		}

		if (is_array($limit) && count($limit) === 2)
		{
			$this->_cnt = count($result);
			$result = dwho_pagination_from_array($result, $limit[0], $limit[1]);
		}

		foreach ($result as &$device)
			$this->_add_entity_displayname_to_device($device);

		return($result);
	}

	private function _device_entity_filtering(&$device, &$result)
	{
		$_SRE = dwho_gct::get('xivo_service');
		$ipbx = &$_SRE->get('ipbx');
		$line_module = $ipbx->get_module('linefeatures');

		$device['number'] = $device['context'] ='';
		switch ($device['status'])
		{
			case 'configured':
				if (($line = $line_module->get_where(array('device' => $device['id']))) !== false)
				{
					$device['number'] = $line['number'];
					$device['context'] = $line['context'];
					if (defined('ENTITY_FILTER_CONTEXTS'))
					{
						$contexts = explode(',', ENTITY_FILTER_CONTEXTS);
						if (!in_array($line['context'], $contexts))
							return(false);
					}
					array_push($result, $device);
				}
				break;
			default:
				array_push($result, $device);
		}
		return(true);
	}

	private function _add_entity_displayname_to_device(&$device)
	{
		$_SRE = dwho_gct::get('xivo_service');
		$_XOBJ = dwho_gct::get('xivo_object');
		$ipbx = &$_SRE->get('ipbx');

		$context_module = $ipbx->get_module('context');
		$entity_module = $_XOBJ->get_module('entity');

		$device['entity_displayname'] = '';

		if (($context = $context_module->get($device['context'])) === false
		|| ($entity = $entity_module->get_where(array('name' => $context['entity']))) === false)
			return(false);

		$device['entity_displayname'] = $entity['displayname'];
	}

	public function get_device_by_ip($ip)
	{
		$uri = $this->_build_uri();
		$query = array();
		array_push($query, array('search',$ip));
		$rs = $this->request_get($uri, $query);

		if($this->_status_code !== 200
		|| empty($rs) === true
		|| ($nb_device = (int) $rs['total']) === 0)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		for ($i=0; $i<$nb_device; $i++)
		{
			$ref = &$rs['items'][$i];
			if ($ip === $ref['ip'])
				return($ref);
		}

		return(false);
	}

	public function reset_to_autoprov($id)
	{
		$uri = $this->_build_uri(array($id, 'autoprov'));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return(true);
	}

	public function synchronize($id)
	{
		$uri = $this->_build_uri(array($id, 'synchronize'));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return(true);
	}

	public function associate_line($device_id, $line_id)
	{
		$uri = $this->_build_uri(array($device_id, 'associate_line', $line_id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return(true);
	}

	public function remove_line($device_id, $line_id)
	{
		$uri = $this->_build_uri(array($device_id, 'remove_line', $line_id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			$this->_build_error_message($rs);
			return(false);
		}

		return(true);
	}
}

?>
