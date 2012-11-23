<?php
#
# XiVO Web-Interface
# Copyright (C) 2012  Avencall
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

$array['element']['profile_id'] = array();
$array['element']['profile_id']['default'] = 0;
$array['element']['profile_id']['setdef'] = true;

$array['element']['xlet_id'] = array();
$array['element']['xlet_id']['default'] = 0;
$array['element']['xlet_id']['setdef'] = true;

$array['element']['layout_id'] = array();
$array['element']['layout_id']['default'] = 0;
$array['element']['layout_id']['setdef'] = true;

$array['element']['closable'] = array();
$array['element']['closable']['default'] = true;

$array['element']['movable'] = array();
$array['element']['movable']['default'] = true;

$array['element']['floating'] = array();
$array['element']['floating']['default'] = true;

$array['element']['scrollable'] = array();
$array['element']['scrollable']['default'] = true;

$array['element']['order'] = array();
$array['element']['order']['default'] = 0;
$array['element']['order']['setdef'] = true;


$array['filter'] = array();
$array['filter']['profile_id'] = array('cast' => 'uint');
$array['filter']['xlet_id'] = array('cast' => 'uint');
$array['filter']['layout_id'] = array('cast' => 'uint');
$array['filter']['closable'] = array('pg_bool' => true);
$array['filter']['movable'] = array('pg_bool' => true);
$array['filter']['floating'] = array('pg_bool' => true);
$array['filter']['scrollable'] = array('pg_bool' => true);
$array['filter']['order'] = array('cast' => 'uint');

?>