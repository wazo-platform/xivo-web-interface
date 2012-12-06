<?php
#
# XiVO Web-Interface
# Copyright (C) 2006-2012  Avencall
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
dwho::load_class('dwho_json');
dwho::load_class('dwho_sort');

$act = isset($_QR['act']) === true ? $_QR['act'] : '';
$id = isset($_QR['id']) === true ? dwho_uint($_QR['id'],1) : 1;
$page = isset($_QR['page']) === true ? dwho_uint($_QR['page'],1) : 1;

$_TPL->load_i18n_file('tpl/www/bloc/cti/profiles/list-values.i18n', 'global');


$mod_ctiphonehintsgroup = &$ipbx->get_module('ctiphonehintsgroup');
$mod_ctipresencesgroup = &$ipbx->get_module('ctipresences');
$mod_cti_profile = &$ipbx->get_module('cti_profile');
$mod_cti_service = &$ipbx->get_module('cti_service');
$mod_cti_preference = &$ipbx->get_module('cti_preference');
$mod_cti_xlet = &$ipbx->get_module('cti_xlet');
$mod_cti_xlet_layout = &$ipbx->get_module('cti_xlet_layout');

$param = array();
$param['act'] = 'list';

switch($act)
{
	case 'add':
		$app_cti_profile = &$ipbx->get_application('cti_profile');
		$result = $fm_save = $error = null;

		$service = array();
		$service['slt'] = array();
		$service['list'] = $mod_cti_service->get_all(null,true,null,null,true);

		if(isset($_QR['fm_send']) === true
		&& dwho_issa('cti_profile',$_QR) === true)
		{
			if($app_cti_profile->set_add($_QR) === false
			|| $app_cti_profile->add() === false)
			{
				$fm_save = false;
				$result = $app_cti_profile->get_result();
				$error = $app_cti_profile->get_error();
			}
			else
				$_QRY->go($_TPL->url('cti/profiles'),$param);
		}

		if($service['list'] !== false && dwho_issa('service',$result) === true
		&&($service['slt'] = dwho_array_intersect_key($result['service'],$service['list'],'service_id')) !== false)
			$service['slt'] = array_keys($service['slt']);

		$phonehintsgroup = $mod_ctiphonehintsgroup->get_all();
		$presencesgroup = $mod_ctipresencesgroup->get_all();
		$list_preference = $mod_cti_preference->get_all();
		$list_xlet = $mod_cti_xlet->get_all();
		$list_xlet_layout = $mod_cti_xlet_layout->get_all();

		$_TPL->set_var('info',$result);
		$_TPL->set_var('error',$error);
		$_TPL->set_var('fm_save',$fm_save);
		$_TPL->set_var('element',$app_cti_profile->get_elements());
		$_TPL->set_var('service',$service);
		$_TPL->set_var('phonehints_group',$phonehintsgroup);
		$_TPL->set_var('presences_group',$presencesgroup);
		$_TPL->set_var('xlet_layout',$list_xlet_layout);
		$_TPL->set_var('xlet',$list_xlet);
		$_TPL->set_var('preference',$list_preference);

		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');
		$dhtml->set_js('js/jscolor/jscolor.js');
		$dhtml->load_js_multiselect_files();
		break;
	case 'edit':
		$app_cti_profile = &$ipbx->get_application('cti_profile');
		if(isset($_QR['id']) === false
		|| ($info = $app_cti_profile->get($_QR['id'])) === false)
			$_QRY->go($_TPL->url('cti/profiles'),$param);

		$fm_save = $error = null;

		$service = array();
		$service['slt'] = array();
		$service['list'] = $mod_cti_service->get_all(null,true,null,null,true);

		if(isset($_QR['fm_send']) === true
		&& dwho_issa('cti_profile',$_QR) === true)
		{
			if($app_cti_profile->set_edit($_QR) === false
			|| $app_cti_profile->edit() === false)
			{
				$fm_save = false;
				$result = $app_cti_profile->get_result();
				$error = $app_cti_profile->get_error();
				$info = array_merge($info,$result);
			}
			else
				$_QRY->go($_TPL->url('cti/profiles'),$param);
		}

		if($service['list'] !== false && dwho_issa('service',$info) === true
		&&($service['slt'] = dwho_array_intersect_key($info['service'],$service['list'],'service_id')) !== false)
			$service['slt'] = array_keys($service['slt']);

		$phonehintsgroup = $mod_ctiphonehintsgroup->get_all();
		$presencesgroup = $mod_ctipresencesgroup->get_all();
		$list_preference = $mod_cti_preference->get_all();
		$list_xlet = $mod_cti_xlet->get_all();
		$list_xlet_layout = $mod_cti_xlet_layout->get_all();

		$_TPL->set_var('id',$_QR['id']);
		$_TPL->set_var('info',$info);
		$_TPL->set_var('error',$error);
		$_TPL->set_var('fm_save',$fm_save);
		$_TPL->set_var('element',$app_cti_profile->get_elements());
		$_TPL->set_var('service',$service);
		$_TPL->set_var('phonehints_group',$phonehintsgroup);
		$_TPL->set_var('presences_group',$presencesgroup);
		$_TPL->set_var('xlet_layout',$list_xlet_layout);
		$_TPL->set_var('xlet',$list_xlet);
		$_TPL->set_var('preference',$list_preference);

		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');
		$dhtml->set_js('js/jscolor/jscolor.js');
		$dhtml->load_js_multiselect_files();
		break;
	case 'delete':
		$param['page'] = $page;

		$app = &$ipbx->get_application('cti_profile');

		if(isset($_QR['id']) === false
				|| ($info = $app->get($_QR['id'])) === false)
			$_QRY->go($_TPL->url('cti/profiles'),$param);

		$app->delete();

		$_QRY->go($_TPL->url('cti/profiles'),$param);
		break;
	default:
		$act = 'list';
		$prevpage = $page - 1;
		$nbbypage = XIVO_SRE_IPBX_AST_NBBYPAGE;

		$app_cti_profile = &$ipbx->get_application('cti_profile',null,false);

		$order = array();
		$order['name'] = SORT_ASC;

		$limit = array();
		$limit[0] = $prevpage * $nbbypage;
		$limit[1] = $nbbypage;

		$list = $app_cti_profile->get_cti_profile_list();
		$total = $app_cti_profile->get_cnt();

		if($list === false && $total > 0 && $prevpage > 0)
		{
			$param['page'] = $prevpage;
			$_QRY->go($_TPL->url('cti/profiles'),$param);
		}

		$_TPL->set_var('pager',dwho_calc_page($page,$nbbypage,$total));
		$_TPL->set_var('list',$list);
}

$_TPL->set_var('act',$act);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/cti/menu');

$menu->set_toolbar('toolbar/cti/profiles');

$_TPL->set_bloc('main','/cti/profiles/'.$act);
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
