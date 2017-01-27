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
$info = $this->get_var('info');
$element = $this->get_var('element');
$interface_list = $this->get_var('interface_list');
$gateway_list = $this->get_var('gateway_list');

?>
<fieldset id="fld-mainconfig-hostname">
	<legend><?=$this->bbf('fld-mainconfig-hostname');?></legend>
<?php

echo	$form->text(array('desc'	=> $this->bbf('fm_mainconfig_hostname'),
			  'name'	=> 'mainconfig[hostname]',
			  'labelid'	=> 'mainconfig-hostname',
			  'size'	=> 15,
#			  'help'	=> $this->bbf('hlp_fm_mainconfig_hostname'),
			  'comment'	=> $this->bbf('cmt_fm_mainconfig_hostname'),
			  'default'	=> $element['mainconfig']['hostname'],
			  'value'	=> $this->get_var('info','mainconfig','hostname'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','mainconfig','hostname'))));

?>
</fieldset>

<fieldset id="fld-mainconfig-domain">
	<legend><?=$this->bbf('fld-mainconfig-domain');?></legend>
<?php

echo	$form->text(array('desc'	=> $this->bbf('fm_mainconfig_domain'),
			  'name'	=> 'mainconfig[domain]',
			  'labelid'	=> 'mainconfig-domain',
			  'size'	=> 15,
#			  'help'	=> $this->bbf('hlp_fm_mainconfig_domain'),
			  'comment'	=> $this->bbf('cmt_fm_mainconfig_domain'),
			  'default'	=> $element['mainconfig']['domain'],
			  'value'	=> $this->get_var('info','mainconfig','domain'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','mainconfig','domain'))));

?>
</fieldset>

<fieldset id="fld-mainconfig-adminpasswd">
	<legend><?=$this->bbf('fld-mainconfig-adminpasswd');?></legend>
<?php

echo	$form->password(array('desc'	=> $this->bbf('fm_mainconfig_adminpasswd'),
			      'name'	=> 'mainconfig[adminpasswd]',
			      'labelid'	=> 'mainconfig-adminpasswd',
			      'size'	=> 15,
#			      'help'	=> $this->bbf('hlp_fm_mainconfig_adminpasswd'),
			      'comment'	=> $this->bbf('cmt_fm_mainconfig_adminpasswd'),
			      'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','mainconfig','adminpasswd')))),

	$form->password(array('desc'	=> $this->bbf('fm_mainconfig_confirmadminpasswd'),
			      'name'	=> 'mainconfig[confirmadminpasswd]',
			      'labelid'	=> 'mainconfig-confirmadminpasswd',
			      'size'	=> 15,
#			      'help'	=> $this->bbf('hlp_fm_mainconfig_confirmadminpasswd'),
			      'comment'	=> $this->bbf('cmt_fm_mainconfig_confirmadminpasswd'),
			      'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','mainconfig','confirmadminpasswd'))));

?>
</fieldset>

<fieldset id="fld-mainconfig-netiface">
	<legend><?=$this->bbf('fld-mainconfig-netiface');?></legend>
<?php

	echo	$form->select(array('desc'	=> $this->bbf('fm_netiface_address'),
					'name'	=> 'netiface_id[address]',
					'labelid'	=> 'netiface-address',
					'comment'	=> $this->bbf('cmt_fm_netiface_address'),
					'default'	=> $element['netiface_id']['address'],
					'empty'	=> false,
					'selected'	=> $info['netiface_id']['address'],
					'error'	=> $this->bbf_args('error_generic',
										   $this->get_var('error','netiface','address'))),
					$interface_list),
	        $form->select(array('desc'	=> $this->bbf('fm_netiface_gateway'),
					'name'	=> 'netiface_id[gateway]',
					'labelid'	=> 'netiface-gateway',
					'comment'	=> $this->bbf('cmt_fm_netiface_gateway'),
					'default'	=> $element['netiface_id']['gateway'],
					'empty'	=> false,
					'selected'	=> $info['netiface_id']['gateway'],
					'error'	=> $this->bbf_args('error_generic',
										   $this->get_var('error','netiface','gateway'))),
					$gateway_list);

?>
</fieldset>

<fieldset id="fld-mainconfig-resolvconf">
	<legend><?=$this->bbf('fld-mainconfig-resolvconf');?></legend>
<?php

echo	$form->text(array('desc'	=> $this->bbf('fm_resolvconf_nameserver1'),
			  'name'	=> 'resolvconf[nameserver1]',
			  'labelid'	=> 'resolvconf-nameserver1',
			  'size'	=> 15,
#			  'help'	=> $this->bbf('hlp_fm_resolvconf_nameserver1'),
			  'comment'	=> $this->bbf('cmt_fm_resolvconf_nameserver1'),
			  'default'	=> $element['resolvconf']['nameserver1'],
			  'value'	=> $this->get_var('info','resolvconf','nameserver1'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','resolvconf','nameserver1')))),

	$form->text(array('desc'	=> $this->bbf('fm_resolvconf_nameserver2'),
			  'name'	=> 'resolvconf[nameserver2]',
			  'labelid'	=> 'resolvconf-nameserver2',
			  'size'	=> 15,
#			  'help'	=> $this->bbf('hlp_fm_resolvconf_nameserver2'),
			  'comment'	=> $this->bbf('cmt_fm_resolvconf_nameserver2'),
			  'default'	=> $element['resolvconf']['nameserver2'],
			  'value'	=> $this->get_var('info','resolvconf','nameserver2'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','resolvconf','nameserver2'))));

?>
</fieldset>

<fieldset id="fld-mainconfig-defaultconfig">
<legend><?=$this->bbf('fld-mainconfig-defaultconfig');?></legend>
    <div class="fm-desc-inline fm-paragraph">

<?php
echo	$form->checkbox(array('paragraph'	=> false,
			      'desc'		=> $this->bbf('fm_mainconfig-defaultconfigbox'),
			      'name'		=> 'default_configuration',
			      'labelid'		=> 'default_configuration',
			      'help'		=> $this->bbf('fm_mainconfig-defaultconfighelptext'),
			      'checked'		=> ($element['mainconfig']['default_configuration']) ? true : false  ,,));

?>
    </div>
</fieldset>

