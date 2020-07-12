## first-fit

如果一个chunk可用且足够大，那么就会使用这个chunk。

## calc_tcache_idx

tcache(thread local caching)

```c
/* We want 64 entries.  This is an arbitrary limit, which tunables can reduce.  */
# define TCACHE_MAX_BINS		64
# define MAX_TCACHE_SIZE	tidx2usize (TCACHE_MAX_BINS-1)

/* Only used to pre-fill the tunables.  */
# define tidx2usize(idx)	(((size_t) idx) * MALLOC_ALIGNMENT + MINSIZE - SIZE_SZ)

/* When "x" is from chunksize().  */
# define csize2tidx(x) (((x) - MINSIZE + MALLOC_ALIGNMENT - 1) / MALLOC_ALIGNMENT)
/* When "x" is a user-provided size.  */
# define usize2tidx(x) csize2tidx (request2size (x))

/* With rounding and alignment, the bins are...
   idx 0   bytes 0..24 (64-bit) or 0..12 (32-bit)
   idx 1   bytes 25..40 or 13..20
   idx 2   bytes 41..56 or 21..28
   etc.  */
```

tcache拥有和fastbin差不多的结构。默认情况下，64个bin，每个bin最多7个chunk

64bit: `IDX = (CHUNKSIZE - 0x11) / 0x10`

## fastbin_dup

free(fastbin)过程中，只检查了链表头部是否和将要free的指针一样。

```c
int *a = malloc(8);
int *b = malloc(8);
int *c = malloc(8);

free(a);
free(b);
free(a);
```

此时fastbin 0x20: `a->b->a`

**附注：**

tcache可以检测到double free。在本机glibc2.31中发现，tcache_entry中使用key指向了tcache_perthread_struct。[malloc.c:4186](https://sourceware.org/git/?p=glibc.git;a=blob;f=malloc/malloc.c;h=f7cd29bc2f93e1082ee77800bd64a4b2a2897055;hb=9ea3686266dca3f004ba874745a4087a89682617#l4186)

```c
typedef struct tcache_entry
{
  struct tcache_entry *next;
  /* This field exists to detect double frees.  */
  struct tcache_perthread_struct *key;
} tcache_entry;
```

tcache检测double free代码：

```c
/* Check to see if it's already in the tcache.  */
tcache_entry *e = (tcache_entry *) chunk2mem (p);

/* This test succeeds on double free.  However, we don't 100%
	   trust it (it also matches random payload data at a 1 in
	   2^<size_t> chance), so verify it's not an unlikely
	   coincidence before aborting.  */
if (__glibc_unlikely (e->key == tcache))
{
    tcache_entry *tmp;
    LIBC_PROBE (memory_tcache_double_free, 2, e, tc_idx);
    for (tmp = tcache->entries[tc_idx];
         tmp;
         tmp = tmp->next)
        if (tmp == e)
            malloc_printerr ("free(): double free detected in tcache 2");
    /* If we get here, it was a coincidence.  We've wasted a
	       few cycles, but don't abort.  */
}
```

# glibc 2.25

## fastbin_dup_into_stack.c

假设fastbin中通过double free得到以下序列。

fastbin: `a->b->a`

第一次malloc得到a后，修改数据，使得第二个a->fd指向栈地址。

### 0ctf2017-babyheap

fill函数中存在对chunk的任意长度溢出。

```python
allocate(0x20)
allocate(0x20)
allocate(0x20)
allocate(0x20)
allocate(0x80)		# smallbin
```

mmap分配空间那块没看懂，怎么就获得了随机空间。。如果有人看懂，麻烦告诉我一下

**泄露**：通过**partial overwrite**修改fd，指向smallbin。溢出修改smallbin->size为0x31，两次分配后得到另一指针指向该smallbin。而后重新修改smallbin->size为0x91，free该smallbin，smallbin->fd和smallbin->bk就会指向main_arena中的区域。使用另一指针打印内容就可以得到libc_base。

**攻击**：0x70 fastbin attack，复写`__malloc_hook`为one_gadget地址。

### ref

https://uaf.io/exploitation/2017/03/19/0ctf-Quals-2017-BabyHeap2017.html

## fastbin_dup_consolidate

double free fastbin到unsorted bin和fastbin。通过分配large bin触发malloc_consolidate，清空fastbin到unsorted bin。[图源](https://raw.githubusercontent.com/cloudburst/libheap/master/heap.png)

![](https://raw.githubusercontent.com/cloudburst/libheap/master/heap.png)

### Hitcon 2016 SleepyHolder

fastbin dup consolidate double free

unsafe unlink

### ref

https://github.com/mehQQ/public_writeup/tree/master/hitcon2016/SleepyHolder

## unsafe unlink

和SleepyHolder一样。。 

how2heap对glibc的版本要求`<2.26`似乎有误？据我测试，**glibc2.31 without tcache也可以运行成功**，只要添加`chunk0_ptr[1] = (uint64_t)malloc_size;`，glibc2.31中看起来只是增加了size的验证。

```c
/* consolidate backward */
if (!prev_inuse(p)) {
    prevsize = prev_size (p);
    size += prevsize;
    p = chunk_at_offset(p, -((long) prevsize));
    if (__glibc_unlikely (chunksize(p) != prevsize))
        malloc_printerr ("corrupted size vs. prev_size while consolidating");
    unlink_chunk (av, p);
}
```

**非fastbin size才会进行前后consolidate。**

### Hitcon2014 stkof

p1分配0x80，p2分配0x80。

p1内伪造fake_chunk，溢出修改p2的pre_size,pre_in_use。free(p2)

覆盖free_got泄露地址，再任意写到atoi_got

## House of spirit

在不可控区域周围构造fakechunk，覆盖某指针，释放再分配，以控制不可控区域。

### hack.lu2014 oreo

在message_ptr周围构造。

## poison null byte

溢出，覆盖已经释放的chunk的size，使其大小变小。free后块使其合并。

### Plaidctf2015 plaiddb

null byte溢出，堆块重叠，libc泄露和fastbin attack

### ref

1. https://4ngelboy.blogspot.com/2016/10/span-display-block-overflow-hidden_10.html
2. https://ctf-wiki.github.io/ctf-wiki/pwn/linux/glibc-heap/off_by_one-zh/#2-plaidctf-2015-plaiddb

## house of lore

**glibc2.31 without tcache可以运行成功。**

smallbin链表的特点是头插尾取的FIFO。

关键的漏洞点在，攻击者可以控制已经释放的smallbin的bk指针。取出smallbin过程中，双向链表尾部被修改为fake_chunk。

程序中的描述对于64位有些问题，但没有影响效果。64位过程如下：

`free((void*)victim)`后，该chunk被放入fastbin，后`void *p2 = malloc(1200);`申请largebin触发malloc_consolidate，将fastbin放入unsortedbin，然后遍历unsortedbin查找chunk导致victim chunk被放入smallbin。

```c
victim_chunk->bk   = stack_buffer_1;
stack_buffer_1->fd = vimtim_chunk;
stack_buffer_1->bk = stack_buffer_2;
stack_buffer_2->fd = stack_buffer_1;
```

## overlapping chunks

溢出，对unsortedbin中的chunk的size进行更改，造成重叠。可以添加`prev_size(nextchunk) == chunk->size`检查修补。

### hack.lu2015 bookstore

overlapping

format string vulnerability：泄露libc，修改fini_array和返回地址

## overlapping chunks 2

```c
  p1 = malloc(1000);
  p2 = malloc(1000);
  p3 = malloc(1000);
  p4 = malloc(1000);
  p5 = malloc(1000);
```

首先free(p4)，溢出，对使用中的chunk p2的size进行更改，free掉p2，与p4合并，造成重叠。

## house of force

覆盖topchun-size为0xffffffff，分配一个能够溢出size_t的evil_size，到目的地址，再分配一次，对其进行修改。

### bctf2016 bcloud

null byte overflow，strcpy溢出。修改free_got为put_plt，泄露libc base，后修改atoi_got。

## unsorted bin into stack

和house of lore差不多，只不过这里是对于unsorted bin。

在栈上伪造chunk，修改unsorted bin尾部chunk的size和bk，使其size不符合下一次分配的要求，使其bk指向栈上伪造的chunk，造成下一次分配落到栈上。

## unsorted bin attack

修改unsorted bin中某一chunk的bk指向某位置p，接下来该chunk分配后，`p+2*SIZE_ZE`处会被覆盖为unsorted bin头结点的地址，也就是unsortedbin地址。

该攻击通常为进一步攻击做准备，比如覆盖global_max_fast

### 0ctf2016 zerostorage

UAF，realloc with topchunk

