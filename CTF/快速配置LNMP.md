## MySQL

添加apt源

 https://dev.mysql.com/downloads/repo/apt/ 

```shell
wget https://dev.mysql.com/get/mysql-apt-config_0.8.12-1_all.deb
dpkg -i mysql-apt-config_*.deb
# 安装时，可以在弹出界面选择需要的 MySQL 版本
rm -i mysql-apt-config_*.deb # 清理刚才下载的 deb 文件（输入 y 确认）
```

```
apt update
apt install mysql-server      # legacy！！！
mysql --version
```

## PHP

添加ppa并安装

```shell
apt-get install software-properties-common -y
add-apt-repository ppa:ondrej/php
# add-apt-repository 后会自动执行 apt update，因此这里可不必执行
apt install php-fpm
# 再安装一些常用的 php 组件
apt install php-mysql php-mbstring php-xml php-curl

php -v
```

## nginx

`apt install nginx`

### nginx php-fpm配置

```
server {
  listen 80;
  listen [::]:80;
  # listen 443 ssl http2;
  # listen [::]:443 ssl http2;
  # include ssl.conf;
  # ssl_certificate /path/to/crt;
  # ssl_certificate_key /path/to/key;
  root /var/www/server_domain_or_IP;
  index index.html index.htm index.php;
  server_name server_domain_or_IP;
  location /.well-known/ { try_files $uri $uri/ =404; }
  location ~ /\. { return 404; }
  location / {
    try_files $uri $uri/ =404;
  }
  location ~ \.php$ {
    include snippets/fastcgi-php.conf;
    fastcgi_pass unix:/run/php/php7.2-fpm.sock;
  }
}
```

 记得将 /etc/nginx/sites-available 中的配置软链接到 /etc/nginx/sites-enabled，以启用该配置。 

## LNMP 默认目录

#### Nginx

配置：/etc/nginx

网站：/var/www

#### MySQL

配置：/etc/mysql

数据：/var/lib/mysql

#### PHP

配置：/etc/php/7.2

## 常用命令

Ubuntu 16.04 / 18.04 使用 systemctl 管理服务。

LNMP 中 service 名称分别为 nginx、mysql、php7.2-fpm

启动：systemctl start *service*

停止：systemctl stop *service*

重载：systemctl reload *service*

重启：systemctl restart *service*

查看状态：systemctl status *service*

开启自启动：systemctl enable *service*

关闭自启动：systemctl disable *service*

如重载 Nginx 可执行：

1. systemctl reload nginx

查看 PHP 状态可执行：

1. systemctl status php7.2-fpm

通过 apt 直接安装，除了能够更快速的完成服务器配置、安装干净外，后期也更加便于维护。