开启SSH，登录以后，在/jffs/文件夹下面新建名为dnsmasq.conf.add的文件，文件内容为

` addn-hosts=/jffs/configs/hosts `

然后在configs/文件夹下面创建hosts文件，格式如下：

` 192.168.0.1 www.example.com `

重启dnsmasq服务

` service restart_dnsmasq `

