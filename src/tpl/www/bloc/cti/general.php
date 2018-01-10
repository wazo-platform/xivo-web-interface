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

$url = &$this->get_module('url');
$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');

$info = $this->get_var('info');
$element = $this->get_var('element');

$error = $this->get_var('error');
$listaccount = $this->get_var('listaccount');

$error_js = array();
$error_nb = count($error['ctimain']);

for($i = 0;$i < $error_nb;$i++):
	$error_js[] = 'dwho.form.error[\'it-ctimain-'.$error['ctimain'][$i].'\'] = true;';
endfor;

if(isset($error_js[0]) === true)
	$dhtml->write_js($error_js);

?>
<div class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>
<div class="sb-content">
<form action="#" method="post" accept-charset="utf-8" class="form-horizontal">
<?php
	echo
		$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),
		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1));
?>

<div id="sb-part-first">

<fieldset id="cti-start-tls">
	<legend><?=$this->bbf('cti-start-tls');?></legend>
	<?=
		$form->checkbox(array(
			'name' => 'cti[ctis_active]',
			'checked' => $info['ctimain']['ctis_active'],
			'label' => $this->bbf('fm_start_tls'),
			'id' => 'it-ctis_active',
			'desc' => $this->bbf('fm_start_tls'))),
		$form->select(array(
			'desc' => $this->bbf('fm_tlscertfile'),
			'name' => 'cti[tlscertfile]',
			'labelid' => 'tlscertfile',
			'key' => 'name',
			'altkey' => 'path',
			'empty' => true,
			'label' => $this->bbf('fm_tlscertfile'),
			'selected'=> $this->get_var('info','ctimain','tlscertfile'),
			'default' => $element['ctimain']['tlscertfile']['default']),
		$this->get_var('tlscertfiles')),
		$form->select(array(
			'desc' => $this->bbf('fm_tlsprivkeyfile'),
			'name' => 'cti[tlsprivkeyfile]',
			'labelid' => 'tlsprivkeyfile',
			'key' => 'name',
			'altkey' => 'path',
			'empty' => true,
			'help' => $this->bbf('hlp_fm_tlsprivkeyfile'),
			'selected' => $this->get_var('info','ctimain','tlsprivkeyfile'),
			'default' => $element['ctimain']['tlsprivkeyfile']['default']),
		$this->get_var('tlsprivkeyfiles'));
	?>
</fieldset>
<?php
	echo	$form->checkbox(array('desc' => $this->bbf('fm_cti_context_separation'),
							'name' => 'cti[context_separation]',
							'labelid' => 'context_separation',
							'checked' => $info['ctimain']['context_separation']));
?>
</div>

<?php
	echo	$form->submit(array('name'	=> 'submit',
				    'id'	=> 'it-submit',
				    'value'	=> $this->bbf('fm_bt-save')));
?>
</form>

	</div>
	<div class="sb-foot xspan"></div>
</div>
