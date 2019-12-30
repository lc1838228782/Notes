# [Heap Overview](https://ctf-wiki.github.io/ctf-wiki/pwn/linux/glibc-heap/heap_overview-zh/)

目前 Linux 标准发行版中使用的堆分配器是 glibc 中的堆分配器：ptmalloc2。ptmalloc2 主要是通过 malloc/free 函数来分配和释放内存块。

需要注意的是，在内存分配与使用的过程中，Linux 有这样的一个基本内存管理思想，**只有当真正访问一个地址的时候，系统才会建立虚拟页面与物理页面的映射关系**。

## 基本操作

### malloc

- 当 n=0 时，返回当前系统允许的堆的最小内存块。
- 当 n 为负数时，由于在大多数系统上，**size_t 是无符号数（这一点非常重要）**，所以程序就会申请很大的内存空间，但通常来说都会失败，因为系统没有那么多的内存可以分配。

### free

- **当 p 为空指针时，函数不执行任何操作。**
- 当 p 已经被释放之后，再次释放会出现乱七八糟的效果，这其实就是 `double free`。
- 除了被禁用 (mallopt) 的情况下，当释放很大的内存空间时，程序会将这些内存空间还给系统，以便于减小程序所使用的内存空间。

## 背后的系统调用

这些函数背后的系统调用主要是 [(s)brk](http://man7.org/linux/man-pages/man2/sbrk.2.html) 函数以及 [mmap, munmap](http://man7.org/linux/man-pages/man2/mmap.2.html) 函数。

![brk&mmap](../images/brk&mmap.png)

初始时，堆的起始地址 [start_brk](http://elixir.free-electrons.com/linux/v3.8/source/include/linux/mm_types.h#L365) 以及堆的当前末尾 [brk](http://elixir.free-electrons.com/linux/v3.8/source/include/linux/mm_types.h#L365) 指向同一地址。根据是否开启 ASLR，两者的具体位置会有所不同

- 不开启 ASLR 保护时，start_brk 以及 brk 会指向 data/bss 段的结尾。
- 开启 ASLR 保护时，start_brk 以及 brk 也会指向同一位置，只是这个位置是在 data/bss 段结尾后的随机偏移处。

具体效果如下图

![program_virtual_address_memory_space](../images/program_virtual_address_memory_space.png)

### brk,sbrk

```c
#include<unistd.h>
int brk(void * addr); 
/* brk调整program break位置，参数为break的新地址。
brk()成功返回0，失败返回-1
并设置errno值为ENOMEM */

void * sbrk(intptr_t increment);
/* sbrk通过increment的正负调整break位置，
sbrk()成功返回原brk的地址，也就是新分配内存的起始位置，失败返回(void *)-1,
并设置errno全局变量的值为ENOMEM */
```

### mmap,munmap

```c
#incldue<sys/mman.h>
void * mmap(void * addr, size_t length,int prot,int flags,int fd,off_t offset);
int munmap(void * addr, size_t length);//addr为mmap函数返回接收的地址，length为请求分配的长度。
```

```text
参数：

（1）、addr:
起始地址，置零让系统自行选择并返回即可.
（2）、length:
长度，不够一页会自动凑够一页的整数倍，我们可以宏定义#define MIN_LENGTH_MMAP 4096为一页大小
（3）、prot:
读写操作权限，PROT_READ可读、PROT_WRITE可写、PROT_EXEC可执行、PROT_NONE映射区域不能读取。（注意PROT_XXXXX与文件本身的权限不冲突，如果在程序中不设定任何权限，即使本身存在读写权限，该进程也不能对其操作）
（4）、flags常用标志:
①MAP_SHARED【share this mapping】、MAP_PRIVATE【Create a private copy-on-write mapping】
MAP_SHARED只能设置文件共享，不能地址共享，即使设置了共享，对于两个进程来说，也不会生效。而MAP_PRIVATE则对于文件与内存都可以设置为私有。
②MAP_ANON【Deprecated】、MAP_ANONYMOUS：匿名映射，如果映射地址需要加该参数，如果不加默认映射文件。MAP_ANON已经过时，只需使用MAP_ANONYMOUS即可
（5）、文件描述符：fd
（6）、文件描述符偏移量：offset
（fd和offset对于一般性内存分配来说设置为0即可）

返回值：

失败返回MAP_FAILED，即(void * (-1))并设置errno全局变量。
成功返回指向mmap area的指针pointer。

常见errno错误：

①ENOMEM：内存不足；
②EAGAIN：文件被锁住或有太多内存被锁住；
③EBADF：参数fd不是有效的文件描述符；
④EACCES：存在权限错误，。如果是MAP_PRIVATE情况下文件必须可读；使用MAP_SHARED则文件必须能写入，且设置prot权限必须为PROT_WRITE。
⑤EINVAL：参数addr、length或者offset中有不合法参数存在。
```

![mmap](https://raw.githubusercontent.com/lc1838228782/pics/master/img/mmap.png)

