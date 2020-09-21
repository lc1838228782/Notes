## 课程

[Compilers | edX](https://www.edx.org/course/compilers)

## Installing directly on linux

- Install packages (If you only intend to use the C++ version, you don't need the jdk). For Ubuntu:

```shell
$ sudo apt-get install flex bison build-essential csh openjdk-6-jdk libxaw7-dev
```

- Copy files from vm.

- Add the bin directory to your $PATH environment variable. If you are using bash, add to your .profile (or .bash_profile, etc. depending on your configuration; note that in Ubuntu have to log out and back in for this to take effect):

```shell
PATH=/usr/class/cs143/cool/bin:$PATH
```

