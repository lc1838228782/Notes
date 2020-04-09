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