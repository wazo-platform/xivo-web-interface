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
?>


<?php
echo $form->text(array('desc'	=> $this->bbf('fm_campaign_name'),
						'name'	=> 'recordingcampaign_name',
						'default' => $act == 'edit'? $info['campaign_name'] : null,
						'size'	=> 15));

echo $form->select(array('desc'	=> $this->bbf('fm_queue_name'),
						'name'		=> 'recordingcampaign_queueid',
						'altkey' => 'id', //altkey = "value" attribute
						'key' => 'ext_name', //key = displayed name
						'selected' => $act == 'edit'? $info['queue_id'] : null),
						$queues_list);

?>
