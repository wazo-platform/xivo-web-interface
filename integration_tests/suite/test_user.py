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

import random
import hashlib

from lib.testcase import TestWebi

from hamcrest import assert_that, equal_to, none


class TestUser(TestWebi):

    asset = 'users'

    FK_TEMPLATE = {'id': 1,
                   'name': None,
                   'keys': [],
                   'links': [{u'href': u'https://webi:9486/1.1/funckeys/templates/1',
                              u'rel': u'func_key_templates'}]}

    ENDPOINT_SIP = {'id': 1,
                    'host': 'dynamic',
                    'username': 'mockusername',
                    'secret': 'mocksecret',
                    'options': [],
                    'links': [{u'href': u'https://confd:9486/1.1/endpoints/sip/1',
                               u'rel': u'endpoint_sip'}]
                    }

    def build_device(self, **kwargs):
        device_id = hashlib.md5(str(random.random())).hexdigest()
        mac = ':'.join('{:02x}'.format(random.randrange(256)) for i in range(6))
        device = {'id': device_id,
                  'description': None,
                  'ip': '127.0.1.1',
                  'links': [{'href': 'https://confd:9486/1.1/devices/{}'.format(device_id),
                             'rel': 'devices'}],
                  'mac': mac,
                  'model': '6731i',
                  'options': None,
                  'plugin': 'null',
                  'sn': None,
                  'status': 'autoprov',
                  'template_id': 'defaultconfigdevice',
                  'vendor': 'Aastra',
                  'version': '3.3.1.4322'}
        device.update(kwargs)
        return device

    def add_autoprov_device(self, device, makelist=True):
        device = dict(device)
        device.pop('links', None)
        device.pop('template_id', None)
        device.pop('status', None)
        device['config_id'] = self.provd.configs.autocreate()
        self.provd.devices.add(device)

        self.confd.add_json_response("/devices/{}".format(device['id']),
                                     device)
        self.confd.add_response("/devices/{}/autoprov".format(device['id']),
                                code=204)

        if makelist:
            self.confd.add_json_response("/devices", {'total': 1,
                                                      'items': [device]})

    def add_sip_user(self, firstname, exten, provcode, lastname=None, device=None):
        fullname = " ".join([firstname, lastname or ""]).strip()
        with self.db.queries() as q:
            user_id = q.insert_user(firstname, lastname)
            line_id = q.insert_sip_line({"username": fullname[0:40],
                                         "callerid": '"{}" <{}>'.format(fullname, exten)},
                                        {"provisioningid": provcode,
                                         "number": exten,
                                         "device": device})
            extension_id = q.insert_extension(exten, "default", "user", user_id)
            q.associate_user_line_extension(user_id, line_id, extension_id)

    def add_sccp_user(self, firstname, exten, lastname=None, device=None):
        fullname = " ".join([firstname, lastname or ""]).strip()
        with self.db.queries() as q:
            user_id = q.insert_user(firstname, lastname)
            line_id = q.insert_sccp_line({"name": exten,
                                          "cid_name": fullname,
                                          "cid_num": exten},
                                         {"provisioningid": 0,
                                          "number": exten,
                                          "device": device})
            extension_id = q.insert_extension(exten, "default", "user", user_id)
            q.associate_user_line_extension(user_id, line_id, extension_id)


class TestUserList(TestUser):

    def test_when_user_has_no_line_then_line_fields_are_blank(self):
        with self.db.queries() as q:
            q.insert_user("UserNoLine")

        page = self.browser.users.go()
        row = page.get_row("UserNoLine")

        assert_that(row.extract('provisioning_code'), equal_to('-'))
        assert_that(row.extract('protocol'), equal_to(''))
        assert_that(row.extract('number'), equal_to('-'))

    def test_when_user_has_sip_line_then_line_fields_are_filled(self):
        self.add_sip_user("UserSipLine", "1324", "132435")

        page = self.browser.users.go()
        row = page.get_row("UserSipLine")

        assert_that(row.extract('provisioning_code'), equal_to('132435'))
        assert_that(row.extract('protocol'), equal_to('sip'))
        assert_that(row.extract('number'), equal_to('1324'))

    def test_when_user_has_sccp_line_then_line_fields_are_filled(self):
        self.add_sccp_user("UserSccpLine", "1325")

        page = self.browser.users.go()
        row = page.get_row("UserSccpLine")

        assert_that(row.extract('provisioning_code'), equal_to('-'))
        assert_that(row.extract('protocol'), equal_to('sccp'))
        assert_that(row.extract('number'), equal_to('1325'))


class TestUserCreate(TestUser):

    def setUp(self):
        super(TestUserCreate, self).setUp()
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/endpoints/sip/\d+", self.ENDPOINT_SIP)
        self.confd.add_response(r"/endpoints/sip/\d+", method='PUT', code=204)

    def test_when_creating_user_with_sip_line_and_extension_then_line_and_extension_created(self):
        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineSip")

        tab = page.lines()
        tab.add_line(protocol="SIP",
                     context="Default",
                     number="1000")

        page.save()

    def test_when_creating_user_with_sccp_line_and_extension_then_line_and_extension_created(self):
        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineSccp")

        tab = page.lines()
        tab.add_line(protocol="SCCP",
                     context="Default",
                     number="1001")
        page.save()

    def test_when_creating_user_with_sip_line_and_device_then_device_associated(self):
        device = self.build_device(mac="00:08:5d:31:ef:e1")
        self.add_autoprov_device(device)

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineDeviceSip")

        tab = page.lines()
        tab.add_line(protocol="SIP",
                     context="Default",
                     number="1002",
                     device=device['mac'])

        page.save()

    def test_when_creating_user_with_sccp_line_and_device_then_device_associated(self):
        device = self.build_device(mac="00:08:5d:31:ef:e2")
        self.add_autoprov_device(device)

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineDeviceSccp")

        tab = page.lines()
        tab.add_line(protocol="SCCP",
                     context="Default",
                     number="1003",
                     device=device['mac'])

        page.save()


class TestUserEdit(TestUser):

    def setUp(self):
        super(TestUserEdit, self).setUp()
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/endpoints/sip/\d+", self.ENDPOINT_SIP)
        self.confd.add_json_response(r"/endpoints/sip/\d+", self.ENDPOINT_SIP)
        self.confd.add_json_response(r"/endpoints/sip/\d+", self.ENDPOINT_SIP)
        self.confd.add_response(r"/endpoints/sip/\d+", method='PUT', code=204)

    def test_when_editing_user_then_user_updated(self):
        with self.db.queries() as q:
            q.insert_user("UserEdit")

        page = self.browser.users.edit("UserEdit")
        page.fill_form(firstname="UserModified")
        page.save()

    def test_given_user_has_sip_line_when_editing_exten_then_user_updated(self):
        self.add_sip_user("UserEditSipExten", "1350", "132500")

        page = self.browser.users.edit("UserEditSipExten")

        tab = page.lines()
        tab.edit_line(number="1351")

        page.save()

    def test_given_user_has_sccp_line_when_editing_exten_then_user_updated(self):
        self.add_sccp_user("UserEditSccpExten", "1352")

        page = self.browser.users.edit("UserEditSccpExten")

        tab = page.lines()
        tab.edit_line(number="1353")

        page.save()

    def test_given_user_has_sip_line_when_adding_device_then_user_updated(self):
        self.add_sip_user("UserEditAddSipDevice", "1354", "132502")

        device = self.build_device(mac="00:08:5d:31:ef:e1")
        self.add_autoprov_device(device)

        page = self.browser.users.edit("UserEditAddSipDevice")

        tab = page.lines()
        tab.edit_line(device="00:08:5d:31:ef:e1")

        page.save()

    def test_given_user_has_sccp_line_when_adding_device_then_user_updated(self):
        self.add_sccp_user("UserEditAddSccpDevice", "1355")

        device = self.build_device(mac="00:08:5d:31:ef:e2")
        self.add_autoprov_device(device)

        page = self.browser.users.edit("UserEditAddSccpDevice")

        tab = page.lines()
        tab.edit_line(device="00:08:5d:31:ef:e2")

        page.save()

    def test_given_user_has_sip_device_when_removing_device_then_user_updated(self):
        device = self.build_device(mac="00:08:5d:31:ef:e3")
        self.add_autoprov_device(device)

        self.add_sip_user("UserEditRemoveSipDevice", "1356", "132506", device['id'])

        page = self.browser.users.edit("UserEditRemoveSipDevice")

        tab = page.lines()
        tab.remove_device()

        page.save()

    def test_given_user_has_sccp_device_when_removing_device_then_user_updated(self):
        device = self.build_device(mac="00:08:5d:31:ef:e4")
        self.add_autoprov_device(device)

        self.add_sccp_user("UserEditRemoveSccpDevice", "1357", device['id'])

        page = self.browser.users.edit("UserEditRemoveSccpDevice")

        tab = page.lines()
        tab.remove_device()

        page.save()

    def test_given_user_has_sip_device_when_changing_device_then_user_updated(self):
        device1 = self.build_device(mac="00:08:5d:31:ef:e5")
        device2 = self.build_device(mac="00:08:5d:31:ef:e6")
        self.add_autoprov_device(device1, makelist=False)
        self.add_autoprov_device(device2, makelist=False)
        self.confd.add_json_response("/devices", {'total': 2, 'items': [device1, device2]})

        self.add_sip_user("UserEditChangeSipDevice", "1358", "101358", device1['id'])

        page = self.browser.users.edit("UserEditChangeSipDevice")

        tab = page.lines()
        tab.edit_line(device=device2['mac'])

        page.save()

    def test_given_user_has_sccp_device_when_changing_device_then_user_updated(self):
        device1 = self.build_device(mac="00:08:5d:31:ef:e7")
        device2 = self.build_device(mac="00:08:5d:31:ef:e8")
        self.add_autoprov_device(device1, makelist=False)
        self.add_autoprov_device(device2, makelist=False)
        self.confd.add_json_response("/devices", {'total': 2, 'items': [device1, device2]})

        self.add_sccp_user("UserEditChangeSccpDevice", "1359", device1['id'])

        page = self.browser.users.edit("UserEditChangeSccpDevice")

        tab = page.lines()
        tab.edit_line(device=device2['mac'])

        page.save()


class TestUserDelete(TestUser):

    def setUp(self):
        super(TestUserDelete, self).setUp()
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/users/\d+/funckeys", self.FK_TEMPLATE)
        self.confd.add_json_response(r"/endpoints/sip/\d+", self.ENDPOINT_SIP)

    def test_when_user_has_no_line_then_user_deleted(self):
        with self.db.queries() as q:
            q.insert_user("UserNoLine")

        page = self.browser.users
        page.delete("UserNoLine")
        assert_that(page.find_row("UserNoLine"), none())

    def test_when_user_has_sip_line_then_user_deleted(self):
        self.add_sip_user("UserDeleteSipLine", "1400", "101400")

        page = self.browser.users
        page.delete("UserDeleteSipLine")
        assert_that(page.find_row("UserDeleteSipLine"), none())

    def test_when_user_has_sccp_line_then_user_deleted(self):
        self.add_sccp_user("UserDeleteSccpLine", "1401")

        page = self.browser.users
        page.delete("UserDeleteSccpLine")
        assert_that(page.find_row("UserDeleteSccpLine"), none())

    def test_when_user_has_sip_device_then_user_deleted(self):
        device = self.build_device()
        self.add_autoprov_device(device)

        self.add_sip_user("UserDeleteSipDevice", "1410", "101410", device=device['id'])

        page = self.browser.users
        page.delete("UserDeleteSipDevice")
        assert_that(page.find_row("UserDeleteSipDevice"), none())

    def test_when_user_has_sccp_device_then_user_deleted(self):
        device = self.build_device()
        self.add_autoprov_device(device)

        self.add_sccp_user("UserDeleteSccpDevice", "1411", device=device['id'])

        page = self.browser.users
        page.delete("UserDeleteSccpDevice")
        assert_that(page.find_row("UserDeleteSccpDevice"), none())
