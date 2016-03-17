import uuid
import random
import dataset

import sqlalchemy as sa

from contextlib import contextmanager
from sqlalchemy.sql import text


class DbHelper(object):

    TEMPLATE = "xivotemplate"

    @classmethod
    def build(cls, user, password, host, port, db):
        tpl = "postgresql://{user}:{password}@{host}:{port}"
        uri = tpl.format(user=user,
                         password=password,
                         host=host,
                         port=port)
        return cls(uri, db)

    def __init__(self, uri, db):
        self.uri = uri
        self.db = db

    def create_engine(self, db=None, isolate=False):
        db = db or self.db
        uri = "{}/{}".format(self.uri, db)
        if isolate:
            return sa.create_engine(uri, isolation_level='AUTOCOMMIT')
        return sa.create_engine(uri)

    def connect(self, db=None):
        db = db or self.db
        return self.create_engine(db).connect()

    def recreate(self):
        engine = self.create_engine("postgres", isolate=True)
        connection = engine.connect()
        connection.execute("""
                           SELECT pg_terminate_backend(pg_stat_activity.pid)
                           FROM pg_stat_activity
                           WHERE pg_stat_activity.datname = '{db}'
                           AND pid <> pg_backend_pid()
                           """.format(db=self.db))
        connection.execute("DROP DATABASE IF EXISTS {db}".format(db=self.db))
        connection.execute("CREATE DATABASE {db} TEMPLATE {template}".format(db=self.db,
                                                                             template=self.TEMPLATE))
        connection.close()

    def execute(self, query, **kwargs):
        with self.create_engine().connect() as connection:
            connection.execute(text(query), **kwargs)

    @contextmanager
    def queries(self):
        db = dataset.connect("{}/{}".format(self.uri, self.db))
        db.begin()
        yield DatabaseQueries(db)
        db.commit()


class DatabaseQueries(object):

    def __init__(self, db):
        self.db = db

    def insert_user(self, firstname, lastname=None):
        template_id = self.insert_func_key_template(private=True)
        entity_id = self.db['entity'].find_one()['id']
        caller_name = " ".join([firstname, lastname or ""]).strip()
        user = {'firstname': firstname,
                'lastname': lastname or "",
                'uuid': str(uuid.uuid4()),
                'description': '',
                'func_key_private_template_id': template_id,
                'callerid': '"{}"'.format(caller_name),
                'entityid': entity_id}

        user_id = self.db['userfeatures'].insert(user)

        for event in ('noanswer', 'busy', 'congestion', 'chanunavail'):
            self.insert_dialaction(event, 'user', str(user_id))

        return user_id

    def insert_queue(self, name='myqueue', number='3000', context='default'):
        queue = dict(name=name,
                     displayname=name,
                     number=number,
                     context=context)

        queue_id = self.db['queuefeatures'].insert(queue)

        self.insert_extension(number, context, 'queue', queue_id)
        self.insert_destination('queue', 'queue_id', queue_id)

        return queue_id

    def insert_func_key_template(self, name=None, private=False):
        template = {'name': name, 'private': private}
        return self.db['func_key_template'].insert(template)

    def insert_dialaction(self, event, category, categoryval, action='none', linked=1):
        dialaction = dict(event=event,
                          category=category,
                          categoryval=categoryval,
                          action=action,
                          linked=linked)
        return self.db['dialaction'].insert(dialaction)

    def insert_func_key(self, func_key_type, destination_type):
        fk_type = self.db['func_key_type'].find_one(name=func_key_type)['id']
        fk_dest = self.db['func_key_destination_type'].find_one(name=destination_type)['id']
        fk = {'type_id': fk_type, 'destination_type_id': fk_dest}
        return self.db['func_key'].insert(fk)

    def insert_destination(self, table, column, destination_id, fk_type='speeddial', dest_type=None):
        func_key_id = self.insert_func_key(fk_type, dest_type or table)
        table = "func_key_dest_{}".format(table)
        dest = {'func_key_id': func_key_id,
                column: destination_id}
        return self.db[table].insert(dest)

    def insert_conference(self, name='myconf', number='2000', context='default'):
        conf = dict(meetmeid=1234,
                    name=name,
                    confno=number,
                    context=context,
                    admin_identification='pin',
                    admin_mode='all',
                    admin_announcejoinleave='no',
                    user_mode='all',
                    user_announcejoinleave='no',
                    emailbody='email',
                    description='')
        conference_id = self.db['meetmefeatures'].insert(conf)

        self.insert_extension(number, context, 'meetme', conference_id)
        self.insert_destination('conference', 'conference_id', conference_id)

        return conference_id

    def insert_extension(self, exten, context, type_, typeval):
        extension = {"context": context,
                     "exten": exten,
                     "type": type_,
                     "typeval": str(typeval)}
        return self.db['extensions'].insert(extension)

    def insert_group(self, name='mygroup', number='1234', context='default'):
        group = dict(name=name,
                     number=number,
                     context=context)
        group_id = self.db['groupfeatures'].insert(group)

        self.insert_extension(number, context, 'group', group_id)

        for event in ('noanswer', 'busy', 'congestion', 'chanunavail'):
            self.insert_dialaction(event, 'group', str(group_id))

        self.insert_destination('group', 'group_id', group_id)

        return group_id

    def insert_agent(self, number='1000', context='default', firstname=None, lastname=None):
        numgroup = self.db['agentgroup'].find_one(name=context)['groupid']
        agent = {'number': number,
                 'context': context,
                 'firstname': firstname,
                 'lastname': lastname,
                 'numgroup': numgroup,
                 'passwd': '',
                 'language': '',
                 'description': ''}
        agent_id = self.db['agentfeatures'].insert(agent)

        for typeval in ('agentstaticlogin', 'agentstaticlogoff', 'agentstaticlogtoggle'):
            extension_id = self.db['extension'].find_one(typeval=typeval)['id']
            func_key_id = self.insert_func_key('speeddial', 'agent')
            func_key = {'func_key_id': func_key_id,
                        'agent_id': agent_id,
                        'extension_id': extension_id}
            self.db['func_key_dest_agent'].insert(func_key)

    def insert_paging(self, number='1234'):
        paging = {"number": number, "timeout": 30}
        paging_id = self.db['paging'].insert(paging)

        self.insert_destination('paging', 'paging_id', paging_id)

        return paging_id

    def insert_callfilter(self, name='bsfilter', type_='bosssecretary', bosssecretary='secretary-simult'):
        callfilter = dict(name=name,
                          type=type_,
                          bosssecretary=bosssecretary)
        return self.db['callfilter'].insert(callfilter)

    def insert_filter_member(self, callfilter_id, member_id, bstype='secretary'):
        member = dict(callfilterid=callfilter_id,
                      type='user',
                      typeval=str(member_id),
                      bstype=bstype)
        member_id = self.dict['callfiltermember'].insert(member)

        self.insert_destination('bsfilter', 'filtermember_id', member_id)

        return member_id

    def associate_line_device(self, line_id, device_id):
        self.db['linefeatures'].update({'device': device_id, 'id': line_id}, ['id'])

    def dissociate_line_device(self, line_id, device_id):
        self.db['linefeatures'].update({'device': None, 'id': line_id}, ['id'])

    def add_func_key_to_user(self, position, func_key_id, user_id, label=None, blf=True):
        template_id = self.db['userfeatures'].find_one(id=user_id)['func_key_private_template_id']
        dest_id = self.db['func_key'].find_one(id=func_key_id)['destination_type_id']
        mapping = dict(position=position,
                       func_key_id=func_key_id,
                       user_id=user_id,
                       label=label,
                       blf=blf,
                       template_id=template_id,
                       destination_type_id=dest_id)
        return self.db['func_key_mapping'].insert(mapping)

    def insert_sip_line(self, sip, line_extra=None):
        query = text("""
                     INSERT INTO usersip (name, secret, context, type, category)
                     VALUES (:name, :secret, :context, :type, :category)
                     RETURNING id
                     """)
        result = self.db.query(query,
                               name=sip['username'],
                               secret=sip.get('secret', 'secret'),
                               context=sip.get('context', 'default'),
                               type=sip.get('type', 'friend'),
                               category=sip.get('category', 'user'))
        sip_id = next(result)['id']

        line = {'context': sip.get('context', 'default'),
                'protocol': 'sip',
                'protocolid': sip_id}
        line.update(line_extra or {})
        return self.insert_line(**line)

    def insert_line(self, **params):
        provisioningid = random.randint(100000, 999999)
        line = dict(protocol=None,
                    protocolid=None,
                    name=None,
                    configregistrar='default',
                    context='default',
                    provisioningid=provisioningid,
                    ipfrom='127.0.0.1')
        line.update(params)
        return self.db['linefeatures'].insert(line)

