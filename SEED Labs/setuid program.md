# Setuid 程序

Setuid(set user ID on execution)，是一种特殊文件权限，当用户运行程序时获得程序属主的权限。如:`passwd` 。

## 查看setuid权限

```shell
$ ll /usr/bin/passwd
-rwsr-xr-x 1 root root 67992 Aug 29 21:00 /usr/bin/passwd*
```

在执行位有一个`s`标志。

## 设置setuid权限

```shell
$ chmod u+s myfile
$ chmod 4777 myscript
$ chmod 4764 myscript
$ chmod 0764 myscript      # 移除权限
```

当文件没有可执行权限时，标志位为`S`。

## Setgid

和setuid一样，当用户运行程序时，获得属组的权限。

```shell
$ chmod g+s myfile
$ chmod 2777 myscript
$ chmod 2764 myscript
$ ll
drwxr-sr-x  4 cliu cliu 4096 Dec  5 19:04 pwn/
```

如果文件夹被设置setgid标志，任何创建在此文件夹的文件，将属于此文件夹属组，而不是创建用户的组。移动或复制的文件不会产生改变。

## Sticky位

sticky是保护目录中文件的权限位。只有文件所有者、目录所有者或者特权用户才可以删除文件，root用户是特权用户的一个示例。

```shell
drwxrwxrwt  23 root root       4096 Dec 11 09:33 tmp/
```

## 查找setuid或setgid文件

```shell
$ find / -type f -perm /6000 -exec stat -c "%A %a %n" {} \;

-rwxr-sr-x 2755 /usr/bin/wall

-rwxr-sr-x 2755 /usr/bin/mlocate

-rwxr-sr-x 2755 /usr/bin/dotlockfile

-rwxr-sr-x 2755 /usr/bin/crontab

-rwsr-sr-x 6755 /usr/bin/at

-rwxr-sr-x 2755 /usr/bin/bsd-write

-rwxr-sr-x 2755 /usr/bin/ssh-agent

-rwsr-xr-x 4755 /usr/bin/mtr

-rwsr-xr-x 4755 /usr/bin/traceroute6.iputils

-rwsr-xr-x 4755 /usr/bin/passwd

-rwsr-xr-x 4755 /usr/bin/pkexec

-rwsr-sr-x 6755 /usr/bin/X

[...]
```

