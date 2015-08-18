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

from lib.pages import Page
from selenium.webdriver.common.by import By


class LoginPage(Page):

    PATH = "/index.php"

    def login(self, username, password, language='en'):
        url = self.build_url(self.PATH,
                             login=username,
                             password=password,
                             language=language,
                             go="/service/ipbx/index.php")
        self.driver.get(url)
        self.wait_for(By.ID, 'loginbox')
