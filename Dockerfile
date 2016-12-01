FROM debian:jessie
MAINTAINER Wazo Maintainers <dev.wazo@gmail.com>

ENV HOME /root
ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -qq && \
    apt-get install --yes \
        nginx \
        php5-fpm \
        php-apc \
        php-amqplib \
        php5-imagick \
        php5-imap \
        php5-mcrypt \
        php5-curl \
        php5-cli \
        php5-gd \
        php5-pgsql \
        php5-common \
        php5-json \
        curl unzip && \
    apt-get clean

#Install javascript deps
WORKDIR /usr/src

COPY ./src /usr/share/xivo-web-interface
COPY ./etc/xivo/web-interface /etc/xivo/web-interface
COPY ./contribs/docker/nginx-webi /etc/nginx/sites-available/default

RUN mkdir /usr/share/xivo/ && \
    echo "docker-version" > /usr/share/xivo/XIVO-VERSION && \
    ln -s /etc/xivo/web-interface/php.ini /etc/php5/fpm/conf.d/xivo.ini && \
    echo "daemon off;" >> /etc/nginx/nginx.conf

WORKDIR /root

EXPOSE 80
EXPOSE 443

CMD service php5-fpm start && nginx
