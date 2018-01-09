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

$info = $this->get_var('info');
$element = $this->get_var('element');

?>
<div class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>

<div class="sb-content">
	<form action="#" method="post" accept-charset="utf-8" onsubmit="dwho.form.select('it-access');">

<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1));
?>

	<div id="accesslist" class="fm-paragraph fm-multilist">
		<p>
			<label id="lb-localnetlist" for="it-localnet">
				<?=$this->bbf('fm_access');?>
			</label>
		</p>
		<div class="slt-list">
<?php
		echo	$form->select(array('name'	=> 'accessfeatures[]',
					    'label'	=> false,
					    'id'	=> 'it-access',
					    'key'	=> true,
					    'altkey'	=> 'host',
					    'help'	=> $this->bbf('hlp_accessfeatures'),
					    'multiple'	=> true,
					    'size'	=> 5,
					    'paragraph'	=> false),
				      $info['accessfeatures']);
?>
		<div class="bt-adddelete">
			<a href="#"
			   onclick="xivo_form_select_add_host_ipv4_subnet('it-access',
									  prompt('<?=$dhtml->escape($this->bbf('accessfeatures_add'));?>'));
				    return(dwho.dom.free_focus());"
			   title="<?=$this->bbf('bt_addaccess');?>">
				<?=$url->img_html('img/site/button/mini/blue/add.gif',
						  $this->bbf('bt_addaccess'),
						  'class="bt-addlist" id="bt-addaccess" border="0"');?></a><br />
			<a href="#"
			   onclick="dwho.form.select_delete_entry('it-access');
				    return(dwho.dom.free_focus());"
			   title="<?=$this->bbf('bt_deleteaccess');?>">
				<?=$url->img_html('img/site/button/mini/orange/delete.gif',
						  $this->bbf('bt_deleteaccess'),
						  'class="bt-deletelist" id="bt-deleteaccess" border="0"');?></a>
		</div>
	</div>
</div>
<div class="clearboth"></div>

	<?=$form->submit(array('name'	=> 'submit',
			       'id'	=> 'it-submit',
			       'value'	=> $this->bbf('fm_bt-save')));?>
</form>
	</div>
	<div class="sb-foot xspan"></div>
</div>
