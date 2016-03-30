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
import os
import subprocess
import logging

import database
import confd
import provd

from xivo_provd_client import new_provisioning_client

from pages import Browser, Page

logger = logging.getLogger(__name__)

ASSET_PATH = os.path.join(os.path.dirname(__file__), '..', '..', 'assets')


def setup_db():
    user = os.environ.get('DB_USER', 'asterisk')
    password = os.environ.get('DB_PASSWORD', 'proformatique')
    host = os.environ.get('DB_HOST', 'localhost')
    port = os.environ.get('DB_PORT', 15433)
    db = os.environ.get('DB_NAME', 'asterisk')

    return database.DbHelper.build(user, password, host, port, db)


def setup_browser():
    virtual = os.environ.get('VIRTUAL_DISPLAY', '1') == '1'
    username = os.environ.get('WEBI_USERNAME', 'root')
    password = os.environ.get('WEBI_PASSWORD', 'proformatique')
    Page.CONFIG['base_url'] = os.environ.get('WEBI_URL', 'http://localhost:10080')
    return Browser(username, password, virtual)


def setup_confd():
    url = os.environ.get('CONFD_URL', 'http://localhost:19487')
    return confd.MockConfd(url)


def setup_provd():
    host = os.environ.get('PROVD_HOST', 'localhost')
    port = os.environ.get('PROVD_PORT', 8666)
    url = "http://{host}:{port}/provd".format(host=host, port=port)
    client = new_provisioning_client(url)
    return provd.ProvdHelper(client)


def setup_docker(asset):
    if os.environ.get('DOCKER', '1') == '1':
        cleanup_docker(asset)
        start_docker(asset)


def stop_docker(asset):
    path = os.path.join(ASSET_PATH, asset)
    os.chdir(path)
    run_cmd(('docker-compose', 'kill'))


def cleanup_docker(asset):
    if os.environ.get('DOCKER', '1') == '1':
        path = os.path.join(ASSET_PATH, asset)
        os.chdir(path)
        run_cmd(('docker-compose', 'kill'))
        run_cmd(('docker-compose', 'rm', '-f'))


def start_docker(asset):
    path = os.path.join(ASSET_PATH, asset)
    os.chdir(path)
    run_cmd(('docker-compose', 'run', '--rm', '--service-ports', 'tests'))


def run_cmd(cmd):
    process = subprocess.Popen(cmd,
                               stdout=subprocess.PIPE,
                               stderr=subprocess.STDOUT)
    out, _ = process.communicate()
    logger.info(out)
    return out
