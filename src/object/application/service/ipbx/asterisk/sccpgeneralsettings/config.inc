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

$array = array();

$array['element'] = array();

$array['element']['directmedia'] = array();
$array['element']['directmedia']['value'] = array('no', 'yes');
$array['element']['directmedia']['default'] = 'no';
$array['element']['directmedia']['set'] = true;

$array['element']['guest'] = array();
$array['element']['guest']['value'] = array('no', 'yes');
$array['element']['guest']['default'] = 'no';
$array['element']['guest']['set'] = true;

$array['element']['max_guests'] = array();
$array['element']['max_guests']['default'] = 100;
$array['element']['max_guests']['setdef'] = true;

$array['element']['dialtimeout'] = array();
$array['element']['dialtimeout']['default'] = 5;
$array['element']['dialtimeout']['setdef'] = true;

$array['element']['keepalive'] = array();
$array['element']['keepalive']['default'] = 10;
$array['element']['keepalive']['setdef'] = true;

$array['element']['language'] = array();
$array['element']['language']['value'] = dwho_i18n::get_supported_language_list();
$array['element']['language']['default'] = 'en_US';

$array['element']['disallow'] = array();
$array['element']['disallow']['value'] = array('all');
$array['element']['disallow']['default'] = 'all';
$array['element']['disallow']['unset'] = true;

$array['element']['allow'] = array();
$array['element']['allow']['value'] = xivo_service_asterisk::get_sccp_codecs_list();
$array['element']['allow']['default'] = '';
$array['element']['allow']['null'] = true;

$array['filter'] = array();

$array['filter']['directmedia'] = array('bool' => true);

$array['filter']['guest'] = array('bool' => true);

$array['filter']['max_guests'] = array(
	'set' => false,
	'cast' => 'uint',
	'chk' => 1,
	'between' => array(1,10000));

$array['filter']['dialtimeout'] = array(
	'set' => false,
	'cast' => 'uint',
	'chk' => 1,
	'between' => array(1,10));

$array['filter']['keepalive'] = array(
	'set' => false,
	'cast' => 'uint',
	'chk' => 1,
	'between' => array(1,120));

$array['filter']['language'] = array(
	'set' => false,
	'chk' => 2,
	'key' => dwho_i18n::get_supported_language_list());

$array['filter']['disallow'] = array('set' => false, 'chk' => 2, 'key' => array('all'));
$array['filter']['allow'] = array('set' => false, 'chk' => 2, 'uvalue' => xivo_service_asterisk::get_sccp_codecs_list());

?>
