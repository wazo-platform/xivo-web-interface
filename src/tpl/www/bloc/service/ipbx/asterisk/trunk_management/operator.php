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

$url = &$this->get_module('url');
$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');

$operator = $this->get_var('operator');
$operator_id = $this->get_var('operator_id');
$configuration = $this->get_var('configuration');

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
                echo    $form->select(array('desc'	=> $this->bbf('operator'),
                            'name'	=> 'operator',
                            'labelid'	=> 'operator',
                            'key'	=> false,
                            'help'	=> $this->bbf('hlp_fm_operator').'<br>'.XIVO_OPERATOR_SIP_CONFIG_DIR,
                            'selected'	=> $operator[$operator_id],
                            'default'	=> ''),
                        $operator,
                        	 'onchange="location.href = \''.$_SERVER[PHP_SELF].'\' + \'?id=\' + this.selectedIndex"');
                if($operator_id > 0):
                    if($configuration['user_config']['trunk']['name']):
                        echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_name'),
                              'name'	=> 'protocol[name]',
                              'labelid'	=> 'protocol-name',
                              'size'	=> 15,
                              'default'	=> $element['protocol']['name']['default'],
                              'value'	=> $info['protocol']['name'],
                              'error'	=> $this->bbf_args('error',
                                       $this->get_var('error', 'protocol', 'name')) ));
                    endif;
                    if($configuration['user_config']['trunk']['username']):
                        echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_username'),
                              'name'	=> 'protocol[username]',
                              'labelid'	=> 'protocol-username',
                              'size'	=> 15,
                              'default'	=> $element['protocol']['username']['default'],
                              'value'	=> $info['protocol']['username'],
                              'error'	=> $this->bbf_args('error',
                                       $this->get_var('error', 'protocol', 'username')) ));
                    endif;
                    if($configuration['user_config']['trunk']['secret']):
                        echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_secret'),
                              'name'	=> 'protocol[secret]',
                              'labelid'	=> 'protocol-secret',
                              'size'	=> 15,
                              'default'	=> $element['protocol']['secret']['default'],
                              'value'	=> $info['protocol']['secret'],
                              'error'	=> $this->bbf_args('error',
                                       $this->get_var('error', 'protocol', 'secret')) ));
                    endif;
                    if($configuration['user_config']['trunk']['callerid']):
                        echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_callerid'),
                              'name'	=> 'protocol[callerid]',
                              'labelid'	=> 'protocol-callerid',
                              'size'	=> 15,
                              'notag'	=> false,
                              'default'	=> $element['protocol']['callerid']['default'],
                              'value'	=> $info['protocol']['callerid'],
                              'error'	=> $this->bbf_args('error',
                                       $this->get_var('error', 'protocol', 'callerid')) ));
                     endif;
                echo    $form->hidden(array('name'	=> 'fm_send',
                              'value'	=> 1));
                endif;
                echo	$form->submit(array('name'	=> 'submit',
                            'id'	=> 'it-submit',
                            'help'	=> $this->bbf('hlp_fm_form'),
                            'value'	=> $this->bbf('fm_bt-save')));
            endif;
        ?>
                </form>
        <?php
        ?>
	</div>
	<div class="sb-foot xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center">&nbsp;</span>
		<span class="span-right">&nbsp;</span>
	</div>
</div>
