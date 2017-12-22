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

$context_list = $this->get_var('context_list');
$umember = $this->get_var('umember');
$queues = $this->get_var('queues');
$qmember = $this->get_var('qmember');
$act = $this->get_var('act');

?>
<div ng-controller="CCAgentsController as ctrl">
	<uib-tabset active="active">
		<uib-tab index="0" heading="<?=$this->bbf('smenu_general');?>">
			<?php
				echo	$form->text(array('desc'	=> $this->bbf('fm_agentfeatures_firstname'),
								'name'	=> 'agentfeatures[firstname]',
								'labelid'	=> 'agentfeatures-firstname',
								'size'	=> 15,
								'default'	=> $element['agentfeatures']['firstname']['default'],
								'value'	=> $info['agentfeatures']['firstname'],
								'error'	=> $this->bbf_args('error',
									$this->get_var('error','agentfeatures','firstname')))),

					$form->text(array('desc'	=> $this->bbf('fm_agentfeatures_lastname'),
								'name'	=> 'agentfeatures[lastname]',
								'labelid'	=> 'agentfeatures-lastname',
								'size'	=> 15,
								'default'	=> $element['agentfeatures']['lastname']['default'],
								'value'	=> $info['agentfeatures']['lastname'],
								'error'	=> $this->bbf_args('error',
									$this->get_var('error','agentfeatures','lastname'))));

				if ($act === 'addagent') {
					echo $form->text(array('desc'	=> $this->bbf('fm_agentfeatures_number'),
								'name'	=> 'agentfeatures[number]',
								'labelid'	=> 'agentfeatures-number',
								'size'	=> 15,
								'default'	=> $element['agentfeatures']['number']['default'],
								'value'	=> $info['agentfeatures']['number'],
								'error'	=> $this->bbf_args('error',
									$this->get_var('error','agentfeatures','number'))));
				} else {
					echo $form->text(array('desc'	=> $this->bbf('fm_agentfeatures_number'),
								'name'	=> 'agentfeatures[number]',
								'labelid'	=> 'agentfeatures-number',
								'size'	=> 15,
								'readonly'	=> true,
								'class'		=> 'it-disabled',
								'default'	=> $element['agentfeatures']['number']['default'],
								'value'	=> $info['agentfeatures']['number'],
								'error'	=> $this->bbf_args('error',
									$this->get_var('error','agentfeatures','number'))));
				}

				echo $form->text(array('desc'	=> $this->bbf('fm_agentfeatures_passwd'),
								'name'	=> 'agentfeatures[passwd]',
								'labelid'	=> 'agentfeatures-passwd',
								'size'	=> 15,
								'default'	=> $element['agentfeatures']['passwd']['default'],
								'value'	=> $info['agentfeatures']['passwd'],
								'error'	=> $this->bbf_args('error',
									$this->get_var('error','agentfeatures','passwd'))));

					if($context_list !== false):
						echo	$form->select(array('desc'	=> $this->bbf('fm_agentfeatures_context'),
											'name'	=> 'agentfeatures[context]',
											'labelid'	=> 'agentfeatures-context',
											'key'	=> 'identity',
											'altkey'	=> 'name',
											'default'	=> $element['agentfeatures']['context']['default'],
											'selected'	=> $info['agentfeatures']['context']),
											$context_list);
					else:
						echo	'<div id="fd-agentfeatures-context" class="txt-center">',
							$url->href_htmln($this->bbf('create_context'),
									'service/ipbx/system_management/context',
									'act=add'),
							'</div>';
					endif;

				echo	$form->select(array('desc'	=> $this->bbf('fm_agentfeatures_language'),
									'name'	=> 'agentfeatures[language]',
									'labelid'	=> 'agentfeatures-language',
									'key'	=> false,
									'empty' => true,
									'default'	=> $element['agentfeatures']['language']['default'],
									'selected'	=> $info['agentfeatures']['language']),
									$element['agentfeatures']['language']['value']),

					$form->select(array('desc'	=> $this->bbf('fm_agentfeatures_numgroup'),
									'name'	=> 'agentfeatures[numgroup]',
									'labelid'	=> 'agentfeatures-numgroup',
									'browse'	=> 'agentgroup',
									'key'	=> 'name',
									'altkey'	=> 'id',
									'default'	=> $this->get_var('group'),
									'selected'	=> $info['agentfeatures']['numgroup']),
									$this->get_var('agentgroup_list')),

					$form->text(array('desc'	=> $this->bbf('fm_agentfeatures_preprocess-subroutine'),
								'name'	=> 'agentfeatures[preprocess_subroutine]',
								'labelid' 	=> 'agentfeatures-preprocess-subroutine',
								'size'	=> 15,
								'default'	=> $element['agentfeatures']['preprocess_subroutine']['default'],
								'value'	=> $info['agentfeatures']['preprocess_subroutine'],
								'error'	=> $this->bbf_args('error',
									$this->get_var('error','agentfeatures','preprocess_subroutine'))));

			?>
		</uib-tab>

		<uib-tab index="1" heading="<?=$this->bbf('smenu_users');?>">
			<?php
				if($umember['list'] !== false):
			?>
				<div id="rightcalllist" class="fm-paragraph fm-description">
						<?=$form->jq_select(array('paragraph'	=> false,
										'label'		=> false,
													'name'    	=> 'user-select[]',
										'id' 		=> 'it-user',
										'key'		=> 'identity',
												'altkey'	=> 'id',
													'selected'  => $umember['slt']),
									$umember['list']);?>
				</div>
				<div class="clearboth"></div>
			<?php
				else:
					echo	'<div class="txt-center">',
						$url->href_htmln($this->bbf('create_user'),
								'service/ipbx/pbx_settings/users',
								'act=add'),
						'</div>';
				endif;
			?>
		</uib-tab>

		<uib-tab index="2" heading="<?=$this->bbf('smenu_queues');?>" select="ctrl.init_tab_queues()">
			<?php
				if(is_array($queues) === true && empty($queues) === false):
			?>
			<div id="queuelist" class="fm-paragraph fm-multilist">
				<div class="row">
					<div class="col-sm-5">
						<?=$form->input_for_ms('queuelist',$this->bbf('ms_seek'))?>
					</div>
				</div>
				
				<div class="row">
					<div class="col-sm-2 slt-outlist">
						<?php
								echo	$form->select(array('name'	=> 'queuelist',
													'label'	=> false,
													'id'	=> 'it-queuelist',
													'multiple'	=> true,
													'size'	=> 5,
													'paragraph'	=> false,
													'key'	=> 'name',
													'altkey'	=> 'name'),
													$qmember['list']);
						?>
					</div>

					<div class="col-sm-1 inout-list">
						<a href="#"
							onclick="xivo_ast_inqueue();
									return(dwho.dom.free_focus());"
							title="<?=$this->bbf('bt_inqueue');?>">
							<?=$url->img_html('img/site/button/arrow-left.gif',
										$this->bbf('bt_inqueue'),
										'class="bt-inlist" id="bt-inqueue" border="0"');?></a><br />
						<a href="#"
							onclick="xivo_ast_outqueue();
									return(dwho.dom.free_focus());"
							title="<?=$this->bbf('bt_outqueue');?>">
							<?=$url->img_html('img/site/button/arrow-right.gif',
										$this->bbf('bt_outqueue'),
										'class="bt-outlist" id="bt-outqueue" border="0"');?></a>

					</div>

					<div class="col-sm-2 slt-inlist">
						<?php
								echo	$form->select(array('name'	=> 'queue-select[]',
													'label'	=> false,
													'id'	=> 'it-queue',
													'multiple'	=> true,
													'size'	=> 5,
													'paragraph'	=> false,
													'key'	=> 'name',
													'altkey'	=> 'name'),
													$qmember['slt']);
						?>
					</div>
				</div>
			</div>

			<div class="sb-list">
				<table class="table table-condensed table-striped table-hover table-bordered">
					<tr class="sb-top">
						<th class="th-left"><?=$this->bbf('col_queue-name');?></th>
						<th class="th-right"><?=$this->bbf('col_queue-penalty');?></th>
					</tr>
					<?php
							foreach($queues as $value):
								$name = $value['name'];

								if(dwho_issa($value['id'],$qmember['info']) === true):
									$class = '';
									$value['member'] = $qmember['info'][$value['id']];
									$penalty = intval($value['member']['penalty']);
								else:
									$class = ' b-nodisplay';
									$value['member'] = null;
									$penalty = '';
								endif;

							echo	'<tr id="queue-',$name,'" class="fm-paragraph',$class,'">',"\n",
								'<td class="td-left">',$name,'</td>',"\n",
								'<td class="td-right">',
								$form->select(array('paragraph'	=> false,
												'name'	=> 'queue['.$name.'][penalty]',
												'id'	=> false,
												'label'	=> false,
												'default'	=> $element['qmember']['penalty']['default'],
												'selected'	=> $penalty),
												$element['qmember']['penalty']['value']),
								'</td>',"\n",
								'</tr>',"\n";
							endforeach;
					?>
					<tr id="no-queue"<?=(empty($qmember['slt']) === false ? ' class="b-nodisplay"' : '')?>>
						<td colspan="4" class="td-single"><?=$this->bbf('no_queue');?></td>
					</tr>
				</table>
			</div>
			<?php
				else:
					echo	'<div class="txt-center">',
						$url->href_htmln($this->bbf('create_queue'),
								'callcenter/settings/queues',
								'act=add'),
						'</div>';
				endif;
			?>
		</uib-tab>

		<uib-tab index="3" heading="<?=$this->bbf('smenu_queueskills');?>">
			<?php
				$this->file_include('bloc/callcenter/settings/agents/queueskills');
			?>
		</uib-tab>

		<uib-tab index="4" heading="<?=$this->bbf('smenu_advanced');?>">
			<?php
					echo $form->select(array('desc'	=> $this->bbf('fm_agentfeatures_autologoff'),
								'name'	=> 'agentfeatures[autologoff]',
								'labelid'	=> 'agentfeatures-autologoff',
								'key'	=> false,
								'help'	=> $this->bbf('hlp_fm_agentfeatures_autologoff'),
								'bbf'	=> 'fm_agentfeatures_autologoff-opt',
								'bbfopt'	=> array('argmode' => 'paramvalue'),
								'default'	=> $element['agentfeatures']['autologoff']['default'],
								'selected'	=> $info['agentfeatures']['autologoff']),
								$element['agentfeatures']['autologoff']['value']);
			?>
			<div class="col-sm-offset-2 fm-paragraph fm-description">
				<p>
					<label id="lb-agentfeatures-description" for="it-agentfeatures-description"><?=$this->bbf('fm_agentfeatures_description');?></label>
				</p>
				<?=$form->textarea(array('paragraph'	=> false,
							'label'	=> false,
							'name'		=> 'agentfeatures[description]',
							'id'		=> 'it-agentfeatures-description',
							'cols'		=> 60,
							'rows'		=> 5,
							'default'	=> $element['agentfeatures']['description']['default']),
							$info['agentfeatures']['description']);?>
			</div>
		</uib-tab>
	</uib-tabset>
</div>
