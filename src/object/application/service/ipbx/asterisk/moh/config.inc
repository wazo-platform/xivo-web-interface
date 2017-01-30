<?php

#
# XiVO Web-Interface
# Copyright 2017 The Wazo Authors  (see the AUTHORS file)
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

$array['option'] = array();
$array['option']['file'] = array();

$array['element'] = array();

$array['element']['name'] = array();
$array['element']['name']['default'] = '';

$array['element']['mode'] = array();
$array['element']['mode']['value'] = array('files','custom','mp3');
$array['element']['mode']['default'] = 'files';

$array['element']['application'] = array();
$array['element']['application']['default'] = '/usr/bin/madplay --mono -a -10 -R 8000 --output=raw:-';

$array['element']['sort'] = array();
$array['element']['sort']['value'] = array('alphabetical', 'random', 'random_start');
$array['element']['sort']['default'] = '';

?>
