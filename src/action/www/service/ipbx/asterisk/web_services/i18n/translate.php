<?php

#
# XiVO Web-Interface
# Copyright (C) 2006-2017  Avencall
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

// set common i18n as default
$_I18N->load_file('tpl/www/struct/service/ipbx/asterisk');
$_I18N->load_file('tpl/tpl');

$translation = dwho_i18n::get_babelfish('global');
$_TPL->set_var('list', $translation);

$_TPL->display('/service/ipbx/'.$ipbx->get_name().'/generic');

?>
