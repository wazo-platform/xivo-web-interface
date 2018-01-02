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
$url = &$this->get_module('url');

$info = $this->get_var('info');
$element = $this->get_var('element');

?>

<div role="tabpanel" class="tab-pane active" id="general">
<?php
		echo	$form->text(array('desc'	=> $this->bbf('fm_protocol_name'),
					  'name'	=> 'protocol[name]',
					  'labelid'	=> 'protocol-name',
					  'size'	=> 18,
					  'disabled'	=> true,
					  'readonly' => true,
					  'class'    => 'it-disabled',
					  'default'	=> $element['protocol']['sccp']['name']['default'],
					  'value'	=> $this->get_var('info','line','name')));

		if($context_list !== false):
			echo	$form->select(array('desc'	=> $this->bbf('fm_protocol_context'),
						    'name'		=> 'protocol[context]',
						    'labelid'	=> 'protocol-context',
							'class'    	=> $this->get_var('act') == 'add' ? '' : 'it-disabled',
							'disabled'	=> $this->get_var('act') == 'add' ? false : true,
						    'key'		=> 'identity',
						    'altkey'	=> 'name',
						    'selected'	=> $context),
					      $context_list);
		else:
			echo	'<div id="fd-protocol-context" class="txt-center">',
				$url->href_htmln($this->bbf('create_context'),
						'service/ipbx/system_management/context',
						'act=add'),
				'</div>';
		endif;
?>
	<fieldset id="fld-codeclist">
		<legend><?=$this->bbf('fld-codeclist');?></legend>
		<?php
			echo	$form->checkbox(array('desc'		=> $this->bbf('fm_codec-custom'),
										  'name'		=> 'codec-active',
										  'labelid'	=> 'codec-active',
										  'checked'	=> $codec_active));
		?>
		<div id="codeclist">
			<?php
				echo	$form->select(array('desc'		=> $this->bbf('fm_protocol_codec-disallow'),
										    'name'		=> 'protocol[disallow]',
										    'labelid'	=> 'protocol-disallow',
										    'key'		=> false,
										    'bbf'		=> 'fm_protocol_codec-disallow-opt',
											'bbfopt'	=> array('argmode' => 'paramvalue')),
				$element['protocol']['sccp']['disallow']['value']);
			?>
			<div class="fm-paragraph fm-description">
				<?=
        $form->jq_select(array('paragraph'	=> false,
										  'label'		=> false,
										  'name'		=> 'protocol[allow][]',
										  'id'			=> 'it-protocol-allow',
										  'key'		=> false,
										  'bbf'		=> 'ast_codec_name_type',
										  'bbfopt'	=> array('argmode' => 'paramvalue'),
										  'selected'  => $allow),
					$element['protocol']['sccp']['allow']['value']);
				?>
				<div class="clearboth"></div>
			</div>
		</div>
	</fieldset>
</div>
