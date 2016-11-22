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

import re

from lib.confd import urljoin
from lib.testcase import TestWebi


class SomeDate(object):

    def __eq__(self, other):
        return re.match('\d\d\d\d-\d\d-\d\dT\d\d:\d\d:\d\d', other) is not None


class TestCallLogs(TestWebi):

    asset = 'webi_base'

    def test_list_call_logs_with_no_end_date(self):
        call_log_page = self.browser.call_logs.go()
        call_log_page.fill_form(start_date='2013-01-01')
        call_log_page.save()

        expected_query = {'start_date': ['2013-01-01T00:00:00'],
                          'end_date': [SomeDate()]}
        self.confd.assert_request_sent(urljoin('call_logs'), query=expected_query, method='GET')
