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

require_once(dwho_file::joinpath(DWHO_PATH_ROOT,'filter.inc'));
xivo_service_asterisk::required(array('abstract','abstract.inc'),true);

class xivo_service_asterisk_cel extends xivo_service_asterisk_abstract
{
	protected $_dso		= null;
	protected $_name	= 'cel';
	protected $_filter	= false;

	public function __construct(&$sre,&$dso)
	{
		if(is_object($sre) === false)
			trigger_error('Invalid service in '.__CLASS__,E_USER_ERROR);

		if(is_object($dso) === false)
			trigger_error('Invalid datastorage in '.__CLASS__,E_USER_ERROR);

		$this->_sre = &$sre;
		$this->_dso = &$dso;

		$this->_load_config();
	}

	public function search_idbeg($idbeg,$order=null,$limit=null)
	{
		$search = array();
		$search['idbeg'] = (int) $idbeg;

		if(($result = $this->_dso->search($search,$order,$limit)) === false)
			return(null);

		return($result);
	}
}

?>
