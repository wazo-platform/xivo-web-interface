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

import threading
import kombu

from kombu.mixins import ConsumerMixin
from Queue import Queue, Empty


class Consumer(ConsumerMixin):

    _exchange = kombu.Exchange('xivo', type='topic')
    _routing_key = 'config.#'

    def __init__(self, connection, message_queue):
        self.connection = connection
        self._queue = kombu.Queue(exchange=self._exchange, routing_key=self._routing_key)
        self._received_messages = message_queue

    def get_consumers(self, Consumer, channel):
        return [Consumer(self._queue, callbacks=[self.on_message])]

    def on_message(self, body, message):
        self._received_messages.put_nowait(body)
        message.ack()

    def get_message(self):
        return self._received_messages.get()


class Bus(object):

    def __init__(self, bus_url):
        self._bus_url = bus_url
        self._messages = Queue()

    def start(self):
        self._start_listening_thread()

    def stop(self):
        self._consumer.should_stop = True
        self._bus_thread.join()
        while not self._messages.empty():
            self._messages.get_nowait()

    def assert_msg_received(self, msg_name, body):
        while not self._messages.empty():
            try:
                message = self._messages.get(timeout=10.0)
                if message['name'] == msg_name and message['data'] == body:
                    return
            except Empty:
                break
        assert False, '{} not received'.format(msg_name)

    def _start_listening_thread(self):
        self._bus_thread = threading.Thread(target=self._start_consuming)
        self._bus_thread.start()

    def _start_consuming(self):
        with kombu.Connection(self._bus_url) as conn:
            self._consumer = Consumer(conn, self._messages)
            self._consumer.run()
