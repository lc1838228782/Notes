## house of orange

修改vtable，2.23

## pwnable.tw seethefile

通过修改`_IO_finish`函数指针，劫持fclose流程

## ciscn2020 pre maj

利用UAF，将造成chunk重叠，将同一chunk同时释放到fastbin，unsorted bin

利用partial overwrite，爆破4bit到stdout，进行修改，在没有show的情况下，泄露libc。

```
_flags = 0xfbad1800
_IO_read_ptr = 0
_IO_read_end = 0
_IO_read_base = 0
_IO_write_base = 0xXXXXXXXXXXXXXX00
```

new_do_write会将其进行恢复，不影响后续输出（后续输出回车似乎出现问题

http://blog.eonew.cn/archives/1190



## re-alloc revenge

