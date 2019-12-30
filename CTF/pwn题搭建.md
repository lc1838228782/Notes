## 本地搭建

```
socat tcp-l:6666,fork exec:./pwn,reuseaddr
```

### 测试

```
$nc 0.0.0.0 6666lsflagpwn
```

也可以加个nohup，还可以再加个&

```
nohup socat tcp-l:6666,fork exec:./pwn,reuseaddr
```



```
nohup socat tcp-l:6666,fork exec:./pwn,reuseaddr&
```

他们的作用分别是
使用&后台运行程序：

- 结果会输出到终端
- 使用Ctrl + C发送SIGINT信号，程序免疫
- 关闭session发送SIGHUP信号，程序关闭

使用nohup运行程序：

- 结果默认会输出到nohup.out
- 使用Ctrl + C发送SIGINT信号，程序关闭
- 关闭session发送SIGHUP信号，程序免疫

## dockr搭建pwn题

开源项目地址: https://github.com/Eadom/ctf_xinetd
克隆下来之后，在当前目录下执行如下命令

### 建造镜像

```
docker build -t pwn-docker:16.04 .
```

若想搭建为18.04的环境，将dockerfile文件里面FROM ubuntu:16.04改为 FROM ubuntu:18.04

```
FROM ubuntu:16.04  #修改的地方RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn/g" /etc/apt/sources.list && \    apt-get update && apt-get -y dist-upgrade && \    apt-get install -y lib32z1 xinetd && apt-get install -y vimRUN useradd -m ctfWORKDIR /home/ctfRUN cp -R /lib* /home/ctf && \    cp -R /usr/lib* /home/ctfRUN mkdir /home/ctf/dev && \    mknod /home/ctf/dev/null c 1 3 && \    mknod /home/ctf/dev/zero c 1 5 && \    mknod /home/ctf/dev/random c 1 8 && \    mknod /home/ctf/dev/urandom c 1 9 && \    chmod 666 /home/ctf/dev/*RUN mkdir /home/ctf/bin && \    cp /bin/sh /home/ctf/bin && \    cp /bin/ls /home/ctf/bin && \    cp /bin/cat /home/ctf/binCOPY ./ctf.xinetd /etc/xinetd.d/ctfCOPY ./start.sh /start.shRUN echo "Blocked by ctf_xinetd" > /etc/banner_failRUN chmod +x /start.shCOPY ./bin/ /home/ctf/RUN chown -R root:ctf /home/ctf && \    chmod -R 750 /home/ctf && \    chmod 740 /home/ctf/flagCMD ["/start.sh"]EXPOSE 9999
```



### 然后创建容器，并做端口转发

```
docker run -p 0.0.0.0:9997:9999 pwn-docker:16.04
```

最后在物理机上访问9997端口,测试成功

```
$nc 0.0.0.0 9997lsbindevflagliblib32lib64pwncat flagctf{please_initialize_this_flag}
```



### 更换题目

可用docker cp命令替换/home/ctf目录下的helloworld，并修改/etc/xinetd.d/ctf里面的题目名称

```
service ctf{    disable = no    socket_type = stream    protocol    = tcp    wait        = no    user        = root    type        = UNLISTED    port        = 9999    bind        = 0.0.0.0    server      = /usr/sbin/chroot    # replace helloworld to your program    server_args = --userspec=1000:1000 /home/ctf ./helloworld  #修改的地方    banner_fail = /etc/banner_fail    # safety options    per_source	= 10 # the maximum instances of this service per source IP address    rlimit_cpu	= 20 # the maximum number of CPU seconds that the service may use    #rlimit_as  = 1024M # the Address Space resource limit for the service    #access_times = 2:00-9:00 12:00-24:00}
```



## 同时搭建多题

开源项目地址： https://github.com/giantbranch/pwn_deploy_chroot

### 准备工作

```
# 安装dockercurl -s https://get.docker.com/ | sh  #已安装过的跳过# 安装 docker compose 和gitapt install docker-compose git# 下载git clone https://github.com/giantbranch/pwn_deploy_chroot.git
```

克隆下来之后，只需三步就可以搭建多道pwn题，并自动生成flag，获取到shell自动cat flag

- 将所有pwn题目放入bin目录（注意名字不带特殊字符，因为会将文件名作为linux用户名）
- python initialize.py
- docker-compose up –build -d

```
$python initialize.py {"flag": "flag{9c900d02-8567-4e16-8952-47a92d2dc06a}", "port": 10000, "filename": "pwn1"}{"flag": "flag{6f785f9e-fb19-4b54-9c6c-dd9b6f62041a}", "port": 10001, "filename": "pwn1_copy1"}{"flag": "flag{c65a3be4-a9d1-4122-b981-7a241ca73027}", "port": 10002, "filename": "pwn1_copy2"}
docker-compose up --build -d
```

测试

```
$nc 0.0.0.0 10000pwn test
```



### 更换题目

先把开启的容器删除掉，再执行上面三步即可。