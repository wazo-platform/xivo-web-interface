<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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

$info = $this->get_var('info');
$element = $this->get_var('element');

?>

<?php
	echo  $form->text(array('desc' 		=> $this->bbf('fm_name'),
							'name' 		=> 'name',
							'labelid'	=> 'name',
							'size' 		=> 15,
							'default' 	=> $element['name']['default'],
							'value' 	=> $info['name'],
							'error' 	=> $this->bbf_args('error', $this->get_var('error', 'name')))),
		$form->select(array('desc' 		=> $this->bbf('fm_type'),
							'name' 		=> 'type',
							'labelid'	=> 'type',
							'key' 		=> 'name',
							'altkey' 	=> false,
							'default' 	=> $element['type']['default'],
							'selected' 	=> $info['type']),
						$this->get_var('types'));
?>
	<div id='div-free-uri'>
<?php
	echo  $form->text(array('desc' 		=> $this->bbf('fm_uri'),
							'name' 		=> 'uri',
							'labelid'	=> 'uri',
							'size' 		=> 30,
							'default' 	=> $element['uri']['default'],
							'value' 	=> $info['uri'],
							'error' 	=> $this->bbf_args('error', $this->get_var('error', 'uri'))));
?>
	</div>
	<div id='div-ldap-uri'>
<?php
	echo    $form->select(array('desc' 		=> $this->bbf('fm_ldapfilter_name'),
								'name' 		=> 'ldapfilter_id',
								'labelid'	=> 'id',
								'key' 		=> 'name',
								'altkey' 	=> 'id',
								'default'	=> '',
								'selected' 	=> $info['ldapfilter_id']),
							$this->get_var('ldap_filters'));
?>
	</div>
	<fieldset id='fld-xivo-form'>
	<legend><?= $this->bbf('fm_xivo_form') ?></legend>
<?php
	echo $form->text(array('desc' => $this->bbf('fm_xivo_username'),
			'name'		=> 'xivo_username',
			'labelid'	=> 'xivo-username',
			'size'		=> 15,
			'help'		=> $this->bbf('hlp_xivo_username'),
			'default'	=> $element['xivo_username']['default'],
			'value'		=> $info['xivo_username'],
			'error'		=> $this->bbf_args('error',
					$this->get_var('error', 'xivo_username')) )),

	$form->password(array('desc' => $this->bbf('fm_xivo_password'),
			'name'		=> 'xivo_password',
			'labelid'	=> 'xivo-password',
			'size'		=> 15,
			'help'		=> $this->bbf('hlp_xivo_password'),
			'default'	=> $element['xivo_password']['default'],
			'value'		=> $info['xivo_password'],
			'error'		=> $this->bbf_args('error',
					$this->get_var('error', 'xivo_password')) )),

	$form->select(array('desc' => $this->bbf('fm_xivo_verify_certificate_select'),
			'name'		=> 'xivo_verify_certificate_select',
			'labelid'	=> 'xivo-verify-certificate-select',
			'key'		=> false,
			'bbf'		=> 'fm_xivo_verify_certificate_select-opt',
			'bbfopt'	=> array('argmode' => 'paramvalue'),
			'default'	=> $element['xivo_verify_certificate_select']['default'],
			'selected'	=> $info['xivo_verify_certificate_select']),
			$element['xivo_verify_certificate_select']['value']),

	$form->text(array('desc' => $this->bbf('fm_xivo_custom_ca_path'),
			'name'		=> 'xivo_custom_ca_path',
			'labelid'	=> 'xivo-custom-ca-path',
			'size'		=> 30,
			'help'		=> $this->bbf('hlp_xivo_custom_ca_path'),
			'default'	=> $element['xivo_custom_ca_path']['default'],
			'value'		=> $info['xivo_custom_ca_path'],
			'error'		=> $this->bbf_args('error',
					$this->get_var('error', 'xivo_custom_ca_path')) ));
?>
	</fieldset>
	<fieldset id='fld-dird-form'>
	<legend><?= $this->bbf('fm_dird_form') ?></legend>
<?php
	echo    $form->select(array('desc' => $this->bbf('fm_dird_phonebook'),
								'name' => 'phonebook_id',
								'labelid' => 'display',
								'key' => 'display',
								'altkey' => 'id',
								'selected' => $info['phonebook_id']),
							$this->get_var('dird_phonebooks'));
?>
	</fieldset>
	<div class="fm-paragraph fm-description">
		<p>
			<label id="lb-description" for="it-description"><?=$this->bbf('fm_description');?></label>
		</p>
		<?=$form->textarea(array('paragraph'	=> false,
					 'label'	=> false,
					 'name'		=> 'description',
					 'id'		=> 'it-description',
					 'cols'		=> 60,
					 'rows'		=> 5,
					 'default'	=> $element['description']['default'],
		          'error'	=> $this->bbf_args('description',
					   $this->get_var('error', 'description')) ),
				   $info['description']);?>
	</div>
