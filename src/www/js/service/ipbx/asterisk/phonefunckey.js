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

function attachEvents(row) {
	row = $(row);
	var fktype = row.find('select[name="phonefunckey[type][]"]');
	var identity = row.find('input[name="phonefunckey[typevalidentity][]"]');
	var hidden = row.find('input[name="phonefunckey[typeval][]"]');
	var remove = row.find('.fkdelete');
	var supervision = row.find('select[name="phonefunckey[supervision][]"]');

	fktype.change(function() {
		identity.unbind('change').val('').show();
		hidden.val('');
		remove.unbind('click')
		row.find('select').unbind('change');
		row.find('.it-disabled').removeClass('it-disabled');
		row.find(".fkbsfilter").hide();
		attachEvents(row);
	});

	attachDestinationChange(row, fktype, identity, hidden);

	supervision.val('1');
	if(xivo_fk_supervision.indexOf(fktype.val()) === -1) {
		supervision.val('0');
		supervision.addClass('it-disabled');
	}

	remove.click(function(e) {
		e.preventDefault();
		row.detach();
	});
}

function attachDestinationChange(row, fktype, identity, hidden) {
	if (fktype.val() in xivo_fk_autocomplete) 
	{
		attachAutocomplete(fktype, identity, hidden);
	}
	else if (fktype.val() == "extenfeatures-bsfilter")
	{
		var bsfilter = row.find(".fkbsfilter");
		identity.hide();
		bsfilter.show();
		hidden.val(bsfilter.val());
		attachFillHidden(bsfilter, hidden);
	}
	else if (xivo_fk_text.indexOf(fktype.val()) != -1)
	{
		attachFillHidden(identity, hidden);
	}
	else {
		identity.addClass("it-disabled");
	}
}

function attachFillHidden(identity, hidden) {
	identity.change(function() {
		hidden.val(identity.val());
	});
}

function attachAutocomplete(fktype, identity, hidden) {
	identity.autocomplete({
		source: function(request, response) {
			var url = xivo_fk_autocomplete[fktype.val()] + dwho_sess_str;
			var body = encodeURI("except=5&search=" + request.term);
			$.post(url, body, function(data) {
				var suggestions = [];
				$(data).each(function(pos, item) {
					suggestions.push({label: item.identity, value: item.id});
				});
				response(suggestions);
			});
		},
		select: function(e, ui) {
			e.preventDefault();
			hidden.val(ui.item.value);
			identity.val(ui.item.label);
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
