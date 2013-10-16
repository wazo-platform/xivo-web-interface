<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2013 Avencall
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

$array['element']['exten'] = array();
$array['element']['exten']['default'] = '';

$array['element']['context'] = array();
$array['element']['context']['default'] = 'default';

$array['element']['type'] = array();
$array['element']['type']['value'] = array('extenfeatures',
					   'featuremap',
					   'generalfeatures',
					   'group',
					   'incall',
					   'meetme',
					   'outcall',
					   'queue',
					   'user',
					   'voicemenu');
$array['element']['type']['default'] = '';

$array['element']['typeval'] = array();
$array['element']['typeval']['default'] = '';

$array['element']['commented'] = array();
$array['element']['commented']['value'] = array(0,1);
$array['element']['commented']['default'] = 0;
$array['element']['commented']['set'] = true;


$array['filter'] = array();
$array['filter']['exten'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^(?:[saotTih]|'.XIVO_SRE_IPBX_AST_REGEXP_DIALPLAN_EXTEN_PATTERN.')$/');
$array['filter']['context'] = array('chk' => 2,'maxlen' => 39,'regexp' => '/^(?:'.XIVO_SRE_IPBX_AST_CONTEXT_FEATURES.'|(?!(?:xivo)-)[a-z0-9_-]+)$/');
$array['filter']['type'] = array('key' => $array['element']['type']['value']);
$array['filter']['typeval'] = array('set' => false,'chk' => 2,'maxlen' => 255,'cast' => 'sgraph');
$array['filter']['commented'] = array('bool' => true);

?>
