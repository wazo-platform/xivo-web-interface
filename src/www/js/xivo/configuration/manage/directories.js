/*
 * XiVO Web-Interface
 * Copyright 2015-2018 The Wazo Authors  (see the AUTHORS file)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

function update_directory_type_info() {
	var directory_type = $('#it-type').val();
	var uri = $('#it-uri').val();
	const default_phonebook_uri = 'postgresql://asterisk:proformatique@localhost/asterisk';
	if (directory_type == '2') {
		$('#fld-dird-form').hide();
		$('#fld-xivo-form').show();
		$('#div-ldap-uri').hide();
		$('#div-free-uri').show();
	} else if (directory_type == '3') {
		$('#fld-xivo-form').hide();
		$('#fld-dird-form').show();
		$('#div-ldap-uri').hide();
		$('#div-free-uri').show();
		$('#it-uri').val(uri || default_phonebook_uri);
	} else if (directory_type == '4') {
		$('#fld-xivo-form').hide();
		$('#fld-dird-form').hide();
		$('#div-ldap-uri').show();
		$('#div-free-uri').hide();
	} else {
		$('#fld-xivo-form').hide();
		$('#fld-dird-form').hide();
		$('#div-ldap-uri').hide();
		$('#div-free-uri').show();
	}
}

function enable_if_custom(element, value) {
	if (value == 'custom') {
		xivo_fm_enabled(element);
	} else {
		xivo_fm_disabled(element);
	}
}

function update_custom_ca_path() {
	var dird_verify_cert = $('#it-xivo-verify-certificate-select').val();
	var auth_verify_cert = $('#it-auth-verify-certificate-select').val();
	enable_if_custom($('#it-xivo-custom-ca-path'), dird_verify_cert);
	enable_if_custom($('#it-auth-custom-ca-path'), auth_verify_cert);
}

$(function() {
	update_directory_type_info();
	update_custom_ca_path();

	$('#it-type').change(update_directory_type_info);
	$('#it-xivo-verify-certificate-select').change(update_custom_ca_path);
	$('#it-auth-verify-certificate-select').change(update_custom_ca_path);
});
