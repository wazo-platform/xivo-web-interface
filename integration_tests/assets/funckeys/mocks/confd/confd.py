from flask import Flask, jsonify

app = Flask(__name__)

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


@app.route('/1.1/devices')
def devices():
    return jsonify(total=0,
                   items={})


@app.route('/1.1/cti_profiles')
def cti_profiles():
    return jsonify(total=len(PROFILES),
                   items=PROFILES)


@app.route('/1.1/<path:path>', methods=['GET', 'POST', 'PUT', 'DELETE'])
def fallback(path):
    return jsonify()


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=9487)
