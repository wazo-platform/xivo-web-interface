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
$queues_list = $this->get_var('queues_list');
$act = $this->get_var('act');
$info = $this->get_var('info');
$errors_list = $this->get_var('errors_list');
$supported_errors = array('duplicated_name', 'empty_name',
		'start_greater_than_end', 'concurrent_campaigns', 'invalid_date_format');

function reformat_date($bad_formatted_date = null) {
	if($bad_formatted_date != null and $bad_formatted_date != '') {
		return date("Y-m-d H:i:s", strtotime($bad_formatted_date));
	}
	return '';
}

if($errors_list != null && !empty($errors_list)) {
	?>
	<div id="report-xivo-error" class="xivo-error xivo-messages">
		<ul>
		<?php 
		foreach($errors_list as $error) {
			if(in_array($error, $supported_errors)) {
				echo "<li>".$this->bbf($error)."</li>";
			} else {
				echo "<li>Error: $error</li>";
			}
			
		}?>
		
		</ul>
	</div>
	<?php 
}
?>
<div id="sr-cel">
	<?php
	if($act == 'edit') {
		echo $form->hidden(array('name' => 'id',
								'value' => $info['id']));
	} ?>
	<div class="fm-paragraph fm-desc-inline">
		<div class="fm-multifield">
			<?php
				echo	$form->text(array('desc'	=> $this->bbf('start_date'),
							  'paragraph'	=> false,
							  'name'	=> 'recordingcampaign_start_date',
							  'labelid'	=> 'start_date',
							  'default'	=>  reformat_date(isset($info)? $info['start_date']:null)));
			?>
		</div>
		<div class="fm-multifield">
			<?php
				echo	$form->text(array('desc'	=> $this->bbf('end_date'),
							  'paragraph'	=> false,
							  'name'	=> 'recordingcampaign_end_date',
							  'labelid'	=> 'end_date',
							  'default'	=>  reformat_date(isset($info)? $info['end_date']:null)));
			?>
		</div>
	</div>
</div>
<?php
echo $form->text(array('desc'	=> $this->bbf('fm_campaign_name'),
						'name'	=> 'recordingcampaign_name',
						'default' => isset($info)? $info['campaign_name'] : null,
						'size'	=> 15));

echo $form->select(array('desc'	=> $this->bbf('fm_queue_name'),
						'name'		=> 'recordingcampaign_queueid',
						'altkey' => 'id', //altkey = "value" attribute
						'key' => 'ext_name', //key = displayed name
						'selected' => isset($info)? $info['queue_id'] : null),
						$queues_list);

?>
