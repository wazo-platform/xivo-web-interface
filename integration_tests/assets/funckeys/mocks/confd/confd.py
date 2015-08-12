import re

from flask import Flask, request, jsonify

app = Flask(__name__)

REQUESTS = []

RESPONSES = {}

PROFILES = [{u'id': 1,
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


@app.before_request
def log_request():
    if not (request.path.startswith('/_requests') or
            request.path.startswith('/_responses')):
        path = request.path
        if path.startswith('/1.1'):
            path = path[4:]
        log = {'method': request.method,
               'path': path,
               'query': dict(request.args),
               'body': request.data,
               'headers': dict(request.headers)}
        REQUESTS.append(log)


@app.route('/_requests', methods=['GET'])
def list_requests():
    return jsonify(requests=REQUESTS)


@app.route('/_requests', methods=['DELETE'])
def delete_requests():
    global REQUESTS
    REQUESTS = []
    return ''


@app.route('/_responses', methods=['POST'])
def add_response():
    response = request.get_json(force=True)
    method = RESPONSES.setdefault(response['method'], {})
    regex = re.compile(response['path'])
    method[regex] = (response['body'], response['code'])
    return ''


@app.route('/_responses', methods=['DELETE'])
def delete_responses():
    global RESPONSES
    RESPONSES = {}
    return ''


@app.route('/1.1/devices')
def devices():
    return jsonify(total=0,
                   items={})


@app.route('/1.1/cti_profiles')
def cti_profiles():
    return jsonify(total=len(PROFILES),
                   items=PROFILES)


@app.route('/1.1/<path:path>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def respond(path):
    path = "/" + path
    method = RESPONSES.get(request.method, {})
    for regex, response in method.iteritems():
        if regex.match(path):
            return response
    return '["Confd mock has no response prepared"]', 400

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=9487, debug=True)
