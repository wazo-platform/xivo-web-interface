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
	var $_phonebook_resource = null;

	public function __construct(&$service,$param=null,$components=true)
	{
		parent::__construct($service);

        if(($_XOBJ = dwho_gct::get('xivo_object')) === false
        || ($this->_entity_module = &$_XOBJ->get_module('entity')) === false
		|| ($dird_client = Client::dird_config(false)) === false
		|| ($this->_phonebook_resource = new PhonebookResource($dird_client)) === false
		|| ($entities = $this->_entity_module->get_all()) === false
		|| ($current_entity = $_SESSION['_USR']->entity_id) === false)
            return(false);

        foreach($entities as $entity) {
            if($current_entity === 0 || $current_entity === $entity['id']) {
                $current_phonebooks = $this->_phonebook_resource->list_phonebooks($entity['name']);
				foreach($current_phonebooks as &$phonebook) {
					$phonebook['entity'] = $entity['name'];
				}
				$this->_phonebooks = array_merge($this->_phonebooks, $current_phonebooks);
            }
        }

		return(true);
	}

	function add_contact($entity, $phonebook_id, $query)
	{
		$dird_fields = $this->_webi_to_dird($query);
		$this->_phonebook_resource->add_contact($entity, $phonebook_id, $dird_fields);
	}

	function edit_contact($entity, $phonebook_id, $contact_uuid, $query)
	{
		$dird_fields = $this->_webi_to_dird($query);
		$this->_phonebook_resource->edit_contact($entity, $phonebook_id, $contact_uuid, $dird_fields);
	}

	private function _webi_to_dird($query)
	{
		$normal_fields = $query['phonebook'];
		$number_fields = $query['phonebooknumber'];
		$address_fields = $query['phonebookaddress'];

		$dird_fields = array();

		foreach($normal_fields as $key => $value) {
			if(!empty($value)) {
				$dird_fields[$key] = $value;
			}
		}

		foreach($number_fields as $key => $value) {
			if(!empty($value)) {
				$dird_fields['number_'.$key] = $value;
			}
		}

		foreach($address_fields as $location => $fields) {
			foreach($fields as $key => $value) {
				if(!empty($value)) {
					$dird_fields['address_'.$location.'_'.$key] = $value;
				}
			}
		}

		return($dird_fields);
	}

	function get_contact($entity, $phonebook_id, $contact_uuid)
	{
		if(($dird_fields = $this->_phonebook_resource->get_contact($entity, $phonebook_id, $contact_uuid)) === false) {
			return(false);
		}

		$normal_fields = array();
		$number_fields = array();
		$address_fields = array();

		foreach($dird_fields as $key => $value) {
			if(empty($value) === true
				|| (($pieces = explode("_", $key)) && empty($pieces))) {
				continue;
			}

			if($pieces[0] === 'number'
				&& count($pieces) === 2) {
				$number_fields[$pieces[1]] = $value;
			} else if($pieces[0] === 'address'
				&& count($pieces) === 3) {
				if(isset($address_fields[$pieces[1]]) === false) {
					$address_fields[$pieces[1]] = array();
				}
				$address_fields[$pieces[1]][$pieces[2]] = $value;
			} else {
				$normal_fields[$key] = $value;
			}
		}

		return(array('phonebook' => empty($normal_fields) ? false : $normal_fields,
                     'phonebooknumber' => empty($number_fields) ? false : $number_fields,
                     'phonebookaddress' => empty($address_fields) ? false : $address_fields));
	}

	function delete_contact($entity, $phonebook_id, $contact_uuid)
	{
		return($this->_phonebook_resource->delete_contact($entity, $phonebook_id, $contact_uuid));
	}

	function get_contact_list($entity, $phonebook_id, $order=null, $limit=null)
	{
		$contacts = $this->_phonebook_resource->list_contacts($entity, $phonebook_id, $order, $limit);
		return($contacts);
	}

	function get_contact_cnt($entity, $phonebook_id)
	{
		$contacts = $this->_phonebook_resource->list_contacts($entity, $phonebook_id);
		return(count($contacts));
	}

	function get_phonebook_cnt()
	{
        return(count($this->_phonebooks));
	}

	function get_phonebook_list($order=null,$limit=null)
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
