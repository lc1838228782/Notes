
# C语言

## 字符串

- 字符串结尾'\0'，也就是0。

- `int strcasecmp(const char *s1, const char *s2)`

忽略大小写比较。相同返回0。

- `char *strchr(const char *str, int c)`

该函数返回在字符串 str 中第一次出现字符 c 的位置，如果未找到该字符则返回 NULL。

- `extern char *strstr(char *str1, const char *str2)`

返回值：若str2是str1的子串，则返回str2在str1的首次出现的地址；如果str2不是str1的子串，则返回NULL.
