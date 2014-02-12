<?php
#
# Copyright (C) 2014  Avencall
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

$array['element']['template_id'] = array();
$array['element']['template_id']['default'] = 0;

$array['element']['func_key_id'] = array();
$array['element']['func_key_id']['default'] = 0;

$array['element']['destination_type_id'] = array();
$array['element']['destination_type_id']['default'] = 0;

$array['element']['label'] = array();
$array['element']['label']['default'] = '';

$array['element']['position'] = array();
$array['element']['position']['default'] = 0;

$array['element']['blf'] = array();
$array['element']['blf']['default'] = false;

$array['filter'] = array();
$array['filter']['template_id'] = array('cast' => 'ulongint');
$array['filter']['func_key_id'] = array('cast' => 'ulongint');
$array['filter']['destination_type_id'] = array('cast' => 'ulongint');
$array['filter']['label'] = array('set' => false,'chk' => 2,'cast' => 'sgraph','maxlen' => 128);
$array['filter']['position'] = array('cast' => 'ulongint');
$array['filter']['blf'] = array('pg_bool' => true);

?>