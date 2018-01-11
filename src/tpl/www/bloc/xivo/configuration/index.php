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

$userstat = $this->get_var('userstat');
$entitystat = $this->get_var('entitystat');
$serverstat = $this->get_var('serverstat');
$ldapserverstat = $this->get_var('ldapserverstat');

?>
<div id="index" class="b-infos">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>
	
	<div class="sb-content sb-list row">
		<div id="xivo-stats" class="col-sm-7">
			<table class="table table-condensed table-striped table-hover">
				<tr class="sb-top">
					<th class="th-left"><?=$this->bbf('stats_col_type');?></th>
					<th class="th-center"><?=$this->bbf('stats_col_enable');?></th>
					<th class="th-center"><?=$this->bbf('stats_col_disable');?></th>
					<th class="th-right"><?=$this->bbf('stats_col_total');?></th>
				</tr>
				<tr class="l-infos-1on2">
					<td class="td-left txt-left"><?=$this->bbf('stats_type-user');?></td>
					<td class="td-center"><?=$userstat['enable']?></td>
					<td class="td-center"><?=$userstat['disable']?></td>
					<td class="td-right"><?=$userstat['total']?></td>
				</tr>
				<tr class="l-infos-2on2">
					<td class="td-left txt-left"><?=$this->bbf('stats_type-entity');?></td>
					<td class="td-center"><?=$entitystat['enable']?></td>
					<td class="td-center"><?=$entitystat['disable']?></td>
					<td class="td-right"><?=$entitystat['total']?></td>
				</tr>
				<!-- hide LDAP servers and servers
				<tr class="l-infos-1on2">
					<td class="td-left txt-left"><?=$this->bbf('stats_type-server');?></td>
					<td class="td-center"><?=$serverstat['enable']?></td>
					<td class="td-center"><?=$serverstat['disable']?></td>
					<td class="td-right"><?=$serverstat['total']?></td>
				</tr>
				<tr class="l-infos-2on2">
					<td class="td-left txt-left"><?=$this->bbf('stats_type-ldapserver');?></td>
					<td class="td-center"><?=$ldapserverstat['enable']?></td>
					<td class="td-center"><?=$ldapserverstat['disable']?></td>
					<td class="td-right"><?=$ldapserverstat['total']?></td>
				</tr>
				-->
			</table>
		</div>
		<div id="xivo-logo" class="col-sm-5">
			<?=$url->img_html('img/site/xivo.gif',XIVO_SOFT_LABEL);?>
			<ul>
				<li><b><?=$this->bbf('info_soft_label');?></b> <?=XIVO_SOFT_LABEL?></li>
				<li><b><?=$this->bbf('info_soft_version');?></b> <?=XIVO_SOFT_VERSION?></li>
			</ul>
		</div>
	</div>
</div>
