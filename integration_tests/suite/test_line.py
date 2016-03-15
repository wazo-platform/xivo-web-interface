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

import json

from hamcrest import assert_that, contains

from lib.testcase import TestWebi


class TestLine(TestWebi):

    asset = 'lines'

    BASE_OPTIONS = [['protocol', 'sip'],
                    ['call-limit', '10'],
                    ['subscribemwi', 'no'],
                    ['amaflags', 'default'],
                    ['regseconds', '0']]

    def build_sip(self, **kwargs):
        options = kwargs.pop('options', [])
        sip = {'host': 'dynamic',
               'id': 20,
               'links': [{'href': 'https://localhost:9487/1.1/endpoints/sip/86',
                          'rel': 'endpoint_sip'}],
               'secret': 'KZZ8HI',
               'type': 'friend',
               'username': 'dxreky'}

        sip.update(kwargs)

        sip['options'] = self.BASE_OPTIONS + options

        return sip


class TestLineCreate(TestLine):

    def test_when_creating_a_line_with_options_then_options_sent_to_confd(self):
        self.confd.add_json_response(r"/endpoints/sip/\d+", self.build_sip(), method="GET")
        self.confd.add_response(r"/endpoints/sip/\d+", method="PUT", code=204)

        options = [["foo", "bar"],
                   ["foo", "baz"],
                   ["spam", "eggs"]]

        line_page = self.browser.lines.add_sip()
        options_tab = line_page.options_tab()
        options_tab.add_options(options)
        line_page.save()

        request = self.confd.request_matching(r"/endpoints/sip/\d+", method="PUT")
        sip = json.loads(request['body'])

        expected = contains(*(self.BASE_OPTIONS + options))
        assert_that(sip['options'], expected)


class TestLineEdit(TestLine):

    def prepare_sip_response(self, **kwargs):
        sip = self.build_sip(**kwargs)
        self.confd.add_json_response(r"/endpoints/sip/\d+", sip, method="GET")
        self.confd.add_json_response(r"/endpoints/sip/\d+", sip, method="GET")
        self.confd.add_json_response(r"/endpoints/sip/\d+", sip, method="GET")
        self.confd.add_response(r"/endpoints/sip/\d+", method="PUT", code=204)

    def test_when_adding_options_to_a_line_then_options_sent_to_confd(self):
        with self.db.queries() as queries:
            queries.insert_sip_line({"username": "addoptions"})

        options = [["foo", "bar"],
                   ["foo", "baz"],
                   ["spam", "eggs"]]

        self.prepare_sip_response()

        line_page = self.browser.lines.edit("addoptions")
        options_tab = line_page.options_tab()
        options_tab.add_options(options)
        line_page.save()

        request = self.confd.request_matching(r"/endpoints/sip/\d+", method="PUT")
        sip = json.loads(request['body'])

        expected = contains(*(self.BASE_OPTIONS + options))
        assert_that(sip['options'], expected)

    def test_when_changing_options_on_a_line_then_options_sent_to_confd(self):
        with self.db.queries() as queries:
            queries.insert_sip_line({"username": "changeoptions"})

        old_options = [["foo", "bar"],
                       ["foo", "baz"],
                       ["spam", "eggs"]]

        new_options = [["newfoo", "newbar"],
                       ["newfoo", "newbaz"],
                       ["spam", "neweggs"]]

        self.prepare_sip_response(options=old_options)

        line_page = self.browser.lines.edit("changeoptions")
        options_tab = line_page.options_tab()
        options_tab.clear()
        options_tab.add_options(new_options)
        line_page.save()

        request = self.confd.request_matching(r"/endpoints/sip/\d+", method="PUT")
        sip = json.loads(request['body'])

        expected = contains(*(self.BASE_OPTIONS + new_options))
        assert_that(sip['options'], expected)

    def test_when_removing_options_on_a_line_then_options_removed_from_confd(self):
        with self.db.queries() as queries:
            queries.insert_sip_line({"username": "rmoptions"})

        options = [["foo", "bar"],
                   ["foo", "baz"],
                   ["spam", "eggs"]]

        self.prepare_sip_response(options=options)

        line_page = self.browser.lines.edit("rmoptions")
        options_tab = line_page.options_tab()
        options_tab.clear()
        line_page.save()

        request = self.confd.request_matching(r"/endpoints/sip/\d+", method="PUT")
        sip = json.loads(request['body'])

        expected = contains(*self.BASE_OPTIONS)
        assert_that(sip['options'], expected)
