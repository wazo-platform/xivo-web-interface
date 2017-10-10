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

$list = glob(XIVO_PROVIDER_SIP_CONFIG_DIR.'/*.json');
$total = count($list);
$configuration = array();
$provider = array();
$provider_index = 0;

if($total > 0)
    $provider[] = '';


if(dwho::load_class('dwho_json') === true)
{
    $apptrunk = &$ipbx->get_application('trunk',
                        array('protocol' => XIVO_SRE_IPBX_AST_PROTO_SIP));

    $result = $fm_save = $error = null;

    if(isset($_QR['fm_send']) === true && dwho_issa('protocol',$_QR) === true)
    {
        if(isset($_QR['protocol']['transport']) === false)
            $_QR['protocol']['transport'] = 'udp';

        if(dwho_issa('trunkfeatures',$_QR) === false)
            $_QR['trunkfeatures'] = Array();

        if($apptrunk->set_add($_QR) === false
        || $apptrunk->add() === false)
        {
				$fm_save = false;
				$result = $apptrunk->get_result();
				$error = $apptrunk->get_error();
        }
        else
            $_QRY->go($_TPL->url('service/ipbx/trunk_management/sip'));
    }
    for($i = $index = 0; $i < $total; $i++):
        $ref = &$list[$i];
        $filesize = filesize($ref);
        if($filesize < XIVO_PROVIDER_SIP_CONFIG_MAX_BYTES)
        {
            $fh = fopen($ref,'r');
            $json = fread($fh, $filesize);
            fclose($fh);
            if(($data = dwho_json::decode($json,true)) !== false
            && count($data) > 0)
            {
                $index += 1;
                if(isset($_QR['index']) === true
                && $_QR['index'] == $index)
                {
                    $provider_index = $index;
                    $configuration = $data;
                }
                $provider[] = $data['provider_name'];
            }
            else
                dwho_report::push('error', 'Error loading one or more configuration files. No data after decoding.');
        }
        else
            dwho_report::push('error', 'Error loading one or more configuration files. File size exceeds limit.');
    endfor;

    if(isset($_QR['index']) === true
    && $provider_index === 0)
        $_QRY->go($_TPL->url('service/ipbx/trunk_management/provider'));
}

if(isset($configuration) === true)
{
    $provider_protocol = $configuration['provider_config']['trunk'];
    $user_protocol = $configuration['user_config']['trunk'];

    foreach($provider_protocol as $key => $value)
    {
        if(isset($user_protocol[$key]) === true)
            dwho_report::push('error', 'Duplicate parameter "'.$key.'" in provider configuration file');
    }
}

$_TPL->set_var('configuration',$configuration);
$_TPL->set_var('provider',$provider);
$_TPL->set_var('provider_index',$provider_index);
$_TPL->set_var('fm_save',$fm_save);
$_TPL->set_var('error',$error);
$_TPL->set_var('protocol',$_QR['protocol']);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/trunk_management/provider');
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
