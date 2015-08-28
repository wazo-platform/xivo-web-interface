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


from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as ec

from lib.pages import Page, ListPage


class VoicemailPage(Page):

    def fill_form(self, **kwargs):
        for key, value in kwargs.items():
            id_ = "it-voicemail-{}".format(key)
            self.fill(By.ID, id_, value)

    def add_options(self, **kwargs):
        for name, value in kwargs.iteritems():
            self.add_option(name, value)

    def add_option(self, name, value):
        nb_rows = self.count_option_rows()

        button = self.driver.find_element_by_id("vm-option-add")
        button.click()
        self.wait().until(lambda d: self.count_option_rows() > nb_rows)

        selector = "#voicemail-options tr:last-child .vm-option-name"
        self.fill(By.CSS_SELECTOR, selector, name)

        selector = "#voicemail-options tr:last-child .vm-option-value"
        self.fill(By.CSS_SELECTOR, selector, value)

    def count_option_rows(self):
        selector = "#voicemail-options tr"
        rows = self.driver.find_elements_by_css_selector(selector)
        return len(rows)

    def email(self):
        selector = "a[href='#email']"
        tab = self.driver.find_element_by_css_selector(selector)
        tab.click()

        self.wait_visible(By.ID, 'sb-part-email')
        return self

    def advanced(self):
        selector = "a[href='#last']"
        tab = self.driver.find_element_by_css_selector(selector)
        tab.click()

        self.wait_visible(By.ID, 'sb-part-last')
        return self

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()

        condition = ec.presence_of_element_located(VoicemailListPage.list_selector)
        self.wait().until(condition)

        return VoicemailListPage(self.driver)


class VoicemailListPage(ListPage):

    url = "/service/ipbx/index.php/pbx_settings/voicemail/"
    list_selector = (By.NAME, "fm-voicemail-list")
    form_selector = (By.ID, "fd-voicemail-fullname")
    form_page = VoicemailPage
