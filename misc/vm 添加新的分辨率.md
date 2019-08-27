```shell
$ cvt 1920 1080 60
$ sudo xrandr --newmode "1920x1080_60.00"  173.00  1920 2048 2248 2576  1080 1083 1088 1120 -hsync +vsync # 上面命令的输出
$ xrandr | grep -e " connected [^(]" | sed -e "s/\([A-Z0-9]\+\) connected.*/\1/" # 获取已连接的显示器
$ sudo xrandr --addmode Virtual1 1920x1080_60.00 # 添加分辨率到显示器
```

**IMPORTANT**
To make Ubuntu remember the resolution at next start, you have to append the below two line  to the `~/.profile`
```shell
xrandr --newmode "1920x1080_60.00"  173.00  1920 2048 2248 2576  1080 1083 1088 1120 -hsync +vsync
xrandr --addmode Virtual1 1920x1080_60.00
```

