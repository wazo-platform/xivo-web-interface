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

$provider = $this->get_var('provider');
$provider_index = $this->get_var('provider_index');
$configuration = $this->get_var('configuration');
$protocol = $this->get_var('protocol');
$provider_protocol = $configuration['provider_config']['trunk'];
$user_protocol = $configuration['user_config']['trunk'];

?>
<div class="b-infos">
	<breadcrumb
		page="<?=$this->bbf('title_content_name');?>">
	</breadcrumb>

	<div class="sb-content">
        <?php
            if(($list = $provider) === false || ($nb = count($list)) === 0):
        ?>
            <td colspan="7" class="td-single"><?=$this->bbf('no_trunk');?></td>
        <?php
            else:
        ?>
            <form class="form-horizontal" action="#" method="post" accept-charset="utf-8" onsubmit="">
        <?php
                foreach($provider_protocol as $key => $value)
                {
                    if(isset($user_protocol[$key]) === false)
                    {
                        echo   $form->hidden(array('name'	=> 'protocol['.$key.']',
                                    'value'	=> $value));
                    }
                }
                echo    $form->select(array('desc'	=> $this->bbf('provider'),
                            'name'	=> 'provider',
                            'labelid'	=> 'provider',
                            'key'	=> false,
                            'help'	=> $this->bbf('hlp_fm_provider').'<br>'.XIVO_PROVIDER_SIP_CONFIG_DIR,
                            'selected'	=> $provider[$provider_index],
                            'default'	=> ''),
                        $provider,
                        	 'onchange="location.href = \''.$_SERVER[PHP_SELF].'\' + \'?index=\' + this.selectedIndex"');
                if($provider_index > 0):
                    $_I18N = dwho_gct::get('dwho_i18n');
                    $_I18N->load_file('tpl/www/bloc/service/ipbx/asterisk/trunk_management/sip/add.php');
                    $trunks_count = 1;
                    foreach($user_protocol as $key => $value)
                    {
                        if(isset($protocol[$key]) === true)
                            $value = $protocol[$key];

                        if($key !== 'host')
                        {
                        echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_'.$key),
                                  'name'	=> 'protocol['.$key.']',
                                  'labelid'	=> 'protocol-'.$key,
                                  'size'	=> 15,
                                  'value'	=> $value,
                                  'error'	=> $this->bbf_args('error',
                                           $this->get_var('error', 'protocol', $key)) ));
                        }
                        else
                        {
                            $hosts = explode(',',$value);
                            $trunks_count = count($hosts);
                            foreach($hosts as $host_key => $host_value)
                            {
                                echo       $form->text(array('desc'	=> $this->bbf('fm_protocol_host').
                                                ($trunks_count > 1 ? ' '.($host_key + 1) : ''),
                                      'name'	=> 'protocol[host-'.($host_key + 1).']',
                                      'labelid'	=> 'protocol-host-'.($host_key + 1),
                                      'size'	=> 15,
                                      'value'	=> $host_value,
                                      'error'	=> $this->bbf_args('error',
                                               $this->get_var('error', 'protocol', 'host')) ));
                            }
                        }
                    }
                    echo    $form->hidden(array('name'	=> 'fm_send',
                                'value'	=> 1)),
                            $form->hidden(array('name'	=> 'act',
                                'value'	=> 'add')),
                            $form->hidden(array('name'	=> 'index',
                                'value'	=> $provider_index)),
                            $form->hidden(array('name'	=> 'trunks_count',
                                'value'	=> $trunks_count));
                endif;
                echo	$form->submit(array('name'	=> 'submit',
                            'id'	=> 'it-submit',
                            'help'	=> $this->bbf('hlp_fm_form'),
                            'value'	=> $this->bbf('fm_bt-save')));
            endif;
        ?>
            </form>
	</div>
	<div class="sb-foot xspan"></div>
</div>
