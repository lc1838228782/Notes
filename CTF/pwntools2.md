常用的模块有下面几个：

- asm : 汇编与反汇编，支持x86/x64/arm/mips/powerpc等基本上所有的主流平台
- dynelf : 用于远程符号泄漏，需要提供leak方法
- elf : 对elf文件进行操作
- gdb : 配合gdb进行调试
- memleak : 用于内存泄漏
- shellcraft : shellcode的生成器
- tubes : 包括tubes.sock, tubes.process, tubes.ssh, tubes.serialtube，分别适用于不同场景的PIPE
- utils : 一些实用的小功能，例如CRC计算，cyclic pattern等

## tubes读写接口

无论哪种PIPE都是继承tube而来，可以用于读写函数主要有:

- interactive() : 直接进行交互，相当于回到shell的模式，在取得shell之后使用
- recv(numb=4096, timeout=default) : 接收指定字节
- recvall() : 一直接收直到EOF
- recvline(keepends=True) : 接收一行，keepends为是否保留行尾的\n
- recvuntil(delims, drop=False) : 一直读到delims的pattern出现为止
- recvrepeat(timeout=default) : 持续接受直到EOF或timeout
- send(data) : 发送数据
- sendline(data) : 发送一行数据，相当于在数据末尾加\n

## 汇编与反汇编

asm进行汇编

```shell
>>> asm('nop')'
\x90'
>>> asm('nop', arch='arm')
'\x00\xf0 \xe3'
```

可以使用context来指定cpu类型以及操作系统

```shell
>>> context.arch      = 'i386'
>>> context.os        = 'linux'
>>> context.endian    = 'little'
>>> context.word_size = 32
```

使用disasm进行反汇编

```shell
>>> print disasm('6a0258cd80ebf9'.decode('hex'))
   0:   6a 02                   push   0x2
   2:   58                      pop    eax
   3:   cd 80                   int    0x80
   5:   eb f9                   jmp    0x0
```

注意，asm需要binutils中的as工具辅助，如果是不同于本机平台的其他平台的汇编，例如在我的x86机器上进行mips的汇编就会出现as工具未找到的情况，这时候需要安装其他平台的cross-binutils。

## Shellcode 生成器

使用shellcraft可以生成对应的架构的shellcode代码，直接使用链式调用的方法就可以得到

```shell
>>> print shellcraft.i386.nop().strip('\n')
    nop
>>> print shellcraft.i386.linux.sh()
    /* push '/bin///sh\x00' */
    push 0x68
    push 0x732f2f2f
    push 0x6e69622f
...
```

## ELF

```shell
>>> e = ELF('/bin/cat')
>>> print hex(e.address)  # 文件装载的基地址
0x400000
>>> print hex(e.symbols['write']) # 函数地址
0x401680
>>> print hex(e.got['write']) # GOT表的地址
0x60b070
>>> print hex(e.plt['write']) # PLT的地址
0x401680
```

一些可用的函数

- asm(address, assembly) : 在指定地址进行汇编
- bss(offset) : 返回bss段的位置，offset是偏移值
- checksec() : 对elf进行一些安全保护检查，例如NX, PIE等。
- disasm(address, n_bytes) : 在指定位置进行n_bytes个字节的反汇编
- offset_to_vaddr(offset) : 将文件中的偏移offset转换成虚拟地址VMA
- vaddr_to_offset(address) : 与上面的函数作用相反
- read(address, count) : 在address(VMA)位置读取count个字节
- write(address, data) : 在address(VMA)位置写入data
- section(name) : dump出指定section的数据

## 好用的函数

`ljust(width, fillchar)`

