# -*- coding: UTF-8 -*-

# Copyright (C) 2015-2016 Avencall
# Copyright (C) 2016 Proformatique Inc.
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

import os
import logging

import bus
import database
import confd
import provd

from xivo_provd_client import new_provisioning_client
from xivo_test_helpers.asset_launching_test_case import AssetLaunchingTestCase

from pages import Browser, Page

logger = logging.getLogger(__name__)

ASSET_PATH = os.path.join(os.path.dirname(__file__), '..', '..', 'assets')


class AssetLauncher(AssetLaunchingTestCase):

    asset = None
    assets_root = ASSET_PATH
    service = 'webi'


def setup_db():
    user = os.environ.get('DB_USER', 'asterisk')
    password = os.environ.get('DB_PASSWORD', 'proformatique')
    host = os.environ.get('DB_HOST', 'localhost')
    port = os.environ.get('DB_PORT')
    if not port:
        port = AssetLauncher.service_port(5432, 'postgres')
    db = os.environ.get('DB_NAME', 'asterisk')

    return database.DbHelper.build(user, password, host, port, db)


def setup_browser():
    virtual = os.environ.get('VIRTUAL_DISPLAY', '1') == '1'
    username = os.environ.get('WEBI_USERNAME', 'root')
    password = os.environ.get('WEBI_PASSWORD', 'proformatique')
    url = os.environ.get('WEBI_URL')
    if not url:
        port = AssetLauncher.service_port(80, 'webi')
        url = 'http://localhost:{port}'.format(port=port)
    Page.CONFIG['base_url'] = url
    return Browser(username, password, virtual)


def setup_bus():
    bus_url = os.environ.get('BUS_URL')
    if not bus_url:
        port = AssetLauncher.service_port(5672, 'rabbitmq')
        bus_url = 'amqp://guest:guest@localhost:{port}'.format(port=port)
    return bus.Bus(bus_url)


def setup_confd():
    url = os.environ.get('CONFD_URL')
    if not url:
        port = AssetLauncher.service_port(9487, 'confd')
        url = 'http://localhost:{port}'.format(port=port)
    return confd.MockConfd(url)


def setup_provd():
    host = os.environ.get('PROVD_HOST', 'localhost')
    port = os.environ.get('PROVD_PORT')
    if not port:
        port = AssetLauncher.service_port(8666, 'provd')
    url = "http://{host}:{port}/provd".format(host=host, port=port)
    client = new_provisioning_client(url)
    return provd.ProvdHelper(client)


def setup_docker(asset):
    if os.environ.get('DOCKER', '1') == '1':
        AssetLauncher.asset = asset
        cleanup_docker(asset)
        start_docker(asset)


def cleanup_docker(asset):
    if os.environ.get('DOCKER', '1') == '1':
        AssetLauncher.asset = asset
        AssetLauncher.pushd(os.path.join(ASSET_PATH, asset))
        AssetLauncher.kill_containers()
        AssetLauncher.rm_containers()


def start_docker(asset):
    AssetLauncher.pushd(os.path.join(ASSET_PATH, asset))
    AssetLauncher.start_containers(bootstrap_container='tests')
