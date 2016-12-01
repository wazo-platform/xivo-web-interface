Dockerfile for XiVO webi

## Install Docker

To install docker on Linux :

    curl -sL https://get.docker.io/ | sh

 or

     wget -qO- https://get.docker.io/ | sh

## Build

To build the image, simply invoke

    docker build -t wazo-webi github.com/wazo-pbx/xivo-web-interface

Or directly in the sources

    docker build -t wazo-webi .

## Usage

To run the container, do the following:

    docker run -d -p 80:80 -p 443:443 -v /conf/nginx:/etc/nginx/sites-enabled -v /conf/ssl:/usr/share/xivo-certs -v /conf/etc:/etc/xivo/web-interface -t wazo-webi


On interactive mode :

    docker run -p 80:80 -p 443:443 -v /conf/nginx:/etc/nginx/sites-enabled -v /conf/ssl:/usr/share/xivo-certs -v /conf/etc:/etc/xivo/web-interface -it wazo-webi bash

To launch.

    service php5-fpm start && nginx

## Infos

- Using docker version 1.5.0 (from get.docker.io) on ubuntu 14.04.
- If you want to using a simple webi to administrate docker use : https://github.com/crosbymichael/dockerui

To get the IP of your container use :

    docker ps -a
    docker inspect <container_id> | grep IPAddress | awk -F\" '{print $4}'
