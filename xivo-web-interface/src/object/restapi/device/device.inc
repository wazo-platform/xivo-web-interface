<?php
#
# XiVO Web-Interface
# Copyright (C) 2013  Avencall
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

class restapi_ressource_device extends xivo_restapi_ressource_abstract
{

	protected $_name      = 'device';
	protected $_base_url  = 'devices';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
	}

	public function get($id)
	{
		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array($id));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 200
		|| empty($rs) === true)
			return(false);

		return($rs);
	}

	public function get_device_by_ip($ip)
	{
		$uri = $this->_build_uri();
		$query = array();
		array_push($query, array('q','{"ip":"'.$ip.'"}'));
		$rs = $this->request_get($uri, $query);

		if($this->_status_code !== 200
		|| empty($rs) === true
		|| (int) $rs['total'] === 0)
			return(false);

		return($rs['items'][0]);
	}

	public function find_all($search='',$order=false,$limit=false)
	{
		$uri = $this->_build_uri();

		$query = array();
		if (dwho_has_len($search) === true)
			array_push($query, array('q','{"*":"'.$search.'"}'));

		if (is_array($order))
		{
			$sort_key = array_keys($order);
			$sort_order = array_values($order);
			if ($sort_order[0] == SORT_DESC)
				$sort_order[0] = -1;
			elseif ($sort_order[0] == SORT_ASC)
				$sort_order[0] = 1;
			array_push($query, array('sort_key', $sort_key[0]));
			array_push($query, array('sort_order', $sort_order[0]));
		}

		if (is_array($limit))
		{
			array_push($query, array('skip', $limit[0]));
			array_push($query, array('limit', $limit[1]));
		}

		$rs = $this->request_get($uri, $query);

		if (is_array($rs) === false
		|| isset($rs['items']) === false
		|| ($rs = $rs['items']) === false)
		{
			dwho_report::push('error', 'An Error occured while retrieving the list of devices');
			return(false);
		}
/*
		if (dwho_has_len($search) === true)
		{
			for ($i=0; $i<$nb; $i++)
			{
				$ref = &$devices[$i];
				foreach ($ref as $value)
					if (!is_string($value) || !preg_match('/'.$search.'/', $value))
						unset($ref[$i]);
			}
		}
*/
		return($rs);
	}

	public function add($data)
	{
		if (is_array($data) === false)
			return(false);

		$uri = $this->_build_uri();
		$rs = $this->request_post($uri,$data);

		if($this->_status_code !== 201)
		{
			dwho_report::push('error', 'fail to create device: '.$rs[0]);
			return(false);
		}

		return($rs['id']);
	}

	public function edit($id, $data)
	{
		if (is_array($data) === false
		|| dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array($id));
		$rs = $this->request_put($uri,$data);

		if($this->_status_code !== 204)
		{
			dwho_report::push('error', 'fail to edit device: '.$rs[0]);
			return(false);
		}

		return(true);
	}

	public function delete($id)
	{
		if(dwho_has_len($id) === false)
			return(false);

		$uri = $this->_build_uri(array($id));
		$rs = $this->request_delete($uri);

		if($this->_status_code !== 204)
		{
			dwho_report::push('error', 'fail to delete device: '.$rs[0]);
			return(false);
		}

		return(true);
	}

	public function reset_to_autoprov($id)
	{
		$uri = $this->_build_uri(array($id, 'autoprov'));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
			return(false);

		return(true);
	}

	public function synchronize($id)
	{
		$uri = $this->_build_uri(array($id, 'synchronize'));
		$rs = $this->request_get($uri);

		if($this->_status_code !== 204)
		{
			dwho_report::push('error', 'fail to synchronize device: '.$rs[0]);
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
			dwho_report::push('error', 'fail to associate line on the device: '.$rs[0]);
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
			dwho_report::push('error', 'fail to remove line on the device: '.$rs[0]);
			return(false);
		}

		return(true);
	}
}

?>