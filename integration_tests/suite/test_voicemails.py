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

import json

from hamcrest import assert_that, has_entries, has_items

from lib.testcase import TestWebi


class TestVoicemail(TestWebi):

    asset = 'voicemails'


class TestVoicemailCreate(TestVoicemail):

    def test_when_adding_voicemail_then_creates_voicemail_in_confd(self):
        expected_voicemail = {u'ask_password': True,
                              u'attach_audio': True,
                              u'delete_messages': True,
                              u'context': u'default',
                              u'email': u"test@example.com",
                              u'language': u"en_US",
                              u'max_messages': 10,
                              u'name': u"My Voicemail",
                              u'number': u'1000',
                              u'pager': u"test@example.com",
                              u'password': u"1234",
                              u'timezone': u'eu-fr',
                              u'enabled': True,
                              u'options': [
                                  ["attachfmt", "G729"],
                                  ["volgain", "2"],
                                  ["emailsubject", "Email subject"],
                                  ["emailbody", "Email body"],
                                  ["imapuser", "imapuser"],
                                  ["imappassword", "imappassword"],
                                  ["imapfolder", "imapfolder"],
                                  ["imapvmsharedid", "imapvmsharedid"],
                                  ["serveremail", "serveremail"],
                                  ["saycid", "yes"],
                                  ["review", "yes"],
                                  ["operator", "yes"],
                                  ["envelope", "yes"],
                                  ["sayduration", "yes"],
                                  ["saydurationm", "1"],
                                  ["sendvoicemail", "yes"],
                                  ["forcename", "yes"],
                                  ["forcegreetings", "yes"],
                                  ["hidefromdir", "yes"],
                                  ["dialout", "default"],
                                  ["callback", "default"],
                                  ["exitcontext", "default"],
                                  ["locale", "en_US"],
                                  ["tempgreetwarn", "yes"],
                                  ["messagewrap", "yes"],
                                  ["moveheard", "yes"],
                                  ["minsecs", "1"],
                                  ["maxsecs", "30"],
                                  ["nextaftercmd", "yes"],
                                  ["backupdeleted", "yes"],
                                  ["passwordlocation", "localdir"]
                              ]}

        self.confd.add_json_response("/voicemails", expected_voicemail, 'POST', 201)

        voicemail = self.browser.voicemails.add()
        voicemail.fill_form(fullname="My Voicemail",
                            mailbox="1000",
                            password="1234",
                            email="test@example.com",
                            language="en_US",
                            maxmsg="10",
                            ask_password=True,
                            deletevoicemail=True,
                            attach=True)

        email = voicemail.email()
        email.fill_form(emailsubject="Email subject",
                        emailbody="Email body",
                        pager="test@example.com")

        advanced = voicemail.advanced()
        advanced.add_options(saycid="yes",
                             review="yes",
                             operator="yes",
                             envelope="yes",
                             sayduration="yes",
                             saydurationm="1",
                             sendvoicemail="yes",
                             forcename="yes",
                             forcegreetings="yes",
                             hidefromdir="yes",
                             dialout="default",
                             callback="default",
                             exitcontext="default",
                             locale="en_US",
                             tempgreetwarn="yes",
                             messagewrap="yes",
                             moveheard="yes",
                             minsecs="1",
                             maxsecs="30",
                             nextaftercmd="yes",
                             backupdeleted="yes",
                             passwordlocation="localdir",
                             attachfmt="G729",
                             volgain="2",
                             imapuser="imapuser",
                             imappassword="imappassword",
                             imapfolder="imapfolder",
                             imapvmsharedid="imapvmsharedid",
                             serveremail="serveremail")

        voicemail.save()

        request = self.confd.request_matching('/voicemails', 'POST')
        voicemail = json.loads(request['body'])
        options = expected_voicemail.pop('options')
        del expected_voicemail['enabled']

        assert_that(voicemail, has_entries(expected_voicemail))
        assert_that(voicemail['options'], has_items(*options))


class TestVoicemailEdit(TestVoicemail):
    def test_given_voicemail_when_editing_then_updates_via_confd(self):
        confd_voicemail = {u'ask_password': True,
                           u'attach_audio': False,
                           u'context': u'default',
                           u'delete_messages': False,
                           u'email': None,
                           u'id': 1,
                           u'language': None,
                           u'max_messages': None,
                           u'name': u'Edited Voicemail',
                           u'number': u'1001',
                           u'pager': None,
                           u'password': None,
                           u'timezone': u'eu-fr',
                           u'enabled': True,
                           u'options': [],
                           u'links': [{u'href': u'https://dev:9486/1.1/voicemails/38',
                                       u'rel': u'voicemails'}],
                           }

        self.confd.add_json_response("/voicemails", {'total': 1, 'items': [confd_voicemail]})
        self.confd.add_json_response("/voicemails/1", confd_voicemail)
        self.confd.add_response("/voicemails/1", method="PUT", code=204)

        voicemail = self.browser.voicemails.edit("Edited Voicemail")
        voicemail.fill_form(email="test@example.com")

        email = voicemail.email()
        email.fill_form(emailbody="Hello world\nThis is an email\nGoodbye|")

        advanced = voicemail.advanced()
        advanced.add_option("saycid", "yes")

        voicemail.save()

        expected_voicemail = has_entries({u'ask_password': True,
                                          u'attach_audio': False,
                                          u'context': u'default',
                                          u'delete_messages': False,
                                          u'email': u"test@example.com",
                                          u'language': None,
                                          u'max_messages': None,
                                          u'name': u'Edited Voicemail',
                                          u'number': u'1001',
                                          u'pager': None,
                                          u'password': None,
                                          u'timezone': u'eu-fr',
                                          u'options': has_items(
                                              [u"emailbody", u"Hello world\r\nThis is an email\r\nGoodbye|"],
                                              [u"saycid", u"yes"])
                                          })

        request = self.confd.request_matching('/voicemails/1', 'PUT')
        assert_that(json.loads(request['body']), expected_voicemail)


class TestVoicemailDelete(TestVoicemail):

    def test_given_voicemail_when_deleting_then_deletes_via_confd(self):
        confd_response = {'total': 1,
                          'items': [
                              {u'ask_password': True,
                               u'attach_audio': False,
                               u'context': u'default',
                               u'delete_messages': False,
                               u'email': None,
                               u'id': 1,
                               u'language': None,
                               u'max_messages': None,
                               u'name': u'Deleted Voicemail',
                               u'number': u'1001',
                               u'pager': None,
                               u'password': None,
                               u'timezone': u'eu-fr',
                               u'enabled': True,
                               u'options': [],
                               u'links': [{u'href': u'https://dev:9486/1.1/voicemails/38',
                                           u'rel': u'voicemails'}],
                               }]
                          }

        self.confd.add_json_response("/voicemails", confd_response)
        voicemails = self.browser.voicemails

        self.confd.add_response("/voicemails/1", method="DELETE", code=204)
        self.confd.add_json_response("/voicemails", {'total': 0, 'items': []})
        voicemails.delete("Deleted Voicemail")

        self.confd.assert_request_sent("/voicemails/1", method="DELETE")
