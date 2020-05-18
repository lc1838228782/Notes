## 确保就绪

`sudo docker info`

## 运行第一个容器

`sudo docker run -i -t ubuntu /bin/bash`

`-i`持久化的标准输入，`-t`表示Docker要创建一个伪tty终端

> docker help run
>
> man docker-run 查看更多docker run的标志

ubuntu是docker公司提供的基础镜像

## 使用容器

`docker ps -a`

默认显示正在运行，`-a`显示所有

## container namig

`sudo docker run --name bob_the_container -it ubuntu /bin/bash`

创建名为bob_the_container的容器。

命名字符`[a-zA-Z0-9_.-]`

`docker rm`删除容器

## restart container

## attach container

`sudo docker attach bob_the_container`

获得shell

## daemon container

`sudo docker run --name daemon_dave -d ubuntu /bin/sh -c "while true; do echo hello world; sleep 1; done"`

## logs

`sudo docker logs name/ID`

`sudo docker logs -f name/ID`监控

`sudo docker logs --tail 10 name/ID`最后10行

`sudo docker logs --tail 0 -f name/ID` 监控最新

`sudo docker logs -ft name/ID` 加时间戳

## log driver

可以 在默认的json-file中配置，也可以单独为某容器配置

`sudo docker run --log-driver="syslog" --name daemon -d ubuntu /bin/sh -c "while true; do echo hello world; sleep 1; done"`

将所有日志输出到syslog，`docker logs`不输出内容。

也可以设置为none，禁用日志。

## view process in container

`sudo docker top name/ID`

## statistic info

`sudo docker stats name1/ID1 name2/ID2`

## running new processes in container

`sudo docker exec -d name/ID touch /etc/new_config_file`

创建shell

`sudo docker exec -it name/ID /bin/bash`

## stop container

`sudo docker stop name/ID`

`sudo docker kill name/ID` 快速停止

`sudo docker ps -n x` 显示最后x个容器，不论运行停止

## restart automatically

`sudo docker run --restart=always -name daemon -d ubuntu`

不论什么错误均会重启

`sudo docker run --restart=on-failure:5 -name daemon -d ubuntu`

在错误关闭五次内重启

## inspect container

`sudo docker inspect name/ID`

按格式输出

`sudo docker inspect --format='{{ .State.Running}}' name/ID`

格式支持完成的go语言模板

## remove container

`sudo docker rm name/ID`

删除所有

```shell
$ sudo docker rm `sudo docker ps -a -q`
```

`-q`表示只返回容器ID

