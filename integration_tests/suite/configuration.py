config = {
    'base_url': 'http://dev'
}


def configure(**kwargs):
    global config
    config.update(kwargs)
