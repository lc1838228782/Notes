## 关闭ASLR

`sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"`

## 开启core dump

`sudo sh -c "echo '/tmp/core.%t' > /proc/sys/kernel/core_pattern`

