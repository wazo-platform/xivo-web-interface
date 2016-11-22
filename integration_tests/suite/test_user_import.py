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

import os.path

from hamcrest import assert_that, contains, contains_string


from lib.testcase import TestWebi

SUITE_PATH = os.path.join(
    os.path.dirname(os.path.realpath(__file__)),
    "files")


class TestUserImport(TestWebi):

    asset = 'webi_base'

    def upload_csv(self, page):
        filepath = os.path.join(SUITE_PATH, "import_users.csv")

        page.upload_file(filepath)
        page.save()

        with open(filepath) as f:
            return f.read().decode('utf8')


class TestUserImportCreate(TestUserImport):

    def test_given_csv_file_when_importing_then_file_contents_sent_to_confd(self):
        response = {'created': []}
        self.confd.add_json_response("/users/import", response, method='POST', code=201)

        page = self.browser.users.import_csv()
        csvdata = self.upload_csv(page)

        self.confd.assert_request_sent("/users/import", method='POST', body=csvdata)

    def test_given_csv_file_contains_error_when_importing_then_error_message_displayed(self):
        error_message = 'Resource Error - Voicemail already exists'
        response = {'errors': [{'message': error_message,
                                'timestamp': 12345678,
                                'details': {
                                    'row_number': 1,
                                    'row': {}
                                }}]}

        self.confd.add_json_response("/users/import", response, method='POST', code=400)

        user_page = self.browser.users
        import_page = user_page.import_csv()
        csvdata = self.upload_csv(import_page)

        errors = user_page.extract_errors()
        assert_that(errors, contains(contains_string(error_message)))

        self.confd.assert_request_sent("/users/import", method='POST', body=csvdata)


class TestUserImportUpdate(TestUserImport):

    def test_given_csv_file_when_updating_then_file_contents_sent_to_confd(self):
        response = {'updated': []}
        self.confd.add_json_response("/users/import", response, method='PUT', code=200)

        page = self.browser.users.update_csv()
        csvdata = self.upload_csv(page)

        self.confd.assert_request_sent("/users/import", method='PUT', body=csvdata)

    def test_given_csv_file_contains_error_when_updating_then_error_message_displayed(self):
        error_message = 'Resource Error - Voicemail already exists'
        response = {'errors': [{'message': error_message,
                                'timestamp': 12345678,
                                'details': {
                                    'row_number': 1,
                                    'row': {}
                                }}]}

        self.confd.add_json_response("/users/import", response, method='PUT', code=400)

        user_page = self.browser.users
        import_page = user_page.update_csv()
        csvdata = self.upload_csv(import_page)

        errors = user_page.extract_errors()
        assert_that(errors, contains(contains_string(error_message)))

        self.confd.assert_request_sent("/users/import", method='PUT', body=csvdata)
