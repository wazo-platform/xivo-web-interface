<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016 Avencall
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
$list = $this->get_var('list');
$element = $this->get_var('element');

$list_configregistrar = $this->get_var('list_configregistrar');
$list_device_line = $this->get_var('list_device_line');
$err = $this->get_var('error','linefeatures');

$list_device = array();
$nb_device = count($list_device_line);
for($i=0; $i<$nb_device; $i++):
	$cur_device = $list_device_line[$i];
	$trimmed_mac = trim($cur_device['mac']);
	$trimmed_ip = trim($cur_device['ip']);
	if(empty($trimmed_mac) === false) {
		$cur_device['display'] = 'MAC: '.$cur_device['mac'];
		$list_device[$i] = $cur_device;
	} else if(empty($trimmed_ip) === false) {
		$cur_device['display'] = 'IP: '.$cur_device['ip'];
		$list_device[$i] = $cur_device;
	}
endfor;

?>
<span id="box-entityid"></span>
<table id="list_linefeatures">
	<thead>
	<tr class="sb-top">
		<th class="th-center"><?=$this->bbf('col_line-protocol');?></th>
		<th class="th-center"><?=$this->bbf('col_line-name');?></th>
		<th class="th-center"><?=$this->bbf('col_line-context');?></th>
		<th class="th-center"><?=$this->bbf('col_line-number');?></th>
		<th class="th-center"><?=$this->bbf('col_line-config_registrar');?></th>
		<th class="th-center"><?=$this->bbf('col_line-device');?></th>
		<th class="th-center"><?=$this->bbf('col_line-num');?></th>
		<th class="th-right">
			<?=$url->href_html($url->img_html('img/site/button/mini/orange/bo-add.gif',
								       $this->bbf('col_line-add'),
								       'border="0"'),
							'#lines',
							null,
							'id="lnk-add-row"',
							$this->bbf('col_line-add'));?>
		</th>
	</tr>
	</thead>
	<tbody id="linefeatures">
<?php
if($list !== false):

	$rs = array();
	$nb = $this->get_var('count');
	for($i = 0;$i < $nb;$i++):
		$ref = &$list[$i];

		if(isset($err[$i]) === true):
			$ref['errdisplay'] = ' l-infos-error';
		else:
			$ref['errdisplay'] = '';
		endif;

		$secureclass = '';
		if(isset($ref['encryption']) === true
		&& $ref['encryption'] === true)
			$secureclass = 'xivo-icon xivo-icon-secure';
?>
	<tr class="fm-paragraph<?=$ref['errdisplay']?>">
		<td class="td-left">
			<?=$form->hidden(array('name' => 'linefeatures[id][]',
						'value' 	=> $ref['id']));?>
			<?=$form->hidden(array('name' => 'linefeatures[protocol][]',
				    	'id'		=> 'linefeatures-protocol',
						'value' 	=> $ref['protocol']));?>
			<?=$form->hidden(array('name' => '',
						'id' 		=> 'context-selected',
						'value' 	=> $ref['context']));?>
			<?=$form->hidden(array('name' => 'linefeatures[name][]',
				    	'id'		=> 'linefeatures-name',
						'value' 	=> $ref['name']));?>
			<span>
				<span class="<?=$secureclass?>">&nbsp;</span>
				<?=$this->bbf('line_protocol-'.$ref['protocol'])?>
			</span>
		</td>
		<td>
			<?=$url->href_html($ref['name'],
				'service/ipbx/pbx_settings/lines',
				array('act' => 'edit', 'id' => $ref['id']));?>
		</td>
		<td>
			<?=$form->select(array('paragraph'	=> false,
					    'name'		=> 'linefeatures[context][]',
					    'id'		=> 'linefeatures-context',
					    'label'		=> false));?>
		</td>
		<td>
			<?=$form->text(array('paragraph'	=> false,
					     'name'		=> 'linefeatures[number][]',
				   		 'id'		=> 'linefeatures-number',
					     'label'	=> false,
					     'size'		=> 5,
					     'value'	=> $ref['number'],
					     'default'	=> ''),
			            'autocomplete="off"');?>
			<div class="dialog-helper" id="numberpool_helper"></div>
		</td>
		<td>
			<?=$form->select(array('paragraph'	=> false,
				    'name'		=> 'linefeatures[configregistrar][]',
				    'id'		=> 'linefeatures-configregistrar',
				    'label'		=> false,
				    'key'		=> 'displayname',
				    'altkey'	=> 'id',
				    'default'	=> 'default',
					'selected'	=> $ref['configregistrar']),
			      $list_configregistrar);?>
		</td>
		<td>
		<?php if ($list_device === false): ?>
			 -
			<?=$form->hidden(array('name' => 'linefeatures[device][]',
					'value' 	=> null,
				    'id'		=> 'linefeatures-device'));?>
		<?php else: ?>
			<?=$form->select(array('paragraph'	=> false,
				    'name'		=> 'linefeatures[device][]',
						'id'		=> 'linefeatures-device',
						'class' => 'linefeatures-device-select2',
						'label'		=> false,
				    'key'		=> 'display',
				    'altkey'	=> 'id',
				    'empty'		=> true,
				    'default'	=> '',
						'selected'	=> $ref['device']),
			      $list_device);?>		
		<?php endif; ?>
		</td>
		<td>
			&nbsp;
			<?=$form->select(array('paragraph'	=> false,
					    'name'		=> 'linefeatures[num][]',
					    'id'		=> 'linefeatures-num',
					    'label'		=> false,
				    	'key'		=> false,
				    	'altkey'	=> false,
						'selected'	=> $ref['num']),
						$element['userfeatures']['linefeatures-num']['value']);?>

		</td>
		<td class="td-right">
			<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
						       $this->bbf('opt_line-delete'),
						       'border="0"'),
							'#lines',
							null,
							'onclick="lnkdroprow(this);"',
							$this->bbf('opt_line-delete'));?>
		</td>
	</tr>
<?php
	endfor;
endif;
?>
	</tbody>
	<tfoot>
	<tr id="no-linefeatures"<?=($list !== false ? ' class="b-nodisplay"' : '')?>>
		<td colspan="8" class="td-single"><?=$this->bbf('no_linefeatures');?></td>
	</tr>
	<tr id="save-before-add-linefeatures" class="b-nodisplay">
		<td colspan="8" class="td-single"><?=$this->bbf('save_before_add_linefeatures');?></td>
	</tr>
	</tfoot>
</table>

<table class="b-nodisplay">
	<tbody id="ex-linefeatures">
	<tr class="fm-paragraph">
		<td class="td-left" id="td_ex-linefeatures-protocol">
			<?=$form->hidden(array('name' => 'linefeatures[id][]',
						'value' 	=> 0,
					    'id'		=> 'linefeatures-id'));?>
			<?=$form->select(array('paragraph'	=> false,
					'name'		=> 'linefeatures[protocol][]',
					'id'		=> 'linefeatures-protocol',
					'label'		=> false,
					'key'		=> false,
					'bbf'		=> 'line_protocol-opt',
					'default'	=> 'sip'),
				array('sip', 'sccp', 'custom'));?>
		</td>
		<td id="td_ex-linefeatures-name">
			<?=$form->hidden(array('name' => 'linefeatures[name][]',
					'value' 	=> null,
					'id'		=> 'linefeatures-name'));?>
		</td>
		<td id="td_ex-linefeatures-context">
			<?=$form->select(array('paragraph'	=> false,
				    'name'		=> 'linefeatures[context][]',
				    'id'		=> 'linefeatures-context',
				    'label'		=> false,
				    'default'	=> 'default'));?>
		</td>
		<td id="td_ex-linefeatures-number">
			<?=$form->text(array('paragraph'	=> false,
					     'name'		=> 'linefeatures[number][]',
					     'id'		=> 'linefeatures-number',
					     'label'	=> false,
					     'size'		=> 5,
					     'default'	=> ''),
			            'autocomplete="off"');?>
			<div class="dialog-helper" id="numberpool_helper"></div>
		</td>
		<td id="td_ex-linefeatures-configregistrar">
			<?=$form->select(array('paragraph'	=> false,
				    'name'		=> 'linefeatures[configregistrar][]',
				    'id'		=> 'linefeatures-configregistrar',
				    'label'		=> false,
				    'key'		=> 'displayname',
				    'altkey'	=> 'id',
				    #'empty'	=> true,
				    'default'	=> 'default'),
			      $list_configregistrar);?>
		</td>
		<td id="td_ex-linefeatures-device">
		<?php if ($list_device === false): ?>
			 -
			<?=$form->hidden(array('name' => 'linefeatures[device][]',
					'value' 	=> null,
				    'id'		=> 'linefeatures-device'));?>
		<?php else: ?>
			<?=$form->select(array('paragraph'	=> false,
				    'name'		=> 'linefeatures[device][]',
				    'id'		=> 'linefeatures-device',
				    'label'		=> false,
				    'key'		=> 'display',
				    'altkey'	=> 'id',
				    'default'	=> '',
				    'empty'		=> true),
			      $list_device);?>
		<?php endif; ?>
		</td>
		<td>
			&nbsp;
		<?php if ($list_device === false): ?>
			 -
			<?=$form->hidden(array('name' => 'linefeatures[num][]',
					'value' 	=> null,
				    'id'		=> 'linefeatures-num'));?>
		<?php else: ?>
			<?=$form->select(array('paragraph'	=> false,
					    'name'		=> 'linefeatures[num][]',
					    'id'		=> 'linefeatures-num',
					    'label'		=> false,
				    	'key'		=> false,
				    	'altkey'	=> false,
				   		'default'	=> 1),
							 $element['userfeatures']['linefeatures-num']['value']);?>
		<?php endif; ?>
		</td>
		<td class="td-right">
			<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
							       $this->bbf('opt_row-delete'),
							       'border="0"'),
							'#lines',
							null,
							'onclick="lnkdroprow(this);"',
							$this->bbf('opt_row-delete'));?>
		</td>
	</tr>
	</tbody>
</table>

