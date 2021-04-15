## 截取视频

```
$ ffmpeg -ss [start] -i [input] -t [duration] -c copy [output]
$ ffmpeg -ss [start] -i [input] -to [end] -c copy [output]
```

示例

```
ffmpeg -ss 00:02:25 -i MIAA-327-C.mp4 -to 02:10:33 -c copy MIAA-327-C_cut.mp4
```

