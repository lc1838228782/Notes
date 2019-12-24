# Ubuntu 配置

修改源为[清华大学源](https://mirrors6.tuna.tsinghua.edu.cn/help/ubuntu/)

安装[vscode](https://code.visualstudio.com/)

修改[pypi源](https://mirrors6.tuna.tsinghua.edu.cn/help/pypi/)

```shell
$ pip install pip -U
$ pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

```shell
# 如果您到 pip 默认源的网络连接较差，临时使用本镜像站来升级 pip：

$ pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pip -U
```

安装[pwn-tools](https://github.com/Gallopsled/pwntools)

```shell
$ sudo apt-get update
$ sudo apt-get install python3 python3-pip python3-dev git libssl-dev libffi-dev build-essential
$ sudo python3 -m pip install --upgrade pip
$ sudo python3 -m pip install --upgrade git+https://github.com/Gallopsled/pwntools.git@dev3
```

安装[LibcSearcher](https://github.com/lieanu/LibcSearcher)

```shell
$ git clone https://github.com/lieanu/LibcSearcher.git
$ cd LibcSearcher
$ python setup.py develop
```

