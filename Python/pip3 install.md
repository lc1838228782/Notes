```shell
sudo apt install python3-pip
pip3 install pip -U
pip3 config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
# or 
sudo python3 -m pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
```

```shell
# error：Traceback (most recent call last): File "/usr/bin/pip3", line 9, in <module> from pip import main ImportError: cannot import name 'main'
sudo python3 -m pip uninstall pip && sudo apt install python3-pip --reinstall
```

