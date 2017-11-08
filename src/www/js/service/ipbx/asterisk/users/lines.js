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


function map_autocomplete_line_free_to(obj,list,exept)
{
    $(obj).show();
    if (list === null || (nb = list.length) === 0)
        return false;
    if(!exept)
        exept = 0;
    $(obj).find('option').each(function(){
        if(exept != $(this).val()
        && dwho_in_array($(this).val(),list))
            $(this).remove();
    });
}

//get available line for a device
function xivo_http_search_line_from_provd(obj,config,exept)
{
    if (config == ''){
        obj.hide();
        return;
    }
    $.ajax({
        url: '/xivo/configuration/ui.php/provisioning/configs?act=get&id='+config,
        async: false,
        dataType: 'json',
        success: function(data) {
            map_autocomplete_line_free_to(obj,data,exept);
        }
    });
}

//get available extensions
function map_autocomplete_extension_to(obj,context)
{
    $.getJSON('/service/ipbx/ui.php/pbx_settings/extension/search/?obj=user&format=jquery&context='+context, function(data) {
        if (data === null || (nb = data.length) === 0)
            return false;
        obj.autocomplete({
            source: data.split('\n')
        });
    });
}

//get available number pool
function xivo_http_search_numpool(context,helper)
{
    var rs = '';
    $.ajax({
        url: '/service/ipbx/ui.php/pbx_settings/extension/search/?context='+context+'&obj=user&getnumpool=1',
        async: false,
        dataType: 'json',
        success: function(data) {
            if (data === null || (nb = data.length) === 0)
                return false;
            for (var i = 0; i< nb; i++)
                rs += data[i]['numberbeg']+' - '+data[i]['numberend']+'<br>';
            $(helper).html(rs);
        }
    });
}

//get list context available for a entity
function xivo_http_search_context_from_entity(entityid)
{
    if (entityid == false)
        return;

    $.ajax({
        url: '/xivo/configuration/ui.php/manage/entity?act=get&id='+entityid+'&contexttype=internal',
        async: false,
        dataType: 'json',
        success: function(data) {
            if (data === null || (nb = data.length) === 0) {
                $('#list_linefeatures').hide();
                $('#box-no_context').show();
                return false;
            }
            $('#box-no_context').hide();
            $('#list_linefeatures').show();
            $('#list_linefeatures').find("#linefeatures-context").each(function(){
                $(this).find('option').remove();
                for (var i = 0; i< nb; i++)
                    $(this).append("<option value=" + data[i]['name'] + ">" + data[i]['displayname'] + "</option>");
            });
            $('#ex-linefeatures').find("#linefeatures-context").each(function(){
                $(this).find('option').remove();
                for (var i = 0; i< nb; i++)
                    $(this).append("<option value=" + data[i]['name'] + ">" + data[i]['displayname'] + "</option>");
            });
            update_row_infos();
        }
    });
}

function lnkdroprow(obj)
{
    $(obj).parents('tr').fadeTo(400, 0, function () {
        $(this).remove();
        $('#save-before-add-linefeatures').show();
    });
}

function get_entityid_val()
{
    it_userfeatures_entityid = $('#it-userfeatures-entityid');
    it_cache_entityid = $('#it-cache_entityid');

    if ((entityid_val = it_userfeatures_entityid.val()) === false)
        entityid_val = it_cache_entityid.val();

    if (!entityid_val)
        return false;

    return(entityid_val);
}

function update_row_infos()
{
    if ((entityid_val = get_entityid_val()) === false)
        return(false);

    enable_disable_add_button();

    nb_row = $('#list_linefeatures > tbody > tr').length;

    if (nb_row == 0) {
        $('#box-entityid').text('');
        it_userfeatures_entityid.removeAttr('disabled');
        it_userfeatures_entityid.removeClass('it-disabled');
        return false;
    }
    else {
        $('#box-entityid').html('<input type="hidden" id="it-cache_entityid" name="userfeatures[entityid]" value="'+entityid_val+'" />');
        it_userfeatures_entityid.attr('disabled','disabled');
        it_userfeatures_entityid.addClass('it-disabled');
    }

    $('#list_linefeatures > tbody').find('tr').each(function() {
        var MaxNbOfLines = 32;

        context = $(this).find("#linefeatures-context");

        context_selected = context.parents('tr').find('#context-selected').val();
        if (context_selected !== null)
            context.find("option[value='"+context_selected+"']").attr("selected","selected");

        if (context.val() !== null) {
            devicenumline = $(context).parents('tr').find("#linefeatures-num");
            config = $(context).parents('tr').find('#linefeatures-device').val();
            xivo_http_search_line_from_provd(devicenumline,config,devicenumline.val());

            var number = context.parents('tr').find('#linefeatures-number');
            number.focus(function(){
                helper = $(this).parent().find('#numberpool_helper');
                context = $(this).parents('tr').find("#linefeatures-context");
                xivo_http_search_numpool(context.val(),helper);
                helper.show('slow');
                map_autocomplete_extension_to($(this),context.val());
            });
            number.blur(function(){
                $(this).parent().find('#numberpool_helper').hide('slow');
            });
            device = $(this).find('#linefeatures-device').val();
            devicenumline = $(this).find("#linefeatures-num");
            if (device == '')
                devicenumline.hide();

            $(this).find('#linefeatures-device').select2();

            $(this).find('#linefeatures-device').change(function() {
                devicenumline = $(this).parents('tr').find("#linefeatures-num");
                $(devicenumline).each(function(){
                    $(this).find('option').remove();
                    for (var i=1; i<=MaxNbOfLines; i++)
                        $(this).append("<option value="+i+">"+i+"</option>");
                });
                xivo_http_search_line_from_provd(devicenumline,$(this).val());
            });
        }
    });
}

function enable_disable_add_button() {
    nb_row = $('#list_linefeatures > tbody > tr').length;
    if (nb_row > 0) {
        $('#lnk-add-row').hide();
        return false;
    }
        $('#lnk-add-row').show();
    return true;
}


$(document).ready(function() {

    xivo_http_search_context_from_entity(get_entityid_val());

    $('#it-userfeatures-entityid').change(function() {
        xivo_http_search_context_from_entity($(this).val());
    });

    enable_disable_add_button();

    $('#lnk-add-row').click(function() {

        if (!enable_disable_add_button())
                return false;

        $('#no-linefeatures').hide('fast');
        row = $('#ex-linefeatures').html();

        $('#list_linefeatures > tbody:last').fadeIn(400, function () {
            $(this).append(row);
        });

        update_row_infos();
        return false;
    });

    $('#linefeatures-device').select2({
        minimumInputLength: 2,
        ajax: {
          url: '/xivo/configuration/ui.php/provisioning/configs',
          dataType: 'json',
          delay: 250,
          data: function (params) {
            var query = {
              term: params.term,
              act: 'search'
            }
            return query;
          }
        }
      });

});
