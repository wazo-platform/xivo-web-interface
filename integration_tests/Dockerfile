FROM xivo/xivo-web-interface

ADD ./src/ /usr/share/xivo-web-interface/

RUN mkdir -p /var/lib/xivo && \
    mkdir -p /var/log/xivo-web-interface && \
    chown www-data:adm /var/log/xivo-web-interface && \
    echo "2015-01-01 00:00:00" > /var/lib/xivo/configured
