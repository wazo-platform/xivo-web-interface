FROM wazopbx/xivo-web-interface

ADD ./src/ /usr/share/xivo-web-interface/

RUN mkdir -p /var/lib/xivo && \
    mkdir -p /var/log/xivo-web-interface && \
    touch /var/log/xivo-web-interface/error.log && \
    chown -R www-data:adm /var/log/xivo-web-interface
