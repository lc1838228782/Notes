## 构建第一个应用(volume 示例)

- 卷可以在容器间共享与重用
- 共享卷不一定要运行相应的容器
- 对卷的修改直接在卷上反映出
- 更新镜像不会包含对卷的修改

卷在宿主机`/var/lib/docker/volumes`目录中，`docker inspect`查看具体位置

jekyll

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lchhdx@qq.com"
ENV REFRESHED_AT 2020-5-11
ENV DEBIAN_FRONTEND=noninteractive

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install ruby ruby-dev libffi-dev build-essential nodejs
RUN gem install --no-document jekyll -v 2.5.3

VOLUME /data
VOLUME /var/www/html
WORKDIR /data

ENTRYPOINT [ "jekyll", "build", "--destination=/var/www/html" ] 
```

apache

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lchhdx@qq.com"

ENV DEBIAN_FRONTEND=noninteractive

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install apache2

VOLUME [ "/var/www/html" ]
WORKDIR /var/www/html

ENV APACHE_RUN_USER www-data
ENV APACHE_RUN_GROUP www-data
ENV APACHE_LOG_DIR /var/log/apache2
ENV APACHE_PID_FILE /var/run/apache2.pid
ENV APACHE_RUN_DIR /var/run/apache2
ENV APACHE_LOCK_DIR /var/lock/apache2

RUN mkdir -p ${APACHE_RUN_DIR} ${APACHE_LOCK_DIR} ${APACHE_LOG_DIR}

EXPOSE 80

ENTRYPOINT [ "/usr/sbin/apachectl" ]
CMD [ "-D", "FOREGROUND" ]
```

创建jekyll容器，build出静态页面

```shell
$ sudo docker run -v /path/to/source:/data/ \
  --name james_blog jekyll
```

创建Apache容器

```shell
$ sudo docker run -d -P --volumes-from james_blog apache
```

`--volumes-from`把james_blog中的卷都加入到新创建的容器。

> 即是删除了使用了卷的最后一个容器，卷中的数据也会持久保存。

备份卷。`--rm`创建一次性使用的容器，结束即删除。

```shell
$ sudo docker run --rm --volumes-from jame_blog \
  -v %(PWD):/backup ubuntu \
  tar cvf /backup/james_blog_backup.tar /var/www/html
```

## 使用Docker构建一个Java应用服务

fetcher

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lchhdx@qq.com"
ENV REFRESHED_AT 2020-5-11

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install wget

VOLUME [ "/var/lib/tomcat9/webapps" ]
WORKDIR /var/lib/tomcat9/webapps/

ENTRYPOINT [ "wget" ]
CMD [ "--help" ]
```

tomcat9

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lchhdx@qq.com"
ENV REFRESHED_AT 2020-5-11

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install tomcat9 default-jdk

ENV CATALINA_HOME /usr/share/tomcat9
ENV CATALINA_BASE /var/lib/tomcat9
ENV CATALINA_PID /var/run/tomcat9.CATALINA_PID
ENV CATALINA_SH /usr/share/tomcat9/bin/catalina.sh
ENV CATALINA_TMPDIR /tmp/tomcat9-tomcat9-tmp

RUN mkdir -p ${CATALINA_TMPDIR}

VOLUME [ "/var/lib/tomcat9/webapps/" ]

EXPOSE 8080

ENTRYPOINT [ "/usr/share/tomcat9/bin/catalina.sh", "run" ]
```

## 多容器的应用栈

nodejs

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lchhdx@qq.com"
ENV REFRESHED_AT 2020-05-11
ENV DEBIAN_FRONTEND=noninteractive

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install nodejs npm
RUN mkdir -p /var/log/nodeapp

ADD nodeapp /opt/nodeapp/

WORKDIR /opt/nodeapp
RUN npm install

VOLUME [ "/var/log/nodeapp" ]

EXPOSE 3000

ENTRYPOINT [ "nodejs", "server.js" ]
```

redis_base

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lchhdx@qq.com"
ENV REFRESHED_AT 2020-05-11

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install software-properties-common redis-server redis-tools

VOLUME [ "/var/lib/redis", "/var/log/redis" ]

EXPOSE 6379

CMD []
```

redis_primary

```dockerfile
FROM redis
LABEL maintainer="lc"
ENV REFRESHED_AT 2020-05-11

ENTRYPOINT [ "redis-server", "--protected-mode no", "--logfile /var/log/redis/redis-server.log" ]
```

redis_replica

```dockerfile
FROM redis
LABEL maintainer="lc"
ENV REFRESHED_AT 2020-05-11

ENTRYPOINT [ "redis-server", "--protected-mode no", "--logfile /var/log/redis/redis-replica.log", "--slaveof redis_primary 6379" ]
```

`-h`指定主机名称，默认设置为容器ID。

## manage container

`sudo docker exec -it name/ID /bin/bash`

