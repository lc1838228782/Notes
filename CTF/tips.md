## Reopen stdout

https://stackoverflow.com/questions/30968734/reopen-stdout-and-stderr-after-closing-them

I am running this on an Ubuntu machine, so I am not sure if it'll work for you, but this is what I did:

```sh
$ exec 1>&0
$ exec 2>&0
```

Suddenly, I had STDOUT and STDERR reconnected. Magic!

**Explanation:** Running the following commands, we get the following output:

```sh
$ ls -l /dev/stdout
lrwxrwxrwx 1 root root 15 Jun 11 23:39 /dev/stdout -> /proc/self/fd/1

$ ls -l /proc/self/fd/1
lrwx------ 1 jay jay 64 Jun 22 01:34 /proc/self/fd/1 -> /dev/pts/10

$ ls -l /proc/self/fd/
total 0
lrwx------ 1 jay jay 64 Jun 22 01:35 0 -> /dev/pts/10
lrwx------ 1 jay jay 64 Jun 22 01:35 1 -> /dev/pts/10
lrwx------ 1 jay jay 64 Jun 22 01:35 2 -> /dev/pts/10
lr-x------ 1 jay jay 64 Jun 22 01:35 3 -> /proc/12224/fd
```

Since all three fd's point to the same thing, we can return them back to normal just by pointing to /dev/pts/10 which the `exec 1>&0` and `exec 2>&0` do

## x64 参数寄存器传参顺序

**x64**

- System V AMD64 ABI (Linux、FreeBSD、macOS 等采用) 中前六个整型或指针参数依次保存在 **RDI, RSI, RDX, RCX, R8 和 R9 寄存器**中，如果还有更多的参数的话才会保存在栈上。
- 内存地址不能大于 0x00007FFFFFFFFFFF，**6 个字节长度**，否则会抛出异常。**?**

## 建立本地环境

接下来我们把这个目标程序作为一个服务绑定到服务器的某个端口上，这里我们可以使用socat这个工具来完成，命令如下：

```shell
#!bash
socat TCP4-LISTEN:10001,fork EXEC:./level1
```

随后这个程序的IO就被重定向到10001这个端口上了，并且可以使用 nc 127.0.0.1 10001来访问我们的目标程序服务了。

## jz, je

`CMP` **subtracts** the operands and sets the flags. Namely, it sets the zero flag if the difference is zero (operands are equal).

`TEST` sets the zero flag, `ZF`, when the result of the **AND** operation is zero. If two operands are equal, their bitwise AND is zero when both are zero. `TEST` also sets the sign flag, `SF`, when the most significant bit is set in the result, and the parity flag, `PF`, when the number of set bits is even.

`JE` [Jump if Equals] tests the zero flag and jumps if the flag is set. `JE` is an alias of `JZ` [Jump if Zero] so the disassembler cannot select one based on the opcode. `JE` is named such because the zero flag is set if the arguments to `CMP` are equal.

So,

```assembly
TEST %eax, %eax
JE   400e77 <phase_1+0x23>
```

jumps if the `%eax` is zero.

## 为什么ldd，info sharedlibrary显示的地址不一样？

`ldd`

因为一些原因，ldd并不意味着得到准确的地址，使用`LD_TRACE_LOADED_OBJECTS` 

```shell
$ LD_TRACE_LOADED_OBJECTS=1 /bin/bash | grep libc
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f44dae1b000)
$ LD_TRACE_LOADED_OBJECTS=1 /bin/bash | grep libc
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f9b35341000)
$ LD_TRACE_LOADED_OBJECTS=1 /bin/bash | grep libc
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fef18efd000)
$ echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
0
$ LD_TRACE_LOADED_OBJECTS=1 /bin/bash | grep libc
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ffff75e7000)
$ LD_TRACE_LOADED_OBJECTS=1 /bin/bash | grep libc
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ffff75e7000)
$ LD_TRACE_LOADED_OBJECTS=1 /bin/bash | grep libc
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007ffff75e7000)
```

`info sharedlibrary` 显示的是.text段地址



https://reverseengineering.stackexchange.com/questions/6657/why-does-ldd-and-gdb-info-sharedlibrary-show-a-different-library-base-addr

## scanf跳过输入——避免破坏canary

`+` `-` 因为正负数的原因可以当做合法输入，而不破坏输入流

## Linux 连续执行命令

`||` 执行到一个命令成功

`&&` 执行到一个命令失败

`;` 顺序执行

`system("*&^%$#@;sh")`

## Python tips

### 泄露地址填充

`ljust(8, '\x00')`向高位填充至8字节。

### 字符串与数字转换

`pack(canary, 32)[::-1].encode('hex')` 将canary打印出来

`unpack('/sh\x00', 32)`