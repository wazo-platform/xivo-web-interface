import re

from flask import Flask, request, jsonify

app = Flask(__name__)

LOGS = []

RESPONSES = {}
PRESERVE = {}

DEFAULTS = {
    '/wizard': {'configured': True},
    '/devices': {'total': 0,
                 'items': []},
    '/voicemails': {'total': 0,
                    'items': []},
    '/cti_profiles': {'total': 4,
                      'items': [{u'id': 1,
                                 u'links': [{u'href': u'https://webi:9486/1.1/cti_profiles/1',
                                             u'rel': u'cti_profiles'}],
                                 u'name': u'Supervisor'},
                                {u'id': 2,
                                 u'links': [{u'href': u'https://webi:9486/1.1/cti_profiles/2',
                                             u'rel': u'cti_profiles'}],
                                 u'name': u'Agent'},
                                {u'id': 4,
                                 u'links': [{u'href': u'https://webi:9486/1.1/cti_profiles/4',
                                             u'rel': u'cti_profiles'}],
                                 u'name': u'Switchboard'},
                                {u'id': 3,
                                 u'links': [{u'href': u'https://webi:9486/1.1/cti_profiles/3',
                                             u'rel': u'cti_profiles'}],
                                 u'name': u'Client'}]
                      }
}


@app.after_request
def log_request(response):
    if not (request.path.startswith('/_logs') or
            request.path.startswith('/_responses')):
        req = request_log()
        rep = response_log(response)
        LOGS.append({'request': req, 'response': rep})
    return response


def request_log():
    path = request.path
    if path.startswith('/1.1'):
        path = path[4:]
    log = {'method': request.method,
           'path': path,
           'query': dict(request.args),
           'body': request.data,
           'headers': dict(request.headers)}
    return log


def response_log(response):
    return {'code': response.status_code,
            'data': response.data}


@app.route('/_logs', methods=['GET'])
def list_logs():
    return jsonify(logs=LOGS)


@app.route('/_logs', methods=['DELETE'])
def delete_logs():
    global LOGS
    LOGS = []
    return ''


@app.route('/_responses', methods=['POST'])
def add_response():
    response = request.get_json(force=True)
    query = tuple(sorted(response['query'].items())) if response['query'] else None
    key = (response['path'], query)
    if response.get('preserve', False):
        method = PRESERVE.setdefault(response['method'], dict())
        method[key] = (response['body'], response['code'])
    else:
        method = RESPONSES.setdefault(response['method'], dict())
        path = method.setdefault(key, [])
        path.append((response['body'], response['code']))
    return ''


@app.route('/_responses', methods=['DELETE'])
def delete_responses():
    global RESPONSES
    global PRESERVE
    RESPONSES = {}
    PRESERVE = {}
    return ''


@app.route('/_responses', methods=['GET'])
def get_responses():
    return jsonify(responses=RESPONSES)


@app.route('/1.1/<path:expected>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def respond(expected):
    expected = "/" + expected
    args = dict(request.args.iteritems())

    method = RESPONSES.get(request.method, {})
    for (path, query), responses in sorted(method.iteritems(), reverse=True):
        if len(responses) > 0 and re.match(path, expected):
            if query is None or dict(query) == args:
                    return responses.pop(0)

    method = PRESERVE.get(request.method, {})
    for (path, query), response in sorted(method.iteritems(), reverse=True):
        if re.match(path, expected):
            if query is None or dict(query) == args:
                return response

    for path, response in DEFAULTS.iteritems():
        if re.match(path, expected):
            return jsonify(response)

    error_msg = '["Confd mock has no response prepared for {} {}"]'
    return error_msg.format(request.method, expected), 400

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=9487, debug=True)
