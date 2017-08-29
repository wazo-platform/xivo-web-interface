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

$list = glob(XIVO_OPERATOR_SIP_CONFIG_DIR.'/*.json');
$total = count($list);
$configuration = array();
$operator = array('');
$operator_id = 0;


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
        {
            $_QRY->go($_TPL->url('service/ipbx/trunk_management/sip'),$param);
        }
    }
    for($i = $id = 0; $i < $total; $i++):
        $ref = &$list[$i];
        $filesize = filesize($ref);
        if($filesize < XIVO_OPERATOR_SIP_CONFIG_MAX_BYTES)
        {
            $fh = fopen($ref,'r');
            $json = fread($fh, $filesize);
            fclose($fh);
            if(($data = dwho_json::decode($json,true)) !== false
            && count($data) > 0)
            {
                $id += 1;
                if(isset($_QR['id']) === true
                && $_QR['id'] == $id)
                {
                    $operator_id = $id;
                    $configuration = $data;
                    $protocol = $configuration['operator_config']['trunk'];
                }
                $operator[] = $data['operator_name'];
            }
            else
                dwho_report::push('error', 'Error loading one or more configuration files. No data after decoding.');
        }
        else
            dwho_report::push('error', 'Error loading one or more configuration files. File size exceeds limit.');
    endfor;

    if(isset($_QR['id']) === true
    && $operator_id === 0)
        $_QRY->go($_TPL->url('service/ipbx/trunk_management/operator'),$param);
}

$dhtml->set_js('js/utils/operator.js');

$_TPL->set_var('configuration',$configuration);
$_TPL->set_var('operator',$operator);
$_TPL->set_var('operator_id',$operator_id);
$_TPL->set_var('fm_save',$fm_save);
$_TPL->set_var('protocol', $protocol);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/trunk_management/operator');
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
