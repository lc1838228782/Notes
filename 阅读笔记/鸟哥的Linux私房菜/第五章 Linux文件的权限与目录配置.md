## 5.2 Linux权限概念

### 5.2.1 Linux文件属性

权限部分，第一个字符：

`d`表示目录

`-`文件

`l`链接文件

`b`设备文件中可供存储的设备（可按块随机读写的设备）

`c`设备文件中的串行端口设备，例如键盘、鼠标（一次性读取设备）



第二栏 表示有多少文件名链接到此节点（inode）

### 5.2.2 如何修改文件属性与权限

`chgrp`、`chown`、

`chmod`：rwx（421）；user，group，others，all

例：`$ chmod u=rwx,go=rx file`

### 5.2.3 目录和文件权限意义

#### 对于文件

`r(read)`:可读取文件实际内容

`w(write)`:可以编辑、新增或者修改文件内容。（不包含删除

`x(execute)`:该文件可被系统执行

#### 对于目录

`r(read contents in directory)`:可以查询目录下的文件名信息，也就是可以ls。

`w(modify contents of directory)`:表示具有可以改动目录列表的权限，即以下权限

- 建立新的文件与目录
- 删除已经存在的文件或目录（无论权限）
- 将已存在文件或目录重命名
- 移动该目录中的文件、目录位置

`x(access directory)`:代表用户可以进入该目录作为工作目录的权限。（一个用户想读写执行某个目录下的文件，必须具有该文件所有父目录的执行的权限）

