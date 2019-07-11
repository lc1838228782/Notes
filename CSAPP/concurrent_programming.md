并发编程的三种方法：进程，I/O多路复用，线程。

## 进程

开销较大。父子进程共享文件表（理解不具体），需要关闭一下描述符。

## I/O多路复用
`#include <sys/select.h>`

select函数挂起进程等待描述符读写就绪。

虽然存在缺点，但目前高性能服务器（如Node.js，nginx和Tornado）使用的都是基于I/O的事件驱动的编程方式。

## 线程

每个线程例程都以一个指针作为输入，并返回一个通用指针。如果传递或返回多个参数，可以定义结构。例程示例：`void *thread(void *vargp)`

1. 创建线程

```c
#include <pthread.h>
typedef void *(func)(void *);

int pthread_create(pthread_t *tid, pthread_attr_t *attr,
                   func *f, void *arg);
// 成功返回0,出错非零
```
tid用来存储所创建线程的ID， attr改变新创建线程的属性， f线程例程，arg输入变量

新线程可以调用`pthread_t pthread_self(void);`获取自己的线程ID。

2. 终止线程

方式一：

```c
#include <pthread.h>

void pthread_exit(void *thread_return);
```
调用`pthread_exit`函数显示终止线程，如果主线程调用，他会等待所有其他对等线程终止，然后终止主线程和整个进程。

方式二：

如果某个对等线程调用exit函数，该函数终止进程及有关的线程。

方式三：

```c
#include <pthread.h>

int pthread_cancel(pthread_t tid);
// 成功返回0,出错返回非0
```
一个对等线程通过以当前线程ID作为参数调用`pthread_cancel`来终止当前线程。

3. 回收已终止线程的资源

```c
#include <pthread.h>

int pthread_join(pthread_t tid, void **thread_return);
// 成功返回0,失败非零
```

pthread_join函数会阻塞，直到线程tid终止，将线程返回的通用指针赋值给thread_return所指向的位置。
> 与wait不同，pthread_join只能等待一个指定线程

4. 分离线程

在任何一个时间点上，线程是可结合的(joinable)或者分离的(detached)。

一个可结合的线程可以被其他线程收回和杀死。在被其他线程回收之前，他的内存资源是不释放的。默认情况下被创建为可结合的，为了避免内存泄漏，每个可结合线程都应该被其他线程显式的收回。

一个分离的线程是不能被其他线程回收或者杀死的。资源在终止时由系统释放。

```c
#include <pthread.h>

int pthread_detach(pthread_t tid);
```

## 12.5 用信号量同步线程

```c
#include <semaphore.h>

int sem_init(sem_t *sem, 0, unsigned int value);
int sem_wait(sem_t *s);     // P(s)
int sem_post(sem_t *s);     // V(s)
```
生产者消费者问题，读者写者问题。

### 基于预线程化的并发服务器

## 12.6 刻画并行程序的性能
1. 强扩展（strong scaling）
- 绝对加速比：顺序执行程序的时间与并行程序执行时间的比值
- 相对加速比：并行程序在单核执行的时间与并行程序执行的时间的比值
- 效率：加速比与核数的比值。衡量有用工作的比例

2. 弱扩展
增加处理器数量所增加的完成工作量。这里同样存在加速比，效率概念。

例如：如果将处理器翻倍，同时每个小时完成了两倍的工作量，那么就有线性加速比和100%效率。

## 12.7 其他并发问题

### 12.7.1 线程安全

线程不安全的四个例子：
1. 不保护共享变量的函数

PV操作保护

2. 保持跨越多个调用的状态的函数

例如：rand函数，当前调用以来于前一次的中间结果

3. 返回指向静态变量的指针的函数

指向一块static变量中。解决方法：lock and copy，加锁执行函数，执行完毕将结果复制到私有的内存区域。

4. 调用线程不安全的函数

函数调用线程不安全的函数不一定线程不安全。

### 12.7.2 可重入性

当它们被多个线程调用时，不会引用任何共享数据。

### 12.7.3 在线程化程序中引用已存在的库函数

大多数Linux函数都是线程安全的，仅少部分不安全。P719列出了部分函数。

Linux系统提供了大多数不安全函数的可重入版本，名字以_r结尾。例如：asctime的可冲入版本为asctime_r。

### 12.7.4 竞争

多线程程序必须对任何可行的轨迹线都能正确工作。

分配块方法。

### 12.7.5 死锁

二元信号量避免死锁的方法：互斥锁加锁顺序规则（某种顺序加锁，相反顺序释放锁）