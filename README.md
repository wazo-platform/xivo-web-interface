# Building with npm before testing in live

    npm i
    npm run build

See 
- devtemp/change_master.list for files changed since the original branch (1116-Refresh-web-interface)
- devtemp/change_new_webi.list for files changed from the original branch

    

# Running functional tests

You need Xephyr.

    apt-get install xserver-xephyr 

1. [XiVO acceptance](https://gitlab.com/xivo.solutions/xivo-acceptance)
2. ```
cd integration_tests
pip install -r test-requirements.txt
make test-setup MANAGE_DB_DIR=/path/to/xivo-manage-db
nosetests suite
```

To change the behaviour of the integration tests, you may configure the
following environment variables:

```
DB_USER (default: asterisk)
DB_PASSWORD (default: proformatique)
DB_HOST (default: localhost)
DB_PORT (default: 15432)
DB_NAME (default: asterisk)
VIRTUAL_DISPLAY (default: 1)
WEBI_URL (default: http://localhost:8080)
CONFD_URL (default: http://localhost:19487)
DOCKER (default: 1) enables the starting/stopping of docker containers for each
    test. May be useful when developing.
```


# To develop - using sshfs

Here's the process one can use to develop:
1. Clone projet
1. Boot xivo VM
1. One xivo VM, 'load' the git sources :
```
cd /usr/share
# save real files
mv xivo-web-interface/ xivo-web-interface.ori
# create dir
mkdir xivo-web-interface
# mount GIT
sshfs -o allow_other <MY_USER>@<MY_LAPTOP>:/path/to/GIT/xivo.solutions/xivo-web-interface/src xivo-web-interface
```

Then all changes made in git are on the xivo (you need to install sshfs on your laptop and the xivo).


# To develop - using nfs

On laptop:

1. Install nfs-kernel-server
1. Change owner of xivo-web-interface directory to nobody:nogroup
1. Add row to file /etc/exports

	/PATH_TO/xivo-web-interface/src  XIVO_IP(rw,sync,no_subtree_check)

1. sudo systemctl restart nfs-kernel-server


On XiVO:

mount -t nfs YOUR_IP:/PATH_TO/xivo-web-interface/src /usr/share/xivo-web-interface

Use IP address of the interface used for ssh connection.
