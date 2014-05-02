<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2014  Avencall
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

$form    = &$this->get_module('form');
$dhtml   = &$this->get_module('dhtml');

$element = $this->get_var('element');
$event   = $this->get_var('event');

echo	'<div id="fd-dialaction-',$event,'-endcall-actiontype" class="b-nodisplay">',
	$form->select(array('desc'	=> $this->bbf('fm_dialaction_endcall-action'),
			    'name'	=> 'dialaction['.$event.'][action]',
			    'labelid'	=> 'dialaction-'.$event.'-endcall-action',
			    'key'	=> false,
			    'bbf'	=> 'fm_dialaction_endcall-action-opt',
			    'bbfopt'	=> array('argmode' => 'paramvalue'),
			    'default'	=> $element['dialaction']['endcall']['default'],
			    'selected'	=> $this->get_var('dialaction',$event,'endcall','action')),
		      $element['dialaction']['endcall']['value'],
		      'onchange="xivo_ast_chg_dialaction_actionarg(\''.$dhtml->escape($event).'\',\'endcall\');"'),
	$form->text(array('desc'	=> $this->bbf('fm_dialaction_endcall-busy-actionarg1'),
			  'name'	=> 'dialaction['.$event.'][actionarg1]',
			  'labelid'	=> 'dialaction-'.$event.'-endcall-busy-actionarg1',
			  'size'	=> 10,
			  'value'	=> $this->get_var('dialaction',$event,'busy','actionarg1'))),
	$form->text(array('desc'	=> $this->bbf('fm_dialaction_endcall-congestion-actionarg1'),
			  'name'	=> 'dialaction['.$event.'][actionarg1]',
			  'labelid'	=> 'dialaction-'.$event.'-endcall-congestion-actionarg1',
			  'size'	=> 10,
			  'value'	=> $this->get_var('dialaction',$event,'congestion','actionarg1')));
echo	'</div>';

?>
