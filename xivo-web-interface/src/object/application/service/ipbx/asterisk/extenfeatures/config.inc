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

$array['element']['agentstaticlogin'] = array();
$array['element']['agentstaticlogin']['default'] = '*31.';

$array['element']['agentstaticlogoff'] = array();
$array['element']['agentstaticlogoff']['default'] = '*32.';

$array['element']['agentstaticlogtoggle'] = array();
$array['element']['agentstaticlogtoggle']['default'] = '*30.';

$array['element']['bsfilter'] = array();
$array['element']['bsfilter']['default'] = '*37.';

$array['element']['callgroup'] = array();
$array['element']['callgroup']['default'] = '*664.';

$array['element']['calllistening'] = array();
$array['element']['calllistening']['default'] = '*34';

$array['element']['callmeetme'] = array();
$array['element']['callmeetme']['default'] = '*667.';

$array['element']['callqueue'] = array();
$array['element']['callqueue']['default'] = '*665.';

$array['element']['callrecord'] = array();
$array['element']['callrecord']['default'] = '*26';

$array['element']['calluser'] = array();
$array['element']['calluser']['default'] = '*666.';

$array['element']['directoryaccess'] = array();
$array['element']['directoryaccess']['default'] = '*36';

$array['element']['enablednd'] = array();
$array['element']['enablednd']['default'] = '*25';

$array['element']['enablevm'] = array();
$array['element']['enablevm']['default'] = '*90';

$array['element']['enablevmbox'] = array();
$array['element']['enablevmbox']['default'] = '*91';

$array['element']['enablevmboxslt'] = array();
$array['element']['enablevmboxslt']['default'] = '*91.';

$array['element']['enablevmslt'] = array();
$array['element']['enablevmslt']['default'] = '*90.';

$array['element']['fwdbusy'] = array();
$array['element']['fwdbusy']['default'] = '*23.';

$array['element']['fwdrna'] = array();
$array['element']['fwdrna']['default'] = '*22.';

$array['element']['fwdunc'] = array();
$array['element']['fwdunc']['default'] = '*21.';

$array['element']['fwdundoall'] = array();
$array['element']['fwdundoall']['default'] = '*20';

$array['element']['autoprov'] = array();
$array['element']['autoprov']['default'] = '*48378';

$array['element']['incallfilter'] = array();
$array['element']['incallfilter']['default'] = '*27';

$array['element']['phonestatus'] = array();
$array['element']['phonestatus']['default'] = '*10';

$array['element']['pickup'] = array();
$array['element']['pickup']['default'] = '*8.';

$array['element']['phoneprogfunckey'] = array();
$array['element']['phoneprogfunckey']['default'] = '*735.';

$array['element']['recsnd'] = array();
$array['element']['recsnd']['default'] = '*9';

$array['element']['vmboxmsgslt'] = array();
$array['element']['vmboxmsgslt']['default'] = '*99.';

$array['element']['vmboxpurgeslt'] = array();
$array['element']['vmboxpurgeslt']['default'] = '*93.';

$array['element']['vmboxslt'] = array();
$array['element']['vmboxslt']['default'] = '*97.';

$array['element']['vmusermsg'] = array();
$array['element']['vmusermsg']['default'] = '*98';

$array['element']['vmuserpurge'] = array();
$array['element']['vmuserpurge']['default'] = '*92';

$array['element']['vmuserpurgeslt'] = array();
$array['element']['vmuserpurgeslt']['default'] = '*92.';

$array['element']['vmuserslt'] = array();
$array['element']['vmuserslt']['default'] = '*96.';

$array['element']['paging'] = array();
$array['element']['paging']['default'] = '*11.';

$array['filter'] = array();
$array['filter']['agentstaticlogin'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['agentstaticlogoff'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['agentstaticlogtoggle'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['bsfilter'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['callgroup'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['calllistening'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['callmeetme'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['callqueue'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['callrecord'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['calluser'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['directoryaccess'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['enablednd'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['enablevm'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['enablevmbox'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['enablevmboxslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['enablevmslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['fwdbusy'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['fwdrna'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['fwdunc'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['fwdundoall'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['autoprov'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['incallfilter'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['phoneprogfunckey'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['phonestatus'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['pickup'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['recsnd'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['vmboxmsgslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['vmboxpurgeslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['vmboxslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['vmusermsg'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*]+$/');
$array['filter']['vmuserpurge'] = array('minlen' => 1,'maxlen' => 40,'regexp' => '/^[0-9\*#]+$/');
$array['filter']['vmuserpurgeslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['vmuserslt'] = array('minlen' => 1,'maxlen' => 40);
$array['filter']['paging'] = array('minlen' => 1,'maxlen' => 40);

?>
