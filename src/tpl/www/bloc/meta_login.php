<?php

#
# XiVO Web-Interface
# Copyright 2016 The Wazo Authors  (see the AUTHORS file)
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

?>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="content-language" content="<?=DWHO_I18N_BABELFISH_LANGUAGE?>">

<meta name="charset" content="utf-8">
<meta name="robots" content="none">
<meta name="distribution" content="iu">
<meta name="title" content="<?=dwho_htmlsc($this->bbf('page_title',php_uname('n')));?>">

<link rel="icon" href="<?=$this->file_time($this->url('favicon.ico'));?>">
<link rel="shortcut icon" href="<?=$this->file_time($this->url('favicon.ico'));?>">

<link rel="stylesheet" type="text/css" href="<?=$this->file_time($this->url('/extra-libs/bootstrap/css/bootstrap.min.css'));?>">
<link rel="stylesheet" type="text/css" href="<?=$this->file_time($this->url('/extra-libs/adminlte/css/adminlte.min.css'));?>">
<link rel="stylesheet" type="text/css" href="<?=$this->file_time($this->url('/extra-libs/wazo/css/login.css'));?>">

<script type="text/javascript" src="<?=$this->file_time($this->url('extra-libs/jquery/jquery-1.9.1.min.js'));?>"></script>
<script type="text/javascript" src="<?=$this->file_time($this->url('extra-libs/bootstrap/js/bootstrap.min.js'));?>"></script>
<script type="text/javascript" src="<?=$this->file_time($this->url('extra-libs/adminlte/js/adminlte.min.js'));?>"></script>

<script type="text/javascript" src="<?=$this->file_time($this->url('js/dwho.js'));?>"></script>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/dwho/dom.js'));?>"></script>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/dwho/form.js'));?>"></script>
<script type="text/javascript" src="<?=$this->file_time($this->url('js/xivo.js'));?>"></script>
