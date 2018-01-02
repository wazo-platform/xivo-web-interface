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

$element = $this->get_var('element');
$info = $this->get_var('info');

$amember = $this->get_var('amember');

?>

<?php
	echo	$form->text(array('desc'	=> $this->bbf('fm_agentgroup_name'),
				  'name'	=> 'agentgroup[name]',
				  'labelid'	=> 'agentgroup-name',
				  'size'	=> 15,
				  'default'	=> $element['agentgroup']['name']['default'],
				  'value'	=> $info['agentgroup']['name'],
				  'error'	=> $this->bbf_args('error',
					   $this->get_var('error','agentgroup','name'))));

	if($amember['list'] !== false):
?>
    <div id="rightcalllist" class="fm-paragraph fm-description">
		<p><label id="lb-agentlist" for="it-agentlist"><?=$this->bbf('fm_agents');?></label></p>
		<?=$form->jq_select(array('paragraph'	=> false,
					 	'label'		=> false,
            			'name'    	=> 'agent-select[]',
						'id' 		=> 'it-agent',
					    'browse'	=> 'agentfeatures',
						'key'		=> 'identity',
				       	'altkey'	=> 'id',
            			'selected'  => $amember['slt']),
					$amember['list']);?>
    </div>
    <div class="clearboth"></div>
<?php
	endif;
?>
	<div class="col-sm-offset-2 fm-paragraph fm-description">
		<p>
			<label id="lb-agentgroup-description" for="it-agentgroup-description"><?=$this->bbf('fm_agentgroup_description');?></label>
		</p>
		<?=$form->textarea(array('paragraph'	=> false,
					 'label'	=> false,
					 'name'		=> 'agentgroup[description]',
					 'id'		=> 'it-agentgroup-description',
					 'cols'		=> 60,
					 'rows'		=> 5,
					 'default'	=> $element['agentgroup']['description']['default'],
					 'error'	=> $this->bbf_args('error',
					   $this->get_var('error','agentgroup','description'))),
				   $info['agentgroup']['description']);?>
	</div>
