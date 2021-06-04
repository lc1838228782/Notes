```shell
$ curl www.sina.com                # 查看网页源码
$ curl -o filename www.sina.com    # 保存为文件, -O参数将服务器回应保存成文件，并将 URL 的最后部分当作文件名
$ curl -L www.sina.com             # 自动跳转(e.g. 301)
$ curl -i www.sina.com             # 连同网页源码，显示响应包 HTTP 头
$ curl -I www.sina.com             # 只显示 HTTP 响应头
$ curl -v www.baidu.com            # 显示请求响应详细过程
$ curl --trace output.txt www.sina.com                              # 显示更加详细
$ curl -X POST --data "data=xxxx" example.com/form.cgi              # POST, 还未实验

$ curl -d'login=emma＆password=123'-X POST https://google.com/login
# 或者
$ curl -d 'login=emma' -d 'password=123' -X POST  https://google.com/login

$ curl --form upload=@localfilename --form press=OK [URL]           # 文件上传 (需要更改字段)

$ curl -e http://www.example.com http://www.example.com             # 添加refer
$ curl --referer http://www.example.com http://www.example.com      # 添加refer

$ curl --user-agent "[User Agent]" [URL]                            # 自定 UA (-A)
$ curl --cookie "name=xxx" www.example.com                          # 自定 cookie
$ curl -c cookies http://example.com                                # 存储cookie 到文件
$ curl -b cookies http://example.com                                # 使用文件中 cookie

$ curl -H "Content-Type:application/json" http://example.com  # 增加头信息
$ curl --header "Content-Type:application/json" http://example.com  # 增加头信息

$ curl --user name:password example.com                             # http 认证

$ curl -k https://example.com                                       # 跳过 SSL 检测
```



https://www.ruanyifeng.com/blog/2019/09/curl-reference.html