<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2015  Avencall
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

?>

<div class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>

<div class="sb-content">
<form action="#" method="post" accept-charset="utf-8">

<div id="sb-part-first">
<?php
echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1));
?>
	<fieldset id="cti-contexts_services">
		<legend><?=$this->bbf('cti-contexts-directories');?></legend>
		<div id="contexts_services" class="fm-paragraph fm-description">
<?=
	$form->jq_select(array('paragraph'	=> false,
						   'label'		=> false,
						   'name'		=> 'directories[]',
						   'id'		=> 'it-directorieslist',
						   'selected'	=> $info['directoriz']['slt'],
						   'key'		=> false),
					 $info['directoriz']['list']);
?>
		</div>
	</fieldset>
</div>

<div class="fm-paragraph fm-description"><p><?=$this->bbf('need-xivo-dird-restart');?></p></div>

<?php

echo	$form->submit(array('name'	=> 'submit',
			    'id'	=> 'it-submit',
			    'value'	=> $this->bbf('fm_bt-save')));

?>
</form>
	</div>
	<div class="sb-foot xspan"></div>
</div>
