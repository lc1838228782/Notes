strace - 跟踪系统调用和信号

### 选项

-v 打印详细的环境变量、stat、termios等

-i 在发生系统调用时，记录打印指令指针

-f 跟踪产生的子进程

-ff 如果指定了 -o filename，分别为每个进程生成 filename.pid 的记录文件



https://man.linuxde.net/strace （介绍简要，并给出了几个例子进行示范）

https://man7.org/linux/man-pages/man1/strace.1.html