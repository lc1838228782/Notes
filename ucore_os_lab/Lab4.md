# Lab4 内核线程管理

## 实验执行流程综述

内核线程是一种特殊的进程，内核线程与用户进程的区别有两个：

- 内核线程只运行在内核态而用户进程会在在用户态和内核态交替运行；
- 所有内核线程直接使用共同的ucore内核内存空间，不需为每个内核线程维护单独的内存空间而用户进程需要维护各自的用户内存空间。

## 进程的属性与特征解析

> #### 1. 资源管理
>
> 在计算机系统中，进程会占用内存和CPU，这都是有限的资源，如果不进行合理的管理，资源会耗尽或无法高效公平地使用，从而会导致计算机系统中的多个进程执行效率很低，甚至由于资源不够而无法正常执行。
>
> 对于用户进程而言，操作系统是它的“上帝”，操作系统给了用户进程可以运行所需的资源，最基本的资源就是内存和CPU。在实验二/三中涉及的内存管理方法和机制可直接应用到进程的内存资源管理中来。在有多个进程存在的情况下，对于CPU这种资源，则需要通过进程调度来合理选择一个进程，并进一步通过进程分派和进程切换让不同的进程分时复用CPU，执行各自的工作。对于无法剥夺的共享资源，如果资源管理不当，多个进程会出现死锁或饥饿现象。
>
> #### 2. 进程状态管理
>
> 用户进程有不同的状态（可理解为“生命”的不同阶段），当操作系统把程序的放到内存中后，这个进程就“诞生”了，不过还没有开始执行，但已经消耗了内存资源，处于“创建”状态；当进程准备好各种资源，就等能够使用CPU时，进程处于“就绪”状态；当进程终于占用CPU，程序的指令被CPU一条一条执行的时候，这个进程就进入了“运行”状态，这时除了继续占用内存资源外，还占用了CPU资源；当进程由于等待某个资源而无法继续执行时，进程可放弃CPU使用，即释放CPU资源，进入“等待”状态；当程序指令执行完毕，由操作系统回收进程所占用的资源时，进程进入了“死亡”状态。
>
> 这些进程状态的转换时机需要操作系统管理起来，而且进程的创建和清除等服务必须由操作系统提供，而且在“运行”与“就绪”/“等待”状态之间的转换，涉及到保存和恢复进程的“执行现场”，也就是进程上下文，这是确保进程即使“断断续续”地执行，也能正确完成工作的必要保证。
>
> #### 3. 进程与线程
>
> 一个进程拥有一个存放程序和数据的的虚拟地址空间以及其他资源。一个进程基于程序的指令流执行，其执行过程可能与其它进程的执行过程交替进行。因此，一个具有执行状态（运行态、就绪态等）的进程是一个被操作系统分配资源（比如分配内存）并调度（比如分时使用CPU）的单位。在大多数操作系统中，这两个特点是进程的主要本质特征。但这两个特征相对独立，操作系统可以把这两个特征分别进行管理。
>
> 这样可以把拥有资源所有权的单位通常仍称作进程，对资源的管理成为进程管理；把指令执行流的单位称为线程，对线程的管理就是线程调度和线程分派。对属于同一进程的所有线程而言，这些线程共享进程的虚拟地址空间和其他资源，但每个线程都有一个独立的栈，还有独立的线程运行上下文，用于包含表示线程执行现场的寄存器值等信息。
>
> 在多线程环境中，进程被定义成资源分配与保护的单位，与进程相关联的信息主要有存放进程映像的虚拟地址空间等。在一个进程中，可能有一个或多个线程，每个线程有线程执行状态（运行、就绪、等待等），保存上次运行时的线程上下文、线程的执行栈等。考虑到CPU有不同的特权模式，参照进程的分类，线程又可进一步细化为用户线程和内核线程。
>
> 到目前为止，我们就可以明确用户进程、内核进程（可把ucore看成一个内核进程）、用户线程、内核线程的区别了。从本质上看，线程就是一个特殊的不用拥有资源的轻量级进程，在ucore的调度和执行管理中，并没有区分线程和进程。且由于ucore内核中的所有内核线程共享一个内核地址空间和其他资源，所以这些内核线程从属于同一个唯一的内核进程，即ucore内核本身。理解了进程或线程的上述属性和特征，就可以进行进程/线程管理的设计与实现了。但是为了叙述上的简便，以下用户态的进程/线程统称为用户进程。

## 实验

### 详细解析initproc生成及调度——ucore Lab4

```c
// file: kern/init/init.c
void
kern_init(void) {
    /* omitted */
	proc_init();
    /* omitted */
    cpu_idle();   
}
```

`kern_init`完成内存管理等操作后，调用`proc_init`，在其中完成了两个内核线程`idleproc`和`initproc`的创建。之后调用了`cpu_idle`完成了首次的内核线程的切换。

```c
// file: kern/process/proc.c
// proc_init - set up the first kernel thread idleproc "idle" by itself and 
//           - create the second kernel thread init_main
void
proc_init(void) {
    int i;

    list_init(&proc_list);					// 初始化链接进程PCB和线程TCB的双向链表
    for (i = 0; i < HASH_LIST_SIZE; i ++) {
        list_init(hash_list + i);			// 为了快速查找，使用了哈希方式，哈希一样的链接在一起
    }

    if ((idleproc = alloc_proc()) == NULL) {// 分配proc_struct空间并初始化，ucore中TCB和PCB的
        panic("cannot alloc idleproc.\n");	// 结构一样
    }

    idleproc->pid = 0;						// 初始化idle_proc进程信息，idle_proc就是内核当前执
    idleproc->state = PROC_RUNNABLE;		// 行的上下文。初始化完成后就没有其他的工作了，所以将其
    idleproc->kstack = (uintptr_t)bootstack;// need_resched设为1，调度其他进程(线程)占用CPU进行
    idleproc->need_resched = 1;				// 工作
    set_proc_name(idleproc, "idle");
    nr_process ++;

    current = idleproc;						// 设置当前进程(线程)

    int pid = kernel_thread(init_main, "Hello world!!", 0);	// 创建第二个内核线程
    if (pid <= 0) {
        panic("create init_main failed.\n");
    }

    initproc = find_proc(pid);
    set_proc_name(initproc, "init");

    assert(idleproc != NULL && idleproc->pid == 0);
    assert(initproc != NULL && initproc->pid == 1);
}
```

`kernel_thread`使用函数`fn`，参数`arg`创建内核线程

```c
// kernel_thread - create a kernel thread using "fn" function
// NOTE: the contents of temp trapframe tf will be copied to 
//       proc->tf in do_fork-->copy_thread function
int
kernel_thread(int (*fn)(void *), void *arg, uint32_t clone_flags) {
    struct trapframe tf;					// 在内核栈中临时分配中断帧
    memset(&tf, 0, sizeof(struct trapframe));
    tf.tf_cs = KERNEL_CS;
    tf.tf_ds = tf.tf_es = tf.tf_ss = KERNEL_DS;
    tf.tf_regs.reg_ebx = (uint32_t)fn;		// 设置函数执行起始地址
    tf.tf_regs.reg_edx = (uint32_t)arg;		// 设置参数数组的起始地址
    tf.tf_eip = (uint32_t)kernel_thread_entry;	// 设置线程开始执行的起始地址
    return do_fork(clone_flags | CLONE_VM, 0, &tf);	// 创建线程空间
}
```

`kernel_thread_entry`在`kern/process/entry.S`中。

```asm
.text
.globl kernel_thread_entry
kernel_thread_entry:        # void kernel_thread(void)

    pushl %edx              # push arg		// 将函数的第一个参数压栈
    call *%ebx              # call fn		// 调用线程执行例程

    pushl %eax              # save the return value of fn(arg)	// 将函数执行结果，作为do_exit
    						#									// 参数压栈
    call do_exit            # call do_exit to terminate current thread // 执行线程退出操作


```

`do_fork`根据clone_flags实际分配进程的空间。

```c
/* do_fork -     parent process for a new child process
 * @clone_flags: used to guide how to clone the child process
 * @stack:       the parent's user stack pointer. if stack==0, It means to fork a kernel thread.
 * @tf:          the trapframe info, which will be copied to child process's proc->tf
 */
int
do_fork(uint32_t clone_flags, uintptr_t stack, struct trapframe *tf) {
    int ret = -E_NO_FREE_PROC;
    struct proc_struct *proc;
    if (nr_process >= MAX_PROCESS) {
        goto fork_out;
    }
    ret = -E_NO_MEM;
    //LAB4:EXERCISE2 YOUR CODE
    /*
     * Some Useful MACROs, Functions and DEFINEs, you can use them in below implementation.
     * MACROs or Functions:
     *   alloc_proc:   create a proc struct and init fields (lab4:exercise1)
     *   setup_kstack: alloc pages with size KSTACKPAGE as process kernel stack
     *   copy_mm:      process "proc" duplicate OR share process "current"'s mm according clone_flags
     *                 if clone_flags & CLONE_VM, then "share" ; else "duplicate"
     *   copy_thread:  setup the trapframe on the  process's kernel stack top and
     *                 setup the kernel entry point and stack of process
     *   hash_proc:    add proc into proc hash_list
     *   get_pid:      alloc a unique pid for process
     *   wakeup_proc:  set proc->state = PROC_RUNNABLE
     * VARIABLES:
     *   proc_list:    the process set's list
     *   nr_process:   the number of process set
     */

    //    1. call alloc_proc to allocate a proc_struct
    //    2. call setup_kstack to allocate a kernel stack for child process
    //    3. call copy_mm to dup OR share mm according clone_flag
    //    4. call copy_thread to setup tf & context in proc_struct
    //    5. insert proc_struct into hash_list && proc_list
    //    6. call wakeup_proc to make the new child process RUNNABLE
    //    7. set ret vaule using child proc's pid
    if ((proc = alloc_proc()) == NULL) {		// 创建TCB(PCB)
        goto fork_out;
    }
    proc->parent = current;						// 设置母进程

    if (setup_kstack(proc) != 0) {				// 创建内核栈，每个线程都有自己的栈
        goto bad_fork_cleanup_proc;
    }
    if (copy_mm(clone_flags, proc) != 0) {		// CLONE_VM指共享程序空间，这里指内核空间
        goto bad_fork_cleanup_kstack;
    }
    copy_thread(proc, stack, tf);				// 设置中断帧和上下文

    bool intr_flag;
    local_intr_save(intr_flag);
    {
        proc->pid = get_pid();
        hash_proc(proc);
        list_add(&proc_list, &(proc->list_link));
        nr_process++;
    }
    local_intr_restore(intr_flag);
    
    wakeup_proc(proc);
    ret = proc->pid;
fork_out:
    return ret;

bad_fork_cleanup_kstack:
    put_kstack(proc);
bad_fork_cleanup_proc:
    kfree(proc);
    goto fork_out;
}
```

`copy_thread`

```c
// copy_thread - setup the trapframe on the  process's kernel stack top and
//             - setup the kernel entry point and stack of process
static void
copy_thread(struct proc_struct *proc, uintptr_t esp, struct trapframe *tf) {
    proc->tf = (struct trapframe *)(proc->kstack + KSTACKSIZE) - 1; // 在程序内核栈开始地方分
    																// 配一个trapframe空间
    *(proc->tf) = *tf;						// 设置程序
    proc->tf->tf_regs.reg_eax = 0;
    proc->tf->tf_esp = esp;                             // set for user process
    proc->tf->tf_eflags |= FL_IF;

    proc->context.eip = (uintptr_t)forkret;
    proc->context.esp = (uintptr_t)(proc->tf);
}
```



