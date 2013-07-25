<?php
#
# XiVO Web-Interface
# Copyright (C) 2013  Avencall
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

$array['element']['user_id'] = array();
$array['element']['user_id']['default'] = 0;

$array['element']['line_id'] = array();
$array['element']['line_id']['default'] = 0;

$array['element']['extension_id'] = array();
$array['element']['extension_id']['default'] = 0;

$array['element']['main_user'] = array();
$array['element']['main_user']['default'] = true;

$array['element']['main_line'] = array();
$array['element']['main_line']['default'] = true;


$array['filter'] = array();

$array['filter']['user_id'] = array('set' => false, 'cast' => 'int');
$array['filter']['line_id'] = array('set' => true, 'cast' => 'int');
$array['filter']['extension_id'] = array('cast' => 'int');
$array['filter']['main_user'] = array('pg_bool' => true);
$array['filter']['main_line'] = array('pg_bool' => true);

?>