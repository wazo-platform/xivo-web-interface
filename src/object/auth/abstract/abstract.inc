<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2015  Avencall
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

dwho::load_class('dwho_network');
dwho::load_class('dwho_uri');
dwho::load_class('dwho_curl');
dwho::load_class('dwho_json');

abstract class xivo_auth_ressource_abstract
{
	var $_rapi			= false;
	var $_curl			= null;
	var $_uri			= null;
	var $_url			= array();
	var $_status_code	= null;
	protected $_httpheaders	= array();

	public function __construct($_RAPI)
	{
		$this->_rapi = &$_RAPI;
		$this->_load_config(true);
		$this->_set_configuration();
		$this->_url = array('scheme'	=> $this->_conf['auth']['scheme'],
				    'host'	=> $this->_conf['auth']['host'],
				    'port'	=> $this->_conf['auth']['port']);

		$this->_uri = new dwho_uri();
		$this->_curl = new dwho_curl();
	}

	private function _load_config($merge_abstract=false)
	{
		if(empty($this->_conf) === false)
			return(null);

		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT,
					   'auth',
					   $this->_name,
					   'config');

		if(($this->_conf = dwho_gat::get_file($dir)) === false)
			$this->_conf = array();

		if ($merge_abstract === true
		&& ($conf = $this->_get_config_abstract()) !== false)
			$this->_conf = array_merge($this->_conf,$conf);
	}

	private function _get_config_abstract()
	{
		$dir = dwho_file::joinpath(XIVO_PATH_OBJECT,
					   'auth',
					   'abstract',
					   'config');

		if(($conf = dwho_gat::get_file($dir)) === false)
			$conf = false;

		return($conf);
	}

	private function _set_configuration()
	{
		$_CF = &dwho_gat::get('_CF');

		if(dwho_issa($this->_name,$_CF['xivo']['ini']) === false)
			return(null);

		$ref = &$_CF['xivo']['ini'][$this->_name];

		if(isset($ref['scheme']) === true)
		{
			$this->_conf['auth']['scheme'] = $ref['scheme'];
		}

		if(isset($ref['host']) === true)
		{
			if(dwho_network::valid_host($ref['host'],
						    (DWHO_NETWORK_HOST_IPV4_DOTDEC | DWHO_NETWORK_HOST_HOSTNAME)) === true)
				$this->_conf['auth']['host'] = $ref['host'];
			else
				trigger_error('Invalid option host for section auth in XIVO init file',E_USER_WARNING);
		}

		if(isset($ref['port']) === true)
		{
			if(dwho_network::valid_port_number($ref['port']) === true)
				$this->_conf['auth']['port'] = $ref['port'];
			else
				trigger_error('Invalid option port for section auth in XIVO init file',E_USER_WARNING);
		}

		if(isset($ref['protocol']) === true)
		{
			$this->_conf['auth']['protocol'] = $ref['protocol'];
		}

		if(isset($ref['connect_timeout']) === true)
		{
			if(dwho_is_uint($ref['connect_timeout']) === true)
				$this->_conf['connect_timeout'] = $ref['connect_timeout'];
			else
				trigger_error('Invalid option connect_timeout for auth in XIVO init file',E_USER_WARNING);
		}

		if(isset($ref['timeout']) === true)
		{
			if(dwho_is_uint($ref['timeout']) === true)
				$this->_conf['timeout'] = $ref['timeout'];
			else
				trigger_error('Invalid option timeout for auth in XIVO init file',E_USER_WARNING);
		}

		return(true);
	}

	private function _create($custom_headers)
	{
		$this->_status_code = null;

		return($this->_curl->load(null,
						array('connect_timeout'	=> $this->_conf['auth']['connect_timeout'],
								'timeout'		=> $this->_conf['auth']['timeout'],
								'ssl_verifyhost' => false,
								'ssl_verifypeer' => false,
								'httpheader'	=> $this->_httpheaders),
								false));
	}

	private function _connect($username=null,$password=null,$path,$query=null)
	{
		if(($uri = $this->get_uri($path, $query)) === false
		|| $this->_curl->set_option('url',$uri) === false)
			return(false);

		if ($username && $password)
                	$this->_curl->set_option('userpwd', $username . ":" . $password);

		return($this->_curl->connect());
	}

	protected function _build_uri($arr=false, $base_url=null)
	{
		if($base_url === null) {
			$base_url = $this->_base_url;
		}

		if (is_array($arr) === false
		|| ($nb = count($arr)) === 0)
			return($base_url);

		$path = $base_url.'/'.implode('/', $arr);

		if ($this->_uri->valid_path($path) === false)
			return(false);

		return($path);
	}

	protected function last_status_code()
	{
		return($this->_status_code);
	}

	protected function request_delete($path,$query=null,$custom_headers=array())
	{
		if($this->_create($custom_headers) === false)
			return(false);

		 $this->_curl->set_option(CURLOPT_CUSTOMREQUEST, 'DELETE');

		if(($r = $this->_connect(null,null,$path,$query)) !== false)
			return($this->fetch_response($r));

		return(false);
	}

	protected function request_get($path,$query=null,$custom_headers=array())
	{
		if($this->_create($custom_headers) === false)
			return(false);

		$this->_curl->set_option('get',true);

		if(($r = $this->_connect(null,null,$path,$query)) !== false)
			return($this->fetch_response($r));

		return(false);
	}

	protected function request_put($path,$data,$query=null,$custom_headers=array())
	{
		if($this->_create($custom_headers) === false)
			return(false);

		$this->_curl->set_option(CURLOPT_CUSTOMREQUEST, 'PUT');
		$this->_curl->set_option('postfields',dwho_json::encode($data));

		if(($r = $this->_connect($path,$query)) !== false)
			return($this->fetch_response($r));

		return(false);
	}

	protected function request_post($username,$password,$path,$data,$query=null,$custom_headers=array())
	{
		if($this->_create($custom_headers) === false)
			return(false);

		$this->_curl->set_option('post',true);
		$this->_curl->set_option('postfields',dwho_json::encode($data));

		if(($r = $this->_connect($username,$password,$path,$query)) !== false)
			return($this->fetch_response($r));

		return(false);
	}

	protected function get_uri($path, $query=null)
	{
		if(dwho_has_len($path) === false)
			return(false);

		$url = $this->_url;
		$path = '/'.$this->_conf['auth']['protocol'].'/'.$path;
		$url['path'] = $path;

		if(is_array($query) === true)
			$url['query'] = $query;

		return $this->_uri->build_uri($url);
	}
}

?>
