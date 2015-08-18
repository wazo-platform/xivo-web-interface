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
import urllib

from selenium.webdriver.support.ui import WebDriverWait, Select
from selenium.webdriver.support import expected_conditions as ec
from selenium.webdriver.common.by import By


class Page(object):

    TIMEOUT = 4
    CONFIG = {'base_url': 'http://localhost:8080'}

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
