/*
 * XiVO Web-Interface
 * Copyright (C) 2006-2015  Avencall
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

var xivo_fk_autocomplete = {
	"extenfeatures-agentstaticlogtoggle": '/callcenter/ui.php/settings/agents?act=search&',
	"extenfeatures-agentstaticlogin": '/callcenter/ui.php/settings/agents?act=search&',
	"extenfeatures-agentstaticlogoff": '/callcenter/ui.php/settings/agents?act=search&',
	"user": '/service/ipbx/ui.php/pbx_settings/users/search/?',
	"group": '/service/ipbx/ui.php/pbx_settings/users/groups/search/?',
	"queue": '/callcenter/ui.php/settings/queues?act=search&',
	"meetme": '/service/ipbx/ui.php/pbx_settings/users/meetme/search/?',
	"extenfeatures-paging": '/service/ipbx/ui.php/pbx_settings/users/paging/search/?'
}

var xivo_fk_text = [
	"custom",
	"generalfeatures-parkpos",
	"extenfeatures-fwdrna",
	"extenfeatures-fwdbusy",
	"extenfeatures-fwdunc",
];

function resetRow(fields) {
	fields.type
		.unbind('change');
	fields.identity
		.unbind('change')
		.removeClass('it-disabled')
		.val('')
		.show();
    fields.identity.autocomplete("destroy");
	fields.destination
		.val('');
	fields.supervision
		.removeClass('it-disabled')
		.val('1');
	fields.remove
		.unbind('click');
	fields.bsfilter
		.hide();
}

function attachEvents(row) {
	row = $(row);

	var fields = {
		type: row.find('select[name="phonefunckey[type][]"]'),
		identity: row.find('input[name="phonefunckey[typevalidentity][]"]'),
		destination: row.find('input[name="phonefunckey[typeval][]"]'),
		remove: row.find('.fkdelete'),
		supervision: row.find('select[name="phonefunckey[supervision][]"]'),
		bsfilter: row.find('.fkbsfilter')
	}

	fields.type.change(function() {
		resetRow(fields)
		attachEvents(row);
	});

	attachDestinationChange(fields)

	if(xivo_fk_supervision.indexOf(fields.type.val()) === -1) {
		fields.supervision.val('0');
		fields.supervision.addClass('it-disabled');
	}

	fields.remove.click(function(e) {
		e.preventDefault();
		row.detach();
	});
}

function attachDestinationChange(fields) {
	var fktype = fields.type.val();
	if (fktype in xivo_fk_autocomplete)
	{
		attachAutocomplete(fields)
	}
	else if (fktype == "extenfeatures-bsfilter")
	{
		fields.identity.hide();
		fields.bsfilter.show();
		fields.destination.val(fields.bsfilter.val());
		attachFillHidden(fields.bsfilter, fields.destination);
	}
	else if (xivo_fk_text.indexOf(fktype) != -1)
	{
		attachFillHidden(fields.identity, fields.destination);
	}
	else {
		fields.identity.addClass("it-disabled");
	}
}

function attachFillHidden(identity, hidden) {
	identity.change(function() {
		hidden.val(identity.val());
	});
}

function attachAutocomplete(fields) {
	fields.identity.autocomplete({
		source: function(request, response) {
			var fktype = fields.type.val();
			var url = xivo_fk_autocomplete[fktype] + dwho_sess_str;
			var body = encodeURI("search=" + request.term);
			var settings = {
				url: url,
				method: "POST",
				data: body,
				timeout: 3000,
				success: function(data) {
					var suggestions = [];
					$(data).each(function(pos, item) {
						suggestions.push({label: item.identity, value: item.id});
					});
					response(suggestions);
				}
			};
			$.ajax(settings);
		},
		select: function(e, ui) {
			e.preventDefault();
			fields.destination.val(ui.item.value);
			fields.identity.val(ui.item.label);
		}
	});
}

$(document).ready(function() {
	$('#add_funckey_button').click(function(e) {
		e.preventDefault();
		var selector = '#phonefunckey tr:last select[name="phonefunckey[fknum][]"]';
		var position = parseInt($(selector).val());
		$('#phonefunckey').append(xivo_fk_row);
		$(selector).val(position + 1);
		attachEvents($('#phonefunckey tr:last'));
		$('#no-phonefunckey').detach();
	});

	$('tbody#phonefunckey tr').each(function(index, row) {
		attachEvents(row);
	});
});
