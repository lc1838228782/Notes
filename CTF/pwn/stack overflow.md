# Stack overflow

一般来说，我们会有如下的覆盖需求

- **覆盖函数返回地址**，这时候就是直接看 EBP 即可。
- **覆盖栈上某个变量的内容**，这时候就需要更加精细的计算了。
- **覆盖 bss 段某个变量的内容**。
- 根据现实执行情况，覆盖特定的变量或地址的内容。

## Basic ROP

`ret2text`: 返回到自身的代码，自身代码中有getshell代码段

`ret2shellcode`: 自己放置shellcode至可执行代码段，返回到此位置

`ret2syscall`: 利用程序自身的gadget，构造shellcode。**ROPGadget工具**

`ret2libc`: 1. 利用程序中的已有函数plt，字符串`/bin/sh`。2. 自行获取函数位置。

```
泄露 __libc_start_main 地址
获取 libc 版本
获取 system 地址与 /bin/sh 的地址
再次执行源程序
触发栈溢出执行 system(‘/bin/sh’)
```

## Intermediate ROP

`ret2csu`: 64位中`__libc_csu_init `函数存在特殊的gadget。 **蒸米--level5**

![img](https://ctf-wiki.github.io/ctf-wiki/pwn/linux/stackoverflow/figure/brop_gadget.png)

`ret2reg`: 找`call reg`和`jmp reg`gadget，设置reg处为shellcode地址。

## Advanced ROP

`ret2_dl_runtime_resolve`: 构造动态链接过程

## Tricks

### [Canary 绕过技术](https://ctf-wiki.github.io/ctf-wiki/pwn/linux/mitigation/canary-zh/#gcc-canary)

1. Canary以`\x00`结尾，覆盖之后使用一定的方法输出。
2. one-by-one爆破Canary，一个进程中的各线程和fork创建的子进程中的Canary都是一样的。
3. 劫持`__stack_chk_fail`函数(ZCTF2017 Login)
4. 覆盖TLS中储存的Canary值(StarCTF2018 babystack)

### 栈迁移

### stack smash 劫持

### partial overwrite