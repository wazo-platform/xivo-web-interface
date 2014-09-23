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

class xivo_service_asterisk_func_key_dest_features extends xivo_service_asterisk_abstract
{
	var $_dso			= null;
	var $_name			= 'func_key_dest_features';
	var $_filter		= false;
	var $_origin		= false;
	var $_origin_list	= false;
	var $_type_list		= false;

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_service_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_service_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_load_config();
	}

	function _prepare($data)
	{
		$data['features_id'] = (int) $data['features_id'];
		$data['func_key_id'] = (int) $data['func_key_id'];
		$data['destination_type_id'] = (int) $data['destination_type_id'];
		return($data);
	}

	public function get_func_key_id_by_feature_type($feature_type)
	{
		return($this->_dso->get_func_key_id_by_feature_type($feature_type));
	}

	public function get_exten_by_features_id($feature_id)
	{
		return($this->_dso->get_exten_by_features_id($feature_id));
	}

	public function get_key_type_by_features_id($feature_id)
	{
		return($this->_dso->get_key_type_by_features_id($feature_id));
	}

}

?>
