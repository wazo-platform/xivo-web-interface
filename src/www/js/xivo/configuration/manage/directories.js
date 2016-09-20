/*
 * XiVO Web-Interface
 * Copyright (C) 2015-2016  Avencall
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
	if (directory_type == '2') {
		$('#fld-dird-form').hide();
		$('#fld-xivo-form').show();
	} else if (directory_type == '4') {
		$('#fld-xivo-form').hide();
		$('#fld-dird-form').show();
	} else {
		$('#fld-xivo-form').hide();
		$('#fld-dird-form').hide();
	}
}

function update_xivo_custom_ca_path() {
	var verify_cert = $('#it-xivo-verify-certificate-select').val();
	if (verify_cert == 'custom') {
		xivo_fm_enabled($('#it-xivo-custom-ca-path'));
	} else {
		xivo_fm_disabled($('#it-xivo-custom-ca-path'));
	}
}

$(function() {
	update_directory_type_info();
	update_xivo_custom_ca_path();

	$('#it-type').change(update_directory_type_info);
	$('#it-xivo-verify-certificate-select').change(update_xivo_custom_ca_path);
});
