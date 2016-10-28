<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016 Avencall
# Copyright (C) 2016 Proformatique, Inc.
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

dwho::load_class('dwho_prefs');
$prefs = new dwho_prefs('phonebook');

$act     = isset($_QR['act']) === true ? $_QR['act'] : '';
$page    = dwho_uint($prefs->get('page', 1));
$search  = strval($prefs->get('search', ''));
$context = strval($prefs->get('context', ''));
$sort    = $prefs->flipflop('sort', 'displayname');

$param = array();
$param['act'] = 'list';

if($search !== '')
	$param['search'] = $search;


$modentity = &$_XOBJ->get_module('entity');
$appphonebook = &$ipbx->get_application('phonebook');

switch($act)
{
	case 'add':
		$result = $fm_save = $error = null;
		if(isset($_QR['fm_send']) === true) {
			$entity = $_QR['entity'];
			$name = $_QR['name'];
			$description = $_QR['description'];

			$appphonebook->add_phonebook($entity, $name, $description);

			$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),'act=list');
		}

		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');

		$_TPL->set_var('info'   ,$result);
		$_TPL->set_var('error'  ,$error);
		$_TPL->set_var('fm_save',$fm_save);
		break;
	case 'add_contact':
		$result = $fm_save = $error = null;
		$entity = $_QR['entity'];
		$phonebook_id = (int)$_QR['phonebook'];

		if(isset($_QR['fm_send']) === true && dwho_issa('phonebook',$_QR) === true) {
			$entity = $_QRY->_orig['qstring']['entity'];
			$phonebook_id = (int)$_QRY->_orig['qstring']['phonebook'];

			$result = $appphonebook->add_contact($entity, $phonebook_id, $_QR);

			$param = array('act' => 'list_contacts',
						   'entity' => $entity,
						   'phonebook' => $phonebook_id);
			$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
		}

		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');

		$_TPL->set_var('entity', $entity);
		$_TPL->set_var('phonebook_id', $phonebook_id);
		$_TPL->set_var('info'   ,$result);
		$_TPL->set_var('error'  ,$error);
		$_TPL->set_var('fm_save',$fm_save);
		$_TPL->set_var('territory',dwho_i18n::get_territory_translated_list());
		break;
	case 'edit_contact':
		if(is_array($_QRY->_orig) === false
			|| isset($_QRY->_orig['qstring']) === false
			|| isset($_QRY->_orig['qstring']['entity']) === false
			|| isset($_QRY->_orig['qstring']['phonebook']) === false
			|| isset($_QRY->_orig['qstring']['id']) === false) {
			$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
		}

		$entity = $_QRY->_orig['qstring']['entity'];
		$phonebook_id = (int)$_QRY->_orig['qstring']['phonebook'];
		$contact_uuid = $_QRY->_orig['qstring']['id'];

		if(isset($_QR['fm_send']) === false
			&& ($info = $appphonebook->get_contact($entity, $phonebook_id, $contact_uuid)) === false) {
			$param = array('act' => 'list_contacts', 'entity' => $entity, 'phonebook' => $phonebook_id);
			$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
		}

		$result = $fm_save = $error = null;
		$return = &$info;

		if(isset($_QR['fm_send']) === true && dwho_issa('phonebook',$_QR) === true)
		{
			$result = $appphonebook->edit_contact($entity, $phonebook_id, $contact_uuid, $_QR);

			$param = array('act' => 'list_contacts',
						   'entity' => $entity,
						   'phonebook' => $phonebook_id);
			$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
			$return = &$result;
		}


		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');

		$_TPL->set_var('entity'          , $entity);
		$_TPL->set_var('phonebook_id'	 , $phonebook_id);
		$_TPL->set_var('id'              , $info['phonebook']['id']);
		$_TPL->set_var('info'            , $return);
		$_TPL->set_var('error'           , $error);
		$_TPL->set_var('phonebookaddress', $return['phonebookaddress']);
		$_TPL->set_var('phonebooknumber' , $return['phonebooknumber']);
		$_TPL->set_var('fm_save'         , $fm_save);
		$_TPL->set_var('territory'       , dwho_i18n::get_territory_translated_list());
		break;
	case 'delete':
		$param['page'] = $page;
		$param['act'] = 'list';

		if(isset($_QR['entity']) && isset($_QR['id'])) {
			$appphonebook->delete_phonebook($_QR['entity'], (int)$_QR['id']);
		}
		$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
		break;
	case 'delete_contact':
		$param['page'] = $page;
		if(isset($_QR['id']) === true
			&& isset($_QR['entity']) === true
			&& isset($_QR['phonebook']) === true) {
			$entity = $_QR['entity'];
			$phonebook_id = (int)$_QR['phonebook'];
			$contact_uuid = $_QR['id'];
			$param['act'] = 'list_contacts';
			$param['entity'] = $entity;
			$param['phonebook'] = $phonebook_id;

			$appphonebook->delete_contact($entity,$phonebook_id,$contact_uuid);
		}

		$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
		break;
	case 'list_contacts':
		$prevpage = $page - 1;
		$nbbypage = XIVO_SRE_IPBX_AST_NBBYPAGE;
		$entity = $_QR['entity'];
		$phonebook_id = (int)$_QR['phonebook'];

		$limit = array();
		$limit[0] = $prevpage * $nbbypage;
		$limit[1] = $nbbypage;

		$list = $appphonebook->get_contact_list($entity, $phonebook_id, $sort, $limit, $search);
		$total = $appphonebook->get_contact_cnt($entity, $phonebook_id);

		$_TPL->set_var('entity', $entity);
		$_TPL->set_var('phonebook_id', $phonebook_id);
		$_TPL->set_var('total',$total);
		$_TPL->set_var('pager',dwho_calc_page($page,$nbbypage,$total));
		$_TPL->set_var('list',$list);
		$_TPL->set_var('search',$search);
		$_TPL->set_var('sort',$sort);
		break;
	case 'list':
	case 'deletes':
	default:
		$act = 'list';
		$prevpage = $page - 1;
		$nbbypage = XIVO_SRE_IPBX_AST_NBBYPAGE;

		$limit = array();
		$limit[0] = $prevpage * $nbbypage;
		$limit[1] = $nbbypage;

		$list = $appphonebook->get_phonebook_list($sort,$limit);
		$total = $appphonebook->get_phonebook_cnt();

		if($list === false && $total > 0 && $prevpage > 0)
		{
			$param['page'] = $prevpage;
			$_QRY->go($_TPL->url('service/ipbx/pbx_services/phonebook'),$param);
		}

		$_TPL->set_var('total',$total);
		$_TPL->set_var('pager',dwho_calc_page($page,$nbbypage,$total));
		$_TPL->set_var('list',$list);
		$_TPL->set_var('search',$search);
		$_TPL->set_var('sort',$sort);
}

$_TPL->set_var('entities',$modentity->get_all());
$_TPL->set_var('act',$act);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/service/ipbx/'.$ipbx->get_name());
$menu->set_toolbar('toolbar/service/ipbx/'.$ipbx->get_name().'/pbx_services/phonebook');

$_TPL->set_bloc('main','service/ipbx/'.$ipbx->get_name().'/pbx_services/phonebook/'.$act);
$_TPL->set_struct('service/ipbx/'.$ipbx->get_name());
$_TPL->display('index');

?>
