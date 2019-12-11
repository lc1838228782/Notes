这题的主要思路是利用第一个变量输入的值作为地址，通过第二个输入利用format string vulnerability改写成85。最后注入shellcode执行。



字符串输入遇到`\x00`会终止。



mmap函数问题

