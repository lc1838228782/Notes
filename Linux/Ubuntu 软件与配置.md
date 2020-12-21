# Ubuntu 配置

修改apt源为[清华大学源](https://mirrors6.tuna.tsinghua.edu.cn/help/ubuntu/)

## 安装[vscode](https://code.visualstudio.com/)

```shell
$sudo apt install ./code
```

[GNU global](https://blog.jaycetyle.com/2018/10/vscode-gnu-global/)

## 修改[pypi源](https://mirrors6.tuna.tsinghua.edu.cn/help/pypi/)

```shell
$ pip install pip -U
$ pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

```shell
# 如果您到 pip 默认源的网络连接较差，临时使用本镜像站来升级 pip：

$ pip install -i https://pypi.tuna.tsinghua.edu.cn/simple pip -U
```

## 安装[TLDR](https://tldr.sh/)

latest: [nvm](https://github.com/nvm-sh/nvm#installing-and-updating)

ali mirrors: https://npm.taobao.org/mirrors

```shell
# install nvm
$ curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.35.3/install.sh | bash
# setting inland nvm mirror
# append to .bash_profile, and `source .bash_profile`, 
# also you can using .bashrc file as well
# NVM_NODEJS_ORG_MIRROR=http://npm.taobao.org/mirrors/node
$ nvm install node
$ npm config set registry https://registry.npm.taobao.org
$ npm install -g tldr
$ tldr --update
```

https://docs.npmjs.com/downloading-and-installing-node-js-and-npm#using-a-node-version-manager-to-install-node-js-and-npm

## 安装其他

```shell
$ sudo apt install fd-find
$ sudo ln -s /usr/bin/fdfind /usr/bin/fd # ubuntu 20.04
$ sudo apt install shellcheck
$ sudo apt install ripgrep
$ sudo apt install feh
$ sudo apt install htop
$ sudo apt install global
```

# sec tools

## 安装[pwntools](https://github.com/Gallopsled/pwntools)

```shell
$ sudo apt-get update
$ sudo apt-get install python3 python3-pip python3-dev git libssl-dev libffi-dev build-essential
$ sudo python3 -m pip install --upgrade pip
$ sudo python3 -m pip install --upgrade git+https://github.com/Gallopsled/pwntools.git@dev3
```

## 安装[LibcSearcher](https://github.com/lieanu/LibcSearcher)

```shell
$ git clone https://github.com/lieanu/LibcSearcher.git
$ cd LibcSearcher
$ python setup.py develop
$ 
```

https://github.com/lieanu/LibcSearcher

https://github.com/niklasb/libc-database

## pwndbg

https://github.com/pwndbg/pwndbg

## one_gadget

https://github.com/david942j/one_gadget

```shell
# 添加 TUNA 源并移除默认源
$ gem sources --add https://mirrors.tuna.tsinghua.edu.cn/rubygems/ --remove https://rubygems.org/
# 列出已有源
$ gem sources -l
# 应该只有 TUNA 一个
$ gem install one_gadget
```

## ropper

[ropper](https://github.com/sashs/Ropper)

```shell
$ pip3 install capstone filebytes keystone-engine pyvex ropper 
```

