/*
 * XiVO Web-Interface
 * Copyright (C) 2006-2016  Avencall
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

function xivo_wizard_onload() {
	dwho.dom.add_event('change', dwho_eid('it-language'), function() {
		this.form['refresh'].value = 1;
		this.form.submit();
	});
	dwho.dom.add_event('click', dwho_eid('it-previous'), function() {
		this.type = 'submit';
	});

    $('form').submit(function(event) {
                $('#validate').css('opacity', '0.4');
                $('#validate').attr('disabled', true);
    });

}

dwho.dom.set_onload(xivo_wizard_onload);
