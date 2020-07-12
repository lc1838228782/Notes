## 对于http/https协议

```shell
$ git config --global http.proxy 'socks5://127.0.0.1:1080'
$ git config --global https.proxy 'socks5://127.0.0.1:1080'
```

针对github

```shell
#只对github.com
$ git config --global http.https://github.com.proxy socks5://127.0.0.1:1080
$ git config --global https.https://github.com.proxy socks5://192.168.64.1:7890
#取消代理
$ git config --global --unset http.https://github.com.proxy
```

If you also want the host name to be resolved using the proxy, use thuzhf's solution below, which uses `socks5h` instead of `socks5`

## 对于ssh协议

在`~/.ssh/config`中添加

```
# 必须是 github.com
Host github.com
   HostName github.com
   User git
   # 走 HTTP 代理
   # ProxyCommand socat - PROXY:127.0.0.1:%h:%p,proxyport=8080
   # 走 socks5 代理（如 Shadowsocks）
   # ProxyCommand nc -v -x 127.0.0.1:1080 %h %p
   # 对于windows
   # ProxyCommand connect -S 127.0.0.1:1080 %h %p
```



## Reference

1. https://stackoverflow.com/questions/15227130/using-a-socks-proxy-with-git-for-the-http-transport
2. https://upupming.site/2019/05/09/git-ssh-socks-proxy/
3. https://blog.systemctl.top/2017/2017-09-28_set-proxy-for-git-and-ssh-with-socks5/