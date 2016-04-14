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

    def custom(self):
        return CustomLinePage(self.driver)

    def sccp(self):
        return SCCPLinePage(self.driver)

    def sip(self):
        return SIPLinePage(self.driver)


class CustomLinePage(LinePage):

    _ID_INTERFACE = 'it-protocol-interface'

    def context(self):
        return self.get_value('it-protocol-context')

    def interface(self):
        return self.get_value(self._ID_INTERFACE)

    def set_interface(self, value):
        self.fill_id(self._ID_INTERFACE, value)


class SCCPLinePage(LinePage):

    _ID_USERNAME = 'it-protocol-name'

    def context(self):
        return self.get_value('it-protocol-context')

    def username(self):
        return self.get_value(self._ID_USERNAME)

    def set_username(self, value):
        self.fill_id(self._ID_USERNAME, value)


class SIPLinePage(LinePage):

    def general_tab(self):
        link = self.driver.find_element_by_css_selector("a[href='#first']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-first')
        return SIPGeneralTab(self.driver)

    def advanced_tab(self):
        link = self.driver.find_element_by_css_selector("a[href='#advanced']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-advanced')
        return SIPAdvancedTab(self.driver)


class SIPGeneralTab(Page):

    _ID_LANGUAGE = 'it-protocol-language'
    _ID_CALLERID = 'it-protocol-callerid'
    _ID_NAT = 'it-sip-protocol-nat'
    _ID_DTMF = 'it-sip-protocol-dtmfmode'
    _ID_MONITORING = 'it-sip-protocol-qualify'

    def username(self):
        return self.get_value('it-protocol-name')

    def secret(self):
        return self.get_value('it-protocol-secret')

    def context(self):
        return self.get_value('it-protocol-context')

    def language(self):
        return self.get_selected_option_value(self._ID_LANGUAGE)

    def set_language(self, value):
        self.select_id(self._ID_LANGUAGE, value)

    def callerid(self):
        return self.get_value(self._ID_CALLERID)

    def set_callerid(self, value):
        self.fill_id(self._ID_CALLERID, value)

    def nat(self):
        return self.get_selected_option_value(self._ID_NAT)

    def set_nat(self, value):
        self.select_id(self._ID_NAT, value)

    def dtmf(self):
        return self.get_selected_option_value(self._ID_DTMF)

    def set_dtmf(self, value):
        self.select_id(self._ID_DTMF, value)

    def monitoring(self):
        return self.get_selected_option_value(self._ID_MONITORING)

    def set_monitoring(self, value):
        self.select_id(self._ID_MONITORING, value)


class SIPAdvancedTab(Page):

    def options(self):
        option_names = [e.get_attribute('value') for e in self.driver.find_elements_by_css_selector("#sip-options .sip-option-name")]
        option_values = [e.get_attribute('value') for e in self.driver.find_elements_by_css_selector("#sip-options .sip-option-value")]
        return zip(option_names, option_values)

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

    def clear_options(self):
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
        page = self._add('sip')
        return page.sip()

    def add_custom(self):
        page = self._add('custom')
        return page.custom()

    def _add(self, protocol):
        url = self.build_url(self.url, act='add', proto=protocol)
        self.driver.get(url)
        self.wait_for_form()
        return self.form_page(self.driver)

    def edit_by_id(self, line_id):
        url = self.build_url(self.url, act='edit', id=str(line_id))
        self.driver.get(url)
        self.wait_for_form()
        return self.form_page(self.driver)
