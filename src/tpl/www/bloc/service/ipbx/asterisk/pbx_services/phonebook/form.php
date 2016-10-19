<?php

#
# XiVO Web-Interface
# Copyright (C) 2016 Avencall
# Copyright (C) 2016 Proformatique
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

$info = $this->get_var('info');
$element = $this->get_var('element');

if ($this->get_var('entity_list') === false) {
	echo $this->bbf('no_internal_context_for_this_entity');
} else {
	$params = array('desc'	=> $this->bbf('fm_phonebook_entity'),
					'name'		=> 'entity',
					'labelid'	=> 'phonebook-entity',
					'key'		=> 'displayname',
					'altkey'	=> 'name',
					'selected'  => $this->get_var('entity'),
					'error'	=> $this->bbf_args('error', $this->get_var('error', 'phonebook', 'entity')));
	if ($this->get_var('act') !== 'add') {
		$params['disabled'] = true;
		$params['class'] = 'it-disabled';
	}
	echo $form->select($params, $this->get_var('entities'));
}

$params = array('desc' => $this->bbf('fm_phonebook_name'),
				'name' => 'name',
				'labelid' => 'phonebook-name',
				'size' => 15,
				'value' => $info['name']);
echo $form->text($params);
?>

<div class="fm-paragraph fm-description">
	<p>
		<label id="lb-phonebook-description" for="it-phonebook-description">
			<?=$this->bbf('fm_phonebook_description');?>
		</label>
	</p>
<?php
	echo  $form->textarea(array('paragraph'	=> false,
								'label'	=> false,
								'name' => 'description',
								'id' => 'it-phonebook-description',
								'cols' => 60,
								'rows' => 5,
								'default' => ''),
							$info['description']);
?>
</div>
