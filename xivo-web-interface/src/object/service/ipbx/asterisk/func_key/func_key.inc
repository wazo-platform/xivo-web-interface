<?php
#
# Copyright (C) 2014  Avencall
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

xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_func_key extends xivo_service_asterisk_abstract
{
	var $_dso			= null;
	var $_name			= 'func_key';
	var $_filter		= false;
	var $_origin		= false;
	var $_origin_list	= false;
	var $_type_list		= false;

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_USER_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_load_config();
	}

	function _prepare($data)
	{
		$data['id'] = (int) $data['id'];
		$data['type_id'] = (int) $data['type_id'];
		$data['destination_type_id'] = (int) $data['destination_type_id'];
		return($data);
	}

	public function get_provd_conf($user_id)
	{
		$conf = array();

		if (($ipbx = &$this->_sre) === false
		|| ($modextensions = &$ipbx->get_module('extensions')) === false
		|| ($rs = $this->get_func_key_v2($user_id)) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];

			$idx = (string) $ref['position'];
			$conf[$idx] = array();
			$conf[$idx]['type'] = (dwho_bool($ref['blf']) === false) ? 'speeddial' : 'blf';
			$conf[$idx]['line'] = 1;
			$conf[$idx]['label'] = $ref['label'];

			$exten= '';
			switch($ref['destination_type_name'])
			{
				case 'user':
					if(($extension = $modextensions->get_by_user_id($ref['dst_user_id'])) !== false)
						$exten = $extension['exten'];
					break;
			}

			$conf[$idx]['value'] = $exten;
		}

		return((empty($conf) ? false : $conf));
	}

	public function get_func_key_v2($user_id)
	{
		return($this->_dso->get_func_key_v2($user_id));
	}
}

?>