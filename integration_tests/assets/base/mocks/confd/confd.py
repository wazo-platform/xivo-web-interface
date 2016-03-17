import re

from flask import Flask, request, jsonify

app = Flask(__name__)

LOGS = []

RESPONSES = {}

DEFAULTS = {
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
    method = RESPONSES.setdefault(response['method'], {})
    path = method.setdefault(response['path'], [])
    path.append((response['body'], response['code']))
    return ''


@app.route('/_responses', methods=['DELETE'])
def delete_responses():
    global RESPONSES
    RESPONSES = {}
    return ''


@app.route('/_responses', methods=['GET'])
def get_responses():
    return jsonify(responses=RESPONSES)


@app.route('/1.1/<path:expected>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def respond(expected):
    expected = "/" + expected
    method = RESPONSES.get(request.method, {})
    for path, responses in method.iteritems():
        if len(responses) > 0 and re.match(path, expected):
            return responses.pop(0)
    for path, response in DEFAULTS.iteritems():
        if re.match(path, expected):
            return jsonify(response)
    return '["Confd mock has no response prepared"]', 400

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=9487, debug=True)
