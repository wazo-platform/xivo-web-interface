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

xivo_object::required(array('abstract','provd.inc'),true);

class xivo_object_provdplugin extends xivo_object_provd_abstract
{
	protected $_name 		= 'provdplugin';
	protected $_base_url 	= '/provd/pg_mgr';

	public function __construct(&$xobj,$param=array())
	{
		$this->_load_init();
	}

	public function get($name)
	{
		$this->reset();

		if(dwho_has_len($name) === false)
			return(false);

		$res = array();

		$uri = $this->_build_uri(array('plugins',$name,'info'));

		$rs = $this->request_get($uri);

		if (is_array($rs) === false
		|| $this->_status_code !== 200
		|| empty($rs['plugin_info']) === true)
		{
			dwho_report::push('error','error: Can\'t get '.$name.' or plugin isn\'t installed.');
			return(false);
		}

		$res['info'] = $rs['plugin_info'];
		$res['pkgs'] = $pkgs = array();
		$res['params'] = false;

		if (($list_installed = $this->get_plugin_pkgs_installed($name)) === false)
			$list_installed = array();

		if (($list_installable = $this->get_plugin_pkgs_installable($name)) === false)
			$list_installable = array();

		$list_old_pni = array_diff_key($list_installed, $list_installable);
		$list_installable = array_merge($list_installable,$list_old_pni);

		ksort($list_installable);

		$list = array();
		foreach ($list_installable as $k => &$v)
		{
			if (isset($list_installed[$k]) === true)
			{
				$v['version'] = $list_installed[$k]['version'];
				$v['type'] = 'installed';
			}
			array_push($list, $v);
		}

		$res['pkgs'] = $list;

		$uri = $this->_build_uri(array('plugins',$name,'configure'));

		$rs = $this->request_get($uri);

		if (is_array($rs) === true
		&& $this->_status_code === 200)
			$res['params'] = $rs['params'];

		return($res);
	}

	public function get_plugin_installed($search='')
	{
		return($this->_get_plugins('installed',$search));
	}

	public function get_plugin_installable($search='')
	{
		return($this->_get_plugins('installable',$search));
	}

	private function _get_plugins($type,$search='')
	{
		switch ($type)
		{
			case 'installed':
				$uri = $this->_build_uri(array('install','installed'));
				break;
			default:
			case 'installable':
				$uri = $this->_build_uri(array('install','installable'));
		}

		$rs = $this->request_get($uri);

		if (is_array($rs) === false
		|| $this->_status_code !== 200
		|| ($rs = $rs['pkgs']) === false
		|| ($nb = count($rs)) === 0)
			return(false);

		foreach($rs as $k => &$r)
		{
			$r['type'] = $type;
			$r['version-'.$type] = $r['version'];
			$r['name'] = $k;

			if ($search !== ''
			&& preg_match('/'.$search.'/', $r['name']) === 0
			&& preg_match('/'.$search.'/', $r['description']) === 0)
				unset($rs[$k]);
		}

		return($rs);
	}

	public function get_plugin_pkgs_installed($plugin)
	{
		return($this->_get_plugins_pkgs('installed',$plugin));
	}

	public function get_plugin_pkgs_installable($plugin)
	{
		return($this->_get_plugins_pkgs('installable',$plugin));
	}

	private function _get_plugins_pkgs($type,$plugin)
	{
		switch ($type)
		{
			case 'installed':
				$uri = $this->_build_uri(array('plugins',$plugin,'install','installed'));
				break;
			default:
			case 'installable':
				$uri = $this->_build_uri(array('plugins',$plugin,'install','installable'));
		}

		$rs = $this->request_get($uri);

		if (is_array($rs) === false
		|| $this->_status_code !== 200
		|| isset($rs['pkgs']) === false
		|| is_array($rs['pkgs']) === false)
			return(false);

		$rs = $rs['pkgs'];
		foreach($rs as $k => &$r)
		{
			$r['type'] = $type;
			$r['version-'.$type] = $r['version'];
			$r['name'] = $k;
		}

		return($rs);
	}

	public function install($name)
	{
		if(dwho_has_len($name) === false)
			return(false);

		$uri = $this->_build_uri(array('install','install'));

		$data = array('id' => $name);

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 201)
			return(false);

		$decoded = $this->decode_header($this->_response['header']);

		if (isset($decoded['Location']) === false)
			return(false);

		return(urlencode($decoded['Location']));
	}

	public function upgrade_pkgs($plugin,$name)
	{
		if(dwho_has_len($plugin) === false
		|| dwho_has_len($name) === false)
			return(false);

		$uri = $this->_build_uri(array('plugins',$plugin,'install','upgrade'));

		$data = array('id' => $name);

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 201)
			return(false);

		$decoded = $this->decode_header($this->_response['header']);

		if (isset($decoded['Location']) === false)
			return(false);

		return(urlencode($decoded['Location']));
	}

	public function install_pkgs($plugin,$name)
	{
		if(dwho_has_len($plugin) === false
		|| dwho_has_len($name) === false)
			return(false);

		$uri = $this->_build_uri(array('plugins',$plugin,'install','install'));

		$data = array('id' => $name);

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 201)
			return(false);

		$decoded = $this->decode_header($this->_response['header']);

		if (isset($decoded['Location']) === false)
			return(false);

		return(urlencode($decoded['Location']));
	}

	public function uninstall($name)
	{
		if(dwho_has_len($name) === false)
			return(false);

		$uri = $this->_build_uri(array('install','uninstall'));

		$data = array('id' => $name);

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 204)
			return(false);

		return(true);
	}

	public function uninstall_pkgs($plugin,$name)
	{
		if(dwho_has_len($plugin) === false
		|| dwho_has_len($name) === false)
			return(false);

		$uri = $this->_build_uri(array('plugins',$plugin,'install','uninstall'));

		$data = array('id' => $name);

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 204)
			return(false);

		return(true);
	}

	public function upgrade($name)
	{
		if(dwho_has_len($name) === false)
			return(false);

		$uri = $this->_build_uri(array('install','upgrade'));

		$data = array('id' => $name);

		$rs = $this->request_post($uri,$data);

		if ($this->_status_code !== 201)
			return(false);

		$decoded = $this->decode_header($this->_response['header']);

		if (isset($decoded['Location']) === false)
			return(false);

		$this->request_delete($decoded['Location']);

		if ($this->_status_code !== 204)
			return(false);

		return(true);
	}

	public function update()
	{
		$uri = $this->_build_uri(array('install','update'));

		$rs = $this->request_post($uri,array());

		if ($this->_status_code !== 201)
			return(false);

		$decoded = $this->decode_header($this->_response['header']);

		if (isset($decoded['Location']) === false)
			return(false);

		return(urlencode($decoded['Location']));
		/*
		$this->request_delete($decoded['Location']);

		if ($this->_status_code !== 204)
			return(false);

		return(true);
		*/
	}
}

?>