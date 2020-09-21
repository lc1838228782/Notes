# XSS

XSS(Cross-site scripting )分为两种类型反射型（临时）XSS(non-persistent XSS)，另一种称为存储型XSS(persistent XSS)。

- 反射型：用户提交的输入字符串会在响应中反射回来。如：google搜索某个不存在的字符串，将提示“XXX搜索不到”这种。
- 存储型：攻击者提交的恶意脚本被服务器持久化存储，下次有受害者访问时，将会触发。

## 缓解措施

- 过滤
- 编码       `<`->`&lt`
- CSP(Content Security Policy) 

