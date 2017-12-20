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

$url = &$this->get_module('url');
$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');

$act = $this->get_var('act');

?>
<div class="b-list">
<form action="#" name="fm-config-list" method="post" accept-charset="utf-8">
<?=$form->hidden(array('name' => DWHO_SESS_NAME,'value'	=> DWHO_SESS_ID));?>
<?=$form->hidden(array('name' => 'act','value'	=> $act));?>
<table class="table table-condensed table-striped table-bordered" id="table-main-listing">
	<tr class="sb-top">
		<th class="th-left xspan"><span class="span-left">&nbsp;</span></th>
		<th class="th-center"><?=$this->bbf('col_name');?></th>
		<th class="th-center col-action"><?=$this->bbf('col_action');?></th>
		<th class="th-right xspan"><span class="span-right">&nbsp;</span></th>
	</tr>
<?php
	if(($list = $this->get_var('list')) === false || ($nb = count($list)) === 0):
?>
	<tr class="sb-content">
		<td colspan="4" class="td-single"><?=$this->bbf('no_config');?></td>
	</tr>
<?php
	else:
		for($i = 0;$i < $nb;$i++):
			$ref = &$list[$i];
?>
	<tr onmouseover="this.tmp = this.className; this.className = 'sb-content l-infos-over';"
	    onmouseout="this.className = this.tmp;"
	    class="sb-content l-infos-<?=(($i % 2) + 1)?>on2">
		<td class="td-left">
			&nbsp;
		</td>
		<td class="txt-left" title="<?=dwho_alttitle($ref['label']);?>">
			<?=dwho_htmlen(dwho_trunc($ref['label'],50,'...',false));?>
		</td>
		<td class="td-right" colspan="2">
<?php
			echo	$url->href_html($url->img_html('img/site/button/edit.gif',
							       $this->bbf('opt_modify'),
							       'border="0"'),
						'xivo/configuration/provisioning/configdevice',
						array('act'	=> 'edit',
						      'id'	=> $ref['id']),
						null,
						$this->bbf('opt_modify'));
			if (isset($ref['deletable']) === true
			&& (bool) $ref['deletable'] === false):
			else:
				echo	$url->href_html($url->img_html('img/site/button/delete.gif',
				 				       $this->bbf('opt_delete'),
								       'border="0"'),
							'xivo/configuration/provisioning/configdevice',
							array('act'	=> 'delete',
				      			      'id'	=> $ref['id']),
							'onclick="return(confirm(\''.$dhtml->escape($this->bbf('opt_delete_confirm')).'\'));"',
							$this->bbf('opt_delete'));
			endif;
?>
		</td>
	</tr>
<?php
		endfor;
	endif;
?>
</table>
</form>
</div>
