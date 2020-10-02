## readelf

```shell
$ readelf -l prog			# 加载相关 section to segment mapping
$ readelf -d prog			# 查看.dynamic段内容
```

## objdump

```shell
$ objdump -s prog			# 详细展开
$ objdump -s --section .comment /path/to/binary   # 获取编译信息
```

## od

## hexdump

## xxd