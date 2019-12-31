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

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/brk%26mmap.png)

初始时，堆的起始地址 [start_brk](http://elixir.free-electrons.com/linux/v3.8/source/include/linux/mm_types.h#L365) 以及堆的当前末尾 [brk](http://elixir.free-electrons.com/linux/v3.8/source/include/linux/mm_types.h#L365) 指向同一地址。根据是否开启 ASLR，两者的具体位置会有所不同

- 不开启 ASLR 保护时，start_brk 以及 brk 会指向 data/bss 段的结尾。
- 开启 ASLR 保护时，start_brk 以及 brk 也会指向同一位置，只是这个位置是在 data/bss 段结尾后的随机偏移处。

具体效果如下图

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/program_virtual_address_memory_space.png)

### brk,sbrk

```c
#include <unistd.h>
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

## Glibc malloc

GNU C标准库的malloc实现源自于ptmalloc(pthreads malloc)，ptmalloc源于dlmalloc(Doug Lea malloc)。

glibc malloc通常使用两种方式分配内存，具体使用哪种方式由请求的大小和某些参数决定。一种是使用连续大段区域进行管理以提高分配效率减少浪费，通常堆区域只有一个，但glibc实现使用了多个堆区域，用来优化多线程的性能，每个区域内部称为一个arena；另一种是使用mmap，通过在请求大量内存时使用，大量指远大于一个页的大小[^1]。

### Arena

Arena分为main arena和thread arena。

glibc中通过增加arena来优化对线程的支持，但并非每个线程都有一个arena，因为代价高和浪费。arena的个数由cpu核心个数所限制，限制如下。这个限制是对于thread arena。

```
For 32 bit systems:
     Number of arena = 2 * number of cores.
For 64 bit systems:
     Number of arena = 8 * number of cores.
```

> 例子：如果一个多线程应用(4 threads---1 main thread + 3 user thread)运行在一个单核的32位系统上。thread arena的限制即为2 * number of cores(1) = 2。在这种情况下，glibc malloc将会确保arenas在几个线程中共享使用。
>
> - 对于主线程调用malloc时，使用main arena分配空间
>
> - 当线程1,2调用malloc时，创建两个新的arena分别供其使用。
>
> - 当线程3调用malloc时，由于已经达到了arena的个数限制，将会复用已有的arenas(main arena，arena 1，arena 2)
>   - 遍历可用的arena，一旦存在可用arena，申请该arena的锁
>   - 如果锁定成功，返回该arena
>   - 如果没有找到可用的arena，阻塞知道有可用的arena。
> - 当thread 3 调用 `malloc` 时(第二次了)，分配器会尝试使用上一次使用的 arena（也即，main arena），从而尽量提高缓存命中率。当 main arena 可用时就用，否则 thread 3 就一直阻塞，直至 main arena 空闲。因此现在 main arena 实际上是被 main thread 和 thread 3 所共享。

### Multiple heaps

在「glibc malloc」中主要有 3 种数据结构：

- [malloc_state](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1671) ——Arena header—— 一个 thread arena 可以维护多个堆，这些堆另外共享同一个 arena header。Arena header 描述的信息包括：bins、top chunk、last remainder chunk 等；

- [heap_info](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/arena.c#L59) ——Heap Header—— 一个 thread arena 可以维护多个堆。每个堆都有自己的堆 Header（注：也即头部元数据）。什么时候 Thread Arena 会维护多个堆呢？ 一般情况下，每个 thread arena 都只维护一个堆，但是当这个堆的空间耗尽时，新的堆（而非连续内存区域）就会被 `mmap` 到这个 aerna 里；
- [malloc_chunk](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1108) ——Chunk header—— 根据用户请求，每个堆被分为若干 chunk。每个 chunk 都有自己的 chunk header。

> **注意**：
>
> - Main arena 无需维护多个堆，因此也无需 heap_info。当空间耗尽时，与 thread arena 不同，main arena 可以通过 `sbrk` 拓展**堆**段，直至堆段「碰」到内存映射段；
> - 与 thread arena 不同，main arena 的 arena header 不是保存在通过 `sbrk` 申请的堆段里，而是作为一个全局变量，可以在 libc.so 的数据段中找到

main arena和thread arena的图示（单堆）

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/mainthreadarena.png)

thread arena图示(多堆)

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/thread%20arena.png)

### Chunk

堆段中存在的 chunk 类型如下：

- Allocated chunk;
- Free chunk;
- Top chunk;
- Last Remainder chunk.

#### Allocated chunk

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/Allocated%20chunk.png)

图中结构体内部各字段的含义依次为：

- [prev_size](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1110)：若前一个 chunk 可用，则此字段赋值为前一个 chunk 的大小；否则，此字段被用来存储前一个 chunk 的用户数据；
- size：此字段赋值本 chunk 的大小，其最后三位包含标志信息：
  - PREV_INUSE (P) – 置「1」表示前个 chunk 被分配；
  - IS_MMAPPED (M) – 置「1」表示这个 chunk 是通过 `mmap` 申请的（较大的内存）；
  - NON_MAIN_ARENA (N) – 置「1」表示这个 chunk 属于一个 thread arena(mmapd'd memory)。

> **注意**：
>
> - malloc_chunk 中的其余结构成员，如 fd、 bk，如果该块已分配，则不会使用，用来存储用户数据；
> - 用户请求的大小被转换为内部实际大小，因为需要额外空间存储 malloc_chunk，此外还需要考虑对齐。

#### Free chunk

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/freechunk.png)

图中结构体内部各字段的含义依次为：

- [prev_size](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1110): 两个相邻 free chunk 会被合并成一个，因此该字段总是保存前一个 allocated chunk 的用户数据；
- [size](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1111): 该字段保存本 free chunk 的大小；
- [fd](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1113): Forward pointer —— 本字段指向同一 bin 中的下个 free chunk；
- [bk](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1114): Backward pointer —— 本字段指向同一 bin 中的上个 free chunk

### Bins

「**bins**」 就是空闲列表数据结构。它们用以保存 free chunks。根据其中 chunk 的大小，bins 被分为如下几种类型：

- Fast bin;
- Unsorted bin;
- Small bin;
- Large bin.

保存这些 bins 的字段为：

- [fastbinsY](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1680): 这个数组用以保存 fast bins；

- bins

  : 这个数组用于保存 unsorted bin、small bins 以及 large bins，共计可容纳 126 个，其中：

  - Bin 1: unsorted bin;
  - Bin 2 - 63: small bins;
  - Bin 64 - 126: large bins.

#### Fast bin

大小为 [16](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L1249) ~ 80 字节的 chunk 被称为「**fast chunk**」。在所有的 bins 中，fast bins 路径享有最快的内存分配及释放速度。

- **数量**：10
  - 每个 fast bin 都维护着一条 free chunk 的单链表，采用单链表是因为链表中所有 chunk 的大小相等，增删 chunk 发生在链表头部即可；—— LIFO
- **chunk 大小**：8 字节递增
  - fast bins 由一系列所维护 chunk 大小以 8 字节递增的 bins 组成。也即，`fast bin[0]` 维护大小为 16 字节的 chunk、`fast bin[1]` 维护大小为 24 字节的 chunk。依此类推……
  - 指定 fast bin 中所有 chunk 大小相同；
- 在 malloc 初始化过程中，最大的 fast bin 的大小被设置为 [64](https://github.com/sploitfun/lsploits/blob/master/glibc/malloc/malloc.c#L794) 而非 80 字节。因为默认情况下只有大小 16 ~ 64 的 chunk 被归为 fast chunk 。
- 无需合并 —— 两个相邻 chunk 不会被合并。虽然这可能会加剧内存碎片化，但也大大加速了内存释放的速度！
- `malloc(fast chunk)`
  - 初始情况下 fast chunck 最大尺寸以及 fast bin 相应数据结构均未初始化，因此即使用户请求内存大小落在 fast chunk 相应区间，服务用户请求的也将是 small bin 路径而非 fast bin 路径；
  - 初始化后，将在计算 fast bin 索引后检索相应 bin；
  - 相应 bin 中被检索的第一个 chunk 将被摘除并返回给用户。
- `free(fast chunk)`
  - 计算 fast bin 索引以索引相应 bin；
  - `free` 掉的 chunk 将被添加到上述 bin 的头部。

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/fastbin.png)

#### Unsorted bin

当 small chunk 和 large chunk 被 `free` 掉时，它们并非被添加到各自的 bin 中，而是被添加在 「**unsorted bin**」 中。这使得分配器可以重新使用最近 `free` 掉的 chunk，从而消除了寻找合适 bin 的时间开销，进而加速了内存分配及释放的效率。

> 在内存分配的时候，在前后检索 fast/small bins 未果之后，在特定条件下，会将 unsorted bin 中的 chunks 转移到合适的 bin 中去，small/large。

- **数量**：1
- unsorted bin 包括一个用于保存 free chunk 的双向循环链表（又名 binlist）；
- **chunk 大小**：无限制，任何大小的 chunk 均可添加到这里。

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/unsortedsmalllarge.png)

#### Small bin

大小小于 512 字节的 chunk 被称为 「**small chunk**」，而保存 small chunks 的 bin 被称为 「**small bin**」。在内存分配回收的速度上，small bin 比 large bin 更快。

- **数量**：62
  - 每个 small bin 都维护着一条 free chunk 的双向循环链表。采用双向链表的原因是，small bins 中的 chunk 可能会从链表中部摘除。这里新增项放在链表的头部位置，而从链表的尾部位置移除项。—— FIFO
- **chunk 大小**：8 字节递增
  - Small bins 由一系列所维护 chunk 大小以 8 字节递增的 bins 组成。举例而言，`small bin[0]` （Bin 2）维护着大小为 16 字节的 chunks、`small bin[1]`（Bin 3）维护着大小为 24 字节的 chunks ，依此类推……
  - 指定 small bin 中所有 chunk 大小均相同，因此无需排序；
- 合并 —— 相邻的 free chunk 将被合并，这减缓了内存碎片化，但是减慢了 `free` 的速度；
- `malloc(small chunk)`
  - 初始情况下，small bins 都是 NULL，因此尽管用户请求 small chunk ，提供服务的将是 unsorted bin 路径而不是 small bin 路径；
  - 第一次调用 `malloc` 时，维护在 malloc_state 中的 small bins 和 large bins 将被初始化，它们都会指向自身以表示其为空；
  - 此后当 small bin 非空，相应的 bin 会摘除其中最后一个 chunk 并返回给用户；
- `free(small chunk)`
  - `free` chunk 的时候，检查其前后的 chunk 是否空闲，若是则合并，也即把它们从所属的链表中摘除并合并成一个新的 chunk，新 chunk 会添加在 unsorted bin 的前端。

#### Large bin

大小大于等于 512 字节的 chunk 被称为「**large chunk**」，而保存 large chunks 的 bin 被称为 「**large bin**」。在内存分配回收的速度上，large bin 比 small bin 慢。

- 数量：63
  - 每个 large bin 都维护着一条 free chunk 的双向循环链表。采用双向链表的原因是，large bins 中的 chunk 可能会从链表中的任意位置插入及删除。
  - 这 63 个 bins
    - 32 个 bins 所维护的 chunk 大小以 64B 递增，也即 `large chunk[0]`(Bin 65) 维护着大小为 512B ~ 568B 的 chunk 、`large chunk[1]`(Bin 66) 维护着大小为 576B ~ 632B 的 chunk，依此类推……
    - 16 个 bins 所维护的 chunk 大小以 512 字节递增；
    - 8 个 bins 所维护的 chunk 大小以 4096 字节递增；
    - 4 个 bins 所维护的 chunk 大小以 32768 字节递增；
    - 2 个 bins 所维护的 chunk 大小以 262144 字节递增；
    - 1 个 bin 维护所有剩余 chunk 大小；
  - 不像 small bin ，large bin 中所有 chunk 大小不一定相同，各 chunk 大小递减保存。最大的 chunk 保存顶端，而最小的 chunk 保存在尾端；
- 合并 —— 两个相邻的空闲 chunk 会被合并；
- `malloc(large chunk)`
  - 初始情况下，large bin 都会是 NULL，因此尽管用户请求 large chunk ，提供服务的将是 next largetst bin 路径而不是 large bin 路劲 。
  - 第一次调用 `malloc` 时，维护在 malloc_state 中的 small bin 和 large bin 将被初始化，它们都会指向自身以表示其为空；
  - 此后当 large bin 非空，如果相应 bin 中的最大 chunk 大小大于用户请求大小，分配器就从该 bin 顶端遍历到尾端，以找到一个大小最接近用户请求的 chunk。一旦找到，相应 chunk 就会被切分成两块：
    - User chunk（用户请求大小）—— 返回给用户；
    - Remainder chunk （剩余大小）—— 添加到 unsorted bin。
  - 如果相应 bin 中的最大 chunk 大小小于用户请求大小，分配器就会扫描 binmaps，从而查找最小非空 bin。如果找到了这样的 bin，就从中选择合适的 chunk 并切割给用户；反之就使用 top chunk 响应用户请求。
- `free(large chunk)` —— 类似于 small chunk 。

#### Top bin

一个 arena 中最顶部的 chunk 被称为「**top chunk**」。它不属于任何 bin 。当所有 bin 中都没有合适空闲内存时，就会使用 top chunk 来响应用户请求。

当 top chunk 的大小比用户请求的大小大的时候，top chunk 会分割为两个部分：

- User chunk，返回给用户；
- Remainder chunk，剩余部分，将成为新的 top chunk。

当 top chunk 的大小比用户请求的大小小的时候，top chunk 就通过 `sbrk`（main arena）或 `mmap`（ thread arena）系统调用扩容。

#### Last remainder chunk

「**last remainder chunk**」即最后一次 small request 中因分割而得到的剩余部分，它有利于改进引用局部性，也即后续对 small chunk 的 `malloc` 请求可能最终被分配得彼此靠近。

那么 arena 中的若干 chunks，哪个有资格成为 last remainder chunk 呢？

当用户请求 small chunk 而无法从 small bin 和 unsorted bin 得到服务时，分配器就会通过扫描 binmaps 找到最小非空 bin。正如前文所提及的，如果这样的 bin 找到了，其中最合适的 chunk 就会分割为两部分：返回给用户的 User chunk 、添加到 unsorted bin 中的 Remainder chunk。这一 Remainder chunk 就将成为 last remainder chunk。

那么引用局部性是如何达成的呢？

当用户的后续请求 small chunk，并且 last remainder chunk 是 unsorted bin 中唯一的 chunk，该 last remainder chunk 就将分割成两部分：返回给用户的 User chunk、添加到 unsorted bin 中的 Remainder chunk（也是 last remainder chunk）。因此后续的请求的 chunk 最终将被分配得彼此靠近。





[^1]: https://www.gnu.org/software/libc/manual/html_node/The-GNU-Allocator.html
[^2]:https://sploitfun.wordpress.com/2015/02/10/understanding-glibc-malloc/
[^3]:https://blog.csdn.net/maokelong95/article/details/51989081