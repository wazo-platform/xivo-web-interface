<?php

#
# XiVO Web-Interface
# Copyright (C) 2016  Avencall
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

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'client.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'dird', 'phonebook', 'phonebook.inc'));

class xivo_application_service_asterisk_dirdphonebook extends xivo_application_asterisk_abstract
{
	var $_phonebook_module = false;
    var $_entity_module = false;
	var $_phonebooks = array();

	public function __construct(&$service,$param=null,$components=true)
	{
		parent::__construct($service);

        if(($_XOBJ = dwho_gct::get('xivo_object')) === false
        || ($this->_entity_module = &$_XOBJ->get_module('entity')) === false)
            return(false);

        $dird_client = Client::dird_config(false);
        $phonebook_resource = new PhonebookResource($dird_client);
        $entities = $this->_entity_module->get_all();
        $current_entity = $_SESSION['_USR']->entity_id;
        foreach($entities as $entity) {
            if($current_entity === 0 || $current_entity === $entity['id']) {
                $current_phonebooks = $phonebook_resource->list_phonebooks($entity['name']);
				foreach($current_phonebooks as &$phonebook) {
					$phonebook['entity'] = $entity['name'];
				}
				$this->_phonebooks = array_merge($this->_phonebooks, $current_phonebooks);
            }
        }

		return(true);
	}

	function get_cnt($arr=null,$disable=null)
	{
        return(count($this->_phonebooks));
	}

	function get_phonebook_list($disable=null,$order=null,$limit=null,$assoc=false)
	{
		if(is_array($order) && count($order > 1)) {
			$sort_column = $order[1];
			$sort_order = $order[0];
			foreach($this->_phonebooks as $i => $phonebook) {
				if(array_key_exists($sort_column, $phonebook)) {
					$sorted[$i] = $phonebook[$sort_column];
				}
			}
			if(isset($sorted) && is_array($sorted)) {
				array_multisort($sorted, $sort_order, $this->_phonebooks);
			}
		}
		if(is_array($limit)) {
			$lower = $limit[0];
			$upper = $lower + $limit[1];
			$subset = array_slice($this->_phonebooks, $lower, $upper);
			return($subset);
		}
        return($this->_phonebooks);
	}
}

?>
