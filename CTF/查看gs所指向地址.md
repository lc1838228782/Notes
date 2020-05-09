## 搜索内存

`find $(canary)`

## 系统调用出中断

> ```
> 2.22 在GDB里如何查看GS:0x10的内容
> 
> http://scz.617.cn:8/unix/201205081619.txt
> 
> Q:
> 
> 逻辑地址
> 
>     段选择子(16-bits):段内偏移(32-bits)
> 
>     也叫远指针。程序中寻址时只能使用逻辑地址。没有办法禁用段机制，但有办法
>     禁用分页机制。
> 
> 线性地址
> 
>     逻辑地址经GDT、LDT转换后得到线性地址。
> 
> 我想在GDB里查看逻辑地址"GS:0x10"的内容。
> 
> A:
> 
> 在GDB里没有通用办法查看逻辑地址的内容，也没有通用办法进行逻辑地址到线性地
> 址的转换。
> 
> 就这个具体的"GS:0x10"而言，有变通办法达到原始目的。
> 
> 检查glibc-2.11.2的源码:
> 
> nptl/sysdeps/i386/tls.h
> 
> 从TLS_INIT_TP()宏的实现可以看出，GS的段基址对应系统调用set_thread_area()的
> 形参u_info->base_addr:
> 
> --------------------------------------------------------------------------
> int set_thread_area ( struct user_desc *u_info );
> 
> struct user_desc
> {
>     unsigned int        entry_number;
>     unsigned long int   base_addr;
>     unsigned int        limit;
>     unsigned int        seg_32bit:1;
>     unsigned int        contents:2;
>     unsigned int        read_exec_only:1;
>     unsigned int        limit_in_pages:1;
>     unsigned int        seg_not_present:1;
>     unsigned int        useable:1;
>     unsigned int        empty:25;
> };
> --------------------------------------------------------------------------
> 
> GS用于访问GLIBC TLS (thread-local storage)，GS段的起始地址对应tcbhead_t结
> 构:
> 
> --------------------------------------------------------------------------
> /*
>  * nptl/sysdeps/i386/tls.h
>  */
> typedef struct
> {
>     /*
>      * Pointer to the TCB. Not necessarily the thread descriptor used by
>      * libpthread.
>      */
>     void       *tcb;
>     dtv_t      *dtv;
>     /*
>      * Pointer to the thread descriptor.
>      *
>      * 指向所属结构的起始地址
>      */
>     void       *self;
>     int         multiple_threads;
>     /*
>      * GS:0x10
>      */
>     uintptr_t   sysinfo;
>     uintptr_t   stack_guard;
>     uintptr_t   pointer_guard;
>     int         gscope_flag;
> #ifndef __ASSUME_PRIVATE_FUTEX
>     int         private_futex;
> #else
>     int         __unused1;
> #endif
>     /*
>      * Reservation of some values for the TM ABI.
>      */
>     void       *__private_tm[5];
> } tcbhead_t;
> --------------------------------------------------------------------------
> $ gdb /usr/bin/col
> (gdb) catch syscall set_thread_area
> Catchpoint 1 (syscall 'set_thread_area' [243])
> (gdb) r
> Starting program: /usr/bin/col
> 
> Catchpoint 1 (call to syscall 'set_thread_area'), 0xb7fe6cff in ?? () from /lib/ld-linux.so.2
> (gdb) x/i $eip-2
> 0xb7fe6cfd:     int    $0x80
> (gdb) i r ebx
> ebx            0xbffffaa0       -1073743200
> (gdb) x/1wx $ebx+4
> 0xbffffaa4:     0xb7e8a8d0
> (gdb) x/1wx *(unsigned int *)($ebx+4)+0x10
> 0xb7e8a8e0:     0xb7fe4400
> (gdb) x/17i *(unsigned int *)(*(unsigned int *)($ebx+4)+0x10)
> 0xb7fe4400:     push   %ecx
> 0xb7fe4401:     push   %edx
> 0xb7fe4402:     push   %ebp
> 0xb7fe4403:     mov    %esp,%ebp
> 0xb7fe4405:     sysenter
> 0xb7fe4407:     nop
> 0xb7fe4408:     nop
> 0xb7fe4409:     nop
> 0xb7fe440a:     nop
> 0xb7fe440b:     nop
> 0xb7fe440c:     nop
> 0xb7fe440d:     nop
> 0xb7fe440e:     jmp    0xb7fe4403
> 0xb7fe4410:     pop    %ebp
> 0xb7fe4411:     pop    %edx
> 0xb7fe4412:     pop    %ecx
> 0xb7fe4413:     ret
> (gdb)
> --------------------------------------------------------------------------
> 
> 拦截系统调用set_thread_area()，检查形参，获得GS段的线性地址(base_addr)，将
> base_addr(0xb7e8a8d0)加0x10，得到GS:0x10对应的线性地址(0xb7e8a8e0)，检查该
> 地址内容为0xb7fe4400。
> 
> 顺便说一句，0xb7fe4400即__kernel_vsyscall()。在没有ASLR机制的年代，后者对
> 应0xffffe400。
> ```

64bit

```c
typedef struct
{
    void *tcb;        /* Pointer to the TCB.  Not necessarily the
                 thread descriptor used by libpthread.  */
    dtv_t *dtv;
    void *self;       /* Pointer to the thread descriptor.  */
    int multiple_threads;
    int gscope_flag;
    uintptr_t sysinfo;
    uintptr_t stack_guard;
    uintptr_t pointer_guard;
    ...
} tcbhead_t;
```

系统调用`arch_prctl(int code, unsigned long addr)`

```
gdb> catch syscall 158
Catchpoint 1 (syscall 'arch_prctl' [158])
```

rsi寄存器所存储就是gs基地址。