<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2015  Avencall
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
$dhtml = &$this->get_module('dhtml');

$info = $this->get_var('info');
$element = $this->get_var('element');

?>

<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_ldapfilter_name'),
				  'name'	=> 'ldapfilter[name]',
				  'labelid'	=> 'ldapfilter-name',
				  'size'	=> 15,
				  'default'	=> $element['ldapfilter']['name']['default'],
				  'value'	=> $info['ldapfilter']['name'],
				  'error'	=> $this->bbf_args('error',
					   $this->get_var('error', 'ldapfilter', 'name'))));

	if(($ldapservers = $this->get_var('ldapservers')) !== false):
		echo	$form->select(array('desc'	=> $this->bbf('fm_ldapfilter_ldapserverid'),
					    'name'	=> 'ldapfilter[ldapserverid]',
					    'labelid'	=> 'ldapfilter-ldapserverid',
					    'invalid'	=> ($this->get_var('act') === 'edit'),
					    'key'	=> 'identity',
					    'altkey'	=> 'id',
					    'default'	=> $element['ldapfilter']['ldapserverid']['default'],
					    'selected'	=> $info['ldapfilter']['ldapserverid']),
				      $ldapservers);
	else:
		echo	'<div class="txt-center">',
			$url->href_htmln($this->bbf('create_ldapserver'),
					'xivo/configuration/manage/ldapserver',
					'act=add'),
			'</div>';
	endif;

	echo	$form->text(array('desc'	=> $this->bbf('fm_ldapfilter_user'),
				  'name'	=> 'ldapfilter[user]',
				  'labelid'	=> 'ldapfilter-user',
				  'size'	=> 15,
				  'default'	=> $element['ldapfilter']['user']['default'],
				  'value'	=> $info['ldapfilter']['user'],
				  'error'	=> $this->bbf_args('error',
					   $this->get_var('error', 'ldapfilter', 'user')))),

		$form->text(array('desc'	=> $this->bbf('fm_ldapfilter_passwd'),
				  'name'	=> 'ldapfilter[passwd]',
				  'labelid'	=> 'ldapfilter-passwd',
				  'size'	=> 15,
				  'default'	=> $element['ldapfilter']['passwd']['default'],
				  'value'	=> $info['ldapfilter']['passwd'],
				  'error'	=> $this->bbf_args('error',
					   $this->get_var('error', 'ldapfilter', 'passwd')))),

		$form->text(array('desc'	=> $this->bbf('fm_ldapfilter_basedn'),
				  'name'	=> 'ldapfilter[basedn]',
				  'labelid'	=> 'ldapfilter-basedn',
				  'size'	=> 30,
				  'default'	=> $element['ldapfilter']['basedn']['default'],
				  'value'	=> $info['ldapfilter']['basedn'],
				  'error'	=> $this->bbf_args('error',
					   $this->get_var('error', 'ldapfilter', 'basedn')))),

		$form->text(array('desc'	=> $this->bbf('fm_ldapfilter_filter'),
				  'name'	=> 'ldapfilter[filter]',
				  'labelid'	=> 'ldapfilter-filter',
				  'size'	=> 30,
				  'notag'	=> false,
				  'default'	=> $element['ldapfilter']['filter']['default'],
				  'value'	=> $info['ldapfilter']['filter'],
				  'help'	=> $this->bbf('help_fm_ldapfilter_filter'),
				  'error'	=> $this->bbf_args('error_fm_ldapfilter_filter',
					   $this->get_var('error', 'ldapfilter', 'filter'))));
?>
	<div class="col-sm-offset-2 fm-paragraph fm-description">
		<p>
			<label id="lb-ldapfilter-description" for="it-ldapfilter-description"><?=$this->bbf('fm_ldapfilter_description');?></label>
		</p>
		<?=$form->textarea(array('paragraph'	=> false,
					 'label'	=> false,
					 'name'		=> 'ldapfilter[description]',
					 'id'		=> 'it-ldapfilter-description',
					 'cols'		=> 60,
					 'rows'		=> 5,
					 'default'	=> $element['ldapfilter']['description']['default']),
				   $info['ldapfilter']['description']);?>
	</div>
