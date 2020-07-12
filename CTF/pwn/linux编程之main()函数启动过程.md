## 1 最简单的程序 

1)编辑helloworld程序

```c
#include <stdio.h>

int main (int argc, char *argv[])
{
    printf("Hello world!\n");
	return 0;
}
```



2) 编译，`$ gcc helloworld.c -o helloworld`
3) 运行，​`$ ./helloworld `
Hello world!

## 2 最简单的程序其实不简单

上面这个helloword程序已经再简单不过了，先来看一看它的反汇编代码：

```assembly
helloworld:     file format elf32-i386
 
 
Disassembly of section .init:
 
080482b0 <_init>:
 80482b0:	53                   	push   %ebx
 80482b1:	83 ec 08             	sub    $0x8,%esp
 80482b4:	e8 00 00 00 00       	call   80482b9 <_init+0x9>
 80482b9:	5b                   	pop    %ebx
 80482ba:	81 c3 3b 1d 00 00    	add    $0x1d3b,%ebx
 80482c0:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 80482c6:	85 c0                	test   %eax,%eax
 80482c8:	74 05                	je     80482cf <_init+0x1f>
 80482ca:	e8 31 00 00 00       	call   8048300 <__gmon_start__@plt>
 80482cf:	e8 dc 00 00 00       	call   80483b0 <frame_dummy>
 80482d4:	e8 97 01 00 00       	call   8048470 <__do_global_ctors_aux>
 80482d9:	83 c4 08             	add    $0x8,%esp
 80482dc:	5b                   	pop    %ebx
 80482dd:	c3                   	ret    
 
Disassembly of section .plt:
 
080482e0 <puts@plt-0x10>:
 80482e0:	ff 35 f8 9f 04 08    	pushl  0x8049ff8
 80482e6:	ff 25 fc 9f 04 08    	jmp    *0x8049ffc
 80482ec:	00 00                	add    %al,(%eax)
	...
 
080482f0 <puts@plt>:
 80482f0:	ff 25 00 a0 04 08    	jmp    *0x804a000
 80482f6:	68 00 00 00 00       	push   $0x0
 80482fb:	e9 e0 ff ff ff       	jmp    80482e0 <_init+0x30>
 
08048300 <__gmon_start__@plt>:
 8048300:	ff 25 04 a0 04 08    	jmp    *0x804a004
 8048306:	68 08 00 00 00       	push   $0x8
 804830b:	e9 d0 ff ff ff       	jmp    80482e0 <_init+0x30>
 
08048310 <__libc_start_main@plt>:
 8048310:	ff 25 08 a0 04 08    	jmp    *0x804a008
 8048316:	68 10 00 00 00       	push   $0x10
 804831b:	e9 c0 ff ff ff       	jmp    80482e0 <_init+0x30>
 
Disassembly of section .text:
 
08048320 <_start>:
 8048320:	31 ed                	xor    %ebp,%ebp
 8048322:	5e                   	pop    %esi
 8048323:	89 e1                	mov    %esp,%ecx
 8048325:	83 e4 f0             	and    $0xfffffff0,%esp
 8048328:	50                   	push   %eax
 8048329:	54                   	push   %esp
 804832a:	52                   	push   %edx
 804832b:	68 60 84 04 08       	push   $0x8048460
 8048330:	68 f0 83 04 08       	push   $0x80483f0
 8048335:	51                   	push   %ecx
 8048336:	56                   	push   %esi
 8048337:	68 d4 83 04 08       	push   $0x80483d4
 804833c:	e8 cf ff ff ff       	call   8048310 <__libc_start_main@plt>
 8048341:	f4                   	hlt    
 8048342:	90                   	nop
 8048343:	90                   	nop
 8048344:	90                   	nop
 8048345:	90                   	nop
 8048346:	90                   	nop
 8048347:	90                   	nop
 8048348:	90                   	nop
 8048349:	90                   	nop
 804834a:	90                   	nop
 804834b:	90                   	nop
 804834c:	90                   	nop
 804834d:	90                   	nop
 804834e:	90                   	nop
 804834f:	90                   	nop
 
08048350 <__do_global_dtors_aux>:
 8048350:	55                   	push   %ebp
 8048351:	89 e5                	mov    %esp,%ebp
 8048353:	53                   	push   %ebx
 8048354:	83 ec 04             	sub    $0x4,%esp
 8048357:	80 3d 14 a0 04 08 00 	cmpb   $0x0,0x804a014
 804835e:	75 3f                	jne    804839f <__do_global_dtors_aux+0x4f>
 8048360:	a1 18 a0 04 08       	mov    0x804a018,%eax
 8048365:	bb 20 9f 04 08       	mov    $0x8049f20,%ebx
 804836a:	81 eb 1c 9f 04 08    	sub    $0x8049f1c,%ebx
 8048370:	c1 fb 02             	sar    $0x2,%ebx
 8048373:	83 eb 01             	sub    $0x1,%ebx
 8048376:	39 d8                	cmp    %ebx,%eax
 8048378:	73 1e                	jae    8048398 <__do_global_dtors_aux+0x48>
 804837a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048380:	83 c0 01             	add    $0x1,%eax
 8048383:	a3 18 a0 04 08       	mov    %eax,0x804a018
 8048388:	ff 14 85 1c 9f 04 08 	call   *0x8049f1c(,%eax,4)
 804838f:	a1 18 a0 04 08       	mov    0x804a018,%eax
 8048394:	39 d8                	cmp    %ebx,%eax
 8048396:	72 e8                	jb     8048380 <__do_global_dtors_aux+0x30>
 8048398:	c6 05 14 a0 04 08 01 	movb   $0x1,0x804a014
 804839f:	83 c4 04             	add    $0x4,%esp
 80483a2:	5b                   	pop    %ebx
 80483a3:	5d                   	pop    %ebp
 80483a4:	c3                   	ret    
 80483a5:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi
 80483a9:	8d bc 27 00 00 00 00 	lea    0x0(%edi,%eiz,1),%edi
 
080483b0 <frame_dummy>:
 80483b0:	55                   	push   %ebp
 80483b1:	89 e5                	mov    %esp,%ebp
 80483b3:	83 ec 18             	sub    $0x18,%esp
 80483b6:	a1 24 9f 04 08       	mov    0x8049f24,%eax
 80483bb:	85 c0                	test   %eax,%eax
 80483bd:	74 12                	je     80483d1 <frame_dummy+0x21>
 80483bf:	b8 00 00 00 00       	mov    $0x0,%eax
 80483c4:	85 c0                	test   %eax,%eax
 80483c6:	74 09                	je     80483d1 <frame_dummy+0x21>
 80483c8:	c7 04 24 24 9f 04 08 	movl   $0x8049f24,(%esp)
 80483cf:	ff d0                	call   *%eax
 80483d1:	c9                   	leave  
 80483d2:	c3                   	ret    
 80483d3:	90                   	nop
 
080483d4 <main>:
 80483d4:	55                   	push   %ebp
 80483d5:	89 e5                	mov    %esp,%ebp
 80483d7:	83 e4 f0             	and    $0xfffffff0,%esp
 80483da:	83 ec 10             	sub    $0x10,%esp
 80483dd:	c7 04 24 c0 84 04 08 	movl   $0x80484c0,(%esp)
 80483e4:	e8 07 ff ff ff       	call   80482f0 <puts@plt>
 80483e9:	b8 00 00 00 00       	mov    $0x0,%eax
 80483ee:	c9                   	leave  
 80483ef:	c3                   	ret    
 
080483f0 <__libc_csu_init>:
 80483f0:	55                   	push   %ebp
 80483f1:	57                   	push   %edi
 80483f2:	56                   	push   %esi
 80483f3:	53                   	push   %ebx
 80483f4:	e8 69 00 00 00       	call   8048462 <__i686.get_pc_thunk.bx>
 80483f9:	81 c3 fb 1b 00 00    	add    $0x1bfb,%ebx
 80483ff:	83 ec 1c             	sub    $0x1c,%esp
 8048402:	8b 6c 24 30          	mov    0x30(%esp),%ebp
 8048406:	8d bb 20 ff ff ff    	lea    -0xe0(%ebx),%edi
 804840c:	e8 9f fe ff ff       	call   80482b0 <_init>
 8048411:	8d 83 20 ff ff ff    	lea    -0xe0(%ebx),%eax
 8048417:	29 c7                	sub    %eax,%edi
 8048419:	c1 ff 02             	sar    $0x2,%edi
 804841c:	85 ff                	test   %edi,%edi
 804841e:	74 29                	je     8048449 <__libc_csu_init+0x59>
 8048420:	31 f6                	xor    %esi,%esi
 8048422:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048428:	8b 44 24 38          	mov    0x38(%esp),%eax
 804842c:	89 2c 24             	mov    %ebp,(%esp)
 804842f:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048433:	8b 44 24 34          	mov    0x34(%esp),%eax
 8048437:	89 44 24 04          	mov    %eax,0x4(%esp)
 804843b:	ff 94 b3 20 ff ff ff 	call   *-0xe0(%ebx,%esi,4)
 8048442:	83 c6 01             	add    $0x1,%esi
 8048445:	39 fe                	cmp    %edi,%esi
 8048447:	75 df                	jne    8048428 <__libc_csu_init+0x38>
 8048449:	83 c4 1c             	add    $0x1c,%esp
 804844c:	5b                   	pop    %ebx
 804844d:	5e                   	pop    %esi
 804844e:	5f                   	pop    %edi
 804844f:	5d                   	pop    %ebp
 8048450:	c3                   	ret    
 8048451:	eb 0d                	jmp    8048460 <__libc_csu_fini>
 8048453:	90                   	nop
 8048454:	90                   	nop
 8048455:	90                   	nop
 8048456:	90                   	nop
 8048457:	90                   	nop
 8048458:	90                   	nop
 8048459:	90                   	nop
 804845a:	90                   	nop
 804845b:	90                   	nop
 804845c:	90                   	nop
 804845d:	90                   	nop
 804845e:	90                   	nop
 804845f:	90                   	nop
 
08048460 <__libc_csu_fini>:
 8048460:	f3 c3                	repz ret 
 
08048462 <__i686.get_pc_thunk.bx>:
 8048462:	8b 1c 24             	mov    (%esp),%ebx
 8048465:	c3                   	ret    
 8048466:	90                   	nop
 8048467:	90                   	nop
 8048468:	90                   	nop
 8048469:	90                   	nop
 804846a:	90                   	nop
 804846b:	90                   	nop
 804846c:	90                   	nop
 804846d:	90                   	nop
 804846e:	90                   	nop
 804846f:	90                   	nop
 
08048470 <__do_global_ctors_aux>:
 8048470:	55                   	push   %ebp
 8048471:	89 e5                	mov    %esp,%ebp
 8048473:	53                   	push   %ebx
 8048474:	83 ec 04             	sub    $0x4,%esp
 8048477:	a1 14 9f 04 08       	mov    0x8049f14,%eax
 804847c:	83 f8 ff             	cmp    $0xffffffff,%eax
 804847f:	74 13                	je     8048494 <__do_global_ctors_aux+0x24>
 8048481:	bb 14 9f 04 08       	mov    $0x8049f14,%ebx
 8048486:	66 90                	xchg   %ax,%ax
 8048488:	83 eb 04             	sub    $0x4,%ebx
 804848b:	ff d0                	call   *%eax
 804848d:	8b 03                	mov    (%ebx),%eax
 804848f:	83 f8 ff             	cmp    $0xffffffff,%eax
 8048492:	75 f4                	jne    8048488 <__do_global_ctors_aux+0x18>
 8048494:	83 c4 04             	add    $0x4,%esp
 8048497:	5b                   	pop    %ebx
 8048498:	5d                   	pop    %ebp
 8048499:	c3                   	ret    
 804849a:	90                   	nop
 804849b:	90                   	nop
 
Disassembly of section .fini:
 
0804849c <_fini>:
 804849c:	53                   	push   %ebx
 804849d:	83 ec 08             	sub    $0x8,%esp
 80484a0:	e8 00 00 00 00       	call   80484a5 <_fini+0x9>
 80484a5:	5b                   	pop    %ebx
 80484a6:	81 c3 4f 1b 00 00    	add    $0x1b4f,%ebx
 80484ac:	e8 9f fe ff ff       	call   8048350 <__do_global_dtors_aux>
 80484b1:	83 c4 08             	add    $0x8,%esp
 80484b4:	5b                   	pop    %ebx
 80484b5:	c3                   	ret
```



从反汇编结果来看，helloworld可执行文件并不只是main函数，除main外，还有几个重要的函数，`_init`，`_fini`，`_start`，`__libc_start_main`，`__libc_csu_init`，`__libc_csu_fini`等。

除main函数外，其它函数从哪来的？分析一下编译好的可执行文件helloworld的库依赖关系如下：

```shell
$ ldd helloworld
	linux-gate.so.1 =>  (0x00876000)
	libc.so.6 => /lib/i386-linux-gnu/libc.so.6 (0x00110000)
	/lib/ld-linux.so.2 (0x006a3000)
```

其中linux-gate.so.1为系统调用相关的动态库，ld-linux.so.2为动态链接库，libc.so.6为GNU C库。
从反汇编出来的相关函数名称(`__libc_start_main`)来看，我们把目标锁定在GNU C库，即glibc，也是GNU开源项目，可以从GNU 源码ftp站点(http://ftp.gnu.org/gnu/libc/)上找到。
`__libc_start_main`函数可以在glibc源码的csu/libc-start.c中找到：

```c

STATIC int LIBC_START_MAIN (int (*main) (int, char **, char **
					 MAIN_AUXVEC_DECL),
			    int argc,
			    char *__unbounded *__unbounded ubp_av,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
			    ElfW(auxv_t) *__unbounded auxvec,
#endif
			    __typeof (main) init,
			    void (*fini) (void),
			    void (*rtld_fini) (void),
			    void *__unbounded stack_end)
     __attribute__ ((noreturn));
```

`__libc_csu_init`，`__libc_csu_fini`函数可以在glibc源码的csu/elf-init.c中找到：

```c
void
__libc_csu_init (int argc, char **argv, char **envp)
{
  /* For dynamically linked executables the preinit array is executed by
     the dynamic linker (before initializing any shared object.  */
 
#ifndef LIBC_NONSHARED
  /* For static executables, preinit happens right before init.  */
  {
    const size_t size = __preinit_array_end - __preinit_array_start;
    size_t i;
    for (i = 0; i < size; i++)
      (*__preinit_array_start [i]) (argc, argv, envp);
  }
#endif
  _init ();
  const size_t size = __init_array_end - __init_array_start;
  for (size_t i = 0; i < size; i++)
      (*__init_array_start [i]) (argc, argv, envp);
}
/* This function should not be used anymore.  We run the executable's
   destructor now just like any other.  We cannot remove the function,
   though.  */
void
__libc_csu_fini (void)
{
#ifndef LIBC_NONSHARED
  size_t i = __fini_array_end - __fini_array_start;
  while (i-- > 0)
    (*__fini_array_start [i]) ();
 
  _fini ();
#endif
}
```

`_start`函数可以在glibc源码的sysdeps/i386/elf/start.S中找到：

```assembly

#include "bp-sym.h"
 
	.text
	.globl _start
	.type _start,@function
_start:
	/* Clear the frame pointer.  The ABI suggests this be done, to mark
	   the outermost frame obviously.  */
	xorl %ebp, %ebp
 
	/* Extract the arguments as encoded on the stack and set up
	   the arguments for `main': argc, argv.  envp will be determined
	   later in __libc_start_main.  */
	popl %esi		/* Pop the argument count.  */
	movl %esp, %ecx		/* argv starts just at the current stack top.*/
 
	/* Before pushing the arguments align the stack to a 16-byte
	(SSE needs 16-byte alignment) boundary to avoid penalties from
	misaligned accesses.  Thanks to Edward Seidl <seidl@janed.com>
	for pointing this out.  */
	andl $0xfffffff0, %esp
	pushl %eax		/* Push garbage because we allocate
				   28 more bytes.  */
 
	/* Provide the highest stack address to the user code (for stacks
	   which grow downwards).  */
	pushl %esp
 
	pushl %edx		/* Push address of the shared library
				   termination function.  */
 
#ifdef SHARED
	/* Load PIC register.  */
	call 1f
	addl $_GLOBAL_OFFSET_TABLE_, %ebx
 
	/* Push address of our own entry points to .fini and .init.  */
	leal __libc_csu_fini@GOTOFF(%ebx), %eax
	pushl %eax
	leal __libc_csu_init@GOTOFF(%ebx), %eax
	pushl %eax
 
	pushl %ecx		/* Push second argument: argv.  */
	pushl %esi		/* Push first argument: argc.  */
 
	pushl BP_SYM (main)@GOT(%ebx)
 
	/* Call the user's main function, and exit with its value.
	   But let the libc call main.    */
	call BP_SYM (__libc_start_main)@PLT
#else
	/* Push address of our own entry points to .fini and .init.  */
	pushl $__libc_csu_fini
	pushl $__libc_csu_init
 
	pushl %ecx		/* Push second argument: argv.  */
	pushl %esi		/* Push first argument: argc.  */
 
	pushl $BP_SYM (main)
 
	/* Call the user's main function, and exit with its value.
	   But let the libc call main.    */
	call BP_SYM (__libc_start_main)
#endif
 
	hlt			/* Crash if somehow `exit' does return.  */
```



`_init`，`_fini`函数可以在glibc源码的sysdeps/generic/initfini.c中找到：

```c
/* The beginning of _init:  */
asm ("\n/*@_init_PROLOG_BEGINS*/");
 
static void
call_gmon_start(void)
{
  extern void __gmon_start__ (void) __attribute__ ((weak)); /*weak_extern (__gmon_start__);*/
  void (*gmon_start) (void) = __gmon_start__;
 
  if (gmon_start)
    gmon_start ();
}
 
SECTION (".init");
extern void __attribute__ ((section (".init"))) _init (void);
void
_init (void)
{
  /* We cannot use the normal constructor mechanism in gcrt1.o because it
     appears before crtbegin.o in the link, so the header elt of .ctors
     would come after the elt for __gmon_start__.  One approach is for
     gcrt1.o to reference a symbol which would be defined by some library
     module which has a constructor; but then user code's constructors
     would come first, and not be profiled.  */
  call_gmon_start ();
 
  asm ("ALIGN");
  asm("END_INIT");
  /* Now the epilog. */
  asm ("\n/*@_init_PROLOG_ENDS*/");
  asm ("\n/*@_init_EPILOG_BEGINS*/");
  SECTION(".init");
}
asm ("END_INIT");
 
/* End of the _init epilog, beginning of the _fini prolog. */
asm ("\n/*@_init_EPILOG_ENDS*/");
asm ("\n/*@_fini_PROLOG_BEGINS*/");
 
SECTION (".fini");
extern void __attribute__ ((section (".fini"))) _fini (void);
void
_fini (void)
{
 
  /* End of the _fini prolog. */
  asm ("ALIGN");
  asm ("END_FINI");
  asm ("\n/*@_fini_PROLOG_ENDS*/");
 
  {
    /* Let GCC know that _fini is not a leaf function by having a dummy
       function call here.  We arrange for this call to be omitted from
       either crt file.  */
    extern void i_am_not_a_leaf (void);
    i_am_not_a_leaf ();
  }
 
  /* Beginning of the _fini epilog. */
  asm ("\n/*@_fini_EPILOG_BEGINS*/");
  SECTION (".fini");
}
asm ("END_FINI");
 
/* End of the _fini epilog.  Any further generated assembly (e.g. .ident)
   is shared between both crt files. */
asm ("\n/*@_fini_EPILOG_ENDS*/");
asm ("\n/*@TRAILER_BEGINS*/");
```

但是为什么不能在libc.so.6中找到`_start`，`_init`，`_fini`这三个函数呢？
是因为GNU把这三个作为了程序启动和结束的最基本运行库函数，分别放在crt1.o，crti.o，crtn.o这三个object文件中供程序链接时使用。
从glibc的源码sysdeps/generic/initfini.c相关注释也可以看出：

```c
/* This file is compiled into assembly code which is then munged by a sed
   script into two files: crti.s and crtn.s.
 
   * crti.s puts a function prologue at the beginning of the
   .init and .fini sections and defines global symbols for
   those addresses, so they can be called as functions.
 
   * crtn.s puts the corresponding function epilogues
   in the .init and .fini sections. */
```

从上面注释来看crti.o，crtn.o分别包含.init和.fini段的开头和结束部分，分析它们的反汇编代码也可以看出：
`$ objdump -d /usr/lib/i386-linux-gnu/crti.o `

```assembly
/usr/lib/i386-linux-gnu/crti.o:     file format elf32-i386
 
 
Disassembly of section .init:
 
00000000 <_init>:
   0:	53                   	push   %ebx
   1:	83 ec 08             	sub    $0x8,%esp
   4:	e8 00 00 00 00       	call   9 <_init+0x9>
   9:	5b                   	pop    %ebx
   a:	81 c3 03 00 00 00    	add    $0x3,%ebx
  10:	8b 83 00 00 00 00    	mov    0x0(%ebx),%eax
  16:	85 c0                	test   %eax,%eax
  18:	74 05                	je     1f <_init+0x1f>
  1a:	e8 fc ff ff ff       	call   1b <_init+0x1b>
 
Disassembly of section .fini:
 
00000000 <_fini>:
   0:	53                   	push   %ebx
   1:	83 ec 08             	sub    $0x8,%esp
   4:	e8 00 00 00 00       	call   9 <_fini+0x9>
   9:	5b                   	pop    %ebx
   a:	81 c3 03 00 00 00    	add    $0x3,%ebx

```

`$ objdump -d /usr/lib/i386-linux-gnu/crtn.o `

```assembly

 /usr/lib/i386-linux-gnu/crtn.o:     file format elf32-i386
 
 
Disassembly of section .init:
 
00000000 <.init>:
   0:	83 c4 08             	add    $0x8,%esp
   3:	5b                   	pop    %ebx
   4:	c3                   	ret    
 
Disassembly of section .fini:
 
00000000 <.fini>:
   0:	83 c4 08             	add    $0x8,%esp
   3:	5b                   	pop    %ebx
   4:	c3                   	ret
```



回过头来看看helloworld程序反汇编代码中的`_init`，`_fini`函数的组成：

```assembly
080482b0 <_init>:
 80482b0:	53                   	push   %ebx
 80482b1:	83 ec 08             	sub    $0x8,%esp
 80482b4:	e8 00 00 00 00       	call   80482b9 <_init+0x9>
 80482b9:	5b                   	pop    %ebx
 80482ba:	81 c3 3b 1d 00 00    	add    $0x1d3b,%ebx
 80482c0:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 80482c6:	85 c0                	test   %eax,%eax
 80482c8:	74 05                	je     80482cf <_init+0x1f>
```



```assembly
80482ca:	e8 31 00 00 00       	call   8048300 <__gmon_start__@plt>
 80482cf:	e8 dc 00 00 00       	call   80483b0 <frame_dummy>
 80482d4:	e8 97 01 00 00       	call   8048470 <__do_global_ctors_aux>
```



```assembly

 80482d9:	83 c4 08             	add    $0x8,%esp
 80482dc:	5b                   	pop    %ebx
 80482dd:	c3                   	ret
```



```assembly

0804849c <_fini>:
 804849c:	53                   	push   %ebx
 804849d:	83 ec 08             	sub    $0x8,%esp
 80484a0:	e8 00 00 00 00       	call   80484a5 <_fini+0x9>
 80484a5:	5b                   	pop    %ebx
 80484a6:	81 c3 4f 1b 00 00    	add    $0x1b4f,%ebx
```



```assembly
80484ac:	e8 9f fe ff ff       	call   8048350 <__do_global_dtors_aux>
```



```assembly
 80484b1:	83 c4 08             	add    $0x8,%esp
 80484b4:	5b                   	pop    %ebx
 80484b5:	c3                   	ret 
```



helloworld程序反汇编代码中的`_init`，`_fini`函数中间多出来的部分是其它库或用户自定义的`.init`和`.fini`段代码。
从上面helloworld程序反汇编来看，程序启动的过程应该：
`_start` -> `__libc_start_main -> main`. 具体一点就是：
`_start` -> `__libc_start_main` -> `__libc_csu_init` -> `main`. 再具体一点就是：
`_start` -> `__libc_start_main` -> `__libc_csu_init` -> `_init` -> `main` -> `_fini`.
可以通过分析`__libc_start_main`函数进一步了解，下面的`__libc_start_main`函数是我精简后的伪代码：

```c
/* Note: the fini parameter is ignored here for shared library.  It
   is registered with __cxa_atexit.  This had the disadvantage that
   finalizers were called in more than one place.  */
STATIC int
LIBC_START_MAIN (int (*main) (int, char **, char ** MAIN_AUXVEC_DECL),
		 int argc, char *__unbounded *__unbounded ubp_av,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
		 ElfW(auxv_t) *__unbounded auxvec,
#endif
		 __typeof (main) init,
		 void (*fini) (void),
		 void (*rtld_fini) (void), void *__unbounded stack_end)
{
  ...
  /* Register the destructor of the dynamic linker if there is any.  */
  if (__builtin_expect (rtld_fini != NULL, 1))
    __cxa_atexit ((void (*) (void *)) rtld_fini, NULL, NULL);
 
  /* Call the initializer of the libc.  This is only needed here if we
     are compiling for the static library in which case we haven't
     run the constructors in `_dl_start_user'.  */
  __libc_init_first (argc, argv, __environ);
 
  /* Register the destructor of the program, if any.  */
  if (fini)
    __cxa_atexit ((void (*) (void *)) fini, NULL, NULL);
 
  /* Call the initializer of the program, if any.  */
  if (init)
    (*init) (argc, argv, __environ MAIN_AUXVEC_PARAM);
 
  /* Nothing fancy, just call the function.  */
  result = main (argc, argv, __environ MAIN_AUXVEC_PARAM);
 
  exit (result);
}
```


`__libc_start_main`函数的参数`main, argc, ubp_av, init, fini, rtld_fini`都是通过`_start`入栈得到。

其中`argc, ubp_av`为传递给`main`函数的参数`argc, argv`。`init`为`__libc_csu_init`函数指针，`fini`为`__libc_csu_fini`函数指针，`rtld_fini`为运行库加载收尾函数指针。

从`__libc_start_main`函数可以看出在`call init`之前还通过`__cxa_atexit`向`exit`函数注册了`rtlf_fini`和`fini`函数，目的是为了在`main`结束后`call exit`自动完成一些收尾工作。

这里的`_init`, `_fini`函数功能还主要负责完成C++程序全局/静态对象的构造与析构，有兴趣的可以深入一下。

`__cxa_atexit`函数用于注册main结束后程序退出时调用的函数，例如：

```shell
$ gcc helloworld2.c -o helloworld2
$ ./helloworld2
Hello world!
Will be exit!
```

可以看到`pre_exit`函数并不是靠`main`来执行的，而是靠`__libc_start_main`函数里面的`exit`函数来调用完成。

为了更进一步理解`_init`函数的作用，我们再改写一下代码：

因为链接器会自动把`.init`段粘贴到一起组成`_init`函数，所以为了不使`_init`函数半途返回退出，我们只能用下面汇编的方法定义哪些程序块是需要被链接到_init函数，且该程序块不能有函数的返回指令，这样我们必须在该程序块结束的地方加入汇编指令告诉编译器后面的程序不属于.init段，因此就有了下面这个测试函数`pre_init`，它会被link到两个section，即`.init`和`.text`段。

```c
#include <stdio.h>
#include <stdlib.h>

void __attribute__((used)) pre_init (void)
{
	asm (".section .init");
	printf("pre-init section .init part\n");
	printf("Call pre_init before main\n");
	asm (".section .text");
    printf("pre-init section .text part\n");
    printf("call pre_init in main\n");
}

void pre_exit (void)
{
    printf("Call pre_exit after main!\n");
}

int main (int argc, char *argv[])
{
    printf("Enter main\n");
    
    __cxa_atexit(pre_exit, NULL, NULL);

    printf("Hello world!\n");
 
    pre_init();

    printf("Exit main\n");

    return 0;
}
```

输出

```shell

$ gcc helloworld3.c -o helloworld3
$ ./helloworld3
pre-init section .init part
Call pre_init before main
Enter main
Hello world!
pre-init section .text part
call pre_init in main
Exit main
Call pre_exit after main!
```


从helloworld3程序的运行结果也可以看出该程序函数的调用过程：

`pre_init` (.init段部分 代码) -> `main` -> `pre_init` (.text段部分 代码) -> `pre_exit`.

同样我们也可以添加`.fini`段代码，链接器会把`.fini`段代码部分链接进`_fini`函数，这样`_fini`函数就会帮我们在`main`函数退出后自动执行这段代码：

```c
void __attribute__((used)) pre_fini (void)
{
    asm (".section .fini");
    printf("pre-fini section .fini part\n");
    printf("Call pre_fini after main\n");
    asm (".section .text");
}
```

输出

```shell
$ gcc helloworld4.c -o helloworld4
$ ./helloworld4
pre-init section .init part
Call pre_init before main
Enter main
Hello world!
pre-init section .text part
call pre_init in main
Exit main
Call pre_exit after main!
pre-fini section .fini part
Call pre_fini after main
```

从以上运行结果可知`pre_fini`函数是在`main`函数退出后才被调用的，但是为什么`pre_fini`打印的内容比`pre_exit`打印的内容要晚出来呢？这是因为`pre_exit`函数注册比`pre_fini`要晚，而通过`__cxa_atexit`注册的过程其实就是建立一个`exit`函数指针链表栈，按栈的规则是后进先出，因此`pre_exit`比`pre_fini`要先被调用。

从helloworld4程序的运行结果也可以看出该程序函数的调用过程：

`pre_init` (.init段部分 代码) -> `main` -> `pre_init` (.text段部分 代码) -> `pre_exit` -> `pre_fini` (.fini段部分 代码).

为了让程序正常运行和结束，链接器ld帮我们做了好多事情，可以用下面来表示：

`ld crt1.o crti.o [usr object] [lib] crtn.o `

最后总结一下，程序启动和结束的过程可以描述为： 

`_start` -> `__libc_start_main` ->  `init` -> `main` -> `exit`.

3 Self-check
如果想对main的启动及结束有进一步的理解，最好的方法就亲自读一读glibc的相关源码。

## 执行顺序



- __libc_csu_init执行.init和.init_array
- __libc_csu_fini执行.fini和.fini_array

并且执行顺序如下：

- __libc_csu_init
- main
- __libc_csu_fini

更细致的说顺序如下：

- .init
- .init_array[0]
- .init_array[1]
- …
- .init_array[n]
- main
- .fini_array[n]
- …
- .fini_array[1]
- .fini_array[0]
- .fini

https://blog.csdn.net/gary_ygl/article/details/8506007