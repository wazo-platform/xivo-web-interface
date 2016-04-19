# -*- coding: UTF-8 -*-

# Copyright (C) 2016 Avencall
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

from __future__ import unicode_literals

from hamcrest import assert_that, equal_to

from lib.confd import urljoin
from lib.testcase import TestWebi


class TestLine(TestWebi):

    asset = 'lines'

    def setUp(self):
        super(TestLine, self).setUp()
        self.line_id = 1
        self.endpoint_id = 2
        self.device_id = 'abcdef'
        self.user_id = 3
        self.extension_id = 4
        self.context = 'default'

    def prepare_add_response(self):
        line = {'id': self.line_id}
        endpoint = {'id': self.endpoint_id}
        self.confd.add_json_response('/lines', line, method='POST', code=201)
        self.confd.add_json_response(urljoin('endpoints', self.protocol), endpoint, method='POST', code=201)
        self.confd.add_response(urljoin('lines',self.line_id, 'endpoints', self.protocol, self.endpoint_id),
                                method='PUT', code=204)

    def assert_line_added(self, endpoint):
        line = {'context': self.context}
        self.confd.assert_json_request('/lines', line, method='POST')
        self.confd.assert_json_request(urljoin('endpoints', self.protocol), endpoint, method='POST')
        self.confd.assert_request_sent(urljoin('lines', self.line_id, 'endpoints', self.protocol, self.endpoint_id), method='PUT')

    def prepare_edit_response(self, endpoint, line_extra=None):
        line = {'id': self.line_id, 'protocol': self.protocol, 'context': self.context}
        if line_extra:
            line.update(line_extra)
        association = {'line_id': self.line_id, 'endpoint_id': self.endpoint_id}
        self.confd.add_json_response(urljoin('lines', self.line_id), line, method='GET')
        self.confd.add_json_response(urljoin('lines', self.line_id), line, method='GET')
        self.confd.add_json_response(urljoin('lines', self.line_id, 'endpoints', self.protocol), association, method='GET')
        self.confd.add_json_response(urljoin('lines', self.line_id, 'endpoints', self.protocol), association, method='GET')
        self.confd.add_json_response(urljoin('endpoints', self.protocol, self.endpoint_id), endpoint, method='GET')
        self.confd.add_json_response(urljoin('endpoints', self.protocol, self.endpoint_id), endpoint, method='GET')
        self.confd.add_response(urljoin('endpoints', self.protocol, self.endpoint_id), method='PUT', code=204)

    def assert_line_edited(self, endpoint):
        self.confd.assert_json_request(urljoin('endpoints', self.protocol, self.endpoint_id), endpoint, method='PUT')

    def prepare_delete_response(self):
        line = {'id': self.line_id, 'protocol': self.protocol}
        endpoint = {'id': self.endpoint_id}
        self.confd.add_json_response(urljoin('lines', self.line_id), line, method='GET')
        endpoint_association = {'line_id': self.line_id, 'endpoint_id': self.endpoint_id}
        self.confd.add_json_response(urljoin('lines', self.line_id, 'endpoints', self.protocol), endpoint_association, method='GET')
        self.confd.add_json_response(urljoin('endpoints', self.protocol, self.endpoint_id), endpoint, method='GET')
        self.confd.add_response(urljoin('lines', self.line_id), method='DELETE', code=204)
        # add device
        device_association = {'device_id': self.device_id}
        self.confd.add_json_response(urljoin('lines', self.line_id, 'devices'), device_association, method='GET')
        self.confd.add_response(urljoin('lines', self.line_id, 'devices', self.device_id), method='DELETE', code=204)
        # add 1 user
        user_association = {'items': [{'user_id': self.user_id}]}
        self.confd.add_json_response(urljoin('lines', self.line_id, 'users'), user_association, method='GET')
        self.confd.add_response(urljoin('users', self.user_id, 'lines', self.line_id), method='DELETE', code=204)
        # add 1 extension
        extension_association = {'items': [{'extension_id': self.extension_id}]}
        self.confd.add_json_response(urljoin('lines', self.line_id, 'extensions'), extension_association, method='GET')
        self.confd.add_response(urljoin('lines', self.line_id, 'extensions', self.extension_id), method='DELETE', code=204)
        extension = {'context': self.context}
        self.confd.add_json_response(urljoin('extensions', self.extension_id), extension, method='GET')
        self.confd.add_response(urljoin('extensions', self.extension_id), method='DELETE', code=204)

    def assert_line_deleted(self):
        self.confd.assert_request_sent(urljoin('lines', self.line_id, 'devices', self.device_id), method='DELETE')
        self.confd.assert_request_sent(urljoin('users', self.user_id, 'lines', self.line_id), method='DELETE')
        self.confd.assert_request_sent(urljoin('lines', self.line_id, 'extensions', self.extension_id), method='DELETE')
        self.confd.assert_request_sent(urljoin('extensions', self.extension_id), method='DELETE')
        self.confd.assert_request_sent(urljoin('lines', self.line_id), method='DELETE')


class TestCustomLine(TestLine):

    protocol = 'custom'

    def test_create_line(self):
        self.prepare_add_response()

        line_page = self.browser.lines.add_custom()
        line_page.set_interface('local/foo')
        line_page.save()

        expected_endpoint = {
            'interface': 'local/foo'
        }
        self.assert_line_added(expected_endpoint)

    def test_view_line(self):
        endpoint = {
            'id': self.endpoint_id,
            'interface': 'local/foo',
        }
        self.prepare_edit_response(endpoint)

        line_page = self.browser.lines.edit_by_id(self.line_id).custom()

        assert_that(line_page.interface(), equal_to('local/foo'))
        assert_that(line_page.context(), equal_to(self.context))

    def test_edit_line(self):
        endpoint = {
            'id': self.endpoint_id,
            'interface': 'local/foo',
        }
        self.prepare_edit_response(endpoint)

        line_page = self.browser.lines.edit_by_id(self.line_id).custom()
        line_page.set_interface('local/bar')
        line_page.save()

        expected_endpoint = {
            'interface': 'local/bar',
        }
        self.assert_line_edited(expected_endpoint)

    def test_delete_line(self):
        self.prepare_delete_response()

        self.browser.lines.delete_by_id(self.line_id)

        self.assert_line_deleted()


class TestSCCPLine(TestLine):

    protocol = 'sccp'

    def prepare_edit_sccp_response(self, endpoint):
        self.line_name = '555'
        self.prepare_edit_response(endpoint, {'name': self.line_name})

    def test_view_line(self):
        endpoint = {
            'id': self.endpoint_id,
            'options': [],
        }
        self.prepare_edit_sccp_response(endpoint)

        line_page = self.browser.lines.edit_by_id(self.line_id).sccp()

        assert_that(line_page.username(), equal_to(self.line_name))
        assert_that(line_page.context(), equal_to(self.context))

    def test_edit_line(self):
        options = [
            ['cid_name', 'John'],
            ['cid_num', '555'],
        ]
        endpoint = {
            'id': self.endpoint_id,
            'options': options,
        }
        self.prepare_edit_sccp_response(endpoint)

        line_page = self.browser.lines.edit_by_id(self.line_id).sccp()
        line_page.save()

        expected_endpoint = {
            'options': options,
        }
        self.assert_line_edited(expected_endpoint)

    def test_delete_line(self):
        self.prepare_delete_response()

        self.browser.lines.delete_by_id(self.line_id)

        self.assert_line_deleted()


class TestSIPLine(TestLine):

    protocol = 'sip'

    def test_create_line(self):
        self.prepare_add_response()

        line_page = self.browser.lines.add_sip()
        general_tab = line_page.general_tab()
        username = general_tab.username()
        line_page.save()

        expected_endpoint = {
            'username': username,
            'options': [],
        }
        self.assert_line_added(expected_endpoint)

    def test_create_line_with_all_fields_filled(self):
        self.prepare_add_response()

        line_page = self.browser.lines.add_sip()
        general_tab = line_page.general_tab()
        general_tab.set_language('fr_FR')
        general_tab.set_callerid('"John" <555>')
        general_tab.set_nat('No')
        general_tab.set_dtmf('RFC 2833')
        general_tab.set_monitoring('Yes')
        advanced_tab = line_page.advanced_tab()
        advanced_tab.add_options([
            ['foo', 'bar'],
            ['host', '169.254.1.1'],
            ['type', 'peer'],
        ])
        line_page.save()

        expected_endpoint = {
            'host': '169.254.1.1',
            'type': 'peer',
            'options': [
                ['language', 'fr_FR'],
                ['callerid', '"John" <555>'],
                ['nat', 'no'],
                ['dtmfmode', 'rfc2833'],
                ['qualify', 'yes'],
                ['foo', 'bar'],
            ],
        }
        self.assert_line_added(expected_endpoint)

    def test_view_line(self):
        endpoint = {
            'id': self.endpoint_id,
            'username': 'foo',
            'secret': 'foosecret',
            'host': 'dynamic',
            'type': 'friend',
            'options': [
                ['call-limit', '10'],  # call-limit is a hidden option
                ['language', 'fr_FR'],
                ['callerid', 'John'],
                ['nat', 'no'],
                ['dtmfmode', 'rfc2833'],
                ['qualify', 'yes'],
                ['opt1', 'val1'],
            ],
        }
        self.prepare_edit_response(endpoint)

        line_page = self.browser.lines.edit_by_id(self.line_id).sip()

        general_tab = line_page.general_tab()
        assert_that(general_tab.username(), equal_to('foo'))
        assert_that(general_tab.secret(), equal_to('foosecret'))
        assert_that(general_tab.context(), equal_to(self.context))
        assert_that(general_tab.language(), equal_to('fr_FR'))
        assert_that(general_tab.callerid(), equal_to('John'))
        assert_that(general_tab.nat(), equal_to('no'))
        assert_that(general_tab.dtmf(), equal_to('rfc2833'))
        assert_that(general_tab.monitoring(), equal_to('yes'))

        advanced_tab = line_page.advanced_tab()
        expected_options = [
            ('host', 'dynamic'),
            ('type', 'friend'),
            ('opt1', 'val1'),
        ]
        assert_that(advanced_tab.options(), equal_to(expected_options))

    def test_edit_line(self):
        endpoint = {
            'id': self.endpoint_id,
            'username': 'foo',
            'secret': 'foosecret',
            'host': 'dynamic',
            'type': 'friend',
            'options': [
                ['call-limit', '10'],  # call-limit is a hidden option
                ['opt1', 'val1'],
            ],
        }
        self.prepare_edit_response(endpoint)

        line_page = self.browser.lines.edit_by_id(self.line_id).sip()
        advanced_tab = line_page.advanced_tab()
        advanced_tab.clear_options()
        advanced_tab.add_option('opt2', 'val2')
        line_page.save()

        expected_endpoint = {
            'options': [
                ['call-limit', '10'],
                ['opt2', 'val2'],
            ],
        }
        self.assert_line_edited(expected_endpoint)

    def test_delete_line(self):
        self.prepare_delete_response()

        self.browser.lines.delete_by_id(self.line_id)

        self.assert_line_deleted()
