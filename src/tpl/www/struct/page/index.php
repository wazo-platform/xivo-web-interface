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

$menu = &$this->get_module('menu');
$this->file_include('bloc/head');

?>
<div id="bc-body">

<div class="navbar navbar-default navbar-fixed-top" id="bc-head">
	<div class="container-fluid">
		<div id="b-tmenu">
	<?php
		$menu->mk_top();
	?>
		</div>
	</div>
</div>
<div class="container-fluid" id="bc-main">
	<div class="row">
	<div class="col-sm-3 col-md-2 sidebar" id="b-lmenu">
<?php
	$menu->mk_left();
?>
	</div>
	<div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main" id="bc-content">
		<div id=messages>
			<?php if (dwho_report::has('error') === true) : echo dwho_report::get_message('error'); endif; ?>
			<?php if (dwho_report::has('warning') === true) : echo dwho_report::get_message('warning'); endif; ?>
			<?php if (dwho_report::has('info') === true) : echo dwho_report::get_message('info'); endif; ?>
			<?php if (dwho_report::has('notice') === true) : echo dwho_report::get_message('notice'); endif; ?>
			<?php if (dwho_report::has('debug') === true) : echo dwho_report::get_message('debug'); endif; ?>
		</div>
		<div id="tooltips"></div>
		<div id="toolbar">
			<?php
				$menu->mk_toolbar();
			?>
		</div>
		<div id="b-content">
			<?php
				$this->mk_struct();
			?>
		</div>
	</div>
</div>
</div>
<div id="bc-foot">
	<div id="b-bmenu">
<?php
	$menu->mk_bottom();
?>
	</div>
</div>
</div>
<?php

$this->file_include('bloc/foot');

?>
