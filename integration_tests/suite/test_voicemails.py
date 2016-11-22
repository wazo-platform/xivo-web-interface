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

import json

from hamcrest import assert_that, has_entries, has_items

from lib.testcase import TestWebi


class TestVoicemail(TestWebi):

    asset = 'webi_base'


class TestVoicemailCreate(TestVoicemail):

    voicemail = {u'ask_password': True,
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

    expected_voicemail = has_entries({u'ask_password': True,
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
                                      u'options': has_items(
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
                                          ["passwordlocation", "localdir"])
                                      })

    def test_when_adding_voicemail_with_0_max_messages_then_creates_voicemail_in_confd(self):
        self.confd.add_json_response("/voicemails", self.voicemail, method='POST', code=201)

        voicemail_page = self.browser.voicemails.add()
        voicemail_page.fill_form(fullname="0 max messages",
                                 mailbox="1100",
                                 maxmsg="0")
        voicemail_page.save()

        self.confd.assert_json_request('/voicemails', {'max_messages': 0}, method='POST')

    def test_when_adding_voicemail_then_creates_voicemail_in_confd(self):
        self.confd.add_json_response("/voicemails", self.voicemail, method='POST', code=201)

        voicemail_page = self.browser.voicemails.add()
        voicemail_page.fill_form(fullname="My Voicemail",
                                 mailbox="1000",
                                 password="1234",
                                 email="test@example.com",
                                 language="en_US",
                                 maxmsg="10",
                                 tz="eu-fr",
                                 ask_password=True,
                                 deletevoicemail=True,
                                 attach_audio="Yes")

        email_tab = voicemail_page.email()
        email_tab.fill_form(emailsubject="Email subject",
                            emailbody="Email body",
                            pager="test@example.com")

        advanced_tab = voicemail_page.advanced()
        advanced_tab.add_options(saycid="yes",
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

        voicemail_page.save()

        request = self.confd.request_matching('/voicemails', 'POST')
        voicemail = json.loads(request['body'])
        assert_that(voicemail, self.expected_voicemail)


class TestVoicemailEdit(TestVoicemail):

    voicemail = {u'ask_password': True,
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

    def test_given_voicemail_when_editing_then_updates_via_confd(self):
        self.confd.add_json_response("/voicemails", {'total': 1,
                                                     'items': [self.voicemail]})
        self.confd.add_json_response("/voicemails/1", self.voicemail)
        self.confd.add_json_response("/voicemails/1", self.voicemail)
        self.confd.add_response("/voicemails/1", method="PUT", code=204)

        voicemail_page = self.browser.voicemails.edit("Edited Voicemail")
        voicemail_page.fill_form(email="test@example.com")

        email_tab = voicemail_page.email()
        email_tab.fill_form(emailbody="Hello world\nThis is an email\nGoodbye|")

        advanced_tab = voicemail_page.advanced()
        advanced_tab.add_option("saycid", "yes")

        voicemail_page.save()

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

        request = self.confd.request_matching('/voicemails/1', method='PUT')
        assert_that(json.loads(request['body']), expected_voicemail)


class TestVoicemailDelete(TestVoicemail):

    voicemail = {u'ask_password': True,
                 u'attach_audio': False,
                 u'context': u'default',
                 u'delete_messages': False,
                 u'email': None,
                 u'id': 1,
                 u'language': None,
                 u'max_messages': None,
                 u'name': u'deleted voicemail',
                 u'number': u'1001',
                 u'pager': None,
                 u'password': None,
                 u'timezone': u'eu-fr',
                 u'enabled': True,
                 u'options': [],
                 u'links': [{u'href': u'https://dev:9486/1.1/voicemails/38',
                             u'rel': u'voicemails'}]}

    def test_given_voicemail_when_deleting_then_deletes_via_confd(self):
        self.confd.add_json_response("/voicemails", {'total': 1,
                                                     'items': [self.voicemail]})
        self.confd.add_json_response("/voicemails/1/users", {'total': 0,
                                                             'items': []})
        self.confd.add_response("/voicemails/1", method="DELETE", code=204)
        self.confd.add_json_response("/voicemails", {'total': 0,
                                                     'items': []})

        voicemail_page = self.browser.voicemails
        voicemail_page.delete(self.voicemail['name'])

        self.confd.assert_request_sent("/voicemails/1", method="DELETE")

    def test_given_voicemail_associated_to_user_when_deleting_then_dissociates_voicemail(self):
        association = {u'enabled': True,
                       u'user_id': 10,
                       u'voicemail_id': 1,
                       u'links': [{u'href': u'https://confd:9486/1.1/voicemails/1',
                                   u'rel': u'voicemails'},
                                  {u'href': u'https://confd:9486/1.1/users/10',
                                   u'rel': u'users'}]
                       }

        self.confd.add_json_response("/voicemails", {'total': 1,
                                                     'items': [self.voicemail]})
        self.confd.add_json_response("/voicemails/1/users", {'total': 1,
                                                             'items': [association]})
        self.confd.add_response("/users/10/voicemails", method="DELETE", code=204)
        self.confd.add_response("/voicemails/1", method="DELETE", code=204)

        voicemail_page = self.browser.voicemails
        voicemail_page.delete(self.voicemail['name'])

        self.confd.assert_request_sent("/voicemails/1", method="DELETE")
        self.confd.assert_request_sent("/users/10/voicemails", method="DELETE")
