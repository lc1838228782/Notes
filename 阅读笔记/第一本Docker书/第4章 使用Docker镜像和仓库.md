## 什么是docker镜像

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/docker fs layers.jpg)

`bootfs rootfs union mount copy on write`

## 列出镜像

`sudo docker images`

宿主机`/var/lib/docker`下存储了docker镜像，容器等

镜像从仓库下载下来，镜像保存在仓库中，而仓库保存在registry中，默认的registry为docker hub。

使用tag，一个仓库保存多个镜像。`sudo docker pull ubuntu:16.04`

用户仓库顶层仓库。

## 拉取，查找镜像

`sudo docker pull fedaro:20`

`sudo docker search keyword`

## 构建镜像

构建镜像有两种方式

- 使用docker commit（不推荐
- 使用Dockerfile和docker build

### 注册登录docker hub

```shell
# hub.docker.com signup

$ sudo docker login
```

### docker commit

`sudo docker commit ID username/repositoryname`

```shell
$ sudo docker commit -m"A new custom image" -a"lc" \
  ID username/repositoryname:tag
```

`-m`提交信息，`-a`作者信息

`sudo docker inspect username/repositoryname:tag`查看镜像详细信息

### 使用Dockerfile构建

Dockerfile使用基本的DSL(Domain Specific Language)。

示例

```shell
$ mkdir static_web
$ cd static_web
$ touch Dockerfile
```

Dockerfile

```dockerfile
# version: 0.0.1
FROM ubuntu:20.04
MAINTAINER C L "name@example.com"
RUN apt update && apt install -y nginx
RUN echo 'Hi, i am in your container' \
    >/usr/share/nginx/html/index.html
EXPOSE 80
```

指令必须大写，每条指令都会创建一个新的镜像层进行提交。

即是Dockerfile由于某条原因没有正常结束，那么用户还是得到一个可以使用的镜像。可以从镜像中运行失败命令，进行调试。

默认情况下，RUN指令会在shell中使用指令包装器`/bin/sh -c`

来执行，如果不希望在shell中运行或不支持shell则可以使用exec格式的`RUN`指令。如`RUN [ "apt", "install", "-y", "nginx" ]`

`EXPOSE`指令告诉Docker该容器内的应用程序将会使用指定端口。

### build

```shell
$ cd static_web
$ sudo docker build -t="username/repository:tag"
```

从git仓库构建

```
$ sudo docker build -t="username/repositoty:tag" \
  git@github.com:username/repository
```

`-f`指定任意名称的dockerfile

`.dockerignore`类似`.gitignore`。

### 构建缓存

哪一步发生修改，就从哪一步开始继续构建

### 构建过程

`sudo docker history name/ID`

### 端口

- 随机映射到宿主机端口

`sudo docker run -d -p 80 --name static_web username/static_web nginx -g "daemon off;"`

`-p`控制docker公开哪些端口给外部主机。

以上指令会随机映射(32768~61000)一个宿主机端口到容器的80

`docker ps`和`docker port name/ID port`可以查看映射情况。

- 指定端口

`sudo docker run -d -p 8080:80 --name static_web username/static_web nginx -g "daemon off"`

将容器中的80映射到宿主机8080

指定到某一地址的任意端口

`sudo docker run -d -p 127.0.0.1::80 --name static_web username/static_web nginx -g "daemon off"`



`-P`指令将所有EXPOSE指令的端口公开

### Dockerfile指令

- `CMD` 指定一条容器启动时运行的命令。命令行中指定的命令会将其替代。

  例`CMD ["/bin/bash", "-l"]`

- `ENTRYPOINT` 指定一条启动时运行的命令，`CMD`和命令行中的命令会当做参数传给`ENTRYPOINT`中的命令。(命令行中使用--entrypoint可覆盖)

- `WORKDIRE`指定命令的工作目录。命令行`-w`可改变。

- `ENV` 设置环境变量。可一次设置多个`ENV RVM_PATH=/home/rvm RVM_ARCHFLAGS="-arch i386"`

  `docker run`中设置，`-e WEB_PORT=8080`

- `USER`指定容器运行用户。 `USER user/uid:group/gid`

- `VOLUME` 示例`VOLUME ["/opt/project", "/data"]`

- `ADD` 将构建目录或URL中的文件，添加到镜像，归档文件会进行解压。自动创建目录0755，UID=0，GID-0。 1. `ADD`将会使后续构建缓存失效。 2. 通过路径结尾是否存在`/`判断目录或者文件

- `COPY`只复制不解压

- `LABEL` 示例：`LABEL key1="value1" key2="value2"`. `inspect`可进行查看。

- `STOPSIGNAL` 设置停止容器时，发送什么信号给容器可以是信号数值或者名称。

- `ARG` 传递构建运行时的变量。只有在Dockerfile定义的参数才可以。`--build-arg <variable>=<value>`

  ```
  ARG build
  ARG webapp_user=user
  ```

  可设置默认值。预定义的ARG变量`HTTP_PROXY http_proxy HTTPS_PROXY https_proxy FTP_PROXY ftp_proxy NO_PROXY no_proxy`

- `ONBUILD` 当一个镜像用作其他镜像的基础镜像时触发执行。子镜像可以触发，孙子镜像不可。inspect同样可以查看。几条指令不能用于`ONBUILD`，`FROM MAINTAINER ONBUILD`。避免递归调用。

## 推送到Docker hub

`sudo docker push username/repository`

## 自动构建

github bitbucket

## 删除镜像

`sudo docker rmi name/ID`

## 自建registry

使用docker registry镜像

使用新registry打标签

`sudo docker tag name/ID docker.example.com:5000/username/repository`

`sudo docker push docker.example.com:5000/username/repository`

`sudo docker run -it docker.example.com:5000/username/repository /bin/bash`

