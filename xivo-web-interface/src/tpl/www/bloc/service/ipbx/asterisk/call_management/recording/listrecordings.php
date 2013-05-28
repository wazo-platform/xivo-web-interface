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
$params = $this->get_var('params');
$pager = $this->get_var('pager');

$exception_message = "";


function table_header($this_local, $page) 
{
echo '<div class="b-list">';
if($page !== '')
	echo '<div class="b-page">',$page,'</div>';
?>
<form>
<table id="table-main-listing">
	<tr class="sb-top">
		<th class="th-left xspan"><span class="span-left">&nbsp;</span></th>
		<th class="th-center"><?=$this_local->bbf('start_time');?></th>
		<th class="th-center"><?=$this_local->bbf('client_id');?></th>
		<th class="th-center"><?=$this_local->bbf('caller');?></th>
		<th class="th-center"><?=$this_local->bbf('agent');?></th>
		<th class="th-center col-action"><?=$this_local->bbf('col_action');?></th>
		<th class="th-right xspan"><span class="span-right">&nbsp;</span></th>
	</tr>
<?php 
}

function print_recordings($recordings_list, $form, $url, $dhtml, $this_local) {
	if($recordings_list === false) {
		print_recordings_error($this_local);
	}	elseif (($nb = count($recordings_list)) === 0) {
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
			<td colspan="10" class="td-single"><?php echo $this_local->bbf('no_recording'); ?></td>
		</tr>
<?php 
}

function print_recordings_list($recordings_list, $nb, $form, $url, $dhtml, $this_local)
{
	for($i = 0;$i < $nb;$i++){
		$recording = get_object_vars($recordings_list[$i]);
				
	?>
		<tr onmouseover="this.tmp = this.className; this.className = 'sb-content l-infos-over';"
		    onmouseout="this.className = this.tmp;"
		    class="sb-content l-infos-<?=(($i % 2) + 1)?>on2">
			<td class="td-left">
				<?=$form->checkbox(array('name'		=> 'recordings[]',
							 //'value'	=> $recording['file_name'],
							 'label'	=> false,
							 'id'		=> 'it-recordings-'.$i,
							 'checked'	=> false,
							 'paragraph'	=> false));?>
			</td>
			<td class="txt-left" title="<?=dwho_alttitle($recording['caller']);?>">
				<label for="it-recordings-<?=$i?>" id="lb-recordings-<?=$i?>">		
	<?php
					echo	$url->img_html('img/site/flag/enable.gif',null,'class="icons-list"');
	?>
				</label>
				<?= $recording['start_time'] ?>
			</td>
			<td>
			<?= $recording['client_id']  ?>	
			</td>
		    <td>
			<?= $recording['caller']  ?>
				
			</td>
			<td>
				<?=
					$recording['agent_no']?>
			</td>
			<td class="td-right" colspan="2">
	<?php
				//display "download" button

	
	
				echo	$url->href_html($url->img_html('img/site/button/file.gif',
						$this_local->bbf('opt_download'),
						'border="0"'),
						'service/ipbx/call_management/recording',
						array('act' => 'download',
								'file' => $recording['filename']));
				echo '&nbsp';
				//display "delete" button
				echo	$url->href_html($url->img_html('img/site/button/delete.gif',
								       $this_local->bbf('opt_delete'),
								       'border="0"'),
							'service/ipbx/call_management/recording',
							array('act'	=> 'delete_recording',
							      'id'	=> $recording['cid'],
								  'campaign' => $recording['campaign_id']),
							'onclick="return(confirm(\''.$dhtml->escape($this_local->bbf('opt_delete_confirm')).'\'));"',
							$this_local->bbf('opt_delete'));
	?>
			</td>
		</tr>
	<?php
		}
}

function table_footer($page) 
{
?>
	<tr class="sb-foot">
		<td class="td-left xspan b-nosize"><span class="span-left b-nosize">&nbsp;</span></td>
		<td class="td-center" colspan="5"><span class="b-nosize">&nbsp;</span></td>
		<td class="td-right xspan b-nosize"><span class="span-right b-nosize">&nbsp;</span></td>
	</tr>
</table>
</form> 
<?php 
	if($page !== '') 
		echo '<div class="b-page">',$page,'</div>';
	echo '</div>';
}


$page = $url->pager($pager['pages'],
		$pager['page'],
		$pager['prev'],
		$pager['next'],
		'service/ipbx/call_management/recording',
		array('act' => 'listrecordings', $params));
table_header($this, $page);
$errors_list = $this->get_var('errors_list');
print_recordings_error($errors_list);
$recordings_list = $this->get_var('recordings');
print_recordings($recordings_list, $form, $url, $dhtml, $this);
table_footer($page);
paginate($page);
?>