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

$form = &$this->get_module('form');
$url = &$this->get_module('url');

$act = $this->get_var('act');
$info = $this->get_var('info');
$error = $this->get_var('error');
$plugininstalled = $this->get_var('plugininstalled');
$listconfigdevice = $this->get_var('listconfigdevice');
$listline = $this->get_var('list');

dwho::load_class('dwho_sort');
$pluginsort = new dwho_sort(array('key' => 'name'));
usort($plugininstalled,array($pluginsort,'str_usort'));

$switchboard_plugins = array();
$basestr = $info['device']['vendor'].', '.$info['device']['model'].',';
$basestr_len = strlen($basestr);
foreach ($plugininstalled as $plugin)
{
	foreach ($plugin['capabilities'] as $k => $v)
	{
		if (substr($k, 0, $basestr_len) === $basestr
		&& isset($v['switchboard'])
		&& $v['switchboard'] === true)
		{
			array_push($switchboard_plugins, $plugin['name']);
		}
	}
}

?>

<script>
var switchboard_plugins = <?php echo json_encode($switchboard_plugins); ?>;
var checkbox_state = <?php echo json_encode(dwho_bool($this->get_var('info','device','options','switchboard'))); ?>;

$(document).ready(function() {
	update_switchboard_checkbox(switchboard_plugins, checkbox_state);

	$('#it-device-switchboard-id').change(function() {
		checkbox_state = $('#it-device-switchboard-id').attr('checked');
	});

	$('#it-device-plugin').change(function() {
		update_switchboard_checkbox(switchboard_plugins, checkbox_state);
	});
});
</script>

<div id="sb-part-first" class="b-nodisplay">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_device_ip'),
				  'name'	=> 'device[ip]',
				  'labelid'	=> 'device-ip',
				  'size'	=> 15,
				  'readonly'=> ($act === 'add') ? false : true,
				  'value'	=> $this->get_var('info','device','ip'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'device', 'ip')) )),

		$form->text(array('desc'	=> $this->bbf('fm_device_mac'),
				  'name'	=> 'device[mac]',
				  'labelid'	=> 'device-mac',
				  'size'	=> 15,
				  'readonly'=> ($act === 'add') ? false : true,
				  'value'	=> $this->get_var('info','device','mac'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'device', 'mac')) )),

		$form->select(array('desc'	=> $this->bbf('fm_device_plugin'),
				  'name'	=> 'device[plugin]',
				  'labelid'	=> 'device-plugin',
				  'empty'	=> true,
				  'key'		=> 'name',
				  'altkey'	=> 'name',
				  'selected'	=> $this->get_var('info','device','plugin'),
				  'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'device', 'plugin'))),
			      $plugininstalled),

		$form->select(array('desc'	=> $this->bbf('fm_device_configdevice'),
				  'name'	=> 'device[template_id]',
				  'labelid'	=> 'device-template_id',
				  'key'		=> 'label',
				  'altkey'	=> 'id',
				  'selected'	=> $this->get_var('info','device','template_id')),
			      $listconfigdevice),

		$form->checkbox(array('desc'	=> $this->bbf('fm_device_switchboard'),
				'name'		=> 'device[options][switchboard]',
				'labelid'	=> 'device-switchboard-id'));
?>
	<div class="fm-paragraph fm-description">
		<p>
			<label id="lb-userfeatures-description" for="it-userfeatures-description"><?=$this->bbf('fm_userfeatures_description');?></label>
		</p>
		<?=$form->textarea(array('paragraph' => false,
					 'label'	=> false,
					 'name'		=> 'device[description]',
					 'labelid'	=> 'device-description',
					 'cols'		=> 60,
					 'rows'		=> 5,
					 'error'	=> $this->bbf_args('error',
						   $this->get_var('error', 'device', 'description')) ),
				   $this->get_var('info','device','description'));?>
	</div>
</div>
<div id="sb-part-last" class="b-nodisplay">
<?php

$nbcap = $busy = 0;
if (isset($info['capabilities'])
&& ($capabilities = $info['capabilities']) !== false):
	if(isset($capabilities['sip.lines']) === true
	&& ($nbcap = (int) $capabilities['sip.lines']) !== 0):
		if (empty($listline) === false)
			$busy = count($listline);
		echo $this->bbf('nb_line_busy-free',array($busy,$nbcap-$busy));
	endif;
endif;

?>
<div class="sb-list">
<table>
	<thead>
	<tr class="sb-top">
		<th class="th-left"><?=$this->bbf('col_line-line');?></th>
		<th class="th-center"><?=$this->bbf('col_line-protocol');?></th>
		<th class="th-center"><?=$this->bbf('col_line-name');?></th>
		<th class="th-center"><?=$this->bbf('col_line-number');?></th>
		<th class="th-center"><?=$this->bbf('col_line-context');?></th>
	</tr>
	</thead>
	<tbody>
<?php
if($listline !== false
&& ($nb = count($listline)) !== 0):
	foreach($listline as $num => $line):
		$secureclass = '';
		if(isset($ref['encryption']) === true
		&& $ref['encryption'] === true)
			$secureclass = 'xivo-icon xivo-icon-secure';
?>
	<tr class="fm-paragraph">
		<td class="td-left"><?=$line['num']?></td>
		<td class="txt-center">
			<span>
				<span class="<?=$secureclass?>">&nbsp;</span>
				<?=$this->bbf('line_protocol-sip')?>
			</span>
		</td>
		<td><?=$line['callerid']?></td>
		<td class>
		<?php 	
			echo $url->href_html(
                        	$line['number'],
                        	'service/ipbx/pbx_settings/lines',
                        	array(
                        	'id' => $line['id'],
                        	'act' => 'edit'
                        ));
		?></td>
		<td><?=$line['context']?></td>
	</tr>
<?php
	endforeach;
endif;
?>
	</tbody>
	<tfoot>
	<tr id="no-device"<?=($listline !== false ? ' class="b-nodisplay"' : '')?>>
		<td colspan="7" class="td-single"><?=$this->bbf('no_lines');?></td>
	</tr>
	</tfoot>
</table>
</div>
</div>
