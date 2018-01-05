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

$tree = $this->get_var('tree');

if(($parent = $this->get_var('parent')) === null):
	$pid = '';
	$plevel = 0;
else:
	$pid = $parent['id'];
	$plevel = $parent['level'];
endif;

if(is_array($tree) === true && empty($tree) === false):
	if($pid === '' && $plevel === 0):
		echo	'<tr><td>';
	endif;

	$keys = array_keys($tree);
	$nb = count($keys);
	$cnt = $nb - 1;

	for($i = 0;$i < $nb;$i++):
		$v = &$tree[$keys[$i]];

		$mod9 = $i % 9;
		$mod3 = $i % 3;

		if($v['level'] === 3):
			echo	'<div class="acl-category panel panel-default access_rights_category"><div class="panel-heading"><h4>',
				$form->checkbox(array('desc'		=> array('format'	=> '%{formfield}$s%{description}$s',
										 'description'	=> $this->bbf('acl',$v['id'])),
						      'name'		=> 'tree[]',
						      'label'		=> 'lb-'.$v['id'],
						      'id'		=> $v['id'],
						      'paragraph'	=> false,
						      'value'		=> $v['path'],
									'checked'		=> $v['access'],
									'controlSize' => 'col-sm-1'),
						'onclick="xivo_form_mk_acl(this);"'),
				'</h4>';

			if(isset($v['child']) === true):
				echo	'<span><a data-toggle="collapse" data-target="#div-'.$v['id'].'" href="#" onclick="return false;"
						  title="',$this->bbf('opt_browse'),'">',
					$url->img_html('img/site/button/more.gif',
						       $this->bbf('opt_browse'),
						       'border="0"'),
					'</a></span>';
			endif;

			echo	'</div>';
		else:
			if($i === 0):
				echo	'<div id="div-'.$v['parent']['id'].'" class="container-fluid collapse"><table id="table-',
					$v['parent']['id'],
					'"><tr><td>',"\n";
			elseif($mod9 === 0):
				echo	'</td></tr><tr><td>',"\n";
			elseif($mod3 === 0):
				echo	'</td><td>';
			endif;

				/* echo $form->checkbox(array('desc'		=> array('format'	=> '%{formfield}$s%{description}$s',
									'description'	=> $this->bbf('acl',$v['id'])), //texte du label traduit
						      'name'		=> 'tree[]',
						      'label'		=> 'lb-'.$v['id'], //id du label
						      'id'		=> $v['id'], //id du checkbox
						      'paragraph'	=> false,
						      'value'		=> $v['path'], //value des checkboxs
									'checked'		=> $v['access'],
									'controlSize' => 'not_a_col_sm'), //class de la div contenant le checkbox
						'onclick="xivo_form_mk_acl(this);"'); */
			echo '<div class="checkbox">',
					 '<label id="lb-'.$v['id'].'"><input '.($v['access']==""?"":"checked").' name="tree[]" type="checkbox" id="'.$v['id'].'" value="'.$v['path'].'" onclick="xivo_form_mk_acl(this);">'.$this->bbf('acl',$v['id']).'</label>',
					 '</div>';

			if($cnt === $i):
				if($mod9 < 3):
					$repeat = 2;
				elseif($mod9 < 6):
					$repeat = 1;
				else:
					$repeat = 0;
				endif;
				echo	str_repeat('',$repeat),'</tr></table></div>',"\n";
			endif;

		endif;

		if(isset($v['child']) === true):

			if(isset($v['parent']) === true):
				$parent = $v['parent'];
			else:
				$parent = null;
			endif;

			$this->file_include('bloc/xivo/configuration/manage/acl/tree',
					    array('tree'	=> $v['child'],
						  'parent'	=> $parent));
		endif;
		if($v['level'] === 3):
			echo	'</div>';
		endif;
	endfor;
	if($pid === '' && $plevel === 0):
		echo	'</td></tr>';
	endif;
endif;

?>
