<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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

$form = &$this->get_module('form');
$url = &$this->get_module('url');

$list = $this->get_var('list');
$type = $this->get_var('type');

echo format_number_text($this, $form, $type);
if($type === 'office'):
	echo format_number_text($this, $form, 'fax');
endif;

$predefined_address_fields = array('address1', 'address2', 'city', 'state', 'zipcode');
foreach($predefined_address_fields as $field){
	echo format_address_text($this, $form, $type, $field);
}

echo	$form->select(array('desc' => $this->bbf('fm_phonebookaddress_country'),
							'name' => 'phonebookaddress['.$type.'][country]',
							'labelid' => 'phonebookaddress-'.$type.'-country',
							'empty' => true,
							'size' => 15,
							'selected' => $this->get_var('phonebookaddress',$type,'country')),
						$this->get_var('territory'));
?>
