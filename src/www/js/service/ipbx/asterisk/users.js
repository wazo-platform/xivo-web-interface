/*
 * XiVO Web-Interface
 * Copyright (C) 2006-2014  Avencall
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

var xivo_ast_users_elt = {
	'links' : {
		'link' : []
	}
};
var xivo_ast_fm_users = {};

var xivo_ast_users_elt_default = {
	'userfeatures-firstname' : {
		it : true
	},
	'userfeatures-lastname' : {
		it : true
	},
	'userfeatures-entity' : {
		it : true
	},
	'userfeatures-ringseconds' : {
		it : true
	},
	'userfeatures-simultcalls' : {
		it : true
	},
	'userfeatures-musiconhold' : {
		it : true
	},
	'userfeatures-enableclient' : {
		it : true
	},
	'userfeatures-loginclient' : {
		it : true
	},
	'userfeatures-passwdclient' : {
		it : true
	},
	'userfeatures-enablehint' : {
		it : true
	},
	'userfeatures-enablexfer' : {
		it : true
	},
	'userfeatures-enableautomon' : {
		it : true
	},
	'userfeatures-callrecord' : {
		it : true
	},
	'userfeatures-incallfilter' : {
		it : true
	},
	'userfeatures-enablednd' : {
		it : true
	},
	'userfeatures-enablerna' : {
		it : true
	},
	'userfeatures-destrna' : {
		it : true
	},
	'userfeatures-enablebusy' : {
		it : true
	},
	'userfeatures-destbusy' : {
		it : true
	},
	'userfeatures-enableunc' : {
		it : true
	},
	'userfeatures-destunc' : {
		it : true
	},
	'userfeatures-bsfilter' : {
		it : true
	},
	'userfeatures-agentid' : {
		it : true
	},
	'userfeatures-outcallerid-type' : {
		it : true
	},
	'userfeatures-outcallerid-custom' : {
		it : true
	},
	'userfeatures-preprocess-subroutine' : {
		it : true
	},
	'userfeatures-description' : {
		it : true
	},
	'userfeatures-callerid' : {
		it : true
	},
	'userfeatures-entityid' : {
		it : true
	},
	'grouplist' : {
		it : true
	},
	'group' : {
		it : true
	},

	'rightcalllist' : {
		it : true
	},
	'rightcall' : {
		it : true
	}
};

var xivo_ast_fm_user_enablerna = {
	'it-userfeatures-destrna' : {
		property : [ {
			readOnly : true,
			className : 'it-readonly'
		}, {
			readOnly : false,
			className : 'it-enabled'
		} ]
	}
};

xivo_attrib_register('ast_fm_user_enablerna', xivo_ast_fm_user_enablerna);

var xivo_ast_fm_user_enablebusy = {
	'it-userfeatures-destbusy' : {
		property : [ {
			readOnly : true,
			className : 'it-readonly'
		}, {
			readOnly : false,
			className : 'it-enabled'
		} ]
	}
};

xivo_attrib_register('ast_fm_user_enablebusy', xivo_ast_fm_user_enablebusy);

var xivo_ast_fm_user_enableunc = {
	'it-userfeatures-destunc' : {
		property : [ {
			readOnly : true,
			className : 'it-readonly'
		}, {
			readOnly : false,
			className : 'it-enabled'
		} ]
	}
};

xivo_attrib_register('ast_fm_user_enableunc', xivo_ast_fm_user_enableunc);

var xivo_ast_fm_user_outcallerid = {
	'fd-userfeatures-outcallerid-custom' : {
		style : [ {
			display : 'none'
		}, {
			display : 'block'
		} ],
		link : 'it-outcallerid-custom'
	},
	'it-userfeatures-outcallerid-custom' : {
		property : [ {
			disabled : true
		}, {
			disabled : false
		} ]
	}
};

xivo_attrib_register('ast_fm_user_outcallerid', xivo_ast_fm_user_outcallerid);

var xivo_ast_fm_cpy_user_name = {
	'userfeatures-callerid' : false
};

function xivo_ast_user_cpy_name() {
	if (dwho_eid('it-userfeatures-firstname') === false
			|| dwho_eid('it-userfeatures-lastname') === false
			|| dwho_eid('it-userfeatures-callerid') === false)
		return (false);

	var name = '';
	var firstname = dwho_eid('it-userfeatures-firstname').value;
	var lastname = dwho_eid('it-userfeatures-lastname').value;

	if (dwho_is_undef(firstname) === false && firstname.length > 0)
		name += firstname;

	if (dwho_is_undef(lastname) === false && lastname.length > 0)
		name += name.length === 0 ? lastname : ' ' + lastname;

	var callerid = dwho_eid('it-userfeatures-callerid').value;

	if (dwho_is_undef(callerid) === true || callerid.length === 0)
		callerid = '';
	else
		callerid = callerid.replace(/^(?:"(.+)"|([^"]+))\s*<[^<]*>$/, '\$1');
}

function xivo_ast_user_chg_name() {
	var name = '';
	var firstname = dwho_eid('it-userfeatures-firstname').value;
	var lastname = dwho_eid('it-userfeatures-lastname').value;

	if (dwho_is_undef(firstname) === false && firstname.length > 0)
		name += firstname;

	if (dwho_is_undef(lastname) === false && lastname.length > 0)
		name += name.length === 0 ? lastname : ' ' + lastname;

	if (xivo_ast_fm_cpy_user_name['userfeatures-callerid'] === true)
		dwho_eid('it-userfeatures-callerid').value = name;

	return (true);
}

function xivo_ast_user_chg_enablerna() {
	if ((enablerna = dwho_eid('it-userfeatures-enablerna')) !== false)
		xivo_chg_attrib('ast_fm_user_enablerna', 'it-userfeatures-destrna',
				Number(enablerna.checked));
}

function xivo_ast_user_chg_enablebusy() {
	if ((enablebusy = dwho_eid('it-userfeatures-enablebusy')) !== false)
		xivo_chg_attrib('ast_fm_user_enablebusy', 'it-userfeatures-destbusy',
				Number(enablebusy.checked));
}

function xivo_ast_user_chg_enableunc() {
	if ((enableunc = dwho_eid('it-userfeatures-enableunc')) !== false)
		xivo_chg_attrib('ast_fm_user_enableunc', 'it-userfeatures-destunc',
				Number(enableunc.checked));
}

function xivo_ast_user_ingroup() {
	dwho.form.move_selected('it-grouplist', 'it-group');

	if ((grouplist = dwho_eid('it-group')) === false
			|| (len = grouplist.length) < 1)
		return (false);

	for (var i = 0; i < len; i++) {
		if ((group = dwho_eid('group-' + grouplist[i].value)) !== false)
			group.style.display = 'table-row';
	}

	if (dwho_eid('it-group').length > 0)
		dwho_eid('no-group').style.display = 'none';

	return (true);
}

function xivo_ast_user_outgroup() {
	dwho.form.move_selected('it-group', 'it-grouplist');

	if ((grouplist = dwho_eid('it-grouplist')) === false
			|| (len = grouplist.length) < 1)
		return (false);

	for (var i = 0; i < len; i++) {
		if ((group = dwho_eid('group-' + grouplist[i].value)) !== false)
			group.style.display = 'none';
	}

	if (dwho_eid('it-group').length === 0)
		dwho_eid('no-group').style.display = 'table-row';

	return (true);
}

function xivo_ast_user_onload() {
	if ((firstname = dwho_eid('it-userfeatures-firstname')) !== false) {
		dwho.dom.add_event('change', firstname, xivo_ast_user_cpy_name);
		dwho.dom.add_event('focus', firstname, xivo_ast_user_cpy_name);
		dwho.dom.add_event('blur', firstname, xivo_ast_user_chg_name);
	}

	if ((lastname = dwho_eid('it-userfeatures-lastname')) !== false) {
		dwho.dom.add_event('change', lastname, xivo_ast_user_chg_name);
		dwho.dom.add_event('focus', lastname, xivo_ast_user_cpy_name);
		dwho.dom.add_event('blur', lastname, xivo_ast_user_chg_name);
	}

	if ((outcallerid_type = dwho_eid('it-userfeatures-outcallerid-type')) !== false) {
		var outcallerid_type_fn = function() {
			xivo_chg_attrib('ast_fm_user_outcallerid',
					'fd-userfeatures-outcallerid-custom',
					Number(outcallerid_type.value === 'custom'));
		};

		outcallerid_type_fn();

		dwho.dom.add_event('change', outcallerid_type, outcallerid_type_fn);
	}

	dwho.dom.add_event('change', dwho_eid('it-userfeatures-enablerna'),
			xivo_ast_user_chg_enablerna);

	dwho.dom.add_event('change', dwho_eid('it-userfeatures-enablebusy'),
			xivo_ast_user_chg_enablebusy);

	dwho.dom.add_event('change', dwho_eid('it-userfeatures-enableunc'),
			xivo_ast_user_chg_enableunc);

	xivo_ast_build_dialaction_array('noanswer');
	xivo_ast_build_dialaction_array('busy');
	xivo_ast_build_dialaction_array('congestion');
	xivo_ast_build_dialaction_array('chanunavail');

	xivo_ast_dialaction_onload();
}

function update_callerid() {
	var firstname = $('#it-userfeatures-firstname').val();
	var lastname = $('#it-userfeatures-lastname').val();
	var callerid = $('#it-userfeatures-callerid').val();

	var name = '';
	if (firstname && firstname.length > 0)
		name += firstname;
	if (lastname && lastname.length > 0)
		name += name.length === 0 ? lastname : ' ' + lastname;
	
	callerid = name.replace(/^(?:"(.+)"|([^"]+))\s*<[^<]*>$/, '\$1');
	
	$('#it-userfeatures-callerid').val(callerid);
	
	//xivo_ast_user_cpy_name();
	//xivo_ast_user_chg_name();
}

$(function() {
	$('#it-userfeatures-firstname').change(update_callerid);
	$('#it-userfeatures-lastname').change(update_callerid);
});

dwho.dom.set_onload(xivo_ast_user_onload);

function updateVoicemailName() {
	var name = $("#it-userfeatures-firstname").val();
	var lastname = $("#it-userfeatures-lastname").val();

	if(lastname != "") {
		name += " " + lastname;
	}

	$("#it-voicemail-name").val(name);
}

function updateVoicemailLanguage() {
	var language = $("#it-userfeatures-language").val();
	$("#it-voicemail-language").val(language);
}

$(function() {
	var voicemail_id = $('#it-voicemail-id').val();
	if (voicemail_id == "") {
		// create
		$("#fld-voicemail-form").hide();
		$("#vm-action-delete").hide();
	} else {
		// update
		$("#vm-action-add").hide();
		$("#vm-action-search").hide();
	}

	$("#it-userfeatures-language").change(updateVoicemailLanguage);
	$("#it-userfeatures-firstname").change(updateVoicemailName);
	$("#it-userfeatures-lastname").change(updateVoicemailName);

	$("#user-vm-add").click(function(e) {
		e.preventDefault();
		$('#fld-voicemail-form').show();
		$('#user-vm-action').val('add');
		$('#fld-voicemail-form input[type="text"]').val('');
		$('#fld-voicemail-form input[type="hidden"]').val('');
		$('#fld-voicemail-form select').val('');
		$('#fld-voicemail-form input[type="checkbox"]').attr('checked', false);
		$('#it-userfeatures-enablevoicemail').attr('checked', true);
		updateVoicemailName();
		updateVoicemailLanguage();
	});

	$("#user-vm-delete").click(function(e) {
		e.preventDefault();
		$("#fld-voicemail-form").hide();
		$('#user-vm-action').val('delete');
	});

	$('#user-vm-search').autocomplete({
		source: function(request, response) {
			$.ajax({
				url: '/service/ipbx/ui.php/pbx_settings/users/voicemail/search/?' + dwho_sess_str,
				data: encodeURI("search=" + request.term),
				method: "POST",
				timeout: 3000,
				success: function(data) {
					var suggestions = [];
					$(data).each(function(pos, item) {
						suggestions.push({label: item.identity, value: item.id});
					});
					response(suggestions);
				}
			});
		},
		select: function(e, ui) {
			e.preventDefault();
			$("#fld-voicemail-form").show();
			$.ajax({
				url: '/service/ipbx/ui.php/pbx_settings/users/voicemail/view/?' + dwho_sess_str,
				method: "POST",
				data: encodeURI("id=" + ui.item.value),
				timeout: 3000,
				success: function(data) {
					$("#user-vm-action").val('search');
					$('#it-userfeatures-enablevoicemail').attr('checked', true);
					$("input[name='voicemail[id]']").val(data.id);
					$("input[name='voicemail[name]']").val(data.name);
					$("input[name='voicemail[number]']").val(data.number);
					$("input[name='voicemail[password]']").val(data.password);
					$("input[name='voicemail[email]']").val(data.email);
					$("input[name='voicemail[context]']").val(data.context);
					$("input[name='voicemail[timezone]']").val(data.timezone);
					$("input[name='voicemail[language]']").val(data.language);
					$("input[name='voicemail[max_messages]']").val(data.max_messages);
					$("input[name='voicemail[ask_password]']").attr('checked', data.ask_password);
					$("input[name='voicemail[attach_audio]']").attr('checked', data.attach_audio);
					$("input[name='voicemail[delete_messages]']").attr('checked', data.delete_messages);
				}
			});
			$("#user-vm-search").val('');
		}
	})
});
