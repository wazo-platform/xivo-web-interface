<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2015  Avencall
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

$act = isset($_QR['act']) === true ? $_QR['act']  : '';
$page = isset($_QR['page']) === true ? dwho_uint($_QR['page'],1) : 1;

xivo::load_class('xivo_directories',XIVO_PATH_OBJECT,null,false);
dwho::load_class('dwho_uri');
$uriobject = new dwho_uri();
$_DIR = new xivo_directories();

$param = array();
$param['act'] = 'list';

$result = $fm_save = $error = null;

define('XIVO_PHONEBOOK_TYPE_CSV_FILE', 0);
define('XIVO_PHONEBOOK_TYPE_WEBSERVICES', 1);
define('XIVO_PHONEBOOK_TYPE_XIVO', 2);
define('XIVO_PHONEBOOK_TYPE_PHONEBOOK', 3);

$types = array(
	XIVO_PHONEBOOK_TYPE_CSV_FILE => array(
		'type' => 'csv',
		'name' => 'CSV file'),
	XIVO_PHONEBOOK_TYPE_WEBSERVICES => array(
		'type' => 'csv_ws',
		'name' => 'CSV Web service'),
	XIVO_PHONEBOOK_TYPE_XIVO => array(
		'type' => 'xivo',
		'name' => 'XiVO'),
	XIVO_PHONEBOOK_TYPE_PHONEBOOK => array(
		'type' => 'phonebook',
		'name' => 'Phonebook'));

switch($act)
{
	case 'add':
		$result = null;

		if(isset($_QR['fm_send']) === true)
		{
			$data = array();
			$data['uri']         = $_QR['uri'];
			$data['eid']         = $_QR['_eid'];
			$data['name']        = $_QR['name'];
			$data['description'] = $_QR['description'];
			$data['dirtype']     = $types[$_QR['type']]['type'];

			if($_QR['type'] == XIVO_PHONEBOOK_TYPE_CSV_FILE)
				$data['uri'] = 'file://' . $_QR['uri'];

			$result = $_DIR->chk_values($data);
			if(($result = $_DIR->chk_values($data)) === false
			|| $_DIR->add($result)                  === false)
			{
				$fm_save = false;
				$result  = $_DIR->get_filter_result();
				$error   = $_DIR->get_filter_error();
			}
			else
			{
				$_QRY->go($_TPL->url('xivo/configuration/manage/directories'), $param);
			}
		}

		$info = null;
		$element = $_DIR->get_element();
		$element['type']['default'] = 'sqlite';

		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');

		$_TPL->set_var('info',$info);
		$_TPL->set_var('element',$element);
		break;
	case 'edit':
		if(isset($_QR['id']) === false
		|| ($info = $_DIR->get($_QR['id'])) === false)
			$_QRY->go($_TPL->url('xivo/configuration/manage/directories'),$param);

		$return = &$info;

		if(isset($_QR['fm_send']) === true)
		{
			$data = array();
			$data['uri'] = $_QR['uri'];
			$data['eid'] = $_QR['_eid'];
			$data['name'] = $_QR['name'];
			$data['description'] = $_QR['description'];
			$data['dirtype'] = $types[$_QR['type']]['type'];

			if($_QR['type'] == XIVO_PHONEBOOK_TYPE_CSV_FILE)
				$data['uri'] = 'file://' . $_QR['uri'];


			if(($result = $_DIR->chk_values($data)) === false
			|| $_DIR->edit($info['id'], $result)    === false)
			{
				$fm_save = false;
				$result = $_DIR->get_filter_result();
				$error = $_DIR->get_filter_error();
			}
			else
			{
				$_QRY->go($_TPL->url('xivo/configuration/manage/directories'),$param);
			}
		}

		$element = $_DIR->get_element();
		$element['type']['default'] = XIVO_PHONEBOOK_TYPE_CSV_FILE;

		$return['type'] = -1;
		foreach($types as $k => $p)
		{
			if(strcasecmp($return['dirtype'], $p['type']) == 0)
				$return['type'] = $k;
		}

		if($return['type'] == XIVO_PHONEBOOK_TYPE_CSV_FILE)
		{
			$uri = substr($return['uri'], strlen('file://'));
			if ($uri !== false)
				$return['uri'] = $uri;
		}

		$dhtml = &$_TPL->get_module('dhtml');
		$dhtml->set_js('js/dwho/submenu.js');
		$_TPL->set_var('id',$info['id']);
		$_TPL->set_var('info',$return);
		$_TPL->set_var('element',$element);
		break;

	case 'delete':
		$param['page'] = $page;

		if(isset($_QR['id']) === true
		&& ($id = intval($_QR['id'])) > 0)
			$_DIR->delete($id);

		$_QRY->go($_TPL->url('xivo/configuration/manage/directories'),$param);
		break;
	default:
		$act = 'list';
		$prevpage = $page - 1;
		$nbbypage = 20;

		$order = array();
		$order['name'] = SORT_ASC;

		$limit = array();
		$limit[0] = $prevpage * $nbbypage;
		$limit[1] = $nbbypage;

		$list = $_DIR->get_all(null,true,$order,$limit);
		$total = $_DIR->get_cnt();

		if($list === false && $total > 0 && $prevpage > 0)
		{
			$param['page'] = $prevpage;
			$_QRY->go($_TPL->url('xivo/configuration/manage/directories'),$param);
		}

		$_TPL->set_var('pager',dwho_calc_page($page,$nbbypage,$total));
		$_TPL->set_var('list',$list);
}

$_TPL->set_var('act',$act);
$_TPL->set_var('fm_save',$fm_save);
$_TPL->set_var('error',$error);
$_TPL->set_var('types', $types);

$menu = &$_TPL->get_module('menu');
$menu->set_top('top/user/'.$_USR->get_info('meta'));
$menu->set_left('left/xivo/configuration');
$menu->set_toolbar('toolbar/xivo/configuration/manage/directories');

$_TPL->set_bloc('main','xivo/configuration/manage/directories/'.$act);
$_TPL->set_struct('xivo/configuration');
$_TPL->display('index');

?>
