#### 泄露地址填充

`ljust(8, '\x00')`向高位填充至8字节。

#### 字符串与数字转换

`pack(canary, 32)[::-1].encode('hex')` 将canary打印出来

`unpack('/sh\x00', 32)`