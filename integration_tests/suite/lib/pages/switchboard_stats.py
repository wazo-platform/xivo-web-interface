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

from lib.pages import Page
from selenium.webdriver.common.by import By


class SwitchboardStatsPage(Page):

    def search(self, start, end):
        self.fill(By.ID, 'it-dbeg', start)
        self.fill(By.ID, 'it-dend', end)
        self.save()

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()
        self.wait_for(By.ID, 'it-submit')

    def go(self):
        url = "/statistics/index.php/switchboard/data"
        url = self.build_url(url)
        self.driver.get(url)
        self.wait_for(By.ID, 'it-switchboard')

        return self
