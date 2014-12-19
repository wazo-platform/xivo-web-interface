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

xivo_service_asterisk::required(array('abstract','datastorage','abstract','sql.inc'),true);

class xivo_service_asterisk_func_key_dest_features_sql extends xivo_service_asterisk_abstract_sql
{
	var $_disable		= null;

	var $_keytypes = array(
		'parkext' => 'park',
		'blindxfer' => 'speeddial',
		'atxfer' => 'speeddial',
		'automon' => 'speeddial',
	);
	var $_exten_regex = '/[0-9\*#]+/';

	public function get_func_key_id_by_feature_type($feature_type)
	{
		$this->_dso->new_select('func_key_dest_features', array('func_key_id'));

		$this->_dso->innerjoin('features',
					'func_key_dest_features.features_id = features.id');

		$this->_dso->where(array('var_name' => $feature_type), 'features');

		$r = $this->_dso->select_singlecolumn();

		if(isset($r[0]) === false)
			return(false);

		return((int) array_pop($r));
    }

	public function get_exten_by_features_id($feature_id)
	{
		$this->_dso->new_select('features', array('var_val'));

		$this->_dso->where(array('id' => $feature_id));

		$r = $this->_dso->select_singlecolumn();
		if(isset($r[0]) === false)
			return(false);

		$exten = array_pop($r);
		if(preg_match($this->_exten_regex, $exten) !== 1)
			return(false);

		return($exten);
	}

	public function get_key_type_by_features_id($feature_id)
	{
		$this->_dso->new_select('features', array('var_name'));

		$this->_dso->where(array('id' => $feature_id));

		$r = $this->_dso->select_singlecolumn();
		if(isset($r[0]) === false)
			return(false);

		$varname = array_pop($r);
		if(isset($this->_keytypes[$varname]) === false)
			return(false);

		return $this->_keytypes[$varname];
	}

}

?>
