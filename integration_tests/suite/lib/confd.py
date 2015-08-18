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

from pprint import pformat
import requests
import json
import re

from hamcrest import assert_that, has_item, has_entries


class MockConfd(object):

    def __init__(self, url):
        self.base_url = url

    def clear(self):
        self.clear_requests()
        self.clear_responses()

    def clear_requests(self):
        url = "{}/_requests".format(self.base_url)
        response = requests.delete(url)
        response.raise_for_status()

    def clear_responses(self):
        url = "{}/_responses".format(self.base_url)
        response = requests.delete(url)
        response.raise_for_status()

    def requests(self):
        url = "{}/_requests".format(self.base_url)
        response = requests.get(url)
        response.raise_for_status()
        return response.json()['requests']

    def add_response(self, path, body='', method='GET', code=200):
        url = "{}/_responses".format(self.base_url)
        data = {'path': path,
                'body': body,
                'method': method,
                'code': code}
        response = requests.post(url, data=json.dumps(data))
        response.raise_for_status()

    def add_json_response(self, path, body=None, method='GET', code=200):
        body = body or {}
        self.add_response(path, json.dumps(body), method, code)

    def assert_request_sent(self, request):
        requests = self.requests()
        msg = pformat(requests)
        assert_that(requests, has_item(has_entries(request)), msg)

    def assert_json_request(self, expected_url, expected_method, expected_body):
        request = self.request_matching(expected_url)
        assert_that(request['method'], equal_to(expected_method))
        body = json.loads(request['body'])
        msg = pformat(request)
        assert_that(body, has_entries(expected_body), msg)

    def request_matching(self, path):
        regex = re.compile(path)
        for request in self.requests():
            if regex.match(request['path']):
                return request
        raise AssertionError("No request matching '{}' found".format(path))
