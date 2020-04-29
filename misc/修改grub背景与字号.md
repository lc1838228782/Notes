可以在`/usr/share/backgrounds`里面挑一张，复制到`/boot/grub/`即可。

```shell
$ sudo grub-mkfont -o /boot/grub/fonts/DejaVuSansMono24.pf2 --size=24 /usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf

$ sudo vim /etc/default/grub # 新建GRUB_FONT=location_to_font

$ sudo update-grub
```



