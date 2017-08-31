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

$operator = $this->get_var('operator');
$operator_index = $this->get_var('operator_index');
$configuration = $this->get_var('configuration');
$protocol = $this->get_var('protocol');
$operator_protocol = $configuration['operator_config']['trunk'];
$user_protocol = $configuration['user_config']['trunk'];

?>
<div class="b-infos">
	<h3 class="sb-top xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center"><?=$this->bbf('title_content_name');?></span>
		<span class="span-right">&nbsp;</span>
	</h3>
	<div class="sb-content">
        <?php
            if(($list = $operator) === false || ($nb = count($list)) === 0):
        ?>
            <td colspan="7" class="td-single"><?=$this->bbf('no_trunk');?></td>
        <?php
            else:
        ?>
            <form action="#" method="post" accept-charset="utf-8" onsubmit="">
        <?php
                foreach($operator_protocol as $key => $value)
                {
                    if(isset($user_protocol[$key]) === false)
                    {
                        echo   $form->hidden(array('name'	=> 'protocol['.$key.']',
                                    'value'	=> $value));
                    }
                }
                echo    $form->select(array('desc'	=> $this->bbf('operator'),
                            'name'	=> 'operator',
                            'labelid'	=> 'operator',
                            'key'	=> false,
                            'help'	=> $this->bbf('hlp_fm_operator').'<br>'.XIVO_OPERATOR_SIP_CONFIG_DIR,
                            'selected'	=> $operator[$operator_index],
                            'default'	=> ''),
                        $operator,
                        	 'onchange="location.href = \''.$_SERVER[PHP_SELF].'\' + \'?index=\' + this.selectedIndex"');
                if($operator_index > 0):
                    $_I18N = dwho_gct::get('dwho_i18n');
                    $_I18N->load_file('tpl/www/bloc/service/ipbx/asterisk/trunk_management/sip/add.php');
                    foreach($user_protocol as $key => $value)
                    {
                        if(isset($protocol[$key]) === true)
                            $value = $protocol[$key];

                        echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_'.$key),
                                  'name'	=> 'protocol['.$key.']',
                                  'labelid'	=> 'protocol-'.$key,
                                  'size'	=> 15,
                                  'value'	=> $value,
                                  'error'	=> $this->bbf_args('error',
                                           $this->get_var('error', 'protocol', $key)) ));
                    }
                    echo    $form->hidden(array('name'	=> 'fm_send',
                                'value'	=> 1)),
                            $form->hidden(array('name'	=> 'act',
                                'value'	=> 'add'));
                    echo    $form->hidden(array('name'	=> 'index',
                                'value'	=> $operator_index));
                endif;
                echo	$form->submit(array('name'	=> 'submit',
                            'id'	=> 'it-submit',
                            'help'	=> $this->bbf('hlp_fm_form'),
                            'value'	=> $this->bbf('fm_bt-save')));
            endif;
        ?>
            </form>
	</div>
	<div class="sb-foot xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center">&nbsp;</span>
		<span class="span-right">&nbsp;</span>
	</div>
</div>
