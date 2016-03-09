# -*- coding: utf-8 -*-

# Copyright (C) 2016 Avencall
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

from lib.testcase import TestWebi


class TestSwitchboardStats(TestWebi):

    asset = 'switchboard_stats'

    def test_given_stats_when_asking_stats_in_time_range_then_asks_confd_with_time_range(self):
        switchboard_id = 12
        start_date = '2015-03-02'
        end_date = '2015-03-07'
        self.confd.add_json_response('/switchboards', {
            'total': 1,
            'items': [{
                'id': switchboard_id,
                'display_name': 'Switchboard',
            }]
        })
        expected_url = '/switchboards/{}/stats'.format(switchboard_id)

        self.browser.switchboard_stats.search(start=start_date, end=end_date)

        self.confd.assert_request_sent(expected_url, query={
            'start_date': ['{}T00:00:00'.format(start_date)],
            'end_date': ['{}T23:59:59'.format(end_date)],
        })
