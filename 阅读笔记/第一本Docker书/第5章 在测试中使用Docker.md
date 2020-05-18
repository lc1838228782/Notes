## 使用Docker测试静态网站

```dockerfile
FROM ubuntu:20.04
LABEL maintainer="lc@mail.com"
ENV REFRESHED_AT 2020-5-10

COPY sources.list /etc/apt/
RUN apt-get -qq update && apt-get -qq install nginx

RUN mkdir -p /var/www/html/website
ADD nginx/global.conf /etc/nginx/conf.d/
ADD nginx/nginx.conf /etc/nginx/

EXPOSE 80
```

NGINX前台运行。NGINX默认守护进程的方式启动，这会导致容器只是短暂运行，在守护进程被fork启动后，发起守护进程的原始进程就会退出，容器就停止运行了。?

```shell
$ sudo docker run -d -p 80 --name website \
  -v $PWD/website:/var/www/html/website:ro \
  nginx nginx
```

`-v`挂载宿主机文件到容器。

`ro`映射权限

volume绕过分层的联合文件系统，绕过镜像

## 使用Docker构建并测试Web应用程序

### Docker互联

- Docker的内部网络
- Docker Networking(Recommend)
- Docker链接

### Docker Networking

```shell
$ sudo docker network create app
$ sudo docker network inspect app
$ sudo docker network ls
$ sudo docker run -d --new=app --name db redis
$ sudo docker run -p 4567 \
  --net=app --name webapp -it \
  -v $PWN/webapp:/opt/webapp sinatra \
  /bin/bash
$ ping db
$ ping db.app # 均可到达
$ sudo docker network connect app db2 # 将已有的容器添加到app网络
$ sudo docker network disconnect app db2
```

一个容器可以属于多个网络，创建复杂的网络模型。

> 可以创建overlay网络，允许跨多台宿主机进行通信

### Docker链接

```shell
$ sudo docker run -d --name redis redis		# 启动一个Redis容器
$ sudo docker run -p 4567 \
  --name webapp --link redis:db -it \
  -v $PWD/webapp:/opt/webapp sinatra \
  /bin/bash
```

`--link`一个是要链接的容器名字，另一个是链接别名。

Redis没有使用`-p`暴露接口

在webapp设置hosts和环境变量。重启容器会自动更新

> 指定主机名`-h` `--hostname` 
>
> 添加hosts记录 例 `--add-host docker:10.0.0.1`，添加主机名称docker和地址10.0.0.1

通过名称(hosts)或者环境变量进行连接。

## 持续集成例子