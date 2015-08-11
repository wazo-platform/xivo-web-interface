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
            self.confd.add_response(url, template)
            return user_id


class TestFuncKeyEdit(TestFuncKey):

    def test_given_user_when_adding_funckey_then_adds_funckey_to_confd(self):
        user_id = self.prepare_user("John", "Doe", self.EMPTY_TEMPLATE)

        users = self.browser.users.go()
        user = users.edit("John Doe")
        user.funckeys().add(type="Do not disturb")
        user.save()

        expected_request = {'method': 'PUT',
                            'url': '/users/{}/funckeys/1'.format(user_id),
                            'body': {'blf': False,
                                     'label': None,
                                     'destination': {'type': 'service',
                                                     'service': 'enablednd'}}}
        self.confd.assert_request_sent(expected_request)

    def test_given_user_when_editing_funckey_then_updates_funckey_in_confd(self):
        user_id = self.prepare_user("Richard", "Smith", self.DND_TEMPLATE)

        users = self.browser.users.go()
        user = users.edit("Richard Smith")
        user.funckeys().edit(key=1,
                             type="Customized",
                             destination="666",
                             label="devil",
                             supervision=True)
        user.save()

        expected_request = {'method': 'PUT',
                            'url': '/users/{}/funckeys/1'.format(user_id),
                            'body': {'blf': True,
                                     'label': 'devil',
                                     'destination': {'type': 'custom',
                                                     'exten': '666'}}}
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
                            'url': '/users/{}/funckeys'.format(user_id)}
        self.confd.assert_request_sent(expected_request)


class TestFuncKeyDelete(TestFuncKey):

    def test_given_user_with_funckey_when_deleting_user_then_deletes_funckeys_in_confd(self):
        position = 2

        with self.db.queries() as queries:
            user_id = queries.insert_user(firstname="John", lastname="Doe")
            queue_id = queries.insert_queue()
            funckey_id = queries.insert_func_key('speeddial', 'queue')
            queries.insert_destination('queue', 'queue_id', funckey_id, queue_id)
            queries.add_func_key_to_user(position, funckey_id, user_id)

        users = self.browser.users.go()
        users.delete("John Doe")

        expected_request = {'method': 'DELETE',
                            'url': '/users/{}/funckeys/{}'.format(user_id, position)}
        self.confd.assert_request_sent(expected_request)


class TestFuncKeyCreate(TestFuncKey):

    def setUp(self):
        super(TestFuncKeyCreate, self).setUp()

        custom_exten = '9999'
        park_position = 701
        fwd_exten = '8888'

        with self.db.queries() as queries:
            user_id = queries.insert_user(firstname="Jimmy", lastname="John")
            secretary_id = queries.insert_user(firstname="Mary", lastname="Mall")
            group_id = queries.insert_group(name='Alcoholics Anonymous')
            queue_id = queries.insert_queue(name='File Ariane')
            conference_id = queries.insert_conference(name='C-F Moisi')
            agent_id = queries.insert_agent(firstname="Mary", lastname="Mall")

            filter_id = queries.insert_callfilter(name='Bull Shit')
            queries.insert_filter_member(filter_id, user_id, 'boss')
            filter_member_id = queries.insert_filter_member(filter_id, secretary_id, 'secretary')

        self.confd_funckeys = {
            '1': {'blf': False, 'label': None, 'type': 'user', 'user_id': user_id},
            '2': {'blf': False, 'label': None, 'type': 'group', 'group_id': group_id},
            '3': {'blf': False, 'label': None, 'type': 'queue', 'queue_id': queue_id},
            '4': {'blf': False, 'label': None, 'type': 'conference', 'conference_id': conference_id},
            '5': {'blf': False, 'label': None, 'type': 'custom', 'exten': custom_exten},
            '6': {'blf': False, 'label': None, 'type': 'onlinerec'},
            '7': {'blf': False, 'label': None, 'type': 'service', 'service': 'phonestatus'},
            '8': {'blf': False, 'label': None, 'type': 'service', 'service': 'recsnd'},
            '9': {'blf': False, 'label': None, 'type': 'service', 'service': 'callrecord'},
            '10': {'blf': False, 'label': None, 'type': 'service', 'service': 'incallfilter'},
            '11': {'blf': False, 'label': None, 'type': 'service', 'service': 'enablednd'},
            '12': {'blf': False, 'label': None, 'type': 'service', 'service': 'pickup'},
            '13': {'blf': False, 'label': None, 'type': 'service', 'service': 'calllistening'},
            '14': {'blf': False, 'label': None, 'type': 'service', 'service': 'directoryaccess'},
            '15': {'blf': False, 'label': None, 'type': 'bsfilter', 'filter_member_id': filter_member_id},
            '16': {'blf': False, 'label': None, 'type': 'service', 'service': 'fwdundoall'},
            '17': {'blf': False, 'label': None, 'type': 'forward', 'service': 'noanswer'},
            '18': {'blf': False, 'label': None, 'type': 'forward', 'service': 'busy'},
            '19': {'blf': False, 'label': None, 'type': 'forward', 'service': 'unconditional'},
            '20': {'blf': False, 'label': None, 'type': 'service', 'service': 'enablevm'},
            '21': {'blf': False, 'label': None, 'type': 'service', 'service': 'vmusermsg'},
            '22': {'blf': False, 'label': None, 'type': 'service', 'service': 'vmuserpurge'},
            '23': {'blf': False, 'label': None, 'type': 'agent', 'action': 'toggle', 'agent_id': agent_id},
            '24': {'blf': False, 'label': None, 'type': 'agent', 'action': 'login', 'agent_id': agent_id},
            '25': {'blf': False, 'label': None, 'type': 'agent', 'action': 'logout', 'agent_id': agent_id},
            '26': {'blf': False, 'label': None, 'type': 'service', 'service': 'paging'},
            '27': {'blf': False, 'label': None, 'type': 'transfer', 'transfer': 'blind'},
            '28': {'blf': False, 'label': None, 'type': 'transfer', 'transfer': 'attended'},
            '29': {'blf': False, 'label': None, 'type': 'parking'},
            '30': {'blf': False, 'label': None, 'type': 'park_position', 'position': park_position},
            '31': {'blf': False, 'label': None, 'type': 'forward', 'service': 'busy', 'exten': fwd_exten},
        }

        self.webi_funckeys = {
            '1': {'type': 'User', 'destination': 'Jimmy John'},
            '2': {'type': 'Group', 'destination': 'Alcoholics Anonymous'},
            '3': {'type': 'Queue', 'destination': 'File Ariane'},
            '4': {'type': 'Conference room', 'destination': 'C-F Moisi'},
            '5': {'type': 'Customized', 'destination': custom_exten},
            '6': {'type': 'Online call recording'},
            '7': {'type': 'Phone status'},
            '8': {'type': 'Sound recording'},
            '9': {'type': 'Call recording'},
            '10': {'type': 'Incoming call filtering'},
            '11': {'type': 'Do not disturb'},
            '12': {'type': 'Group Interception'},
            '13': {'type': 'Listen to online calls'},
            '14': {'type': 'Directory access'},
            '15': {'type': 'Filtering Boss - Secretary'},
            '16': {'type': 'Disable all forwarding'},
            '17': {'type': 'Enable / Disable forwarding on no answer'},
            '18': {'type': 'Enable / Disable forwarding on busy'},
            '19': {'type': 'Enable / Disable forwarding unconditional'},
            '20': {'type': 'Enable voicemail'},
            '21': {'type': 'Reach the voicemail'},
            '22': {'type': 'Delete messages from voicemail'},
            '23': {'type': 'Connect/Disconnect an agent', 'destination': 'Mary Mall'},
            '24': {'type': 'Connect an agent', 'destination': 'Mary Mall'},
            '25': {'type': 'Disconnect an agent', 'destination': 'Mary Mall'},
            '26': {'type': 'Paging'},
            '27': {'type': 'Blind transfer'},
            '28': {'type': 'Indirect transfer'},
            '29': {'type': 'Parking'},
            '30': {'type': 'Parking position', 'destination': park_position},
            '31': {'type': 'Enable / Disable forwarding on busy', 'destination': fwd_exten},
        }

    def test_when_creating_user_with_func_key_then_creates_func_key_in_confd(self):
        users = self.browser.users.go()

        user = users.add()
        user.fill_form(firstname="John", lastname="Doe")
        funckeys = user.funckeys()

        webi_funckeys = sorted(self.webi_funckeys.items(), key=lambda x: int(x[0]))
        for _, funckey in webi_funckeys:
            funckeys.add(**funckey)

        user.save()

        for position, funckey in self.confd_destinations.items():
            path = r'/users/\d+/funckeys/{}'.format(position)
            expected = {'method': 'PUT',
                        'body': funckey}
            request = self.confd.request_matching(path)
            assert_that(request, has_entries(expected))
