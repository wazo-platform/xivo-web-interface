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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'manager', 'abstract.inc'));

class func_key_manager extends manager_abstract
{

	protected $_name			= 'func_key';
	protected $_func_key		= false;
	protected $_func_key_type	= false;
	protected $_func_key_destination_type	= false;
	protected $_func_key_dest_user	= false;
	protected $_func_key_dest_group	= false;
	protected $_func_key_dest_queue	= false;
	protected $_func_key_mapping	= false;
	protected $_func_key_template	= false;

	public function __construct($_MGR, $param=null)
	{
		parent::__construct($_MGR, $param);
		$this->_load_components();
	}

	private function _load_components()
	{
		$this->_func_key = $this->_service->get_module('ipbx','func_key');
		$this->_func_key_type = $this->_service->get_module('ipbx','func_key_type');
		$this->_func_key_destination_type = $this->_service->get_module('ipbx','func_key_destination_type');
		$this->_func_key_dest_user = $this->_service->get_module('ipbx','func_key_dest_user');
		$this->_func_key_dest_group = $this->_service->get_module('ipbx','func_key_dest_group');
		$this->_func_key_dest_queue = $this->_service->get_module('ipbx','func_key_dest_queue');
		$this->_func_key_mapping = $this->_service->get_module('ipbx','func_key_mapping');
		$this->_func_key_template = $this->_service->get_module('ipbx','func_key_template');
	}

	public function get_type_id_by_name($name)
	{
		if (($fk_type = $this->_func_key_type->get_where(array('name' => $name))) === false)
		{
			dwho_report::push('error', 'Error func_key_type with name "'.$name.'" not exist');
			return(false);
		}
		return((int) $fk_type['id']);
	}

	public function get_destination_type_id_by_name($name)
	{
		if (($fk_dst_type = $this->_func_key_destination_type->get_where(array('name' => $name))) === false)
		{
			dwho_report::push('error', 'Error func_key_destination_type with name "'.$name.'" not exist');
			return(false);
		}
		return((int) $fk_dst_type['id']);
	}

	public function add_fk_mapping($data)
	{
		if (($func_key_id = $this->_get_fk_id_with_dst($data['dst_type'], $data['dst_id'])) === false
		|| ($fk_dst_type_id = $this->_get_dst_type_id_with_dst($data['dst_type'])) === false)
			return(false);

		$func_key_mapping = array(
			'template_id' => (int) $data['template_id'],
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id,
			'position' => (int) $data['position'],
			'blf' => (bool) $data['blf'],
			'label' => (string) $data['label']
		);

		if(($rs = $this->_func_key_mapping->chk_values($func_key_mapping)) === false)
			return(false);

		$where = array(
			'template_id' => (int) $data['template_id'],
			'func_key_id' => (int)$func_key_id
		);
		if (($this->_func_key_mapping->get_where($where)))
			$this->_func_key_mapping->edit($where, $rs);
		else
			$this->_func_key_mapping->add($rs);

		return(true);
	}

	public function delete_fk_mapping($data)
	{
		if (($func_key_id = $this->_get_fk_id_with_dst($data['dst_type'], $data['dst_id'])) === false)
			return(false);

		$where = array(
			'template_id' => (int) $data['template_id'],
			'func_key_id' => (int)$func_key_id
		);
		$this->_func_key_mapping->delete($where);
	}

	public function delete_fk_mapping_with_template_id($template_id)
	{
		$where = array(
			'template_id' => (int) $template_id
		);
		$this->_func_key_mapping->delete_where($where);
	}

	private function _get_dst_type_id_with_dst($dst_type)
	{
		switch($dst_type)
		{
			case 'user':
			case 'group':
			case 'queue':
				$fk_dst_type_id = (int) $this->get_destination_type_id_by_name($dst_type);
				break;
			default:
				dwho_report::push('error', 'Error: unknown fk_dst_type "'.$dst_type);
				return(false);
		}

		if (!is_int($fk_dst_type_id) || $fk_dst_type_id <= 0)
		{
			dwho_report::push('error', 'Error: fk_dst_type_id "'.$fk_dst_type_id.'" is not valid');
			return(false);
		}

		return((int) $fk_dst_type_id);
	}

	private function _get_fk_id_with_dst($dst_type, $dst_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id($dst_type, $dst_id)) === false)
		{
			switch($dst_type)
			{
				case 'user':
					$func_key_id = $this->add_dst_for_user($dst_id);
					break;
				case 'group':
					$func_key_id = $this->add_dst_for_group($dst_id);
					break;
				case 'queue':
					$func_key_id = $this->add_dst_for_queue($dst_id);
					break;
				default:
					dwho_report::push('error', 'Error: unknown fk_dst_type "'.$dst_type);
					return(false);
			}
		}

		if (!is_int($func_key_id) || $func_key_id <= 0)
		{
			dwho_report::push('error', 'Error: func_key_id "'.$func_key_id.'" is not valid');
			return(false);
		}

		return((int) $func_key_id);
	}

	public function add_private_tpl($name)
	{
		$func_key_template = array(
			'name' => $name,
			'private' => true
		);

		if(($rs = $this->_func_key_template->chk_values($func_key_template)) === false
		|| ($func_key_template_id = $this->_func_key_template->add($rs)) === false)
			return(false);

		return((int) $func_key_template_id);
	}

	public function delete_template($template_id)
	{
		if ($this->_func_key_mapping->delete_where(array('template_id' => $template_id)) === false
		|| $this->_func_key_template->delete($template_id) === false)
			return(false);
	}

	public function add_dst_for_user($user_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('user');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest_user = array(
			'user_id' => (int) $user_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_user->chk_values($func_key_dest_user)) === false
		|| $this->_func_key_dest_user->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_user($user_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('user', $user_id)))
			$this->_delete_func_key((int) $func_key_id, 'user');

		if (($fk_list = $this->_func_key->get_func_key_by_user_id($user_id)) !== false
		&& ($nb_fk = count($fk_list)) !== 0)
		{
			for($i = 0;$i < $nb_fk;$i++)
			{
				$fk = &$fk_list[$i];
				$this->_delete_func_key((int) $fk['func_key_id'], 'user');
			}
		}

		return(true);
	}

	public function add_dst_for_group($group_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('group');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'group_id' => (int) $group_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_group->chk_values($func_key_dest)) === false
		|| $this->_func_key_dest_group->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_group($group_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('group', $group_id)))
			$this->_delete_func_key((int) $func_key_id, 'group');

		return(true);
	}

	public function add_dst_for_queue($queue_id)
	{
		$fk_type_id = $this->get_type_id_by_name('speeddial');
		$fk_dst_type_id = $this->get_destination_type_id_by_name('queue');

		$func_key = array(
			'type_id' => (int) $fk_type_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if(($rs = $this->_func_key->chk_values($func_key)) === false
		|| ($func_key_id = $this->_func_key->add($rs)) === false)
			return(false);

		$func_key_dest = array(
			'queue_id' => (int) $queue_id,
			'func_key_id' => (int) $func_key_id,
			'destination_type_id' => (int) $fk_dst_type_id
		);

		if (($rs = $this->_func_key_dest_queue->chk_values($func_key_dest)) === false
		|| $this->_func_key_dest_queue->add($rs) === false)
			return(false);

		return((int) $func_key_id);
	}

	public function delete_dst_for_queue($queue_id)
	{
		if (($func_key_id = $this->_func_key->get_func_key_id_with_dest_type_id('queue', $queue_id)))
			$this->_delete_func_key((int) $func_key_id, 'queue');

		return(true);
	}

	private function _delete_func_key($func_key_id, $dst_type)
	{
		$where_fk_id = array('func_key_id' => $func_key_id);
		if ($this->_func_key_mapping->delete_where($where_fk_id) === false)
			return(false);

		$fk_dst_type_id = $this->get_destination_type_id_by_name($dst_type);

		$where_func_key_dest = $where_fk_id;
		$where_func_key_dest['destination_type_id'] = (int) $fk_dst_type_id;

		switch ($dst_type)
		{
			case 'user':
				if ($this->_func_key_dest_user->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'group':
				if ($this->_func_key_dest_group->delete_where($where_func_key_dest) === false)
					return(false);
				break;
			case 'queue':
				if ($this->_func_key_dest_queue->delete_where($where_func_key_dest) === false)
					return(false);
				break;
		}

		if ($this->_func_key->delete($func_key_id) === false)
			return(false);
	}
}

?>
