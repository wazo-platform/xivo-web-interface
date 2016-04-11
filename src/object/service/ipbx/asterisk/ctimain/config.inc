<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2016  Avencall
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

$array['element']['ctis_active'] = array();
$array['element']['ctis_active']['value'] = array(0,1);
$array['element']['ctis_active']['default'] = 1;
$array['element']['ctis_active']['set'] = true;

$array['element']['tlscertfile'] = array();
$array['element']['tlscertfile']['default'] = '';
$array['element']['tlsprivkeyfile'] = array();
$array['element']['tlsprivkeyfile']['default'] = '';

$array['element']['context_separation']['value'] = array(0,1);
$array['element']['context_separation']['default'] = 1;
$array['element']['context_separation']['set'] = true;

$array['element']['live_reload_conf']['value'] = array(0,1);
$array['element']['live_reload_conf']['default'] = 1;
$array['element']['live_reload_conf']['set'] = true;

dwho::load_class('dwho_network');

$array['filter'] = array();
$array['filter']['ctis_active'] = array('set' => true, 'chk' => 2, 'cast' => 'bool');
$array['filter']['tlscertfile'] = array('set' => false, 'chk' => 2, 'cast' => 'sgraph', 'maxlen' => 128);
$array['filter']['tlsprivkeyfile'] = array('set' => false, 'chk' => 2, 'cast' => 'sgraph', 'maxlen' => 128);
$array['filter']['context_separation'] = array('set' => true, 'chk' => 2, 'cast' => 'bool');
$array['filter']['live_reload_conf'] = array('set' => true, 'chk' => 2, 'cast' => 'bool');

?>
