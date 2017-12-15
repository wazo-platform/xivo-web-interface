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

$element = $this->get_var('element');
$info = $this->get_var('info');

$yesno = array($this->bbf('no'), $this->bbf('yes'));

?>

<div id="sb-part-first">
<?=
	$form->select(array('desc'	=> $this->bbf('fm_contexts_name'),
			    'name'	=> 'contexts-name',
			    'labelid'	=> 'contexts-name',
			    'key'	=> false,
			    'selected'	=> $info['cticontexts']['name']),
		      $info['displays']['pbxctx']);
?>
<?=
	$form->select(array('desc'	=> $this->bbf('fm_contexts_display'),
			    'name'	=> 'contexts-display',
			    'labelid'	=> 'contexts-display',
			    'key'	=> false,
			    'selected'	=> $info['cticontexts']['display']),
		      $info['displays']['list']);
?>
	<div class="fm-paragraph fm-description">
		<fieldset id="cti-contexts_services">
			<legend><?=$this->bbf('cti-contexts-directories');?></legend>
			<div id="contexts_services" class="fm-paragraph fm-description">
<?=
	$form->jq_select(array('paragraph'	=> false,
			       'label'		=> false,
			       'name'		=> 'directories[]',
			       'id'		=> 'it-directorieslist',
			       'selected'	=> $info['directories']['slt'],
			       'key'		=> false),
			 $info['directories']['list']);
?>
			</div>
		</fieldset>
	</div>

	<div class="col-sm-offset-2 fm-paragraph fm-description">
		<p>
			<label id="lb-contexts-description" for="it-contexts-description"><?=$this->bbf('fm_contexts_description');?></label>
		</p>
<?=
	$form->textarea(array('paragraph'	=> false,
			      'label'		=> false,
			      'name'		=> 'contexts[description]',
			      'id'		=> 'it-contexts-description',
			      'cols'		=> 60,
			      'rows'		=> 5,
			      'default'	=> $element['cticontexts']['description']['default']),
			$info['cticontexts']['description']);
?>
	</div>
</div>

<div class="fm-paragraph fm-description"><p><?=$this->bbf('need-xivo-dird-restart');?></p></div>
