<?php
#
# Copyright (C) 2016  Avencall
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

$pager = $this->get_var('pager');
$act = $this->get_var('act');
$sort = $this->get_var('sort');

$param = array();

$page = $url->pager($pager['pages'],
		    $pager['page'],
		    $pager['prev'],
		    $pager['next'],
		    'service/ipbx/pbx_services/phonebook',
		    array('act' => $act,$param));
?>
<div class="b-list">
<?php
	if($page !== ''):
		echo	'<div class="b-total">',
			$this->bbf('number_contact-result',
				   '<b>'.$this->get_var('total').'</b>'),
			'</div><div class="b-page">',
			$page,
			'</div><div class="clearboth"></div>';
	endif;
?>
<form action="#" name="fm-contact-list" method="post" accept-charset="utf-8">
<?php
	echo	$form->hidden(array('name'	=> DWHO_SESS_NAME,
				    'value'	=> DWHO_SESS_ID)),

		$form->hidden(array('name'	=> 'act',
				    'value'	=> $act)),

		$form->hidden(array('name'	=> 'page',
				    'value'	=> $pager['page'])),

		$form->hidden(array('name'	=> 'search',
				    'value'	=> ''));
?>
<table id="table-main-listing">
	<tr class="sb-top">
		<th class="th-left xspan"><span class="span-left">&nbsp;</span></th>
		<th class="th-center">
			<span class="title <?= $sort[1]=='displayname'?'underline':''?>">
				<?=$this->bbf('col_displayname');?>
			</span>
<?php
	echo	$url->href_html(
					$url->img_html('img/updown.png', $this->bbf('col_sort_displayname'), 'border="0"'),
					'service/ipbx/pbx_services/phonebook',
					array('act'	=> 'list', 'sort' => 'displayname'),
					null,
					$this->bbf('col_sort_displayname'));
?>
		</th>
		<th class="th-center">
			<span class="title <?= $sort[1]=='society'?'underline':''?>">
				<?=$this->bbf('col_society');?>
			</span>
<?php
	echo	$url->href_html(
					$url->img_html('img/updown.png', $this->bbf('col_sort_society'), 'border="0"'),
					'service/ipbx/pbx_services/phonebook',
					array('act'	=> 'list', 'sort' => 'society'),
					null,
					$this->bbf('col_sort_society'));
?>
		</th>
		<th class="th-center">
			<span class="title <?= $sort[1]=='office-number'?'underline':''?>">
				<?=$this->bbf('col_office-number');?>
			</span>
<?php
	echo	$url->href_html(
					$url->img_html('img/updown.png', $this->bbf('col_sort_office-number'), 'border="0"'),
					'service/ipbx/pbx_services/phonebook',
					array('act'	=> 'list', 'sort' => 'office-number'),
					null,
					$this->bbf('col_sort_office-number'));
?>
		</th>
		<th class="th-center">
			<span class="title <?= $sort[1]=='mobile-number'?'underline':''?>">
				<?=$this->bbf('col_mobile-number');?>
			</span>
<?php
	echo	$url->href_html(
					$url->img_html('img/updown.png', $this->bbf('col_sort_mobile-number'), 'border="0"'),
					'service/ipbx/pbx_services/phonebook',
					array('act'	=> 'list', 'sort' => 'mobile-number'),
					null,
					$this->bbf('col_sort_mobile-number'));
?>
		</th>
		<th class="th-center">
			<span class="title <?= $sort[1]=='email'?'underline':''?>">
				<?=$this->bbf('col_email');?>
			</span>
<?php
	echo	$url->href_html(
					$url->img_html('img/updown.png', $this->bbf('col_sort_email'), 'border="0"'),
					'service/ipbx/pbx_services/phonebook',
					array('act'	=> 'list', 'sort' => 'email'),
					null,
					$this->bbf('col_sort_email'));
?>
		</th>
		<th class="th-center col-action"><?=$this->bbf('col_action');?></th>
		<th class="th-right xspan"><span class="span-right">&nbsp;</span></th>
	</tr>


<?php
	if(($list = $this->get_var('list')) === false || ($nb = count($list)) === 0):
?>
	<tr class="sb-content">
		<td colspan="8" class="td-single"><?=$this->bbf('no_phonebook_contacts');?></td>
	</tr>
<?php
	endif;
?>

    <tr class="sb-foot">
        <td class="td-left xspan b-nosize"><span class="span-left b-nosize">&nbsp;</span></td>
        <td class="td-center" colspan="6"><span class="b-nosize">&nbsp;</span></td>
        <td class="td-right xspan b-nosize"><span class="span-right b-nosize">&nbsp;</span></td>
	</tr>
</table>
</form>
</div>