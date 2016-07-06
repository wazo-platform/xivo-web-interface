# -*- coding: UTF-8 -*-

# Copyright (C) 2015-2016 Avencall
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
from hamcrest import assert_that, has_entries, equal_to

from lib.testcase import TestWebi


def mock_template(*keys):
    keys = {pos: mock_funckey(**key)
            for pos, key in enumerate(keys, 1)}
    return {'id': 1,
            'name': None,
            'keys': keys,
            'links': [{u'href': u'https://webi:9486/1.1/funckeys/templates/1',
                       u'rel': u'func_key_templates'}]}


def mock_funckey(**funckey):
    funckey.setdefault('inherited', False)
    funckey.setdefault('label', None)
    funckey.setdefault('blf', False)
    return funckey


class TestFuncKey(TestWebi):

    DND = {'destination': {'type': 'service',
                           'href': None,
                           'service': 'enablednd'}}

    CUSTOM = {'destination': {u'exten': u'9999',
                              u'href': None,
                              u'type': u'custom'}}

    asset = 'funckeys'

    def create_user(self, firstname, lastname):
        with self.db.queries() as queries:
            user_id = queries.insert_user(firstname, lastname)

        self.confd.add_response("/users/{}/voicemail".format(user_id), code=404)
        return user_id


class TestFuncKeyEdit(TestFuncKey):

    def test_given_user_when_adding_funckey_then_adds_funckey_to_confd(self):
        user_id = self.create_user("John", "Doe")
        template = mock_template()

        template_url = "/users/{}/funckeys".format(user_id)
        funckey_url = "/users/{}/funckeys".format(user_id)

        self.confd.add_json_response(template_url, template)
        self.confd.add_response(funckey_url, method='PUT', code=204)

        users = self.browser.users
        user = users.edit("John Doe")
        user.funckeys().add(type="Do not disturb")
        user.save()

        expected_funckey = {'keys': {'1': {'blf': True,
                                           'label': None,
                                           'destination': {'type': 'service',
                                                           'service': 'enablednd'}}}}
        self.confd.assert_json_request(funckey_url, expected_funckey, 'PUT')

    def test_given_user_when_editing_funckey_then_updates_funckey_in_confd(self):
        template = mock_template(self.DND)
        user_id = self.create_user("Richard", "Smith")

        template_url = "/users/{}/funckeys".format(user_id)
        funckey_url = "/users/{}/funckeys".format(user_id)

        self.confd.add_json_response(template_url, template)
        self.confd.add_json_response(template_url, template)
        self.confd.add_json_response(template_url, template)
        self.confd.add_response(funckey_url, method='PUT', code=204)

        users = self.browser.users
        user = users.edit("Richard Smith")
        user.funckeys().edit(key=1,
                             type="Customized",
                             destination="666",
                             label="devil",
                             supervision=True)
        user.save()

        expected_funckey = {'keys': {'1': {'blf': True,
                                           'label': 'devil',
                                           'destination': {'type': 'custom',
                                                           'exten': '666'}}}}

        self.confd.assert_json_request(funckey_url, expected_funckey, method='PUT')

    def test_given_user_when_removing_funckey_then_updates_funckey_in_confd(self):
        user_id = self.create_user("Daffy", "Duck")
        dnd_template = mock_template(self.DND)
        empty_template = mock_template()

        template_url = "/users/{}/funckeys".format(user_id)
        funckey_url = "/users/{}/funckeys".format(user_id)

        self.confd.add_json_response(template_url, dnd_template)
        self.confd.add_json_response(template_url, dnd_template)
        self.confd.add_json_response(template_url, dnd_template)
        self.confd.add_json_response(template_url, empty_template)
        self.confd.add_json_response(funckey_url, self.DND)
        self.confd.add_response(funckey_url, method='PUT', code=204)

        users = self.browser.users
        user = users.edit("Daffy Duck")
        user.funckeys().remove(1)
        user.save()

        self.confd.assert_request_sent(funckey_url, method="PUT")

    def test_given_user_with_funckey_when_editing_then_funckey_appears_on_page(self):
        user_id = self.create_user("George", "Clooney")
        funckey = mock_funckey(blf=False,
                               label='speedy',
                               destination=self.CUSTOM['destination'])
        template = mock_template(funckey)

        template_url = "/users/{}/funckeys".format(user_id)

        self.confd.add_json_response(template_url, template)

        users = self.browser.users
        user = users.edit("George Clooney")

        webi_funckey = user.funckeys().get(1)
        expected_funckey = {'key': 1,
                            'type': 'Customized',
                            'destination': '9999',
                            'label': 'speedy',
                            'supervision': False}
        assert_that(webi_funckey, has_entries(expected_funckey))

        expected_url = '/users/{}/funckeys'.format(user_id)
        self.confd.assert_request_sent(expected_url)

    def test_given_funckey_has_no_supervision_when_editing_then_supervision_disabled(self):
        user_id = self.create_user("Nicole", "Kidman")
        funckey = mock_funckey(blf=True,
                               destination=self.CUSTOM['destination'])
        template = mock_template(funckey)

        template_url = "/users/{}/funckeys".format(user_id)

        self.confd.add_json_response(template_url, template)

        users = self.browser.users
        user = users.edit("Nicole Kidman")
        funckey = user.funckeys().get(1)

        assert_that(funckey['supervision'], equal_to(True))


class TestFuncKeyDelete(TestFuncKey):

    def test_given_user_with_funckey_when_deleting_user_then_deletes_funckeys_in_confd(self):
        user_id = self.create_user("John", "Doe")
        custom_template = mock_template(self.CUSTOM)
        empty_template = mock_template()

        template_url = "/users/{}/funckeys".format(user_id)
        funckey_url = "/users/{}/funckeys/1".format(user_id)

        self.confd.add_json_response(template_url, custom_template)
        self.confd.add_json_response(template_url, custom_template)
        self.confd.add_json_response(template_url, empty_template)
        self.confd.add_response(funckey_url, method='DELETE', code=204)

        users = self.browser.users
        users.delete("John Doe")

        self.confd.assert_request_sent(funckey_url, method="DELETE")


class TestFuncKeyCreate(TestFuncKey):

    def setUp(self):
        super(TestFuncKeyCreate, self).setUp()

        custom_exten = '9999'
        park_position = 701
        fwd_exten = '8888'

        with self.db.queries() as queries:
            user_id = queries.insert_user(firstname="Jimmy", lastname="John")
        #    secretary_id = queries.insert_user(firstname="Mary", lastname="Mall")
        #    group_id = queries.insert_group(name='Alcoholics Anonymous')
        #    queue_id = queries.insert_queue(name='File Ariane')
        #    conference_id = queries.insert_conference(name='C-F Moisi')
        #    agent_id = queries.insert_agent(firstname="Mary", lastname="Mall")

        #    filter_id = queries.insert_callfilter(name='Bull Shit')
        #    queries.insert_filter_member(filter_id, self.user_id, 'boss')
        #    filter_member_id = queries.insert_filter_member(filter_id, secretary_id, 'secretary')

        self.confd_funckeys = {
            '1': {'blf': True, 'label': None, 'destination': {'type': 'user', 'user_id': user_id}},
            #'2': {'blf': False, 'label': None, 'destination': {'type': 'group', 'group_id': group_id}},
            #'3': {'blf': False, 'label': None, 'destination': {'type': 'queue', 'queue_id': queue_id}},
            #'4': {'blf': True, 'label': None, 'destination': {'type': 'conference', 'conference_id': conference_id}},
            '5': {'blf': True, 'label': None, 'destination': {'type': 'custom', 'exten': custom_exten}},
            '6': {'blf': False, 'label': None, 'destination': {'type': 'onlinerec'}},
            '7': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'phonestatus'}},
            '8': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'recsnd'}},
            '9': {'blf': True, 'label': None, 'destination': {'type': 'service', 'service': 'callrecord'}},
            '10': {'blf': True, 'label': None, 'destination': {'type': 'service', 'service': 'incallfilter'}},
            '11': {'blf': True, 'label': None, 'destination': {'type': 'service', 'service': 'enablednd'}},
            '12': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'pickup'}},
            '13': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'calllistening'}},
            '14': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'directoryaccess'}},
            #'15': {'blf': True, 'label': None, 'destination': {'type': 'bsfilter', 'filter_member_id': filter_member_id}},
            '16': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'fwdundoall'}},
            '17': {'blf': True, 'label': None, 'destination': {'type': 'forward', 'forward': 'noanswer'}},
            '18': {'blf': True, 'label': None, 'destination': {'type': 'forward', 'forward': 'busy'}},
            '19': {'blf': True, 'label': None, 'destination': {'type': 'forward', 'forward': 'unconditional'}},
            '20': {'blf': True, 'label': None, 'destination': {'type': 'service', 'service': 'enablevm'}},
            '21': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'vmusermsg'}},
            '22': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'vmuserpurge'}},
            #'23': {'blf': True, 'label': None, 'destination': {'type': 'agent', 'action': 'toggle', 'agent_id': agent_id}},
            #'24': {'blf': True, 'label': None, 'destination': {'type': 'agent', 'action': 'login', 'agent_id': agent_id}},
            #'25': {'blf': True, 'label': None, 'destination': {'type': 'agent', 'action': 'logout', 'agent_id': agent_id}},
            #'26': {'blf': False, 'label': None, 'destination': {'type': 'service', 'service': 'paging'}},
            '27': {'blf': False, 'label': None, 'destination': {'type': 'transfer', 'transfer': 'blind'}},
            '28': {'blf': False, 'label': None, 'destination': {'type': 'transfer', 'transfer': 'attended'}},
            '29': {'blf': False, 'label': None, 'destination': {'type': 'parking'}},
            '30': {'blf': True, 'label': None, 'destination': {'type': 'park_position', 'position': park_position}},
            '31': {'blf': True, 'label': None, 'destination': {'type': 'forward', 'forward': 'busy', 'exten': fwd_exten}},
        }

        self.webi_funckeys = [
            {'key': '1', 'type': 'User', 'destination': 'Jimmy John'},
            #{'key': '2', 'type': 'Group', 'destination': 'Alcoholics Anonymous'},
            #{'key': '3', 'type': 'Queue', 'destination': 'File Ariane'},
            #{'key': '4', 'type': 'Conference room', 'destination': 'C-F Moisi'},
            {'key': '5', 'type': 'Customized', 'destination': custom_exten},
            {'key': '6', 'type': 'Online call recording'},
            {'key': '7', 'type': 'Phone status'},
            {'key': '8', 'type': 'Sound recording'},
            {'key': '9', 'type': 'Call recording'},
            {'key': '10', 'type': 'Incoming call filtering'},
            {'key': '11', 'type': 'Do not disturb'},
            {'key': '12', 'type': 'Group Interception'},
            {'key': '13', 'type': 'Listen to online calls'},
            {'key': '14', 'type': 'Directory access'},
            #{'key': '15', 'type': 'Filtering Boss - Secretary'},
            {'key': '16', 'type': 'Disable all forwarding'},
            {'key': '17', 'type': 'Enable / Disable forwarding on no answer'},
            {'key': '18', 'type': 'Enable / Disable forwarding on busy'},
            {'key': '19', 'type': 'Enable / Disable forwarding unconditional'},
            {'key': '20', 'type': 'Enable voicemail'},
            {'key': '21', 'type': 'Reach the voicemail'},
            {'key': '22', 'type': 'Delete messages from voicemail'},
            #{'key': '23', 'type': 'Connect/Disconnect an agent', 'destination': 'Mary Mall'},
            #{'key': '24', 'type': 'Connect an agent', 'destination': 'Mary Mall'},
            #{'key': '25', 'type': 'Disconnect an agent', 'destination': 'Mary Mall'},
            #{'key': '26', 'type': 'Paging'},
            {'key': '27', 'type': 'Blind transfer'},
            {'key': '28', 'type': 'Indirect transfer'},
            {'key': '29', 'type': 'Parking'},
            {'key': '30', 'type': 'Parking position', 'destination': park_position},
            {'key': '31', 'type': 'Enable / Disable forwarding on busy', 'destination': fwd_exten},
        ]

    def test_when_creating_user_with_func_key_then_creates_func_key_in_confd(self):
        template = mock_template()
        self.confd.add_json_response(r"/users/\d+/funckeys", template)
        self.confd.add_response(r"/users/\d+/funckeys", method='PUT', code=204)

        users = self.browser.users
        user = users.add()
        user.fill_form(firstname="John", lastname="Doe")
        funckeys = user.funckeys()

        for funckey in self.webi_funckeys:
            funckeys.add(**funckey)

        user.save()

        url = r"/users/\d+/funckeys"
        self.confd.assert_json_request(url, {'keys': self.confd_funckeys}, 'PUT')
