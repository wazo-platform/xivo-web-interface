<?php

# Copyright 2018 The Wazo Authors  (see the AUTHORS file)
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

xivo_object::required(array('abstract','abstract.inc'),true);

class xivo_object_tenant extends xivo_object_abstract
{
	var $_dso			= null;
	var $_name			= 'tenant';
	var $_filter		= false;
	var $_origin		= false;
	var $_origin_list	= false;
	var $_identity		= false;

	public function __construct(&$xobj,&$dso)
	{
		parent::__construct($xobj);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_dso = &$dso;
		$this->_identity = &$this->_xobj->get_identity($this->_name);
	}

	function add($arr,$id=0)
	{
		if(is_array($arr) === false)
			return(false);

		if (($ret = $this->_dso->add($arr,false)) === false)
            return(false);

		return($ret);
	}

}

?>
