<?php
#
# XiVO Web-Interface
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


$array = array();

$array['element'] = array();

$array['element']['entity_id'] = array();
$array['element']['entity_id']['default'] = null;

$array['element']['user_id'] = array();
$array['element']['user_id']['default'] = null;

$array['element']['line_id'] = array();
$array['element']['line_id']['default'] = null;

$array['element']['queue_id'] = array();
$array['element']['queue_id']['default'] = null;

$array['element']['group_id'] = array();
$array['element']['group_id']['default'] = null;

$array['element']['voicemail_id'] = array();
$array['element']['voicemail_id']['default'] = null;

$array['element']['agent_id'] = array();
$array['element']['agent_id']['default'] = null;

$array['element']['meetme_id'] = array();
$array['element']['meetme_id']['default'] = null;

$array['element']['incall_id'] = array();
$array['element']['incall_id']['default'] = null;

$array['element']['callfilter_id'] = array();
$array['element']['callfilter_id']['default'] = null;

$array['element']['pickup_id'] = array();
$array['element']['pickup_id']['default'] = null;

$array['element']['schedule_id'] = array();
$array['element']['schedule_id']['default'] = null;

$array['element']['device_id'] = array();
$array['element']['device_id']['default'] = null;


$array['filter'] = array();

$array['filter']['entity_id'] = array('cast' => 'ulongint');
$array['filter']['user_id'] = array('cast' => 'ulongint');
$array['filter']['line_id'] = array('cast' => 'ulongint');
$array['filter']['queue_id'] = array('cast' => 'ulongint');
$array['filter']['group_id'] = array('cast' => 'ulongint');
$array['filter']['voicemail_id'] = array('cast' => 'ulongint');
$array['filter']['agent_id'] = array('cast' => 'ulongint');
$array['filter']['meetme_id'] = array('cast' => 'ulongint');
$array['filter']['incall_id'] = array('cast' => 'ulongint');
$array['filter']['callfilter_id'] = array('cast' => 'ulongint');
$array['filter']['pickup_id'] = array('cast' => 'ulongint');
$array['filter']['schedule_id'] = array('cast' => 'ulongint');
$array['filter']['device_id'] = array('set' => false,'chk' => 2,'regexp' => '/^[a-z0-9]+$/','maxlen' => 32);

?>