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

from functools import partial

from selenium.webdriver.support.ui import Select
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.action_chains import ActionChains
from selenium.common.exceptions import NoSuchElementException

from lib.pages import Page, ListPage


class UserPage(Page):

    def fill_form(self, **kwargs):
        for name, value in kwargs.iteritems():
            id_ = "it-userfeatures-{}".format(name)
            self.fill(By.ID, id_, value)

    def voicemail(self):
        link = self.driver.find_element_by_css_selector("a[href='#voicemail']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-voicemail')

        return VoicemailTab(self.driver)

    def funckeys(self):
        link = self.driver.find_element_by_css_selector("a[href='#funckeys']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-funckeys')

        return FuncKeyTab(self.driver)

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()
        self.wait_for(By.NAME, 'fm-users-list')


class VoicemailTab(Page):

    def get(self):
        search = self.driver.find_element_by_id("user-vm-search").get_attribute('value')
        enabled = self.driver.find_element_by_id("it-userfeatures-enablevoicemail").is_selected()
        name = self.driver.find_element_by_id("it-voicemail-name").get_attribute('value')
        number = self.driver.find_element_by_id("it-voicemail-number").get_attribute('value')
        password = self.driver.find_element_by_id("it-voicemail-password").get_attribute('value')
        email = self.driver.find_element_by_id("it-voicemail-email").get_attribute('value')
        context = Select(self.driver.find_element_by_id("it-voicemail-context")).first_selected_option.get_attribute('value')
        time_zone = Select(self.driver.find_element_by_id("it-voicemail-timezone")).first_selected_option.get_attribute('value')
        language = Select(self.driver.find_element_by_id("it-voicemail-language")).first_selected_option.get_attribute('value')
        max_messages = int(self.driver.find_element_by_id("it-voicemail-max-messages").get_attribute('value'))
        ask_password = self.driver.find_element_by_id("it-voicemail-ask-password").is_selected()
        attach_audio = Select(self.driver.find_element_by_id("it-voicemail-attach_audio")).first_selected_option.get_attribute('value')
        if attach_audio == '':
            attach_audio = None
        elif attach_audio == '1':
            attach_audio = True
        elif attach_audio == '0':
            attach_audio = False
        delete_message = self.driver.find_element_by_id("it-voicemail-delete-messages").is_selected()

        return {
            'search': search,
            'enabled': enabled,
            'name': name,
            'number': number,
            'password': password,
            'email': email,
            'context': context,
            'time_zone': time_zone,
            'language': language,
            'max_messages': max_messages,
            'ask_password': ask_password,
            'attach_audio': attach_audio,
            'delete_message': delete_message,
        }

    def select_voicemail(self, search):
        element = self.driver.find_element_by_id("user-vm-search")
        element.clear()

        selector = '.ui-autocomplete[style*="display: block"]'
        condition = ec.presence_of_element_located((By.CSS_SELECTOR, selector))

        # make sure there isn't any other autocomplete still lying around
        self.wait().until_not(condition)

        element.send_keys(search)
        self.wait().until(condition)

        ActionChains(self.driver).send_keys_to_element(element, Keys.DOWN, Keys.RETURN).perform()
        self.wait().until_not(condition)


class FuncKeyTab(Page):

    KEY_XPATH = "//tr[td/select[@name='phonefunckey[fknum][]']/option[@selected='selected' and @value='{key}']]"

    AUTOCOMPLETE = ('User',
                    'Group',
                    'Queue',
                    'Conference room',
                    'Paging',
                    'Connect/Disconnect an agent',
                    'Connect an agent',
                    'Disconnect an agent')

    def get(self, keynum):
        xpath = self.KEY_XPATH.format(key=keynum)
        line = self.driver.find_element_by_xpath(xpath)

        key = line.find_element_by_name('phonefunckey[fknum][]')
        type = line.find_element_by_name('phonefunckey[type][]')
        label = line.find_element_by_name('phonefunckey[label][]')
        supervision = line.find_element_by_name('phonefunckey[supervision][]')

        key_value = int(key.get_attribute('value'))
        type_value = type.find_element_by_css_selector('option[selected="selected"]').text
        label_value = label.get_attribute('value')
        supervision_value = supervision.get_attribute('value') == '1'

        destination_value = self.get_destination_value(line)

        return {'key': key_value,
                'type': type_value,
                'label': label_value,
                'supervision': supervision_value,
                'destination': destination_value}

    def get_destination_value(self, line):
        try:
            destination = line.find_element_by_name('phonefunckey[typevalidentity][]')
        except NoSuchElementException:
            destination = line.find_element_by_name('phonefunckey[fkbsfilter][]')

        if destination.tag_name == 'select':
            return destination.find_element_by_css_selector('option[selected="selected"]').text

        return destination.get_attribute('value')

    def add(self, type, key=None, destination=None, label=None, supervision=None):
        line = self.add_line()
        self._fill(line, type, key, destination, label, supervision)

    def edit(self, key, type=None, destination=None, label=None, supervision=None):
        xpath = self.KEY_XPATH.format(key=key)
        line = self.driver.find_element_by_xpath(xpath)
        self._fill(line, type, key, destination, label, supervision)

    def remove(self, key):
        xpath = self.KEY_XPATH.format(key=key)
        line = self.driver.find_element_by_xpath(xpath)

        element = line.find_element_by_css_selector('.fkdelete')
        element.click()

        condition = ec.presence_of_element_located((By.XPATH, xpath))
        self.wait().until_not(condition)

    def _fill(self, line, type, key, destination, label, supervision):
        if type:
            self.select_name('phonefunckey[type][]', type, line)

        if key:
            self.select_name('phonefunckey[fknum][]', str(key), line)

        if label:
            line.find_element_by_name('phonefunckey[label][]').clear()
            self.fill_name('phonefunckey[label][]', label, line)

        if destination:
            self.fill_destination(line, type, destination)

        if supervision is not None:
            value = "Enabled" if supervision else "Disabled"
            self.select_name('phonefunckey[supervision][]', value, line)

    def add_line(self):
        total = self.count_lines()

        btn = self.driver.find_element_by_id('add_funckey_button')
        btn.click()

        self.wait().until(partial(self.check_count, total))

        return self.last_line()

    def count_lines(self):
        selector = "tbody#phonefunckey tr"
        try:
            return len(self.driver.find_elements_by_css_selector(selector))
        except NoSuchElementException:
            return 0

    def check_count(self, total, driver):
        return self.count_lines() > total

    def last_line(self):
        selector = 'tbody#phonefunckey tr:last-child'
        return self.driver.find_element_by_css_selector(selector)

    def fill_destination(self, line, fktype, destination):
        element = self.find_destination(line)
        element.clear()

        if element.tag_name == 'select':
            Select(element).select_by_visible_text(destination)
        elif fktype in self.AUTOCOMPLETE:
            self.select_autocomplete(line, element, destination)
        else:
            element.send_keys(destination)

    def find_destination(self, line):
        try:
            return line.find_element_by_name("phonefunckey[typevalidentity][]")
        except NoSuchElementException:
            return line.find_element_by_name("phonefunckey[fkbsfilter][]")

    def select_autocomplete(self, line, element, destination):
        selector = '.ui-autocomplete[style*="display: block"]'
        condition = ec.presence_of_element_located((By.CSS_SELECTOR, selector))

        # make sure there isn't any other autocomplete still lying around
        self.wait().until_not(condition)

        element.send_keys(destination)
        self.wait().until(condition)

        ActionChains(self.driver).send_keys_to_element(element, Keys.DOWN, Keys.RETURN).perform()
        self.wait().until_not(condition)


class UserListPage(ListPage):

    url = "/service/ipbx/index.php/pbx_settings/users/"
    list_selector = (By.NAME, "fm-users-list")
    form_selector = (By.ID, "sr-users")
    form_page = UserPage
