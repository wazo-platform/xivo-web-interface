# -*- coding: utf-8 -*-

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

import unittest

from lib import testsetup


class TestWebi(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        testsetup.setup_docker(cls.asset)

        cls.db = testsetup.setup_db()
        cls.db.recreate()

        cls.provd = testsetup.setup_provd()
        cls.provd.recreate()

        cls.browser = testsetup.setup_browser()
        cls.browser.start()

        cls.confd = testsetup.setup_confd()

        cls.bus = testsetup.setup_bus()

    @classmethod
    def tearDownClass(cls):
        cls.browser.stop()
        testsetup.cleanup_docker(cls.asset)

    def setUp(self):
        self.addCleanup(self.confd.clear)
