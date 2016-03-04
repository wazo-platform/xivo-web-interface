/*
 * XiVO Web-Interface
 * Copyright (C) 2016  Avencall
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


$(document).ready(function() {
    $.datepicker.setDefaults({
        dateFormat: 'yy-mm-dd',
        changeYear: true,
        firstDay: 1,
        selectOtherMonths: true,
        dayNamesMin: xivo_date_day_min,
        dayNamesShort: xivo_date_day_short,
        dayNames: xivo_date_day,
        monthNames: xivo_date_month,
        monthNamesShort: xivo_date_month_short,
        nextText: xivo_date_next,
        prevText: xivo_date_prev,
        showAnim: 'fold',
        showMonthAfterYear: true,
        showWeek: true,
        weekHeader: 'W',
    });

    $("#it-dbeg").datepicker();
    $("#it-dend").datepicker();
});
