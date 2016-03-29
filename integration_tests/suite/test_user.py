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
import string
import unittest

from lib.testcase import TestWebi

from hamcrest import assert_that, equal_to, none


class TestUser(TestWebi):

    asset = 'users'

    FK_TEMPLATE = {'id': 1,
                   'name': None,
                   'keys': [],
                   'links': [{u'href': u'https://webi:9486/1.1/funckeys/templates/1',
                              u'rel': u'func_key_templates'}]}

    def setUp(self):
        super(TestUser, self).setUp()
        self.db.recreate()
        self.browser.login()

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

    def add_autoprov_device(self, makelist=True, **device):
        orig_device = self.build_device(**device)
        device = dict(orig_device)
        device.pop('links', None)
        device.pop('template_id', None)
        device.pop('status', None)
        device['config_id'] = self.provd.configs.autocreate()
        self.provd.devices.add(device)

        self.confd.add_json_response("/devices/{}".format(device['id']),
                                     device,
                                     preserve=True)
        self.confd.add_response("/devices/{}/autoprov".format(device['id']),
                                code=204)

        if makelist:
            self.confd.add_json_response("/devices",
                                         {'total': 1,
                                          'items': [device]},
                                         preserve=True)

        return orig_device

    def add_sip_user(self, firstname, exten, provcode, device=None):
        user_id = self.add_user(firstname)
        extension = self.add_extension(exten, "default", "user", user_id)
        line = self.add_line(provisioning_code=provcode,
                             device_id=device,
                             protocol="sip")
        sip = self.add_sip()

        self.associate_resources(user_id, line['id'], extension['id'], "sip", sip['id'], device)

        return line

    def add_sccp_user(self, firstname, exten, device=None):
        user_id = self.add_user(firstname)
        extension = self.add_extension(exten, "default", "user", user_id)
        line = self.add_line(device_id=device, protocol="sccp")
        sccp = self.add_sccp()

        self.associate_resources(user_id, line['id'], extension['id'], "sccp", sccp['id'], device)

        return line

    def add_custom_user(self, firstname, exten, device=None):
        user_id = self.add_user(firstname)
        extension = self.add_extension(exten, "default", "user", user_id)
        line = self.add_line(device_id=device, protocol="custom")
        custom = self.add_custom()

        self.associate_resources(user_id, line['id'], extension['id'], "custom", custom['id'], device)

        return line

    def add_extension(self, exten, context, type_, typeval):
        with self.db.queries() as q:
            extension_id = q.insert_extension(exten, context, type_, typeval)

        url = "/extensions/{}".format(extension_id)
        extension = {'id': extension_id, 'exten': exten, 'context': context}
        self.confd.add_json_response(url, extension, preserve=True)

        return extension

    def add_user(self, firstname):
        with self.db.queries() as q:
            user_id = q.insert_user(firstname)

        self.confd.add_response("/users/{}/voicemail".format(user_id), code=404)
        self.confd.add_json_response("/users/{}/funckeys".format(user_id), self.FK_TEMPLATE, preserve=True)

        return user_id

    def add_line(self, **extra):
        line = self.build_line(**extra)

        with self.db.queries() as q:
            q.insert_line(id=line['id'],
                          context=line['context'])

        url = "/lines/{}".format(line['id'])
        self.confd.add_json_response(url, line, preserve=True)

        return line

    def build_line(self, **extra):
        line = {
            'id': random.randint(1, 9999),
            'context': 'default',
            'device_id': None,
            'device_slot': 1,
            'name': None,
            'protocol': None,
            'provisioning_extension': '000000',
            'provisioning_code': '000000',
            'caller_id_name': None,
            'caller_id_num': None,
        }
        line.update(extra)
        return line

    def add_sip(self, **extra):
        sip = self.build_sip(**extra)
        url = "/endpoints/sip/{}".format(sip['id'])
        self.confd.add_json_response(url, sip, preserve=True)
        return sip

    def build_sip(self, **extra):
        username = ''.join(random.choice(string.ascii_letters) for _ in range(8))
        secret = ''.join(random.choice(string.ascii_letters) for _ in range(8))
        sip = {
            'id': random.randint(1, 9999),
            'username': username,
            'secret': secret,
            'type': 'friend',
            'host': 'dynamic',
            'options': []
        }
        sip.update(extra)
        return sip

    def add_sccp(self, **extra):
        sccp = self.build_sccp(**extra)
        url = "/endpoints/sccp/{}".format(sccp['id'])
        self.confd.add_json_response(url, sccp, preserve=True)
        return sccp

    def build_sccp(self, **extra):
        sccp = {
            'id': random.randint(1, 9999),
            'options': []
        }
        sccp.update(extra)
        return sccp

    def add_custom(self, **extra):
        custom = self.build_custom(**extra)
        url = "/endpoints/custom/{}".format(custom['id'])
        self.confd.add_json_response(url, custom, preserve=True)
        return custom

    def build_custom(self, **extra):
        interface = ''.join(random.choice(string.ascii_letters) for _ in range(8))
        custom = {
            'id': random.randint(1, 9999),
            'interface': interface,
            'enabled': True
        }
        custom.update(extra)
        return custom

    def associate_resources(self, user_id, line_id, extension_id, endpoint, endpoint_id, device_id=None):
        with self.db.queries() as q:
            q.associate_user_line_extension(user_id, line_id, extension_id)

        user_url = "/users/{}/lines".format(user_id)
        line_url = "/lines/{}/users".format(line_id)

        user_lines = {'total': 1,
                      'items': [
                          {
                              'main_line': True,
                              'main_user': True,
                              'user_id': user_id,
                              'line_id': line_id
                          }
                      ]}
        self.confd.add_json_response(user_url, user_lines, preserve=True)
        self.confd.add_json_response(line_url, user_lines, preserve=True)

        url = "/lines/{}/extensions".format(line_id)
        line_extensions = {'total': 1,
                           'items': [
                               {
                                   'line_id': line_id,
                                   'extension_id': extension_id
                               }
                           ]}
        self.confd.add_json_response(url, line_extensions, preserve=True)

        url = "/lines/{}/endpoints/{}".format(line_id, endpoint)
        line_endpoint = {'line_id': line_id,
                         'endpoint': endpoint,
                         'endpoint_id': endpoint_id}
        self.confd.add_json_response(url, line_endpoint, preserve=True)

        url = "/lines/\d+/devices".format(line_id)
        if device_id:
            line_device = {'line_id': line_id,
                           'device_id': device_id}
            self.confd.add_json_response(url, line_device, preserve=True)
        else:
            self.confd.add_response(url, code=404, preserve=True)

    def add_empty_user(self, firstname):
        with self.db.queries() as q:
            user_id = q.insert_user(firstname)
            self.confd.add_json_response("/users/{}/funckeys".format(user_id),
                                         self.FK_TEMPLATE,
                                         preserve=True)
            self.confd.add_json_response("/users/{}/lines".format(user_id),
                                         {'total': 0, 'lines': []},
                                         preserve=True)


class TestUserList(TestUser):

    def test_when_user_has_no_line_then_line_fields_are_blank(self):
        self.add_empty_user("UserNoLine")

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

    def test_when_user_has_custom_line_then_line_fields_are_filled(self):
        self.add_custom_user("UserCustomLine", "1326")

        page = self.browser.users.go()
        row = page.get_row("UserCustomLine")

        assert_that(row.extract('provisioning_code'), equal_to('-'))
        assert_that(row.extract('protocol'), equal_to('custom'))
        assert_that(row.extract('number'), equal_to('1326'))


class TestUserCreate(TestUser):

    def simulate_line_creation(self, endpoint, protocol, device=None):
        line = self.add_line()
        self.confd.add_json_response("/lines", line, method="POST", code=201)

        url = "/endpoints/{}".format(protocol)
        self.confd.add_json_response(url, endpoint, method="POST", code=201)

        url = r"/lines/\d+/endpoints/{}/\d+".format(protocol)
        self.confd.add_response(url, method="PUT", code=204)

        self.confd.add_response("/users/\d+", method="PUT", code=204)

        self.confd.add_response(r"/lines/\d+/devices", code=404)
        if device:
            self.confd.add_response(r"/lines/\d+/devices/[a-z0-9]+", method="PUT", code=204)

    def test_when_creating_user_with_sip_line_and_extension_then_line_and_extension_created(self):
        sip = self.add_sip()
        self.simulate_line_creation(sip, "sip")

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineSip")

        tab = page.lines()
        tab.add_line(protocol="SIP",
                     context="Default",
                     number="1000")

        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/sip", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sip/\d+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    def test_when_creating_user_with_sccp_line_and_extension_then_line_and_extension_created(self):
        sccp = self.add_sccp()
        self.simulate_line_creation(sccp, "sccp")

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineSccp")

        tab = page.lines()
        tab.add_line(protocol="SCCP",
                     context="Default",
                     number="1001")
        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/sccp", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sccp/\d+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    def test_when_creating_user_with_custom_line_and_extension_then_line_and_extension_created(self):
        custom = self.add_custom()
        self.simulate_line_creation(custom, "custom")

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineCustom")

        tab = page.lines()
        tab.add_line(protocol="Customized",
                     context="Default",
                     number="1002")
        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/custom", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/custom/\d+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    def test_when_creating_user_with_sip_line_and_device_then_device_associated(self):
        device = self.add_autoprov_device(mac="00:08:5d:31:ef:e1")

        sip = self.add_sip()
        self.simulate_line_creation(sip, "sip", device)

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineDeviceSip")

        tab = page.lines()
        tab.add_line(protocol="SIP",
                     context="Default",
                     number="1002",
                     device=device['mac'])

        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/sip", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sip/\d+", method="PUT")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    def test_when_creating_user_with_sccp_line_and_device_then_device_associated(self):
        device = self.add_autoprov_device(mac="00:08:5d:31:ef:e2")

        sccp = self.add_sccp()
        self.simulate_line_creation(sccp, "sccp", device)

        page = self.browser.users.add()
        page.fill_form(firstname="CreateLineDeviceSccp")

        tab = page.lines()
        tab.add_line(protocol="SCCP",
                     context="Default",
                     number="1003",
                     device=device['mac'])

        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/sccp", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sccp/\d+", method="PUT")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")


class TestUserEdit(TestUser):

    def simulate_line_update(self, line, device=None):
        self.confd.add_response(r"/lines/\d+", method="PUT", code=204)
        self.confd.add_response(r"/users/\d+", method="PUT", code=204)

    def simulate_line_add(self, line, endpoint):
        self.confd.add_json_response(r"/lines", line, method="POST", code=201)
        self.confd.add_json_response(r"/endpoints/(sip|sccp|custom)", endpoint, method="POST", code=201)
        self.confd.add_response(r"/lines/\d+/endpoints/(sip|sccp|custom)/\d+", method="PUT", code=204)
        self.confd.add_response(r"/lines/\d+/devices", code=404)
        self.confd.add_response(r"/users/\d+", method="PUT", code=204)

    def simulate_line_remove(self, line, device):
        self.confd.add_response(r"/lines/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/endpoints/sip/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/lines/\d+/extensions/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/lines/\d+/endpoints/sip/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE", code=204)
        self.confd.add_response(r"/users/\d+/lines/\d+", method="DELETE", code=204)

    def simulate_device_update(self, device=None):
        method = "PUT" if device else "DELETE"
        self.confd.add_response("/lines/\d+/devices/[a-z0-9]+", method=method, code=204)

    def test_when_editing_user_then_user_updated(self):
        self.add_empty_user("UserEdit")

        page = self.browser.users.edit("UserEdit")
        page.fill_form(firstname="UserModified")
        page.save()

        self.browser.users.get_row("UserModified")

    def test_given_user_has_sip_line_when_editing_exten_then_user_updated(self):
        line = self.add_sip_user("UserEditSipExten", "1350", "132500")
        self.simulate_line_update(line)

        page = self.browser.users.edit("UserEditSipExten")

        tab = page.lines()
        tab.edit_line(number="1351")

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")

    def test_given_user_has_sccp_line_when_editing_exten_then_user_updated(self):
        line = self.add_sccp_user("UserEditSccpExten", "1352")
        self.simulate_line_update(line)

        page = self.browser.users.edit("UserEditSccpExten")

        tab = page.lines()
        tab.edit_line(number="1353")

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")

    def test_given_user_has_custom_line_when_editing_exten_then_user_updated(self):
        line = self.add_custom_user("UserEditCustomExten", "1360")
        self.simulate_line_update(line)

        page = self.browser.users.edit("UserEditCustomExten")

        tab = page.lines()
        tab.edit_line(number="1361")

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")

    def test_given_user_has_sip_line_when_adding_device_then_user_updated(self):
        device = self.add_autoprov_device(mac="00:08:5d:31:ef:e1")

        line = self.add_sip_user("UserEditAddSipDevice", "1354", "132502")
        self.simulate_line_update(line)
        self.simulate_device_update(device)

        page = self.browser.users.edit("UserEditAddSipDevice")

        tab = page.lines()
        tab.edit_line(device="00:08:5d:31:ef:e1")

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")
        self.confd.assert_request_sent("/lines/\d+/devices/[a-z0-9]+", method="PUT")

    def test_given_user_has_sccp_line_when_adding_device_then_user_updated(self):
        device = self.add_autoprov_device(mac="00:08:5d:31:ef:e2")

        line = self.add_sccp_user("UserEditAddSccpDevice", "1355")
        self.simulate_line_update(line)
        self.simulate_device_update(device)

        page = self.browser.users.edit("UserEditAddSccpDevice")

        tab = page.lines()
        tab.edit_line(device="00:08:5d:31:ef:e2")

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")
        self.confd.assert_request_sent("/lines/\d+/devices/[a-z0-9]+", method="PUT")

    def test_given_user_has_sip_device_when_removing_device_then_user_updated(self):
        device = self.add_autoprov_device(mac="00:08:5d:31:ef:e3")

        line = self.add_sip_user("UserEditRemoveSipDevice", "1356", "132506", device['id'])
        self.simulate_line_update(line)
        self.simulate_device_update()

        page = self.browser.users.edit("UserEditRemoveSipDevice")

        tab = page.lines()
        tab.remove_device()

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")
        self.confd.assert_request_sent("/lines/\d+/devices/[a-z0-9]+", method="DELETE")

    def test_given_user_has_sccp_device_when_removing_device_then_user_updated(self):
        device = self.add_autoprov_device(mac="00:08:5d:31:ef:e4")

        line = self.add_sccp_user("UserEditRemoveSccpDevice", "1357", device['id'])
        self.simulate_line_update(line)
        self.simulate_device_update()

        page = self.browser.users.edit("UserEditRemoveSccpDevice")

        tab = page.lines()
        tab.remove_device()

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")
        self.confd.assert_request_sent("/lines/\d+/devices/[a-z0-9]+", method="DELETE")

    def test_given_user_has_sip_device_when_changing_device_then_user_updated(self):
        device1 = self.add_autoprov_device(makelist=False, mac="00:08:5d:31:ef:e5")
        device2 = self.add_autoprov_device(makelist=False, mac="00:08:5d:31:ef:e6")
        self.confd.add_json_response("/devices", {'total': 2, 'items': [device1, device2]})

        line = self.add_sip_user("UserEditChangeSipDevice", "1358", "101358", device1['id'])
        self.simulate_line_update(line)
        self.simulate_device_update()
        self.simulate_device_update(device2)

        page = self.browser.users.edit("UserEditChangeSipDevice")

        tab = page.lines()
        tab.edit_line(device=device2['mac'])

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")
        self.confd.assert_request_sent("/lines/\d+/devices/{}".format(device1['id']), method="DELETE")
        self.confd.assert_request_sent("/lines/\d+/devices/{}".format(device2['id']), method="PUT")

    def test_given_user_has_sccp_device_when_changing_device_then_user_updated(self):
        device1 = self.add_autoprov_device(makelist=False, mac="00:08:5d:31:ef:e7")
        device2 = self.add_autoprov_device(makelist=False, mac="00:08:5d:31:ef:e8")
        self.confd.add_json_response("/devices", {'total': 2, 'items': [device1, device2]})

        line = self.add_sccp_user("UserEditChangeSccpDevice", "1359", device1['id'])
        self.simulate_line_update(line)
        self.simulate_device_update()
        self.simulate_device_update(device2)

        page = self.browser.users.edit("UserEditChangeSccpDevice")

        tab = page.lines()
        tab.edit_line(device=device2['mac'])

        page.save()

        self.confd.assert_request_sent("/lines/\d+", method="PUT")
        self.confd.assert_request_sent("/lines/\d+/devices/{}".format(device1['id']), method="DELETE")
        self.confd.assert_request_sent("/lines/\d+/devices/{}".format(device2['id']), method="PUT")

    def test_given_user_has_no_line_when_adding_sip_line_then_user_updated(self):
        self.add_empty_user("UserEditAddSipLine")
        device = self.add_autoprov_device()
        line = self.add_line(context="default")
        sip = self.add_sip()

        self.simulate_line_add(line, sip)
        self.simulate_device_update(device)

        page = self.browser.users.edit("UserEditAddSipLine")
        page.lines().add_line(protocol="SIP",
                              context="Default",
                              number="1100",
                              device=device['mac'])
        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/sip", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sip/\d+", method="PUT")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    def test_given_user_has_no_line_when_adding_sccp_line_then_user_updated(self):
        self.add_empty_user("UserEditAddSccpLine")
        device = self.add_autoprov_device()
        line = self.add_line(context="default")
        sccp = self.add_sccp()

        self.simulate_line_add(line, sccp)
        self.simulate_device_update(device)

        page = self.browser.users.edit("UserEditAddSccpLine")
        page.lines().add_line(protocol="SCCP",
                              context="Default",
                              number="1200",
                              device=device['mac'])
        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/sccp", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sccp/\d+", method="PUT")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    def test_given_user_has_no_line_when_adding_custom_line_then_user_updated(self):
        self.add_empty_user("UserEditAddCustomLine")
        line = self.add_line(context="default")
        custom = self.add_custom()

        self.simulate_line_add(line, custom)

        page = self.browser.users.edit("UserEditAddCustomLine")
        page.lines().add_line(protocol="Customized",
                              context="Default",
                              number="1300")
        page.save()

        self.confd.assert_json_request(r"/lines", {"context": "default"}, method="POST")
        self.confd.assert_json_request(r"/endpoints/custom", {}, method="POST")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/custom/\d+", method="PUT")
        self.confd.assert_request_sent(r"/users/\d+", method="PUT")

    @unittest.skip("table user_line is still managed by webi")
    def test_given_user_has_sip_line_when_removing_line_then_user_updated(self):
        device = self.add_autoprov_device()
        line = self.add_sip_user("UserEditRemoveSipLine", "1500", device['id'])

        self.simulate_line_remove(line, device)

        page = self.browser.users.edit("UserEditRemoveSipLine")

        tab = page.lines()
        tab.remove_line()

        page.save()

        self.confd.assert_request_sent(r"/lines/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/endpoints/sip/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sip/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE")
        self.confd.assert_request_sent(r"/users/\d+/lines/\d+", method="DELETE")

    @unittest.skip("table user_line is still managed by webi")
    def test_given_user_has_sccp_line_when_removing_line_then_user_updated(self):
        device = self.add_autoprov_device()
        line = self.add_sccp_user("UserEditRemoveSccpLine", "1500", device['id'])

        self.simulate_line_remove(line, device)

        page = self.browser.users.edit("UserEditRemoveSccpLine")
        page.lines().remove_line()
        page.save()

        self.confd.assert_request_sent(r"/lines/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/endpoints/sccp/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sccp/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE")
        self.confd.assert_request_sent(r"/users/\d+/lines/\d+", method="DELETE")

    @unittest.skip("table user_line is still managed by webi")
    def test_given_user_has_custom_line_when_removing_line_then_user_updated(self):
        device = self.add_autoprov_device()
        line = self.add_custom_user("UserEditRemoveCustomLine", "1500", device['id'])

        self.simulate_line_remove(line, device)

        page = self.browser.users.edit("UserEditRemoveCustomLine")
        page.lines().remove_line()
        page.save()

        self.confd.assert_request_sent(r"/lines/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/endpoints/custom/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/custom/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE")
        self.confd.assert_request_sent(r"/users/\d+/lines/\d+", method="DELETE")


class TestUserDelete(TestUser):

    def simulate_line_delete(self, line):
        protocol = line['protocol']
        url = r"/lines/\d+/endpoints/{}/\d+".format(protocol)
        self.confd.add_response(url, method="DELETE", code=204)
        self.confd.add_response(r"/lines/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/users/\d+/lines/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/lines/\d+/extensions/\d+", method="DELETE", code=204)
        self.confd.add_response(r"/endpoints/(sip|sccp|custom)/\d+", method="DELETE", code=204)

    def simulate_device_delete(self, line, device):
        self.confd.add_response(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE", code=204)

    def test_when_user_has_no_line_then_user_deleted(self):
        self.add_empty_user("UserNoLine")

        page = self.browser.users
        page.delete("UserNoLine")
        assert_that(page.find_row("UserNoLine"), none())

    def test_when_user_has_sip_line_then_user_deleted(self):
        line = self.add_sip_user("UserDeleteSipLine", "1400", "101400")
        self.simulate_line_delete(line)

        page = self.browser.users
        page.delete("UserDeleteSipLine")
        assert_that(page.find_row("UserDeleteSipLine"), none())

        self.confd.assert_request_sent(r"/users/\d+/lines/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/extensions/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sip/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/endpoints/sip/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+", method="DELETE")

    def test_when_user_has_sccp_line_then_user_deleted(self):
        line = self.add_sccp_user("UserDeleteSccpLine", "1401")
        self.simulate_line_delete(line)

        page = self.browser.users
        page.delete("UserDeleteSccpLine")
        assert_that(page.find_row("UserDeleteSccpLine"), none())

        self.confd.assert_request_sent(r"/users/\d+/lines/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/extensions/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/sccp/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/endpoints/sccp/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+", method="DELETE")

    def test_when_user_has_custom_line_then_user_deleted(self):
        line = self.add_custom_user("UserDeleteCustomLine", "1450")
        self.simulate_line_delete(line)

        page = self.browser.users
        page.delete("UserDeleteCustomLine")
        assert_that(page.find_row("UserDeleteCustomLine"), none())

        self.confd.assert_request_sent(r"/users/\d+/lines/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/extensions/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+/endpoints/custom/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/endpoints/custom/\d+", method="DELETE")
        self.confd.assert_request_sent(r"/lines/\d+", method="DELETE")

    def test_when_user_has_sip_device_then_user_deleted(self):
        device = self.add_autoprov_device()
        line = self.add_sip_user("UserDeleteSipDevice", "1410", "101410", device=device['id'])

        self.simulate_line_delete(line)
        self.simulate_device_delete(line, device)

        page = self.browser.users
        page.delete("UserDeleteSipDevice")
        assert_that(page.find_row("UserDeleteSipDevice"), none())

        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE")

    def test_when_user_has_sccp_device_then_user_deleted(self):
        device = self.add_autoprov_device()
        line = self.add_sccp_user("UserDeleteSccpDevice", "1411", device=device['id'])

        self.simulate_line_delete(line)
        self.simulate_device_delete(line, device)

        page = self.browser.users
        page.delete("UserDeleteSccpDevice")
        assert_that(page.find_row("UserDeleteSccpDevice"), none())

        self.confd.assert_request_sent(r"/lines/\d+/devices/[a-z0-9]+", method="DELETE")
