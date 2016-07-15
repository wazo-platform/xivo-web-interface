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

from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as ec
from selenium.common.exceptions import WebDriverException

from lib.pages import Page


class CallLogPage(Page):

    def save(self):
        try:
            self.driver.find_element_by_id("it-submit").click()
        except WebDriverException:
            # calendar overlaps the button
            self.hide_calendar()
            self.driver.find_element_by_id("it-submit").click()

    def hide_calendar(self):
        # click somewhere else
        non_interactive = self.driver.find_element_by_id("logo")
        non_interactive.click()

        self.wait_visible(By.ID, 'it-submit')
        self.wait().until(ec.element_to_be_clickable((By.ID, 'it-submit')))

    def fill_form(self, start_date=None, end_date=None):
        if start_date:
            self.fill_id('it-dbeg', start_date)
        if end_date:
            self.fill_id('it-dend', end_date)

    def go(self):
        url = '/service/ipbx/index.php/call_management/cel'
        url = self.build_url(url)
        self.driver.get(url)
        self.wait_for(By.ID, 'sr-cel')

        return self
