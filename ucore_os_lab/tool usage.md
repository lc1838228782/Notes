## qemu

`sudo ln -s /usr/bin/qemu-system-i386 /usr/bin/qemu`建立软连接

`-qemu -s -S -hda t.img -monitor stdio`

-s：等待gdb连接，到port 1234

-S：在启动时不启动cpu需要在monitor中输入c继续工作

-monitor：重定向monitor到主机设备

## gdb

`target remote localhost:1234`       连接qemu远程调试

`set arch i8086`

`b *0x7c00 `              在0x7c00处中断

`x /2i $pc `

next       nexti     step    stepi

## find

`find . -name "*.[chS]" -exec grep -Hn LABX {} \;`

`grep -H` 输出文件名称，`grep -n`输出行号