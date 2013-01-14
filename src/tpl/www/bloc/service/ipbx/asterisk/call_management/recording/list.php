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

$url = &$this->get_module('url');
$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');
//$act = $this->get_var('act');



function reformat_date($bad_formatted_date) {
	if($bad_formatted_date != null and $bad_formatted_date != '') {
		return date("Y-m-d H:i:s", strtotime($bad_formatted_date));
	}
	return '';
}

function table_header($this_local) 
{
?>
<div class="b-list">
<form>
<table id="table-main-listing">
	<tr class="sb-top">
		<th class="th-left xspan"><span class="span-left">&nbsp;</span></th>
		<th class="th-center"><?=$this_local->bbf('campaign_name');?></th>
		<th class="th-center"><?=$this_local->bbf('queue_name');?></th>
		<th class="th-center"><?=$this_local->bbf('start_date');?></th>
		<th class="th-center"><?=$this_local->bbf('end_date');?></th>
		<th class="th-center col-action"><?=$this_local->bbf('col_action');?></th>
		<th class="th-right xspan"><span class="span-right">&nbsp;</span></th>
	</tr>
<?php 
}

function print_recordings($recordings_list, $form, $url, $dhtml, $this_local) {
	if($recordings_list == null || ($nb = count($recordings_list)) == 0) {
		print_recordings_empty_list($this_local);
	} else {
		print_recordings_list($recordings_list, $nb, $form, $url, $dhtml, $this_local);
	}	
}
	
	
function print_recordings_error($errors_list) {
if($errors_list != null && !empty($errors_list)) {
	?>
		<div id="report-xivo-error" class="xivo-error xivo-messages">
			<ul>
			<?php 
			foreach($errors_list as $error) {
				echo "<li>$error</li>";
			}?>
			
			</ul>
		</div>
		<?php 
	}
}

function print_recordings_empty_list($this_local)
{
?>	
		<tr class="sb-content">
			<td colspan="10" class="td-single"><?php echo $this_local->bbf('no_recording_campaign'); ?></td>
		</tr>
<?php 
}

function print_recordings_list($recordings_list, $nb, $form, $url, $dhtml, $this_local)
{
	for($i = 0;$i < $nb;$i++):

		$recording = get_object_vars(&$recordings_list[$i]);

		if($recording['activated'] === true):
			$icon = 'disable';
		else:
			$icon = 'enable';
		endif;
			
?>
	<tr onmouseover="this.tmp = this.className; this.className = 'sb-content l-infos-over';"
	    onmouseout="this.className = this.tmp;"
	    class="sb-content l-infos-<?=(($i % 2) + 1)?>on2">
		<td class="td-left">
			<?=$form->checkbox(array('name'		=> 'recordings[]',
						 'value'	=> $recording['campaign_id'],
						 'label'	=> false,
						 'id'		=> 'it-recordings-'.$i,
						 'checked'	=> false,
						 'paragraph'	=> false));?>
		</td>
	    <td class="txt-left curpointer"
	    	title="<?=dwho_alttitle($recording['campaign_id']);?>"
	    	onclick="location.href = dwho.dom.node.lastchild(this);">
<?php
				echo	$url->img_html('img/site/flag/'.$icon.'.gif',null,'class="icons-list"'),
					//dwho_trunc($recording['campaign_name'],15,'...',false);
				$url->href_html(dwho_trunc($recording['campaign_name'],40,'...',false),
						'service/ipbx/call_management/recording',
						array('act'	=> 'listrecordings',
								'campaign'	=> $recording['id']));
?>
			
		</td>
		<td>
			<?= $recording["queue_number"] . ": " .  $recording['queue_display_name']?>
		</td>
		<td>
		<?= reformat_date($recording['start_date'])	?>
		</td>
		<td><?= reformat_date($recording['end_date'])	?>
		</td>
		
		<td class="td-right" colspan="2">
<?php
			echo	$url->href_html($url->img_html('img/site/button/edit.gif',
							       $this_local->bbf('opt_modify'),
							       'border="0"'),
						'service/ipbx/call_management/recording',
						array('act'	=> 'edit',
						      'id'=> $recording['id']),
						null,
						$this_local->bbf('opt_modify')),"\n",
						$url->img_html('img/site/button/delete.gif',
							       $this_local->bbf('opt_delete'),
							       'border="0" onclick="alert(\''.$dhtml->escape($this_local->bbf('not_implemented')).'\');"');
						
?>
		</td>
	</tr>
<?php
		endfor;
}

function table_footer() 
{
?>
	<tr class="sb-foot">
		<td class="td-left xspan b-nosize"><span class="span-left b-nosize">&nbsp;</span></td>
		<td class="td-center" colspan="5"><span class="b-nosize">&nbsp;</span></td>
		<td class="td-right xspan b-nosize"><span class="span-right b-nosize">&nbsp;</span></td>
	</tr>
</table>
</form>
</div>
<?php 
}


table_header($this);
$errors_list = $this->get_var('errors_list');
print_recordings_error($errors_list);
$recordings_list = $this->get_var('recordingcampaigns');
print_recordings($recordings_list, $form, $url, $dhtml, $this);
table_footer();
