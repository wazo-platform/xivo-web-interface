# -*- coding: utf-8 -*-

# Copyright (C) 2016 Avencall
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>

from flask import Flask, request, jsonify

app = Flask(__name__)

DEFAULT = {
    "data": {
    "xivo_user_uuid": None,
    "expires_at": "2016-09-26T11:14:27.031225",
    "token": "4305e190-70c8-4752-a950-e8401f648994",
    "acls": [
        "confd.#",
        "dird.tenants.#"
    ],
    "issued_at": "2016-09-26T10:14:27.031253",
    "auth_id": "1"}}


@app.route('/0.1/token', methods=['POST'])
def token_create():
    return jsonify(DEFAULT)

@app.route('/0.1/token/<string:token>', methods=['GET', 'DELETE'])
def token(token):
    if request.method == 'GET':
        return jsonify(DEFAULT)
    else:
        return '', 204



if __name__ == "__main__":
    app.run(host="0.0.0.0", port=9497, debug=True)
