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
$info = $this->get_var('info');
$tree = $this->get_var('tree');

function create_category($id, $checked, $path, $label, $child) {
	echo	'<tr><th>',
		'<div class="panel panel-primary acces_rights_group">',
			'<div class="panel-heading">',
				'<h5><label id="lb-'.$id.'" for="'.$id.'">',
					'<input '.($checked==""?"":"checked").' name="tree[]" type="checkbox" id="'.$id.'" value="'.$path.'" onclick="xivo_form_mk_acl(this);"> ',
					$label,
				'</label></h5>',
			'</div>',
		'</div>',
	'</th></tr>';
}

?>
<div id="acl" class="b-infos b-form">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>
	<div class="sb-content">
		<form action="#" method="post" accept-charset="utf-8">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'act',
				    'value'	=> 'acl')),

		$form->hidden(array('name'	=> 'fm_send',
				    'value'	=> 1)),

		$form->hidden(array('name'	=> 'id',
				    'value'	=> $info['id'])),

		'<table class="table table-condensed">';

	$ref = &$tree['xivo'];

	if(dwho_issa('child',$ref) === true && empty($ref['child']) === false):
		foreach($ref['child'] as $v):
			create_category($v['id'], $v['access'], $v['path'], $this->bbf('acl',$v['id']));
			if(isset($v['child']) === true):
				$this->file_include('bloc/xivo/configuration/manage/acl/tree',
						    array('tree'	=> $v['child'],
								'parent'	=> null,
								'id' => $v['id']));
			endif;
		endforeach;
	endif;

	$ref = &$tree['service'];

	if(dwho_issa('child',$ref) === true && empty($ref['child']) === false):
		foreach($ref['child'] as $v):
			create_category($v['id'], $v['access'], $v['path'], $this->bbf('acl',$v['id']));
			if(isset($v['child']) === true):
				$this->file_include('bloc/xivo/configuration/manage/acl/tree',
						    array('tree'	=> $v['child'],
							  'parent'	=> null,
								'id' => $v['id']));
			endif;
		endforeach;
	endif;

	$ref = &$tree['statistics'];

	if(dwho_issa('child',$ref) === true && empty($ref['child']) === false):
		foreach($ref['child'] as $v):
			create_category($v['id'], $v['access'], $v['path'], $this->bbf('acl',$v['id']));
			if(isset($v['child']) === true):
				$this->file_include('bloc/xivo/configuration/manage/acl/tree',
						    array('tree'	=> $v['child'],
							  'parent'	=> null,
								'id' => $v['id']));
			endif;
		endforeach;
	endif;

	$ref = &$tree['cti'];

	if(dwho_issa('child',$ref) === true && empty($ref['child']) === false):
		foreach($ref['child'] as $v):
			create_category($v['id'], $v['access'], $v['path'], $this->bbf('acl',$v['id']));
			if(isset($v['child']) === true):
				$this->file_include('bloc/xivo/configuration/manage/acl/tree',
						    array('tree'	=> $v['child'],
							  'parent'	=> null,
								'id' => $v['id']));
			endif;
		endforeach;
	endif;

	$ref = &$tree['callcenter'];

	if(dwho_issa('child',$ref) === true && empty($ref['child']) === false):
		foreach($ref['child'] as $v):
			create_category($v['id'], $v['access'], $v['path'], $this->bbf('acl',$v['id']));
			if(isset($v['child']) === true):
				$this->file_include('bloc/xivo/configuration/manage/acl/tree',
						    array('tree'	=> $v['child'],
							  'parent'	=> null,
								'id' => $v['id']));
			endif;
		endforeach;
	endif;

	echo	'</table>',

		$form->submit(array('name'	=> 'submit',
				    'id'	=> 'it-submit',
				    'value'	=> $this->bbf('fm_bt-save')));
?>
		</form>
	</div>
</div>
