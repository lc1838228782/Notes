C语言程序只有一个locale。

C++的locale是一个类，可以创造任意个实例。

**Default locale**：C local

**Native locale**：`setlocale(LC_ALL, "");`或者`std::setlocale("")`，会使用环境变量中设置`LANG`。

**Multiple locale**：

例如：invoice for multiple locales

C语言

```c
double price;
char buf[SZ];
while ( ... )     // processing the German invoice
{
   std::setlocale(LC_ALL, "En_US");
   std::fscanf(priceFile,"%lf",&price);
   // convert $ to DM according to the current exchange rate
   std::setlocale(LC_ALL,"De_DE");
   std::strfmon(buf,SZ,"%n",price);
   std::fprintf(invoiceFile,"%s",buf);
}
```

C++语言

```c++
priceFile.imbue(std::locale("En_US"));
invoiceFile.imbue(std::locale("De_DE"));
moneytype price;
while ( ... )  // processing the German invoice
{
   priceFile >> price;
   // convert $ to DM according to the current exchange rate
   invoiceFile << price;
}
```

`cout, cin`在程序初始化时已被设置为C locale。main函数中的`std::locale::global(std::locale("")); `不会影响其

## 函数

```c++
locale();					// 返回当前全局locale
```



## 引用

https://svn.apache.org/repos/asf/stdcxx/trunk/doc/stdlibug/24-3.html

https://en.cppreference.com/w/cpp/locale/locale

https://en.cppreference.com/w/c/locale/setlocale