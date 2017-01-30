<?php

#
# XiVO Web-Interface
# Copyright 2017 The Wazo Authors  (see the AUTHORS file)
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

dwho::load_class('dwho_http');

dwho_file::required(array(XIVO_PATH_OBJECT,'application','service','ipbx','asterisk.inc'),true);
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'client.inc'));
require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'moh', 'moh.inc'));

class xivo_application_service_asterisk_moh extends xivo_application_asterisk_abstract
{
	protected $_conf		= array();
	private $_moh			= null;

	public function __construct(&$service,&$dso)
	{
		parent::__construct($service);

		$this->_load_config(dirname(__FILE__));

		if(($size = $this->_verify_can_upload_file()) === false)
			die();

		$this->_conf['option']['file']['size'] = $size;

		$client = ConfdClient::default_config();
		$this->_moh = new MOHResource($client);

		return(true);
	}

	protected function _get_config_file_maxsize()
	{
		if(($import_file = $this->_chk_exists_cfg('option','file',true)) !== false
		&& isset($import_file['size']) === true)
			return(dwho_get_memory($import_file['size'],false));

		return(false);
	}

	public function get_all_category()
	{
		$response = $this->_moh->get_all();
		if ($response === null) {
			return false;
		}

		$r = array();
		foreach ($response['items'] as $moh)
		{
			$r[$moh['name']] = $moh;
		}

		return $r;
	}

	public function get_all_by_category()
	{
		$response = $this->_moh->get_all();
		if ($response === null) {
			return false;
		}

		return $response['items'];
	}

	public function get($uuid)
	{
		$this->_info = $this->_origin = array();

		$moh = $this->_moh->get($uuid);
		if ($moh === null) {
			return false;
		}

		$this->_info['moh'] = $this->_origin['moh'] = $moh;

		return $this->_info;
	}

	public function set_add($arr)
	{
		return $this->_set('add',$arr);
	}

	public function set_edit($arr)
	{
		if(empty($this->_info) === true) {
			return false;
		}

		return $this->_set('edit',$arr);
	}

	private function _set($action,$arr)
	{
		$this->_reset();

		if ($action === 'add') {
			$moh = array(
				'name' => $arr['name'],
			);
		} else {
			$moh = $this->_info['moh'];
		}

		if (!empty($arr['mode'])) {
			$moh['mode'] = $arr['mode'];
		}
		if (!empty($arr['application'])) {
			$moh['application'] = $arr['application'];
		} else {
			$moh['application'] = null;
		}
		if (!empty($arr['sort'])) {
			$moh['sort'] = $arr['sort'];
		} else {
			$moh['sort'] = null;
		}

		$this->_set_result('moh', $moh);

		return true;
	}

	function add()
	{
		$moh = $this->_moh->create($this->_result['moh']);

		return $moh !== null;
	}

	function edit()
	{
		if (empty($this->_info) === true) {
			return false;
		}

		return $this->_moh->update($this->_result['moh']);
	}

	function delete()
	{
		if (empty($this->_info) === true) {
			return false;
		}

		return $this->_moh->delete($this->_info['moh']['uuid']);
	}

	function download_file($filename)
	{
		if (empty($this->_info) === true) {
			return false;
		}

		return $this->_moh->download_file($this->_info['moh']['uuid'], $filename);
	}

	function upload_file()
	{
		if (empty($this->_info) === true) {
			return false;
		}

		$fileinfo = $this->_get_uploaded_fileinfo();
		if ($fileinfo === false)
			return false;

		return $this->_moh->upload_file($this->_info['moh']['uuid'], $fileinfo['name'], $fileinfo['data']);
	}
	

	function delete_file($filename)
	{
		if (empty($this->_info) === true) {
			return false;
		}

		return $this->_moh->delete_file($this->_info['moh']['uuid'], $filename);
	}

	public function get_elements()
	{
		return $this->_conf['element'];
	}

	private function _get_uploaded_fileinfo()
	{
		$http_response = dwho_http::factory('response');
		if ($http_response === false) {
			dwho_report::push('error', 'could not parse HTTP request');
			return false;
		}

		$fileinfo = $http_response->upload_file('filename', $this->_conf['option']['file']);
		if ($fileinfo === false) {
			dwho_report::push('error', 'could not upload file');
			return false;
		}

		$data = file_get_contents($fileinfo['tmp_name']);
		dwho_file::rm($fileinfo['tmp_name']);
		if ($data === false) {
			dwho_report::push('error', 'could not read file');
			return false;
		}

		$fileinfo['data'] = $data;
		return $fileinfo;
	}
}

?>
