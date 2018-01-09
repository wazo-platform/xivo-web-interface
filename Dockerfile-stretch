FROM debian:stretch
MAINTAINER Wazo Maintainers <dev.wazo@gmail.com>

ENV HOME /root
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -qq && \
    apt-get install --yes \
        nginx \
        php-fpm \
        php-amqplib \
        php-imagick \
        php-imap \
        php-mcrypt \
        php-curl \
        php-cli \
        php-gd \
        php-pgsql \
        php-common \
        php-json \
        curl unzip && \
    apt-get clean

#Install javascript deps
WORKDIR /usr/src

COPY src /usr/share/xivo-web-interface
COPY etc/xivo/web-interface /etc/xivo/web-interface
COPY contribs/docker/nginx-webi /etc/nginx/sites-available/default

RUN mkdir -p /var/lib/xivo && \
    mkdir -p /var/log/xivo-web-interface && \
    touch /var/log/xivo-web-interface/error.log && \
    chown -R www-data:adm /var/log/xivo-web-interface && \
    mkdir /usr/share/xivo/ && \
    echo "docker-version" > /usr/share/xivo/XIVO-VERSION && \
    ln -s /etc/xivo/web-interface/php.ini /etc/php/7.0/fpm/conf.d/xivo.ini && \
    echo "daemon off;" >> /etc/nginx/nginx.conf

WORKDIR /root

EXPOSE 80
EXPOSE 443

CMD service php7.0-fpm start && nginx
