# Lab7: 同步互斥

## 理论

互斥、死锁、饥饿

**互斥**是指某一资源同时只允许一个进程对其进行访问，具有唯一性和排它性，但互斥不用限制进程对资源的访问顺序，即访问可以是无序的。

**同步**是指在进程间的执行必须严格按照规定的某种先后次序来运行，即访问是有序的，这种先后次序取决于要系统完成的任务需求。

### 17 临界区

空闲则入、忙则等待、有限等待、让权等待（可选）

**实现的三种办法**：禁用中断、软件方法、更高级的抽象方法

方法比较主要考虑性能：并发级别

#### 禁用中断

```c
local_irq_save(unsigned long flags);
// critical section
local_irq_restore(unsigned long flags);
```

进入临界区：禁用所有中断，并保存标志

离开临界区：使能所有中断、并恢复标志

**缺点**：

- 禁用中断后，进程无法被停止：使整个系统都停下来；可能导致其他进程饥饿
- 临界区可能很长：无法确定响应中断所需的时间
- 不得不使用时，才采用

#### 基于软件的同步解决方法

**Peterson算法(双进程)**

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200229200859621.png)

**Dekkers算法(多进程)**

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200229201728124.png)

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200229201801745.png)

**方法分析**：复杂，需要进程间共享变量；需要忙等待，浪费CPU时间。

#### 更高级的抽象算法

现代CPU体系结构都提供一些特殊的原子操作指令。

**Test-and-Set**

相当于以下过程

```c
boolean TestAndSet (boolean *target)‏
{
	boolean rv = *target;
    *target = true;
    return rv:
}
```

**exchange**

```c
void Exchange (boolean *a, boolean *b)‏
{
	boolean temp = *a;
	*a = *b;
	*b = temp:
}
```

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/os%20lock.png)

图左：spinlock(自旋锁无法实现先进先出)

**原子操作指令锁特征**

**优点**：

- 适用于单处理器或共享主存的多处理器中的任意数量的进程同步
- 简单容易证明
- 支持多临界区

**缺点**

- 可能的忙等待，消耗处理器时间

- 可能导致饥饿

  进程离开临界区时，唤醒的次序

- 死锁

  拥有临界区的低优先级进程

  请求访问临界区的高优先级进程获得处理器并等待临界区

### 18 同步互斥

#### 信号量(semaphore)

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/semaphore.png)

**特性**：

-  信号量是被保护的整数变量
  - 初始化完成后，只能通过P()和V()操作修改
  - 由操作系统保证，PV是原子操作
- P()可能阻塞，V()不会阻塞

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/semaphore%20implement.png)

**两种信号量**：二进制信号量、资源信号量。(二者等价，基于一个可以实现另一个)

**使用**：互斥访问、条件同步

##### 生产者消费者问题

**问题描述**：一个或者多个生产者在生成数据后放在一个缓冲区里；单个消费者从缓冲区取出数据区里；任何时候只能有一个生产者或者消费者进入临界区。

**问题分析**

- 任何一个时刻只能有一个线程操作缓冲区(互斥访问)
- 缓冲区空时，消费者必须等待生产者(条件同步)
- 缓冲区满时，生产者必须等待消费者(条件同步)

**信号量描述约束**

- 二进制信号量mutex
- 资源信号量fullBuffers
- 资源信号量emptyBuffers

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/producer_consumer.png)

**信号量的困难**

读/开发代码困难、容易出错(使用的信号量被占用，忘记释放信号量)、不能够处理死锁

#### 管程

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/monitor.png)

管程是一种用于多线程互斥访问共享资源的程序结构

- 采用面向对象方法，简化了线程间的同步控制
- 任一时刻最多只有一个线程执行管程代码
- 正在管程中的线程可临时放弃管程的互斥访问，等待事件出现时恢复

管程的使用

- 在对象/模块中，收集相关共享数据
- 定义访问共享数据的方法

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/monitor_compo.png)

**条件变量(Condition Variable)**

- 条件变量是管程内的等待机制
  -  进入管程的线程因资源被占用而进入等待状态
  - 每个条件变量表示一种等待原因，对应一个等待队列
- Wait()操作
  - 将自己阻塞在等待队列中
  - 唤醒一个等待者或释放管程的互斥访问
- Signal()操作
  - 将等待队列中的一个线程唤醒
  - 如果等待队列为空，则等同空操作

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/condition_variable_implement.png)

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/resolve_PC_monitor.png)

**管程分类**

Hoare管程存在两个缺陷：

1. 若产生signal的进程在管程内还未结束，则需要两次额外的切换：阻塞这个进程需要一次切换；管程可用时恢复有需要一次切换
2. 与信号相关的进程调度必须非常可靠。产生一个signal必须立即激活相关条件队列中的一个进程，调度程序必须保证在激活前没有其他进程进入管程，否则进程被激活的条件就发生改变。

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/classification_of_monitor.png)

**Hansen管程**

Hansen管程signal被notify所取代，可解释为：当一个正在管程中的进程执行`notify(x)`时，会使得`x`条件队列得到通知，但发信号的进程继续执行。通知的结果是未来会调度到该队列中的某个进程进行执行。但是，不能保证在他之前没有其他进程进入管程，因而被调度的这个等待进程必须重新检查条件。

这其中的区别就在于`if`和`while`的使用。这个方案对条件变量多了至少一次的检查，得到的回报是：不再有额外的进程切换，对等待进程之后什么时候运行没有任何限制。

因为`if`没有再次的检查机制，必须立即切换到等待的进程中，以防其他的进程进入管程破坏条件，所以是Hoare管程。

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/2implement_of_monitor.png)

##### 读者写者问题

**问题描述**

- 共享数据的两类使用者

  读者：只读取数据，不修改

  写者：读取和修改数据

- 分析

  “读-读”允许，“读-写”互斥，“写-写”互斥

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/monitor_writer.png)![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/monitor_reader.png)

## 实验



