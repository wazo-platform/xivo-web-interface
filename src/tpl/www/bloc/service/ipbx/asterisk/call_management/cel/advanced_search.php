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

$result = $this->get_var('result');

?>
<div id="sr-cel" class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>

<div class="sb-content">

<div id="sb-part-first"<?=($result !== false ? ' class="b-nodisplay"' : '')?>>
<form action="#" method="post" accept-charset="utf-8">
<?=$form->hidden(array('name' => DWHO_SESS_NAME,'value' => DWHO_SESS_ID))?>
<?=$form->hidden(array('name' => 'act','value' => 'search'))?>
<?=$form->hidden(array('name' => 'fm_send','value' => 1))?>
<div class="fm-paragraph fm-desc-inline">
	<div class="fm-multifield">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_dbeg'),
				  'paragraph'	=> false,
				  'name'	=> 'dbeg',
				  'labelid'	=> 'dbeg',
				  'size'=> '20',
				  'default'	=> dwho_i18n::strftime_l('%Y-%m-%d',null)));
?>
	</div>
	<br class="clearboth">
	<div class="fm-multifield">
<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_dend'),
				  'paragraph'	=> false,
				  'name'	=> 'dend',
				  'size'=> '20',
				  'labelid'	=> 'dend'));
?>
	</div>
	<br class="clearboth">
</div>
<?=$form->submit(array('name' => 'submit','id' => 'it-submit','value' => $this->bbf('fm_bt-search')))?>

</form>
</div>
<div class="sb-list">

</div>
	</div>
	<div class="sb-foot xspan"></div>
</div>
