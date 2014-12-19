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
	var $_progfunckeys = array('fwdbusy', 'agentstaticlogoff', 'agentstaticlogtoggle', 'callrecord', 'enablednd', 'enablevm', 'agentstaticlogin', 'fwdrna', 'fwdunc', 'incallfilter');

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
		|| ($modphonefunckey = &$ipbx->get_module('phonefunckey')) === false
		|| ($moddestfeatures = &$ipbx->get_module('func_key_dest_features')) === false
		|| ($moddestagent = &$ipbx->get_module('func_key_dest_agent')) === false
		|| ($modpaging = &$ipbx->get_module('paging')) === false
		|| ($modagent = &$ipbx->get_module('agentfeatures')) === false
		|| ($rs = $this->_dso->get_func_key_by_user_id($user_id)) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		for($i = 0;$i < $nb;$i++)
		{
			$ref = &$rs[$i];

			$idx = (string) $ref['position'];
			$conf[$idx] = array();
			$conf[$idx]['type'] = (dwho_bool($ref['blf']) === false) ? 'speeddial' : 'blf';
			$conf[$idx]['line'] = 1;
			$conf[$idx]['label'] = dwho_has_len($ref['label']) ? $ref['label'] : '';

			$exten= '';
			switch($ref['destination_type_name'])
			{
				case 'user':
					if(($extension = $modextensions->get_by_user_id($ref['user_id'])) !== false)
						$exten = $extension['exten'];
					break;
				case 'group':
					if(($extension = $modextensions->get_by_group_id($ref['group_id'])) !== false)
						$exten = $extension['exten'];
					break;
				case 'queue':
					if(($extension = $modextensions->get_by_queue_id($ref['queue_id'])) !== false)
						$exten = $extension['exten'];
					break;
				case 'conference':
					if(($extension = $modextensions->get_by_conference_id($ref['conference_id'])) !== false)
						$exten = $extension['exten'];
					break;
				case 'service':
					if(($extension = $modextensions->get($ref['extension_id'])) !== false)
					{
						if (in_array($extension['typeval'], $this->_progfunckeys))
						{
							$exten = $modphonefunckey->build_progfunckey($user_id, $extension['exten']);
						} else {
							$exten = $modphonefunckey->build_extenfeatures($extension['exten']);
						}
					}
					break;
				case 'forward':
					if(($extension = $modextensions->get($ref['forward_id'])) !== false)
					{
						$exten = $modphonefunckey->build_progfunckey($user_id, $extension['exten'], $ref['number']);
					}
					break;
				case 'park_position':
					if ($ref['park_position'] !== false)
						$exten = $ref['park_position'];
					break;
				case 'features':
					$key_type = $moddestfeatures->get_key_type_by_features_id($ref['features_id']);
					$feature_exten = $moddestfeatures->get_exten_by_features_id($ref['features_id']);

					if($key_type !== false && $feature_exten !== false) {
						$exten = $feature_exten;
						$conf[$idx]['type'] = $key_type;
					}
					break;
				case 'paging':
					$paging = $modpaging->get($ref['paging_id']);
					$extension_id = $modextensions->get_extenfeature_id_by_typeval('paging');
					$extension = $modextensions->get($extension_id);
					$exten = $modphonefunckey->build_extenfeatures($extension['exten'], $paging['number']);
					break;
				case 'custom':
					$exten = $ref['exten'];
					break;
				case 'agent':
					$extension = $modextensions->get($ref['agent_extension_id']);
					$agent_number = '*'.$ref['agent_id'];

					if($extension !== false) {
						$exten = $modphonefunckey->build_progfunckey($user_id, $extension['exten'], $agent_number);
					}
					break;
				case 'bsfilter':
					$extension_id = $modextensions->get_extenfeature_id_by_typeval('bsfilter');
					$extension = $modextensions->get($extension_id);

					if($extension !== false) {
						$exten = $modphonefunckey->build_extenfeatures($extension['exten'], $ref['filtermember_id']);
					}
					break;
			}

			$conf[$idx]['value'] = $exten;
		}

		return((empty($conf) ? false : $conf));
	}

	public function get_func_key_by_user_id($user_id)
	{
		return($this->_dso->get_func_key_by_user_id($user_id));
	}

	public function get_func_key_by_template_id($template_id)
	{
		return($this->_dso->get_func_key_by_template_id($template_id));
	}

	public function get_func_key_id_with_dest_type_id($dst_type, $dst_id)
	{
		return($this->_dso->get_func_key_id_with_dest_type_id($dst_type, $dst_id));
	}

	public function get_func_key_id($dst_type, $dst_id, $pkey)
	{
		return($this->_dso->get_func_key_id($dst_type, $dst_id, $pkey));
	}
}

?>
