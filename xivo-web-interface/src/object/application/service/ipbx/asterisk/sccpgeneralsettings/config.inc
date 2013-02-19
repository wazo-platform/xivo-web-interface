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

$array = array();

$array['element'] = array();

$array['element']['directmedia'] = array();
$array['element']['directmedia']['value'] = array('no', 'yes');
$array['element']['directmedia']['default'] = 'no';
$array['element']['directmedia']['set'] = true;

$array['element']['dialtimeout'] = array();
$array['element']['dialtimeout'] = range(1,10);
$array['element']['dialtimeout']['default'] = 5;
$array['element']['dialtimeout']['setdef'] = true;

$array['element']['language'] = array();
$array['element']['language']['value'] = dwho_i18n::get_supported_language_list();
$array['element']['language']['default'] = 'en_US';

$array['filter'] = array();

$array['filter']['directmedia'] = array('bool' => true);

$array['filter']['dialtimeout'] = array(
	'cast' => 'uint',
	'chk' => 1,
	'between' => array(1,10));

$array['filter']['language'] = array(
	'set' => false,
	'chk' => 2,
	'key' => dwho_i18n::get_supported_language_list());

?>
