# -*- coding: UTF-8 -*-

# Copyright (C) 2015 Avencall
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

import unittest

from hamcrest import assert_that, has_entries

from lib import setup as testsetup


class TestFuncKey(unittest.TestCase):

    EMPTY_TEMPLATE = {u'id': 1,
                      u'keys': {},
                      u'links': [{u'href': u'https://webi:9486/1.1/funckeys/templates/1',
                                  u'rel': u'func_key_templates'}],
                      u'name': u'John Doe'}

    DND_TEMPLATE = {u'id': 1,
                    u'keys': {u'1': {u'blf': False,
                                     u'destination': {u'href': None,
                                                      u'service': u'enablednd',
                                                      u'type': u'service'},
                                     u'id': 10,
                                     u'inherited': False,
                                     u'label': None}},
                    u'links': [{u'href': u'http://webi:9487/1.1/funckeys/templates/1',
                                u'rel': u'func_key_templates'}],
                    u'name': u'Richard Smith'}

    CUSTOM_TEMPLATE = {u'id': 1,
                       u'keys': {u'1': {u'blf': True,
                                        u'destination': {u'exten': u'9999', u'href': None, u'type': u'custom'},
                                        u'id': 20,
                                        u'inherited': False,
                                        u'label': u'speedy'}},
                       u'links': [{u'href': u'https://webi:9487/1.1/funckeys/templates/1',
                                   u'rel': u'func_key_templates'}],
                       u'name': u'George Clooney'}

    asset = 'funckeys'

    @classmethod
    def setUpClass(cls):
        testsetup.setup_docker(cls.asset)

        cls.db = testsetup.setup_db()
        cls.db.recreate()

        cls.browser = testsetup.setup_browser()
        cls.browser.start()
        cls.browser.login.login('root', 'proformatique')

        cls.confd = testsetup.setup_confd()

    @classmethod
    def tearDownClass(cls):
        cls.browser.stop()

    def setUp(self):
        self.addCleanup(self.confd.clear)

    def prepare_user(self, firstname, lastname, template):
        with self.db.queries() as queries:
            user_id = queries.insert_user(firstname, lastname)
            url = "/users/{}/funckeys".format(user_id)
            self.confd.add_json_response(url, template)
            return user_id


class TestFuncKeyEdit(TestFuncKey):

    def test_given_user_when_adding_funckey_then_adds_funckey_to_confd(self):
        user_id = self.prepare_user("John", "Doe", self.EMPTY_TEMPLATE)
        expected_url = "/users/{}/funckeys/1".format(user_id)

        self.confd.add_response(expected_url, method='PUT', code=204)

        users = self.browser.users.go()
        user = users.edit("John Doe")
        user.funckeys().add(type="Do not disturb")
        user.save()

        expected_funckey = {'blf': True,
                            'label': None,
                            'destination': {'type': 'service',
                                            'service': 'enablednd'}}
        self.confd.assert_json_request(expected_url, 'PUT', expected_funckey)

    def test_given_user_when_editing_funckey_then_updates_funckey_in_confd(self):
        user_id = self.prepare_user("Richard", "Smith", self.DND_TEMPLATE)
        expected_url = "/users/{}/funckeys/1".format(user_id)

        self.confd.add_response(expected_url, method='PUT', code=204)

        users = self.browser.users.go()
        user = users.edit("Richard Smith")
        user.funckeys().edit(key=1,
                             type="Customized",
                             destination="666",
                             label="devil",
                             supervision=True)
        user.save()

        expected_funckey = {'blf': True,
                            'label': 'devil',
                            'destination': {'type': 'custom',
                                            'exten': '666'}}
        self.confd.assert_json_request(expected_url, 'PUT', expected_funckey)

    def test_given_user_when_removing_funckey_then_updates_funckey_in_confd(self):
        user_id = self.prepare_user("Daffy", "Duck", self.DND_TEMPLATE)
        expected_url = "/users/{}/funckeys/1".format(user_id)

        self.confd.add_response(expected_url, method='DELETE', code=204)

        users = self.browser.users.go()
        user = users.edit("Daffy Duck")
        user.funckeys().remove(1)
        user.save()

        expected_request = {'method': 'DELETE',
                            'path': expected_url}
        self.confd.assert_request_sent(expected_request)

    def test_given_user_with_dnd_when_editing_then_dnd_appears_on_funckey_page(self):
        user_id = self.prepare_user("George", "Clooney", self.CUSTOM_TEMPLATE)

        users = self.browser.users.go()
        user = users.edit("George Clooney")

        funckey = user.funckeys().get(1)
        expected_funckey = {'key': 1,
                            'type': 'Customized',
                            'destination': '9999',
                            'label': 'speedy',
                            'supervision': True}
        assert_that(funckey, has_entries(expected_funckey))

        expected_request = {'method': 'GET',
                            'path': '/users/{}/funckeys'.format(user_id)}
        self.confd.assert_request_sent(expected_request)


class TestFuncKeyDelete(TestFuncKey):

    def test_given_user_with_funckey_when_deleting_user_then_deletes_funckeys_in_confd(self):
        user_id = self.prepare_user("John", "Doe", self.CUSTOM_TEMPLATE)
        position = 1

        expected_url = "/users/{}/funckeys/{}".format(user_id, position)
        self.confd.add_response(expected_url, method='DELETE', code=204)

        users = self.browser.users.go()
        users.delete("John Doe")

        expected_request = {'method': 'DELETE',
                            'path': expected_url}
        self.confd.assert_request_sent(expected_request)


class TestFuncKeyCreate(TestFuncKey):

    def setUp(self):
        super(TestFuncKeyCreate, self).setUp()

        custom_exten = '9999'
        park_position = 701
        fwd_exten = '8888'

        with self.db.queries() as queries:
            self.user_id = queries.insert_user(firstname="Jimmy", lastname="John")
            #secretary_id = queries.insert_user(firstname="Mary", lastname="Mall")
            #group_id = queries.insert_group(name='Alcoholics Anonymous')
            #queue_id = queries.insert_queue(name='File Ariane')
            #conference_id = queries.insert_conference(name='C-F Moisi')
            #agent_id = queries.insert_agent(firstname="Mary", lastname="Mall")

            #filter_id = queries.insert_callfilter(name='Bull Shit')
            #queries.insert_filter_member(filter_id, self.user_id, 'boss')
            #filter_member_id = queries.insert_filter_member(filter_id, secretary_id, 'secretary')

        self.confd.add_json_response("/users/{}/funckeys".format(self.user_id), self.EMPTY_TEMPLATE)

        self.confd_funckeys = {
            '1': {'blf': True, 'label': None, 'destination': {'type': 'user', 'user_id': self.user_id}},
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
        self.confd.add_json_response(r"/users/\d+/funckeys", self.EMPTY_TEMPLATE)

        for position in self.confd_funckeys.keys():
            self.confd.add_response(r"/users/\d+/funckeys/{}".format(position),
                                    method='PUT',
                                    code=204)

        users = self.browser.users.go()
        user = users.add()
        user.fill_form(firstname="John", lastname="Doe")
        funckeys = user.funckeys()

        for funckey in self.webi_funckeys:
            funckeys.add(**funckey)

        user.save()

        for position, funckey in self.confd_funckeys.items():
            url = r"/users/\d+/funckeys/{}".format(position)
            self.confd.assert_json_request(url, 'PUT', funckey)
