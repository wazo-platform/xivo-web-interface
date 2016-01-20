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

from lib.pages import Page, ListPage


class LinePage(Page):

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()
        self.wait_for(By.NAME, 'fm-lines-list')

    def fill_form(self, form):
        for key, value in form.items():
            name = 'protocol[{}]'.format(key)
            self.fill(By.NAME, name, value)

    def signaling_tab(self):
        link = self.driver.find_element_by_css_selector("a[href='#signalling']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-signalling')
        return self

    def t38_tab(self):
        link = self.driver.find_element_by_css_selector("a[href='#t38']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-t38')
        return self

    def advanced_tab(self):
        link = self.driver.find_element_by_css_selector("a[href='#advanced']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-advanced')
        return self

    def options_tab(self):
        link = self.driver.find_element_by_css_selector("a[href='#sip-options']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-sip-options')
        return OptionTab(self.driver)


class OptionTab(Page):

    def add_options(self, options):
        for name, value in options:
            self.add_option(name, value)

    def add_option(self, name, value):
        nb_rows = self.count_option_rows()

        button = self.driver.find_element_by_id("sip-option-add")
        button.click()
        self.wait().until(lambda d: self.count_option_rows() > nb_rows)

        selector = "#sip-options tr:last-child .sip-option-name"
        self.fill(By.CSS_SELECTOR, selector, name)

        selector = "#sip-options tr:last-child .sip-option-value"
        self.fill(By.CSS_SELECTOR, selector, value)

    def count_option_rows(self):
        selector = "#sip-options tr"
        rows = self.driver.find_elements_by_css_selector(selector)
        return len(rows)

    def clear(self):
        for i in range(self.count_option_rows()):
            self.remove_option()

    def remove_option(self, position=0):
        count = self.count_option_rows()
        selector = "#sip-options tr .sip-option-remove"
        buttons = self.driver.find_elements_by_css_selector(selector)
        button = buttons[position]
        button.click()
        self.wait().until(lambda d: self.count_option_rows() < count)


class LineListPage(ListPage):

    url = "/service/ipbx/index.php/pbx_settings/lines/"
    list_selector = (By.NAME, "fm-lines-list")
    form_selector = (By.ID, "sr-lines")
    form_page = LinePage

    def add_sip(self):
        url = self.build_url(self.url, act='add', proto='sip')
        self.driver.get(url)
        self.wait_for_form()
        return self.form_page(self.driver)
