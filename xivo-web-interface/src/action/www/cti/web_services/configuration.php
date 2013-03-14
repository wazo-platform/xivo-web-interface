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

$access_category = 'configuration';
$access_subcategory = '';

include(dwho_file::joinpath(dirname(__FILE__),'_common.php'));

$starttime = microtime(true);

$ctidirectories = &$ipbx->get_module('ctidirectories');
$ctidirectoryfld = &$ipbx->get_module('ctidirectoryfields');
$ldapfilter = &$ipbx->get_module('ldapfilter');

xivo::load_class('xivo_ldapserver',XIVO_PATH_OBJECT,null,false);
$ldapserver = new xivo_ldapserver();

$load_directories = $ctidirectories->get_all();

$out = array('directories' => array());

# DIRECTORIES
if(isset($load_directories) === true && is_array($load_directories) === true)
{
	$dirout = array();

	foreach($load_directories as $dir)
	{
		$uri = $dir['uri'];
		if(strpos($uri, 'ldapfilter://') === 0)
		{
			if(is_null($filterid = $ldapfilter->get_primary(array('name'=> substr($uri, 13)))))
				continue;

			$filter = $ldapfilter->get($filterid);
			$server = $ldapserver->get($filter['ldapserverid']);

			// formatting ldap uri
			$uri  = sprintf("%s://%s:%s@%s:%s/%s???%s",
				($server['securitylayer'] == 'ssl' ? 'ldaps' : 'ldap'),
				$filter['user'],
				$filter['passwd'],
				$server['host'],
				$server['port'],
				$filter['basedn'],
				rawurlencode($filter['filter']));
		}

		$dird_match_direct = dwho_json::decode($dir['match_direct'], true);
		$dird_match_reverse = dwho_json::decode($dir['match_reverse'], true);

		$dirid = $dir['name'];
		$dirout[$dirid]['uri'] = $uri;
		$dirout[$dirid]['delimiter'] = $dir['delimiter'];
		$dirout[$dirid]['name'] = $dir['description'];
		$dirout[$dirid]['match_direct'] = $dird_match_direct == false ? array() : $dird_match_direct;
		$dirout[$dirid]['match_reverse'] = $dird_match_reverse == false ? array() : $dird_match_reverse;

		$fields = $ctidirectoryfld->get_all_where(array('dir_id' => $dir['id']));
		foreach($fields as $field)
			$dirout[$dirid]['field_' . $field['fieldname']] = array($field['value']);
	}
	$out['directories'] = $dirout;
}

$out['bench'] = (float) (microtime(true) - $starttime);

$_TPL->set_var('info',$out);
$_TPL->set_var('sum',$_QRY->get('sum'));
$_TPL->display('/genericjson');

?>
