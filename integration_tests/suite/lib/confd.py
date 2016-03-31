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

from pprint import pformat
import requests
import json
import re

from hamcrest import assert_that, has_entries, equal_to


class MockConfd(object):

    def __init__(self, url):
        self.base_url = url

    def clear(self):
        self.clear_logs()
        self.clear_responses()

    def clear_logs(self):
        url = "{}/_logs".format(self.base_url)
        response = requests.delete(url)
        response.raise_for_status()

    def clear_responses(self):
        url = "{}/_responses".format(self.base_url)
        response = requests.delete(url)
        response.raise_for_status()

    def logs(self):
        url = "{}/_logs".format(self.base_url)
        response = requests.get(url)
        response.raise_for_status()
        return response.json()['logs']

    def requests(self):
        return [l['request'] for l in self.logs()]

    def remaining_responses(self):
        url = "{}/_responses".format(self.base_url)
        response = requests.get(url)
        response.raise_for_status()
        return response.json()['responses']

    def add_response(self, path, body='', method='GET', code=200, query=None, preserve=False):
        url = "{}/_responses".format(self.base_url)
        data = {'path': r"^{}$".format(path),
                'body': body,
                'method': method,
                'code': code,
                'query': query,
                'preserve': preserve}
        response = requests.post(url, data=json.dumps(data))
        response.raise_for_status()

    def add_json_response(self, path, body=None, method='GET', code=200, query=None, preserve=False):
        body = body or {}
        self.add_response(path, json.dumps(body), method, code, query, preserve)

    def assert_request_sent(self, url, method='GET', query=None, body=None):
        request = self.request_matching(url, method)
        if query:
            assert_that(request['query'], equal_to(query), pformat(self.logs()))
        if body:
            assert_that(request['body'], equal_to(body), pformat(self.logs()))

    def assert_json_request(self, path, expected_body, method='GET'):
        request = self.request_matching(path, method)
        body = json.loads(request['body'])
        assert_that(body, has_entries(expected_body), pformat(self.logs()))

    def request_matching(self, path, method='GET'):
        requests = self.requests()
        regex = re.compile(path)
        for request in requests:
            if regex.match(request['path']) and request['method'] == method:
                return request

        msg = "No requests found matching {} '{}'.\n{}"
        formatted_logs = pformat(self.logs())
        raise AssertionError(msg.format(method, path, formatted_logs))
