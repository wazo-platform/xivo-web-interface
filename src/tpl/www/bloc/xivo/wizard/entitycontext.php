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

$element = $this->get_var('element');

?>
<fieldset id="fld-entity">
	<legend><?=$this->bbf('fld-entity');?></legend>
<?php

echo    $form->text(array('desc'	=> $this->bbf('fm_entity_displayname'),
			  'name'	=> 'entity[displayname]',
			  'labelid'	=> 'entity-displayname',
			  'size'	=> 15,
			  'required'=> true,
			  'comment'	=> $this->bbf('cmt_fm_entity_displayname'),
			  'value'	=> $this->get_var('info','entity','displayname'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error','entity','displayname'))));

?>
</fieldset>

<fieldset id="fld-context-internal">
	<legend><?=$this->bbf('fld-context-internal');?></legend>
<?php

echo	$form->text(array('desc'	=> $this->bbf('fm_context_internal-displayname'),
			  'name'	=> 'context[internal][displayname]',
			  'labelid'	=> 'context-internal-displayname',
			  'size'	=> 15,
			  'required'=> true,
			  'comment'	=> $this->bbf('cmt_fm_context_internal-displayname'),
			  'default'	=> $this->bbf('fm_context_internal-displayname-default'),
			  'value'	=> $this->get_var('info','context_internal','displayname'),
			  'error'	=> $this->bbf_args('error_generic',
							   is_string($this->get_var('error', 'context_internal', 'displayname'))))),


$form->text(array('desc'	=> $this->bbf('fm_context_internal-numberbeg'),
			  'name'	=> 'context[internal][numberbeg]',
			  'labelid'	=> 'context-internal-numberbeg',
			  'size'	=> 15,
			  'required'=> true,
			  'comment'	=> $this->bbf('cmt_fm_context_internal-numberbeg'),
			  'default'	=> $element['context']['internal']['numberbeg']['default'],
			  'value'	=> $this->get_var('info','context_internal','numberbeg'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error', 'context_internal', 'numberbeg')))),
										    
	$form->text(array('desc'	=> $this->bbf('fm_context_internal-numberend'),
			  'name'	=> 'context[internal][numberend]',
			  'labelid'	=> 'context-internal-numberend',
			  'size'	=> 15,
			  'required'=> true,
			  'comment'	=> $this->bbf('cmt_fm_context_internal-numberend'),
			  'default'	=> $element['context']['internal']['numberend']['default'],
			  'value'	=> $this->get_var('info','context_internal','numberend'),
			  'error'	=> $this->bbf_args('error_fm_context_internal_numberend',
							   is_string($this->get_var('error', 'context_internal', 'numberend')))));

?>
</fieldset>

<fieldset id="fld-context-incall">
	<legend><?=$this->bbf('fld-context-incall');?></legend>
<?php

echo    $form->text(array('desc'	=> $this->bbf('fm_context_incall-displayname'),
			  'name'	=> 'context[incall][displayname]',
			  'labelid'	=> 'context-incall-displayname',
			  'size'	=> 15,
			  'required'=> true,
			  'comment'	=> $this->bbf('cmt_fm_context_incall-displayname'),
			  'default'	=> $this->bbf('fm_context_incall-displayname-default'),
			  'value'	=> $this->get_var('info','context_incall','displayname'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error', 'context_incall', 'displayname')))),

	$form->text(array('desc'	=> $this->bbf('fm_context_incall-numberbeg'),
			  'name'	=> 'context[incall][numberbeg]',
			  'labelid'	=> 'context-incall-numberbeg',
			  'size'	=> 15,
			  'comment'	=> $this->bbf('cmt_fm_context_incall-numberbeg'),
			  'default'	=> $element['context']['incall']['numberbeg']['default'],
			  'value'	=> $this->get_var('info','context_incall','numberbeg'),
			  'error'	=> $this->bbf_args('error_generic',
							   $this->get_var('error', 'context_incall', 'numberbeg')))),

	$form->text(array('desc'	=> $this->bbf('fm_context_incall-numberend'),
			  'name'	=> 'context[incall][numberend]',
			  'labelid'	=> 'context-incall-numberend',
			  'size'	=> 15,
			  'comment'	=> $this->bbf('cmt_fm_context_incall-numberend'),
			  'default'	=> $element['context']['incall']['numberend']['default'],
			  'value'	=> $this->get_var('info','context_incall','numberend'),
			  'error'	=> $this->bbf_args('error_fm_context_incall_numberend',
							   is_string($this->get_var('error', 'context_incall', 'numberend'))))),

	$form->select(array('desc'	=> $this->bbf('fm_context_incall-didlength'),
			    'name'	=> 'context[incall][didlength]',
			    'labelid'	=> 'context-incall-didlength',
			    'key'	=> false,
			    'comment'	=> $this->bbf('cmt_fm_context_incall-didlength'),
			    'default'	=> $element['context']['incall']['didlength']['default'],
			    'selected'	=> $this->get_var('info','context_incall','didlength')),
		      $element['context']['incall']['didlength']['value']);

?>
</fieldset>

<fieldset id="fld-context-outcall">
	<legend><?=$this->bbf('fld-context-outcall');?></legend>
<?php

echo    $form->text(array('desc'	=> $this->bbf('fm_context_outcall-displayname'),
			  'name'	=> 'context[outcall][displayname]',
			  'labelid'	=> 'context-outcall-displayname',
			  'size'	=> 15,
			  'required'=> true,
			  'comment'	=> $this->bbf('cmt_fm_context_outcall-displayname'),
              'default'	=> $this->bbf('fm_context_outcall-displayname-default'),
			  'value'	=> $this->get_var('info','context_outcall','displayname'),
			  'error'	=> $this->bbf_args('error_generic',
							   is_string($this->get_var('error', 'context_outcall', 'displayname')))));

?>
</fieldset>
