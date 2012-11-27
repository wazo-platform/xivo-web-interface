<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2011  Avencall
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

$service = $this->get_var('service');
$presences = $this->get_var('presences_group');
$phonehints = $this->get_var('phonehints_group');
$xlet_layout = $this->get_var('xlet_layout');
$xlet = $this->get_var('xlet');
$preference = $this->get_var('preference');

$yesno = array($this->bbf('no'), $this->bbf('yes'));

?>

<div id="sb-part-first" class="b-nodisplay">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_profiles_name'),
				'name'	=> 'cti_profile[name]',
				'labelid'	=> 'profiles-name',
				'size'	=> 15,
				'default'	=> $element['cti_profile']['name']['default'],
				'value'	=> $info['cti_profile']['name']));
?>

	<fieldset id="cti-profiles_status">
	<legend><?=$this->bbf('cti-profiles-status');?></legend>
<?php
	echo $form->select(array('desc'		=> $this->bbf('fm_profiles_presence'),
					'name'		=> 'cti_profile[presence_id]',
					'id'		=> false,
					'label'		=> false,
					'key'		=> 'name',
					'altkey'	=> 'id',
					'selected'	=> $info['cti_profile']['presence_id']
			 ),$presences);

	echo $form->select(array('desc'		=> $this->bbf('fm_profiles_phonehints'),
					'name'		=> 'cti_profile[phonehints_id]',
					'id'		=> false,
					'label'		=> false,
					'key'		=> 'name',
					'altkey'	=> 'id',
					'selected'	=> $info['cti_profile']['phonehints_id']
			 ),$phonehints);
?>
	</fieldset>

	<fieldset>
	<legend><?=$this->bbf('cti-profiles-services')?></legend>
<?php
	if(isset($service['list']) === true
	&& $service['list'] !== false):
?>
		<div id="queuelist" class="fm-paragraph fm-description">
				<?=$form->jq_select(array('paragraph'	=> false,
							 	'label'		=> false,
								'name'		=> 'services[]',
								'id' 		=> 'it-services',
								'bbf'		=> 'service-',
								'key'		=> 'key',
								'altkey'	=> 'id',
								'selected'  => $service['slt']),
							$service['list']);?>
		</div>
		<div class="clearboth"></div>
<?php
	endif;
?>
	</fieldset>
</div>

<div id="sb-part-xlets" class="b-nodisplay">
<?php
	$type = 'xlets';
?>
	<div class="sb-list">
		<table>
			<thead>
			<tr class="sb-top">
				<th class="th-left"><?=$this->bbf('col_'.$type.'-name');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-args');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-f');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-c');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-m');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-s');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-num');?></th>
				<th class="th-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/orange/bo-add.gif',
									  $this->bbf('col_'.$type.'-add'),
									  'border="0"'),
								'#',
								null,
								'onclick="dwho.dom.make_table_list(\''.$type.'\',this); return(dwho.dom.free_focus());"',
								$this->bbf('col_'.$type.'-add'));?>
				</th>
			</tr>
			</thead>
			<tbody id="xlets">
<?php
		if ($info['xlet']):
			$count = count($info['xlet']);
			for($i = 0;$i < $count;$i++):
				$errdisplay = '';
?>
			<tr class="fm-paragraph<?=$errdisplay?>">
				<td class="td-left txt-center">
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[id]['.$i.']',
									'id'		=> false,
									'label'		=> false,
									'bbf'		=> 'xlet-',
									'key'		=> 'plugin_name',
									'altkey'	=> 'id',
									'selected'	=> $info['xlet'][$i]['xlet_id']),
								$xlet);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[layout]['.$i.']',
									'id'		=> false,
									'label'		=> false,
									'size'		=> 15,
									'key'		=> 'name',
									'altkey'	=> 'id',
									'selected'	=> $info['xlet'][$i]['layout_id']),
								$xlet_layout);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[floating][]',
									'id'		=> false,
									'label'		=> false,
									'selected'	=> $info['xlet'][$i]['floating']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[closable][]',
									'id'		=> false,
									'label'		=> false,
									'selected'	=> $info['xlet'][$i]['closable']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[movable][]',
									'id'		=> false,
									'label'		=> false,
									'selected'	=> $info['xlet'][$i]['movable']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[scrollable][]',
									'id'		=> false,
									'label'		=> false,
									'selected'	=> $info['xlet'][$i]['scrollable']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->text(array('paragraph'	=> false,
								'name'		=> 'xlet[order]['.$i.']',
								'id'		=> false,
								'label'		=> false,
								'size'		=> 4,
								'disabled'	=> false,
								'value'		=> $info['xlet'][$i]['order']));?>
				</td>
				<td class="td-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
									$this->bbf('opt_'.$type.'-delete'),
									'border="0"'),
								'#',
								null,
								'onclick="dwho.dom.make_table_list(\''.$type.'\',this,1); return(dwho.dom.free_focus());"',
								$this->bbf('opt_'.$type.'-delete'));?>
				</td>
			</tr>

<?php
			endfor;
		endif;
?>
			</tbody>
			<tfoot>
			<tr id="no-<?=$type?>"<?=($info['xlet']) ? ' class="b-nodisplay"' : ''?>>
				<td colspan="8" class="td-single"><?=$this->bbf('no_'.$type);?></td>
			</tr>
			</tfoot>
		</table>
		<table class="b-nodisplay">
			<tbody id="ex-<?=$type?>">
			<tr class="fm-paragraph">
				<td class="td-left txt-center">
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[id][]',
									'id'		=> false,
									'label'		=> false,
									'bbf'		=> 'xlet-',
									'key'		=> 'plugin_name',
									'altkey'	=> 'id'),
							$xlet);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[layout][]',
									'id'		=> false,
									'label'		=> false,
									'key'		=> 'name',
									'altkey'	=> 'id'),
							$xlet_layout);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[floating][]',
									'id'		=> false,
									'label'		=> false,
									'default'	=> $element['xlet']['floating']['default']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[closable][]',
									'id'		=> false,
									'label'		=> false,
									'default'	=> $element['xlet']['closable']['default']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[movable][]',
									'id'		=> false,
									'label'		=> false,
									'default'	=> $element['xlet']['movable']['default']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->select(array('paragraph'	=> false,
									'name'		=> 'xlet[scrollable][]',
									'id'		=> false,
									'label'		=> false,
									'default'	=> $element['xlet']['scrollable']['default']
							 ),
							 $yesno);?>
				</td>
				<td>
					<?=$form->text(array('paragraph'	=> false,
								 'name'		=> 'xlet[order][]',
								 'size'		=> 4));?>
				</td>
				<td class="td-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
									$this->bbf('opt_'.$type.'-delete'),
									'border="0"'),
									'#',
									null,
									'onclick="dwho.dom.make_table_list(\''.$type.'\',this,1); return(dwho.dom.free_focus());"',
									$this->bbf('opt_'.$type.'-delete'));?>
				</td>
			</tr>
			</tbody>
		</table>
	</div>
</div>

<?php
	$type = 'preferences';
?>
<div id="sb-part-last" class="b-nodisplay">
	<div class="sb-list">
		<table>
			<thead>
			<tr class="sb-top">
				<th class="th-left"><?=$this->bbf('col_'.$type.'-name');?></th>
				<th class="th-center"><?=$this->bbf('col_'.$type.'-args');?></th>
				<th class="th-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/orange/bo-add.gif',
									$this->bbf('col_'.$type.'-add'),
									'border="0"'),
									'#',
									null,
									'onclick="dwho.dom.make_table_list(\''.$type.'\',this); return(dwho.dom.free_focus());"',
									$this->bbf('col_'.$type.'-add'));?>
				</th>
			</tr>
			</thead>
			<tbody id="preferences">
<?php
		if ($info['preference']):
			$count = count($info['preference']);
			for($i = 0;$i < $count;$i++):
				$errdisplay = '';
?>
			<tr class="fm-paragraph<?=$errdisplay?>">
				<td class="td-left txt-center">
					<?=$form->select(array('paragraph'	=> false,
								'name'		=> 'preference[id][]',
								'id'		=> false,
								'label'		=> false,
								'bbf'		=> 'pref-',
								'key'		=> 'option',
								'altkey'	=> 'id',
								'selected'	=> $info['preference'][$i]['preference_id']),
							$preference);?>
				</td>
				<td>
					<?=$form->text(array('paragraph'	=> false,
								'name'		=> 'preference[value][]',
								'id'		=> false,
								'label'		=> false,
								'size'		=> 45,
								'value'		=> $info['preference'][$i]['value']));?>
				</td>
				<td class="td-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
									$this->bbf('opt_'.$type.'-delete'),
									'border="0"'),
									'#',
									null,
									'onclick="dwho.dom.make_table_list(\''.$type.'\',this,1); return(dwho.dom.free_focus());"',
									$this->bbf('opt_'.$type.'-delete'));?>
				</td>
			</tr>
<?php
			endfor;
		endif;
?>
			</tbody>
			<tfoot>
			<tr id="no-<?=$type?>"<?=($info['preference']) ? ' class="b-nodisplay"' : ''?>>
				<td colspan="3" class="td-single"><?=$this->bbf('no_'.$type);?></td>
			</tr>
			</tfoot>
		</table>
		<table class="b-nodisplay">
			<tbody id="ex-<?=$type?>">
			<tr class="fm-paragraph">
				<td class="td-left txt-center">
					<?=$form->select(array('paragraph'	=> false,
								'name'		=> 'preference[id][]',
								'id'		=> false,
								'label'		=> false,
								'bbf'		=> 'pref-',
								'key'		=> 'option',
								'altkey'	=> 'id'),
							$preference);?>
				</td>
				<td>
					<?=$form->text(array('paragraph'	=> false,
								 'name'		=> 'preference[value][]',
								 'id'		=> false,
								 'label'	=> false,
								 'size'		=> 15));?>
				</td>
				<td class="td-right">
					<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
									$this->bbf('opt_'.$type.'-delete'),
									 'border="0"'),
									'#',
									null,
									'onclick="dwho.dom.make_table_list(\''.$type.'\',this,1); return(dwho.dom.free_focus());"',
									$this->bbf('opt_'.$type.'-delete'));?>
				</td>
			</tr>
			</tbody>
		</table>
	</div>
</div>
