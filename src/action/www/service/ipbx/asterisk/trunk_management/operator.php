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

// TODO: replace with $appoperator
//$list = $appoperator->get_operator_list($search,$limit);
//$total = $appoperator->get_cnt();
$list = glob(XIVO_OPERATOR_SIP_CONFIG_DIR.'/*.json');
$total = count($list);
$configuration = array();
$operator = array('');
$operator_id = 0;

if(dwho::load_class('dwho_json') === true)
{
    for($i = 0; $i < $total; $i++):
        $ref = &$list[$i];
        $fh = fopen($ref,'r');
        $json = fread($fh, XIVO_OPERATOR_SIP_CONFIG_MAX_BYTES);
        fclose($fh);

        if(($data = dwho_json::decode($json,true)) !== false)
        {
            if(isset($_QR['id']) === true
            && $_QR['id'] == $i + 1)
            {
                $operator_id = $i + 1;
                $configuration = $data;
            }
            $operator[] = $data['operator_config']['trunk']['name'];
        }
    endfor;
}

$dhtml->set_js('js/utils/operator.js');

$_TPL->set_var('configuration',$configuration);
$_TPL->set_var('operator',$operator);
$_TPL->set_var('operator_id',$operator_id);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/trunk_management/operator');
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
