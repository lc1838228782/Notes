# Lab2 物理内存管理

## 段式存储管理

## 页式存储管理

页帧（帧、物理页面、Frame、Page Frame）

页面（页、逻辑页面、Page）

## 段页式存储管理

## x86 特权级

Linux和UCore只用到ring0和ring3.

**段选择子**

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200202114237000.png)

RPL 段寄存器 DS，ES，FS，GS

CPL 段寄存器CS，SS

**段描述符**

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200202113940707.png)

DPL 段描述符，门描述符

**特权级检查**

- 访问门时	CPL <= DPL[门] & CPL >= DPL[段]

- 访问段时    MAX(CPL, RPL) <= DPL[段]

  *RPL：确保特权代码不会替代应用程序访问一个段，除非应用程序拥有访问那个段的权限。*

  *栈段的特权级必须和CPL相同*

**TSS段描述符**

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/tss%20descriptor.png)

有task register指向此项描述符。

## 自映射机制

页目录项当做页表。连续的4M虚拟地址空间。

节省4KB，以及有以下优点：

> ​	如果我们这时需要按虚拟地址的地址顺序显示整个页目录表和页表的内容，则要查找页目录表的页目录表项内容，根据页目录表项内容找到页表的物理地址，再转换成对应的虚地址，然后访问页表的虚地址，搜索整个页表的每个页目录项。这样过程比较繁琐。
>
> ​	我们需要有一个简洁的方法来实现这个查找。ucore做了一个很巧妙的地址自映射设计，把页目录表和页表放在一个连续的4MB虚拟地址空间中，并设置页目录表自身的虚地址<-->物理地址映射关系。这样在已知页目录表起始虚地址的情况下，通过连续扫描这特定的4MB虚拟地址空间，就很容易访问每个页目录表项和页表项内容。

## Task 1： 实现 first-fit 连续物理内存分配算法

需要重写`default_init`, `default_init_memmap`, `default_alloc_pages`, `default_free_pages`

### 流程

1. 熟悉list操作(lib/list.h)，例如`list_init, list_add(list_add_after), list_add_before, list_del, list_next, list_prev`。

   some MACROs：

   `le2page`(in memlayout.h), (and in future labs: `le2vma` (in vmm.h), `le2proc` (inproc.h), etc).

2. `default_init`：可以重用demo

3. `default_init_memmap`：该函数用于初始化空闲区块。初始化空闲区块，首先需要初始化page，包括以下步骤：

   （1）设置`p->flags`的`PG_property`，这意味着这一页是有效的。P.S. 在`pmm_init`中，`PG_reserved`已经被设置了

   （2）如果该页空闲且不是空闲区块的第一页，`p->property`应设置为0。

   （3）如果该页空闲且是空闲区块的第一页，`p->property`应设置为区块页面的个数。

   （3）`p->ref`设置为0

   然后，使用`p->page_link`链接进`free_list`。

   最后，更新free_area的nr_free

4. `default_alloc_pages`：查找可供使用的空闲区块(size >= n)，返回首地址。

```
(4.1)
 *      So you should search the free list like this:
 *          list_entry_t le = &free_list;
 *          while((le=list_next(le)) != &free_list) {
 *          ...
 *      (4.1.1)
 *          In the while loop, get the struct `page` and check if `p->property`
 *      (recording the num of free pages in this block) >= n.
 *              struct Page *p = le2page(le, page_link);
 *              if(p->property >= n){ ...
 *      (4.1.2)
 *          If we find this `p`, it means we've found a free block with its size
 *      >= n, whose first `n` pages can be malloced. Some flag bits of this page
 *      should be set as the following: `PG_reserved = 1`, `PG_property = 0`.
 *      Then, unlink the pages from `free_list`.
 *          (4.1.2.1)
 *              If `p->property > n`, we should re-calculate number of the rest
 *          pages of this free block. (e.g.: `le2page(le,page_link))->property
 *          = p->property - n;`)
 *          (4.1.3)
 *              Re-caluclate `nr_free` (number of the the rest of all free block).
 *          (4.1.4)
 *              return `p`.
 *      (4.2)
 *          If we can not find a free block with its size >=n, then return NULL.
```



4. `default_free_pages`：释放且尽可能合并空闲区块。

```
 *  (5.1)
 *      According to the base address of the withdrawed blocks, search the free
 *  list for its correct position (with address from low to high), and insert
 *  the pages. (May use `list_next`, `le2page`, `list_add_before`)
 *  (5.2)
 *      Reset the fields of the pages, such as `p->ref` and `p->flags` (PageProperty)
 *  (5.3)
 *      Try to merge blocks at lower or higher addresses. Notice: This should
 *  change some pages' `p->property` correctly.
```



完成物理内存初始化工作后，内存的布局：

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/memory%20layout.png)

## Task 2：实现寻找虚拟地址对应的页表项

获取`la`所对应的页表项的地址。

注意对地址的加减操作对象。

可以用`[]`取对应项

## Task 3：释放某虚地址所在的页并取消对应二级页表项的映射



## Questions

- `static inline`什么作用？ static inline lgdt并没inline进去

- `asm volatile ("ljmp %0, $1f\n 1:\n" :: "i" (KERNEL_CS));`为什么这条语句是跳转到下一条指令执行？