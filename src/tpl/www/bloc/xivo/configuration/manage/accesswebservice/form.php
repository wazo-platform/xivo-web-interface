<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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
$url = &$this->get_module('url');

$info = $this->get_var('info');
$element = $this->get_var('element');

$acl = array();
if($info !== null) {
	$acl = $info['acl'];
}

?>
<div id="sb-part-first" class="b-nodisplay">

<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_name'),
			  'name'	=> 'name',
			  'labelid'	=> 'name',
			  'size'	=> 15,
			  'default'	=> $element['name']['default'],
			  'value'	=> $info['name'],
			  'error'	=> $this->bbf_args('error',
				   $this->get_var('error', 'name')))),

	$form->text(array('desc'	=> $this->bbf('fm_login'),
			  'name'	=> 'login',
			  'labelid'	=> 'login',
			  'size'	=> 15,
			  'default'	=> $element['login']['default'],
			  'value'	=> $info['login'],
			  'error'	=> $this->bbf_args('error',
				   $this->get_var('error', 'login')))),

	$form->text(array('desc'	=> $this->bbf('fm_passwd'),
			  'name'	=> 'passwd',
			  'labelid'	=> 'passwd',
			  'size'	=> 15,
			  'default'	=> $element['passwd']['default'],
			  'value'	=> $info['passwd'],
			  'error'	=> $this->bbf_args('error',
				   $this->get_var('error', 'passwd')))),

	$form->text(array('desc'	=> $this->bbf('fm_host'),
			  'name'	=> 'host',
			  'labelid'	=> 'host',
			  'size'	=> 15,
			  'default'	=> $element['host']['default'],
			  'value'	=> $info['host'],
			  'error'	=> $this->bbf_args('error',
				   $this->get_var('error', 'host'))));
?>
	<div class="fm-paragraph fm-description">
		<p>
			<label id="lb-description" for="it-description"><?=$this->bbf('fm_description');?></label>
		</p>
		<?=$form->textarea(array('paragraph'	=> false,
					 'label'	=> false,
					 'name'		=> 'description',
					 'id'		=> 'it-description',
					 'cols'		=> 60,
					 'rows'		=> 5,
					 'default'	=> $element['description']['default'],
					 'error'	=> $this->bbf_args('error',
					   $this->get_var('error', 'description'))),
				   $info['description']);?>
	</div>
</div>

<div id="sb-part-last" class="b-nodisplay">
<?php
	$type = 'disp';
	$errdisplay = '';
?>
	<div class="sb-list">
		<table>
			<thead>
				<tr class="sb-top">

					<th class="th-left"><?=$this->bbf('fm_acl');?></th>
					<th class="th-right th-rule">
						<?=$url->href_html($url->img_html('img/site/button/mini/orange/bo-add.gif',
								   $this->bbf('col_add'),
								   'border="0"'),
								   '#',
								   null,
								   'onclick="dwho.dom.make_table_list(\'disp\',this); return(dwho.dom.free_focus());"',
								   $this->bbf('col_add'));?>
					</th>
				</tr>
			</thead>
			<tbody id="disp">

			<?php foreach($acl as $policy): ?>
					<tr class="fm-paragraph<?=$errdisplay?>">
						<td class="td-left">
<?php
							echo $form->text(array('paragraph'	=> false,
									       'name'			=> 'acl[]',
									       'id'				=> false,
									       'label'		=> false,
									       'size'			=> 100,
									       'key'			=> false,
									       'value'		=> $policy,
									       'default'	=> $element['acl']['default'],
									       'error'	=> $this->bbf_args('error',
									           $this->get_var('error', 'acl'))));
?>
						</td>
						<td class="td-right">
							<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
									   $this->bbf('opt_'.$type.'-delete'),
									   'border="0"'),
									   '#',
									   null,
									   'onclick="dwho.dom.make_table_list(\''.$type.'\',this,1); return(dwho.dom.free_focus());"',
									   $this->bbf('opt_'.$type.'-delete'));?>
						</td>
					</tr>
			<?php endforeach ?>

			</tbody>
			<tfoot>
				<tr id="no-<?=$type?>" class="b-nodisplay"/>
			</tfoot>
		</table>

		<table class="b-nodisplay">
			<tbody id="ex-<?=$type?>">
				<tr class="fm-paragraph">
					<td class="td-left">
<?php
						echo $form->text(array('paragraph'	=> false,
								       'name'			=> 'acl[]',
								       'id'		 		=> false,
								       'label'		=> false,
								       'size'			=> 100,
								       'key'			=> false,
								       'default'	=> $element['acl']['default']));
 ?>
					</td>
					<td class="td-right">
						<?=$url->href_html($url->img_html('img/site/button/mini/blue/delete.gif',
								   $this->bbf('opt_'.$type.'-delete'),
								   'border="0"'),
								   '#',
								   null,
								   'onclick="dwho.dom.make_table_list(\''.$type.'\',this,1); return(dwho.dom.free_focus());"',
								   $this->bbf('opt_'.$type.'-delete'));?>
					</td>
				</tr>
			</tbody>
		</table>
	</div>
</div>
