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

$client_url = $this->bbf('download_soft_url_xivo-client',XIVO_DOWNLOAD_URL);
$doc_url = $this->bbf('url_xivo-documentation',XIVO_DOC_URL);
$doc_api_url = $this->bbf('url_xivo-documentation',XIVO_DOC_API_URL);
$blog_url = $this->bbf('url_xivo-blog',XIVO_BLOG_URL);

?>
<div class="two_col_separator">	
	<div class="b-infos col-sm-6">
		<h3 class="sb-top xspan">
			<?=$this->bbf('title_content_name');?>
		</h3>
		<div class="sb-content">
			<dl>
				<dt><?=$this->bbf('info_download_xivo-client');?></dt>
				<dd><?=$url->href_html($client_url,
									$client_url,
									null,
									'target="_blank"',
									null,
									false,
									null,
									false,
									false);?>
				<dt><?=$this->bbf('info_xivo-documentation');?></dt>
				<dd><?=$url->href_html($doc_url,
									$doc_url,
									null,
									'target="_blank"',
									null,
									false,
									null,
									false,
									false);?>
				<dt><?=$this->bbf('info_xivo-documentation-api');?></dt>
				<dd><?=$url->href_html($doc_api_url,
									$doc_api_url,
									null,
									'target="_blank"',
									null,
									false,
									null,
									false,
									false);?>
				<dt><?=$this->bbf('info_xivo-blog');?></dt>
				<dd><?=$url->href_html($blog_url,
									$blog_url,
									null,
									'target="_blank"',
									null,
									false,
									null,
									false,
									false);?>
			</dl>
		</div>
	</div>
	<div class="b-infos col-sm-5 col-sm-offset-1">
		<h3 class="sb-top xspan"><?=$this->bbf('title_contact_content_name');?></h3>
		<div class="sb-content">
			<div class="logo"><?=$url->img_html('img/site/logo_avencall_rgb_quadri_en.png',XIVO_AV_FR_CORP_NAME);?></div>
			<div class="clear"></div>
		</div>
		<div class="sb-content">
			<dl class="body">
				<dt><?=$this->bbf('info_address');?></dt>
					<dd><?=XIVO_AV_FR_CORP_ADDRESS?></dd>
					<dd><?=XIVO_AV_FR_CORP_ZIPCODE?> <?=XIVO_AV_FR_CORP_CITY?></dd>
					<dd><?=XIVO_AV_FR_CORP_COUNTRY?></dd>
				<dt><?=$this->bbf('info_phone');?></dt>
					<dd><?=XIVO_AV_FR_CORP_PHONE?></dd>
				<dt><?=$this->bbf('info_fax');?></dt>
					<dd><?=XIVO_AV_FR_CORP_FAX?></dd>
				<dt><?=$this->bbf('info_e-mail');?></dt>
					<dd><?='<a href="mailto:'.XIVO_AV_FR_CORP_EMAIL.'" title="'.XIVO_AV_FR_CORP_EMAIL.'">'.XIVO_AV_FR_CORP_EMAIL.'</a>'?></dd>
				<dt><?=$this->bbf('info_websites');?></dt>
					<dd><?='<a href="http://'.XIVO_AV_FR_CORP_URL.'" title="'.XIVO_AV_FR_CORP_URL.'" target="_blank">'.XIVO_AV_FR_CORP_URL.'</a>'?></dd>
					<dd><?='<a href="http://'.XIVO_SOFT_URL.'" title="'.XIVO_SOFT_LABEL.'" target="_blank">'.XIVO_SOFT_URL.'</a>'?></dd>
			</dl>
			<div class="clear"></div>
		</div>
	</div>
</div>