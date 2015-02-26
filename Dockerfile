## Image to build from sources
FROM octohost/php5

MAINTAINER XiVO Team "dev@avencall.com"
ENV HOME /root

ADD ./src/ /usr/share/xivo-web-interface/
ADD ./etc/xivo/web-interface/php.ini /etc/php5/fpm/conf.d/xivo.ini
RUN mkdir -p /etc/xivo/web-interface/
RUN ln -s /etc/xivo/web-interface/datastorage.conf /etc/xivo
RUN mkdir /usr/share/xivo/
RUN echo "docker-version" > /usr/share/xivo/XIVO-VERSION

WORKDIR /root

EXPOSE 80
EXPOSE 443

CMD service php5-fpm start && nginx
