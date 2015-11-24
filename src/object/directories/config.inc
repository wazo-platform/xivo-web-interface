<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2015  Avencall
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

$array['element']['name'] = array();
$array['element']['name']['default'] = '';

$array['element']['uri'] = array();
$array['element']['uri']['default'] = '';

$array['element']['dirtype'] = array();
$array['element']['dirtype']['default'] = 'sgbd';

$array['element']['description'] = array();
$array['element']['description']['default'] = '';

$array['element']['xivo_username'] = array();
$array['element']['xivo_username']['default'] = '';
$array['element']['xivo_username']['null'] = true;

$array['element']['xivo_password'] = array();
$array['element']['xivo_password']['default'] = '';
$array['element']['xivo_password']['null'] = true;

$array['element']['xivo_verify_certificate'] = array();
$array['element']['xivo_verify_certificate']['default'] = false;

$array['element']['xivo_verify_certificate_select'] = array();
$array['element']['xivo_verify_certificate_select']['value'] = array('yes', 'no', 'custom');
$array['element']['xivo_verify_certificate_select']['default'] = 'no';
$array['element']['xivo_verify_certificate_select']['unset'] = true;

$array['element']['xivo_custom_ca_path'] = array();
$array['element']['xivo_custom_ca_path']['default'] = '';
$array['element']['xivo_custom_ca_path']['null'] = true;


dwho::load_class('dwho_network');

$array['filter'] = array();
$array['filter']['name'] = array('set' => false, 'chk' => 2, 'regexp' => '/^[a-zA-Z0-9_\.-]+$/','maxlen' => 64);
$array['filter']['uri'] = array('set' => false, 'chk' => 2, 'regexp' => '/^.*$/','maxlen' => 255);
$array['filter']['dirtype'] = array('set' => false, 'chk' => 2, 'regexp' => '/^[a-z0-9_\.-]+$/','maxlen' => 255);
$array['filter']['description'] = array('set' => false,'chk' => 2,'maxlen' => 1000,'cast' => 'sgraph_crlf_tab','eol' => true);
$array['filter']['xivo_username'] = array('set' => false, 'chk' => 2, 'maxlen' => 64, 'cast' => 'alnumpunct');
$array['filter']['xivo_password'] = array('set' => false, 'chk' => 2, 'maxlen' => 64, 'cast' => 'alnumpunct');
$array['filter']['xivo_verify_certificate'] = array('pg_bool' => true);
$array['filter']['xivo_custom_ca_path'] = array('set' => false, 'chk' => 2, 'maxlen' => 200, 'cast' => 'alnumpunct');

?>
