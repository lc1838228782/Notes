## 安装

ubuntu 20.04

`$ sudo  apt install docker.io`

[install docker engine on ubuntu doc](https://docs.docker.com/engine/install/ubuntu/)

docker hub mirror

`/etc/docker/daemon.json`

```
{
  "registry-mirrors": ["https://hub-mirror.c.163.com/"]
}
```



### docker与ufw

设置`/etc/default/ufw`的

`DEFAULT_FORWARD_POLICY="DROP"`为`ACCEPT`

后重新加载`sudo ufw reload`

？

## docker守护进程

docker用户组。

修改守护进程网络`sudo docker daemon -H tcp://0.0.0.0:2375`

docker客户端不会自己修改，需指定`docker -H :port`

可以设置`DOCKER_HOST`环境变量省略此步骤

`export DOCKER_PORT="tcp://0.0.0.0:2375"`



`sudo docker daemon -H unix://home/docker/docker.sock`指定非默认套接字



`sudo docker daemon -H tcp://0.0.0.0:2375 -H unix://home/docker/docker.sock`绑定多个地址



`$ sudo docker daemon -D`开启守护进程调试模式



永久改动需修改启动配置，Ubuntu，编辑`/etc/default/docker`，修改`DOCKER_OPTS`变量。



