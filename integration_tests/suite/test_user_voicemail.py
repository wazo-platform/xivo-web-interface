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

from hamcrest import assert_that, has_entries

from lib.testcase import TestWebi


class TestUserVoicemail(TestWebi):

    asset = 'user_voicemail'


class TestUserChooseVoicemail(TestUserVoicemail):

    voicemail = {u'ask_password': True,
                 u'attach_audio': True,
                 u'context': u'default',
                 u'delete_messages': True,
                 u'email': 'my-email@example.com',
                 u'id': 1,
                 u'language': 'fr_FR',
                 u'max_messages': 5,
                 u'name': u'selected voicemail',
                 u'number': u'1001',
                 u'pager': None,
                 u'password': '12345',
                 u'timezone': u'eu-fr',
                 u'enabled': True,
                 u'options': [],
                 u'links': [{u'href': u'https://localhost:9486/1.1/voicemails/1',
                             u'rel': u'voicemails'}]}

    def test_given_voicemail_not_associated_when_selecting_for_association_then_voicemail_parameters_are_loaded(self):
        self.confd.add_json_response("/voicemails", {'total': 1,
                                                     'items': [self.voicemail]})
        self.confd.add_json_response("/voicemails/1", self.voicemail)
        user_page = self.browser.users.add()
        voicemail_tab = user_page.voicemail()

        voicemail_tab.select_voicemail(self.voicemail['number'])

        voicemail = voicemail_tab.get()
        assert_that(voicemail, has_entries({
            'enabled': True,
            'name': self.voicemail['name'],
            'number': self.voicemail['number'],
            'password': self.voicemail['password'],
            'email': self.voicemail['email'],
            'context': self.voicemail['context'],
            'time_zone': self.voicemail['timezone'],
            'language': self.voicemail['language'],
            'max_messages': self.voicemail['max_messages'],
            'ask_password': self.voicemail['ask_password'],
            'attach_audio': self.voicemail['attach_audio'],
            'delete_message': self.voicemail['delete_messages'],
        }))
