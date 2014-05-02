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


function xivo_ast_get_application_displayname(app)
{
    if(dwho_is_object(xivo_ast_application[app]) === true
    && dwho_is_undef(xivo_ast_application[app].displayname) === false)
        return(xivo_ast_application[app].displayname);

    return(false);
}

function xivo_ast_get_application_identityfunc(app)
{
    if(dwho_is_object(xivo_ast_application[app]) === true
    && dwho_is_undef(xivo_ast_application[app].identityfunc) === false)
        return(xivo_ast_application[app].identityfunc);

    return(false);
}

function xivo_ast_application_sanitize_arg(str)
{
    return(str.replace(/[\|,]/g,''));
}

function xivo_ast_chg_ipbxapplication(app)
{
    for(property in xivo_ast_application)
    {
        if((appkey = xivo_ast_get_application_displayname(property)) === false)
            continue;

        appkey = appkey.toLowerCase();

        if(app !== appkey && (appid = dwho_eid('fd-ipbxapplication-'+appkey)) !== false)
        {
            appid.style.display = 'none';
            dwho.form.reset_child_field(appid,false);
        }
    }

    if(app !== null && (appid = dwho_eid('fd-ipbxapplication-'+app)) !== false)
        appid.style.display = 'block';
}

var xivo_ast_application = {
    'macro|vmauthenticate':
                {displayname:    'VMAuthenticate',
                 identityfunc:    xivo_ast_application_get_vmauthenticate_identity},
    'absolutetimeout':    {displayname:    'AbsoluteTimeout'},
    'agi':            {displayname:    'AGI'},
    'answer':        {displayname:    'Answer'},
    'authenticate':        {displayname:    'Authenticate'},
    'background':        {displayname:    'BackGround'},
    'digittimeout':        {displayname:    'DigitTimeout'},
    'goto':            {displayname:    'Goto'},
    'gotoif':        {displayname:    'GotoIf'},
    'macro':        {displayname:    'Macro'},
    'mixmonitor':        {displayname:    'MixMonitor'},
    'monitor':        {displayname:    'Monitor'},
    'noop':            {displayname:    'NoOp'},
    'playback':        {displayname:    'Playback'},
    'record':        {displayname:    'Record'},
    'responsetimeout':    {displayname:    'ResponseTimeout'},
    'read':            {displayname:    'Read'},
    'set':            {displayname:    'Set'},
    'setcallerid':        {displayname:    'SetCallerID'},
    'setcidname':        {displayname:    'SetCIDName'},
    'setcidnum':        {displayname:    'SetCIDNum'},
    'setlanguage':        {displayname:    'SetLanguage'},
    'stopmonitor':        {displayname:    'StopMonitor'},
    'wait':            {displayname:    'Wait'},
    'waitexten':        {displayname:    'WaitExten'},
    'waitforring':        {displayname:    'WaitForRing'},
    'waitmusiconhold':    {displayname:    'WaitMusicOnHold'}};
