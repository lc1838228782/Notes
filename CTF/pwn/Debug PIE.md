## 1 pwndbg

```
b *$rebase(0xoffset)
```

## 2 ida

ida远程调试。

在ida安装路径下dbgsrv文件夹中，有各类平台的debug server。复制到可执行文件的目录下，设置ida。

## 3 gdb (>=8.1)

`starti`命令

## 4 gdb (<8.1)

Setting a breakpoint on an unmapped address before starting the target process does this, effectively. It's not correct functionality, but rather a side-effect of the failure to set the breakpoint.

```
(gdb) break *0
Breakpoint 1 at 0x0
(gdb) r
Starting program: /home/user/ld.so 
Error in re-setting breakpoint 1: Warning:
Cannot insert breakpoint 1.
Cannot access memory at address 0x0

Warning:
Cannot insert breakpoint 1.
Cannot access memory at address 0x0

(gdb) x/i $pc
=> 0x7faae3a25cd0:      mov    rdi,rsp
```



## ref

1. https://reverseengineering.stackexchange.com/questions/8724/set-a-breakpoint-on-gdb-entry-point-for-stripped-pie-binaries-without-disabling
2. https://blog.csdn.net/Maxmalloc/article/details/86037537