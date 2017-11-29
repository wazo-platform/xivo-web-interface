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
$dhtml = &$this->get_module('dhtml');
?>
<div id="login_body" state="login">
	<div id="login_form">
	<div id="xivo-logo">
			<img src="img/xivo/xivo_logo.png" />
			<img src="img/xivo/xivo_illustration.png" />
	</div>
	<div id="xivo-description">
		<?=$this->bbf('login_description');?>
	</div>

		<form class="loginForm" action="#" method="post" accept-charset="utf-8">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

	    $form->text(array('name'	=> 'login',
				  'id'		=> 'it-login',
				  'size'	=> 20,
					'group' => 'form-group',
					'lbClass' => '',
					'controlSize' => '',
				  'value'	=> $this->bbf('fm_login'))),

			$form->password(array('name'	=> 'password',
				      'id'	=> 'it-password',
				      'size'	=> 20,
							'group' => 'form-group',
							'paragraph' => false,
							'lbClass' => '',
							'controlSize' => '',
				      'value'	=> $this->bbf('fm_password'))),

		$form->select(array('desc'	=> $this->bbf('fm_language'),
				    'name'	=> 'language',
				    'id'	=> 'it-language',
						'group' => 'form-group',
						'lbClass' => '',
						'controlSize' => '',
				    'selected'	=> DWHO_I18N_BABELFISH_LANGUAGE),
			      $this->get_var('language'))
?>
			<br/>
			<div class="form-group text-center">
<?php
		echo $form->submit(array('name'	=> 'submit',
				    'id'	=> 'it-submit',
				    'value'	=> $this->bbf('fm_bt-connection')));
?>
			</div>
		</form>
	</div>
</div>
<script type="text/javascript">
dwho.dom.set_onload(function ()
{
	dwho.form.set_events_text_helper('it-login');
	dwho.form.set_events_text_helper('it-password');
});
</script>
