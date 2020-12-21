## UTF-8 BOM

> UTF-8 不需要 BOM，尽管 Unicode 标准允许在 UTF-8 中使用 BOM。
> 所以不含 BOM 的 UTF-8 才是标准形式，在 UTF-8 文件中放置 BOM 主要是微软的习惯（顺便提一下：把带有 BOM 的小端序 UTF-16 称作「Unicode」而又不详细说明，这也是微软的习惯）。
> BOM（byte order mark）是为 UTF-16 和 UTF-32 准备的，用于标记字节序（byte order）。微软在 UTF-8 中使用 BOM 是因为这样可以把 UTF-8 和 ASCII 等编码明确区分开，但这样的文件在 Windows 之外的操作系统里会带来问题。
>
> 「UTF-8」和「带 BOM 的 UTF-8」的区别就是有没有 BOM。即文件开头有没有 U+FEFF。



> Windows 该死的记事本有个臭名昭著的破毛病就是在 UTF-8 文件开头加 BOM，所以**不要用**记事本来编辑文件。



## 引用

[什么是UTF-8 BOM](https://www.zhihu.com/question/20167122)

[Windows记事本](https://www.zhihu.com/question/20138814)

[rdbuf](https://stackoverflow.com/questions/2141749/what-does-ifstreamrdbuf-actually-do)