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
$error = $this->get_var('error');
$element = $this->get_var('element');

$context_list = $this->get_var('context_list');
$ipbxinfos = $this->get_var('info','ipbx');

$protocol = $this->get_var('proto');

$allow = array();
if(empty($info)):
	$context = '';
else:
	if (isset($info['extra']['allow'])) {
		$allow = explode(',',$info['extra']['allow']);
	}
	$context = (string) dwho_ak('context',$info['line'],true);
endif;

$codec_active = empty($allow) === false;
?>
<div class="tab-content">
<?php
$filename = dirname(__FILE__).'/protocol/'.$protocol.'.php';
if (is_readable($filename) === true):
	include($filename);
endif;
?>

<div role="tabpanel" class="tab-pane" id="ipbxinfos">
<div class="sb-list">
<table class="table table-condensed table-striped table-bordered">
	<thead>
	<tr class="sb-top">
		<th class="th-left"><?=$this->bbf('col_line-key');?></th>
		<th class="th-right"><?=$this->bbf('col_line-value');?></th>
	</tr>
	</thead>
<?php
$i = 0;
if($ipbxinfos !== false
&& ($nb = count($ipbxinfos)) !== 0):
	foreach($ipbxinfos as $info_key => $info_value):
?>
	<tbody>
	<tr onmouseover="this.tmp = this.className; this.className = 'sb-content l-infos-over';"
	    onmouseout="this.className = this.tmp;"
	    class="fm-paragraph l-infos-<?=(($i++ % 2) + 1)?>on2">
		<td class="td-left"><?=$info_key?></td>
		<td class="td-right"><?=$info_value?></td>
	</tr>
<?php
	endforeach;
else:
?>
	<tfoot>
	<tr<?=($ipbxinfos !== false ? ' class="b-nodisplay"' : '')?>>
		<td colspan="2" class="td-single"><?=$this->bbf('no_ipbxinfos_found');?></td>
	</tr>
	</tfoot>
<?php
endif;
?>
	</tbody>
</table>
</div>
</div>
</div>
