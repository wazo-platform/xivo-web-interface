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

import abc

import urllib

from selenium.common.exceptions import NoSuchElementException
from selenium.webdriver.support.ui import WebDriverWait, Select
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.common.by import By
from selenium.webdriver.common.alert import Alert


class Page(object):

    TIMEOUT = 4
    CONFIG = {'base_url': 'http://localhost:10080'}

    def __init__(self, driver):
        self.driver = driver

    def build_url(self, *parts, **kwargs):
        path = '/'.join(parts)
        url = "{}/{}".format(self.CONFIG['base_url'].rstrip('/'), path.lstrip('/'))
        if kwargs:
            url += "?{}".format(urllib.urlencode(kwargs))
        return url

    def wait(self):
        return WebDriverWait(self.driver, self.TIMEOUT)

    def wait_for(self, by, arg):
        condition = ec.presence_of_element_located((by, arg))
        self.wait().until(condition)

    def wait_visible(self, by, arg):
        condition = ec.visibility_of_element_located((by, arg))
        self.wait().until(condition)

    def fill(self, by, arg, value, root=None):
        root = root or self.driver
        element = root.find_element(by, arg)
        if element.tag_name == 'select':
            Select(element).select_by_visible_text(value)
        elif element.get_attribute('type') == 'checkbox':
            if element.is_selected() and value is False:
                element.click()
            elif not element.is_selected() and value is True:
                element.click()
        else:
            element.clear()
            element.send_keys(value)

    def fill_name(self, name, value, root=None):
        self.fill(By.NAME, name, value, root)

    def fill_id(self, id_, value, root=None):
        self.fill(By.ID, id_, value, root)

    def select(self, by, arg, value, root=None):
        root = root or self.driver
        element = root.find_element(by, arg)
        Select(element).select_by_visible_text(value)

    def select_name(self, name, value, root=None):
        self.select(By.NAME, name, value, root)

    def select_id(self, id_, value, root=None):
        self.select(By.ID, id_, value, root)

    def get_value(self, id_, root=None):
        root = root or self.driver
        element = root.find_element_by_id(id_)
        return element.get_attribute('value')

    def get_selected_option_value(self, id_, root=None):
        root = root or self.driver
        element = root.find_element_by_id(id_)
        return Select(element).first_selected_option.get_attribute('value')

    def extract_errors(self):
        try:
            container = self.driver.find_element_by_id("report-xivo-error")
        except NoSuchElementException:
            return []

        return [message.text
                for message in container.find_elements_by_css_selector("li")]


class ListPage(Page):

    __metaclass__ = abc.ABCMeta

    line_xpath = "//tr[td[contains(@title, '{name}')]]"
    edit_xpath = "{}/td/a[@title='Edit']".format(line_xpath)
    delete_xpath = "{}/td/a[@title='Delete']".format(line_xpath)

    @abc.abstractproperty
    def url(self):
        return

    @abc.abstractproperty
    def list_selector(self):
        return

    @abc.abstractproperty
    def form_selector(self):
        return

    @abc.abstractproperty
    def form_page(self):
        return

    def go(self):
        url = self.build_url(self.url)
        self.driver.get(url)

        self.wait_for_list_form()

        return self

    def add(self):
        self.add_form()
        self.wait_for_form()
        return self.form_page(self.driver)

    def add_form(self):
        url = self.build_url(self.url, act='add')
        self.driver.get(url)

    def wait_for_form(self):
        condition = ec.presence_of_element_located(self.form_selector)
        self.wait().until(condition)

    def wait_for_list_form(self):
        condition = ec.presence_of_element_located(self.list_selector)
        self.wait().until(condition)

    def edit(self, name):
        xpath = self.edit_xpath.format(name=name)

        button = self.driver.find_element_by_xpath(xpath)
        button.click()

        self.wait_for_form()

        return self.form_page(self.driver)

    def edit_by_id(self, id_):
        url = self.build_url(self.url, act='edit', id=str(id_))
        self.driver.get(url)
        self.wait_for_form()
        return self.form_page(self.driver)

    def delete(self, name):
        xpath = self.delete_xpath.format(name=name)

        button = self.driver.find_element_by_xpath(xpath)
        button.click()

        condition = ec.alert_is_present()
        self.wait().until(condition)
        Alert(self.driver).accept()

        condition = ec.presence_of_element_located((By.XPATH, xpath))
        self.wait().until_not(condition)

    def delete_by_id(self, id_):
        url = self.build_url(self.url, act='delete', id=str(id_))
        self.driver.get(url)
        self.wait_for_list_form()

    def find_row(self, text):
        table = self.driver.find_element_by_id("table-main-listing")
        xpath = '//tr[td[contains(., "{}")]]'.format(text)
        try:
            return ListRow(table.find_element_by_xpath(xpath))
        except NoSuchElementException:
            return None

    def get_row(self, text):
        table = self.driver.find_element_by_id("table-main-listing")
        xpath = '//tr[td[contains(., "{}")]]'.format(text)
        return ListRow(table.find_element_by_xpath(xpath))


class ListRow(object):

    def __init__(self, row):
        self.row = row

    def extract(self, column):
        selector = "td.col_{}".format(column)
        box = self.row.find_element_by_css_selector(selector)
        return box.text
