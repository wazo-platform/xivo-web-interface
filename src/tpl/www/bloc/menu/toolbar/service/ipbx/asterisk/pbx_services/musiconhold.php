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
$cat = $this->get_var('cat');
?>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo_toolbar.js'));?>"></script>

<form action="#" method="post" accept-charset="utf-8">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'act',
				    'value'	=> 'list'));
?>

<div class="form-group form-inline">
	<toolbar-buttons actions="['add','addfile']" page="musiconhold"></toolbar-buttons>
	<?php
			echo	$form->select(array('name'	=> 'cat',
						    'id'	=> 'it-toolbar-category',
						    'empty'	=> $this->bbf('toolbar_fm_category'),
						    'key'	=> true,
						    'altkey'	=> 'category',
						    'paragraph'	=> false,
						    'selected'	=> $cat),
					      $this->get_var('list_cats'),
					      'onchange="this.form[\'act\'].value = this.value === \'\'
										    ? \'list\'
										    : \'listfile\';
							 this.form.submit();"');
	?>
</div>
</form>
