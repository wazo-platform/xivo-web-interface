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

$url = &$this->get_module('url');
$form = &$this->get_module('form');
$dhtml = &$this->get_module('dhtml');

$pager = $this->get_var('pager');
$act = $this->get_var('act');
$sort = $this->get_var('sort');

?>
<div class="b-infos">
	<h3 class="sb-top xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center"><?=$this->bbf('title_content_name');?></span>
		<span class="span-right">&nbsp;</span>
	</h3>
	<div class="sb-content">
        <?php
            if(($list = $this->get_var('operator')) === false || ($nb = count($list)) === 0):
        ?>
            <td colspan="7" class="td-single"><?=$this->bbf('no_trunk');?></td>
        <?php
            else:
                echo    $form->select(array('desc'	=> $this->bbf('operator'),
                            'name'	=> 'operator',
                            'labelid'	=> 'operator',
                            'key'	=> false,
                            'help'	=> $this->bbf('hlp_fm_operator'),
                            'selected'	=> $this->get_var('operator',''),
                            'default'	=> ''),
                        $this->get_var('operator'),
                        	 'onchange="alert();"'),

                        $form->text(array('desc'	=> '&nbsp;',
                          'name'	=> 'protocol[host-static]',
                          'labelid'	=> 'protocol-host-static',
                          'size'	=> 15,
                          'default'	=> $element['protocol']['host-static']['default'],
                          'value'	=> ($host_static === true ? $host : ''),
                          'error'	=> $this->bbf_args('error',
                        $this->get_var('error', 'protocol', 'host-static')) ));
            endif;
        ?>
	</div>
	<div class="sb-foot xspan">
		<span class="span-left">&nbsp;</span>
		<span class="span-center">&nbsp;</span>
		<span class="span-right">&nbsp;</span>
	</div>
</div>
