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

    def no_answer(self):
        link = self.driver.find_element_by_css_selector("a[href='#dialaction']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-dialaction')

        return NoAnswerTab(self.driver)

    def lines(self):
        link = self.driver.find_element_by_css_selector("a[href='#lines']")
        link.click()
        self.wait_visible(By.ID, 'sb-part-lines')

        return LineTab(self.driver)

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()
        self.wait_for(By.NAME, 'fm-users-list')


class LineTab(Page):

    def add_line(self, **form):
        add = self.driver.find_element_by_id("lnk-add-row")
        add.click()
        self.wait_visible(By.CSS_SELECTOR, "tbody#linefeatures tr.fm-paragraph")
        self.fill_form(form)

    def edit_line(self, **form):
        self.fill_form(form)

    def remove_line(self):
        selector = "tbody#linefeatures tr td:last-child a"
        btn = self.driver.find_element_by_css_selector(selector)
        btn.click()

        condition = ec.invisibility_of_element_located((By.CSS_SELECTOR, "tbody#linefeatures tr"))
        self.wait().until(condition)

    def remove_device(self):
        dropdown = self.driver.find_element_by_id("s2id_linefeatures-device")
        dropdown.click()
        self.wait_visible(By.CSS_SELECTOR, ".select2-input")

        first = self.driver.find_element_by_css_selector("li.select2-result:first-child")
        first.click()

        self.wait().until(partial(self.text_appears, ""))

    def fill_form(self, form):
        for name, value in form.iteritems():
            if name == "device":
                self.select_device(value)
            else:
                name = "linefeatures[{}][]".format(name)
                self.fill(By.NAME, name, value)

    def select_device(self, value):
        dropdown = self.driver.find_element_by_id("s2id_linefeatures-device")
        dropdown.click()
        self.wait_visible(By.CSS_SELECTOR, ".select2-input")

        search = self.driver.find_element_by_css_selector(".select2-input")
        search.clear()
        search.send_keys(value)
        self.wait().until(self.list_has_one_result)

        search.send_keys(Keys.RETURN)
        self.wait().until(partial(self.mac_appears, value))

    def list_has_one_result(self, driver):
        elements = self.driver.find_elements_by_css_selector("li.select2-result")
        return len(elements) == 1

    def mac_appears(self, value, driver):
        return self.text_appears("MAC: {}".format(value), driver)

    def text_appears(self, value, driver):
        selector = "#s2id_linefeatures-device .select2-choice span"
        span = self.driver.find_element_by_css_selector(selector)
        return span.text.strip() == value


class NoAnswerTab(Page):

    def no_answer(self):
        return NoAnswerSection(self.driver, 'noanswer')

    def busy(self):
        return NoAnswerSection(self.driver, 'busy')

    def congestion(self):
        return NoAnswerSection(self.driver, 'congestion')

    def fail(self):
        return NoAnswerSection(self.driver, 'chanunavail')


class NoAnswerSection(Page):

    def __init__(self, driver, section):
        super(NoAnswerSection, self).__init__(driver)
        self.section = section

    def select_destination(self, destination):
        id = 'it-dialaction-{}-actiontype'.format(self.section)
        self.fill_id(id, destination)

    def redirection_list(self):
        selector = '#fld-dialaction-{} div[style*="display: block"] select'.format(self.section)
        try:
            dropdown = Select(self.driver.find_element_by_css_selector(selector))
        except NoSuchElementException:
            return None
        return [o.text for o in dropdown.options]


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
        self.wait().until(self.check_number_filled)

    def check_number_filled(self, driver):
        number = self.driver.find_element_by_id("it-voicemail-number").get_attribute('value')
        return number != ""


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


class ImportPage(Page):

    def upload_file(self, filepath):
        self.fill_id("it-import", filepath)

    def save(self):
        btn = self.driver.find_element_by_id("it-submit")
        btn.click()
        self.wait_for(By.NAME, 'fm-users-list')


class UserListPage(ListPage):

    url = "/service/ipbx/index.php/pbx_settings/users/"
    list_selector = (By.NAME, "fm-users-list")
    form_selector = (By.ID, "sr-users")
    form_page = UserPage

    def import_csv(self):
        url = self.build_url(self.url, act='import')
        self.driver.get(url)
        return ImportPage(self.driver)

    def update_csv(self):
        url = self.build_url(self.url, act='update_import')
        self.driver.get(url)
        return ImportPage(self.driver)
