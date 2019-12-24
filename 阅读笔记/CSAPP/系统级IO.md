## Unix I/O

**打开文件**，一个程序通过要求内核打开文件，宣告访问一个I/O设备。内核返回一个非负整数，叫做描述符，标识所有对此文件的操作。内核记录有关打开此文件的所有信息。

**默认文件**，每个进程开始时存在描述符：标准输入(0)，标准输出(1)，标准错误(2)。在头文件`<unistd.h>`通过`STDIN_FILENO`，`STDOUT_FILENO`，`STDERR_FILENO`定义。

**改变位置**，内核保持着一个文件位置k，是从文件开始的字节偏移量，seek函数可操作。

## 打开关闭文件，读写文件

```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(char *filename, int flags, mode_t mode)
```
成功返回新文件描述符，出错为-1。
每个进程都有umask，可限制mode。

```
#include <unistd.h>

int close(int fd)
```
成功返回0,出错返回1。

```
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t n);       //成功为读的字节数，EOF为0，出错为-1
ssize_t write(int fd, const void *buf, size_t n); //成功为写的字节数，出错为-1
```
ssize_t为signed的size_t

