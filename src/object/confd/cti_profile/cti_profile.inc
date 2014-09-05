<?php
#
# XiVO Web-Interface
# Copyright (C) 2013-2014  Avencall
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

require_once(dwho_file::joinpath(XIVO_PATH_OBJECT,'confd', 'abstract.inc'));

class confd_ressource_cti_profile extends xivo_confd_helpers_abstract
{

	protected $_name      = 'cti_profile';
	protected $_base_url  = 'cti_profiles';

	public function __construct($_RAPI, $param=null)
	{
		parent::__construct($_RAPI, $param);
	}
	
	public function get_cti_profile_id_with_name($profile_name) {
		$all_profiles = parent::find_all();
		foreach($all_profiles as $profile) {
			if($profile['name'] == $profile_name)
				return $profile['id'];
		}
		return false;
	}
}

?>
