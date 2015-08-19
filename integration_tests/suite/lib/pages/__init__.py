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


from page import Page, ListPage
from login import LoginPage
from user import UserListPage
from voicemail import VoicemailListPage

from selenium import webdriver
from pyvirtualdisplay import Display


class Browser(object):

    pages = {'login': LoginPage,
             'users': UserListPage}

    def __init__(self, username, password, virtual=True):
        self.username = username
        self.password = password
        self.display = Display(visible=virtual, size=(1024, 768))

    def start(self):
        self.display.start()
        self.driver = webdriver.Firefox()
        self.driver.set_window_size(1024, 768)
        LoginPage(self.driver).login(self.username, self.password)

    def __getattr__(self, name):
        page = self.pages[name](self.driver)
        return page.go()

    def stop(self):
        self.driver.close()
        self.display.stop()
