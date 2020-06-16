# Unicode和UTF-8

- Unicode是字符集
- UTF-8是编码规则

其中：

- 字符集：为每一个字符分配一个唯一的ID（学名为码位/码点/Code Point）
- 编码规则：将码位转换为字节序列的规则（编码/解码）

用通信理论的思路可以理解为:
unicode是信源编码，对字符集数字化;
utf8是信道编码，为更好的存储和传输。

# Python2

- str是Python2中表示bytes流的字符串
- unicode是用来表示Unicode的字符串

常见的引号定义的字符串，其中存储的是bytes，加个“u”前缀，则会得到一个unicode对象。可以自己插入`\u`开头的unicode字符。

```python
>>> str_seq = 'Hello World!'
>>> type(str_seq)
<type 'str'>

>>> unicode_seq = u'Hello World! \u00f8'
>>> type(unicode_seq)
<type 'unicode'>
>>> print(unicode_seq)
Hello World! ø
```

## encode和decode方法

> unicode.encode() -> bytes:只有对于unicode对象我们才应该使用.encode()方法。这一方法用来将一系列unicode编码为bytes流。
> bytes.decode() -> unicode: 只有对于bytes，或说Python2中的str对象，我们才应该调用.decode()方法。这一方法将一系列bytes流解码为原本的unicode码点。

encode和decode方法均可接收encoding与errors两个参数，分别用来指定编码/解码格式以及出现编码/解码错误的时候该方法如何反应。

需要注意的是，编码/解码前后字符串的长度(len()方法的返回值)有可能是不同的，因为二者长度的意义不同。对于unicode来说，长度为其中字符(码点)的数目，而str的长度则为其中bytes的数目(因为对str来说并没有字符的概念)。所以一旦某个unicode字符被编码为超过一个byte的长度，二者的长度就会不同。

```python
>>> unicode_seq = u'Hello World! \u00f8'
>>> len(unicode_seq)
14
>>> utf8_encoded_bytes = unicode_seq.encode('utf-8')
>>> type(utf8_encoded_bytes)
<type 'str'>
>>> len(utf8_encoded_bytes)
15
>>> unicode_seq
u'Hello World! \xf8' 
# 此处的\xf8即为\u00f8，二者为同一个码点，只是省略了开头的0
>>> utf8_encoded_bytes
'Hello World! \xc3\xb8'
>>> utf8_encoded_bytes.decode('utf-8')
u'Hello World! \xf8'
```

### Encoding Errors

比如使用错误的编码格式对一个unicode对象调用了encode方法。这往往是由于指定的编码格式不足以编码指定unicode字符串中的某些字符导致的。例如如果我们尝试使用ASCII编码上述含有ø的字符串，由于ascii并不能表示这个字符，所以导致UnicodeEncodeError。错误信息中包含所试图使用的编码：ascii；无法编码的字符：u’\xf8’，以及这个字符的位置。错误信息中的codec是coder/decoder的简称。

```python
>>> unicode_seq.encode('ascii')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
UnicodeEncodeError: 'ascii' codec can't encode character u'\xf8' in position 13: ordinal not in range(128)
```

### Decoding Errors

比如我们试图使用ASCII解码使用UTF-8编码的bytes字符串，则会得到Decode Error。这个解码异常与上文的编码异常类似，都是因为某些字符超出了ASCII可表示的范围

```python
>>> utf8_encoded_bytes.decode('ascii')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
UnicodeDecodeError: 'ascii' codec can't decode byte 0xc3 in position 13: ordinal not in range(128)
```

又或者我们使用UTF-8解码并不符合UTF-8编码规范的错误bytes字符串，也会出现错误。事实上UTF-8的有点质疑在于其能够识别不符合编码规范的随机bytes流，所以出现错误的数据并不能够成功解码。

```python
>>> wrong_seq.decode('utf-8')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/usr/local/Cellar/python/2.7.13/Frameworks/Python.framework/Versions/2.7/lib/python2.7/encodings/utf_8.py", line 16, in decode
    return codecs.utf_8_decode(input, errors, True)
UnicodeDecodeError: 'utf8' codec can't decode byte 0x89 in position 1: invalid start byte
```

最为诡异的解码错误出现在如下这种情况中。如果我们对于一个bytes字符串再次调用`encode()`方法，事实上程序会抛出一个`UnicodeDecodeError`。

```python
>>> utf8_encoded_bytes.encode('utf-8')
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
UnicodeDecodeError: 'ascii' codec can't decode byte 0xc3 in position 13: ordinal not in range(128)
```

这是由于Python2所谓的implicit conversion导致的。也就是说，Python2会尝试自动解码一些字符串以方便编程。这一过程如果在数据中仅有ASCII字符时非常有用。但一旦出现非ASCII字符，就会带来无尽的痛苦。上述代码中的utf8_encoded_bytes.encode('utf-8')事实上等同于`utf8_encoded_bytes.decode(sys.getdefaultencoding()).encode('utf-8')`。

### Implicit Conversion

当我们试图连接unicode和bytes时，python会以默认编码格式解码bytes字符串来产生一个unicode字符串。如下面例子所示。当我们想要将unicode字符串u'Hello'与bytes字符串' World!'组合起来时，Python2会首先尝试使用默认编码格式解码bytes字符串。

默认编码格式往往是ASCII。因为ASCII被广泛的使用，且ASCII是大部分编码格式的子集，因此在大部分情况下是安全的。

```python
>>> u'Hello' + ' World!'
u'Hello World!'

# 上述过程等同于下面的代码
>>> u'Hello' + (' World!'.decode(sys.getdefaultencoding()))
u'Hello World!'
>>> sys.getdefaultencoding()
'ascii'

# 如果byte字符串不能被ascii解码，则会报错
>>> utf8_encoded_bytes
'Hello World! \xc3\xb8'
>>> u'Hello' + utf8_encoded_bytes
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
UnicodeDecodeError: 'ascii' codec can't decode byte 0xc3 in position 13: ordinal not in range(128)

>>> u'Hello' + utf8_encoded_bytes.decode(sys.getdefaultencoding())
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
UnicodeDecodeError: 'ascii' codec can't decode byte 0xc3 in position 13: ordinal not in range(128)
```

由上面的例子看出，如果我们试图组合的byte字符串可以被ASCII解码，则这一过程是安全的。否则就会导致UnicodeDecodeError。所以如果我们的代码中不小心混用了unicode字符串和byte字符串，当所有数据都是ASCII字符的情况下，一切可以相安无事。否则就会报错。

### 错误处理

上面提到encode()和decode()方法接受两个参数，分别是encoding与errors。后者用来指定编码/解码出错时程序如何反应。errors参数可以是['strict', 'replace', 'ignore', 'xmlcharrefreplace']其中之一。

默认为’strict’。这一情况下对于解码和编码错误方法会抛出异常。

当指定为’replace’时，标准替代字符将会替代无法编码的字符。

当指定为’ignore’时，所有无法解码的字符会被直接忽略。

当指定为’xmlcharrefreplace’时，所有原有的码字会被使用XML转义符保存，因此当输出信息会被XML或者HTML时仍能被转码。

```python
>>> print(utf8_encoded_bytes.decode('ascii', 'replace'))
Hello World! ��
>>> utf8_encoded_bytes.decode('ascii', 'ignore')
u'Hello World! '
>>> print(utf8_encoded_bytes.decode('ascii', 'ignore'))
Hello World!
```

# Python3

Python3中仍有两种字符串类型str和bytes。但在Python3中，str中存储的是一系列unicode码字。如果仍要处理一系列bytes字符串，则需要在字符串直接量前面加 **b** 前缀。这种情况下你将会得到一个`bytes`类型的对象。

```python
>>> py3_str = 'Hello World! \u00f8'
>>> type(py3_str)
<class 'str'>
>>> u_str = u'Hello World!'
>>> type(u_str)
<class 'str'>
>>> py3_bytes = b'Hello World!'
>>> type(py3_bytes)
<class 'bytes'>
```

总结起来，就是Python2中的str在Python3中为bytes，Python 2中的unicode是Python 3中的str。这一设计事实上更为合理，因为在程序中当你使用一个变量存储一些文字信息时，你想要处理的是unicode。而bytes仅仅应当在需要的时候使用，例如进行文件IO或者网络IO时。其余时候代码并不应该关心文字本身是被如何编码为bytes的。

与此同时，Python3在Unicode支持方面所做的最大的一项改变是其不再对byte字符串进行自动解码。如果我们想要在Python3中直接对bytes字符串和Unicode字符串相加，则程序会报错，如下面的例子所示。

```python
>>> py3_str = 'Hello World! \xf8'
>>> py3_bytes = b'Hello World!'
>>> py3_str + py3_bytes
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: Can't convert 'bytes' object to str implicitly
```

Python2与Python3在unicode处理方面另一个非常大的不同在于如何衡量字符串是否相等这一方面。在Python2中，如果一个unicode字符串和一个byte字符串均仅包含ASCII对应的byte编码时，则它们被认为是相等的两个字符串，==操作将会返回True。而在Python3则不存在这一情况。这一区别会导致Python2与Python3程序极大的不兼容性。例如如果在Python2中使用一个byte字符串作为字典的键(key)，使用相应的unicode字符串仍可以取出这一键对应的值(value)。在Python3中则无法找到。如下面的例子中所示。

```python
# Python2
>>> py2_str = u'Hello World!'
>>> py2_bytes = 'Hello World!'
>>> py2_str == py2_bytes
True

# Python3
>>> py3_str = 'Hello World!'
>>> py3_bytes = b'Hello World!'
>>> py3_str == py3_bytes
False
```

**因此，在Python3中，如果需要混合bytes和unicode，我们需要显式进行类型转换。**

## Python3中的文件IO

因为上述Python3中对于unicode和byte的处理方式，Python3中文件的读取过程也做了相应的改变，也就是说，Python3的open()方法与Python2是不同的。Python2的open方法签名为：open(name[, mode[, buffering]])。Python3的open方法签名为：open(file, mode='r', buffering=-1, encoding=None, errors=None, newline=None, closefd=True, opener=None)。

Python文件读取有两种模式：二进制(binary)和文本(text)。

在Python2中，二者的不同仅仅在于对于行结尾的处理方式上，基本上没有任何本质的不同。

在Python3中，二者将会产生完全不同的结果。如果使用文本(text)模式读取文件，例如使用'r'或'w'，从文中读取的数据将被解码成为unicode/str对象。如果在read方法中不指定encoding=参数，则默认编码格式将被使用，程序通过locale.getpreferredencoding()方法获得编码格式。否则程序使用encoding参数指定的编码方式解码。而如果使用二进制(binary)模式读取文件，例如'rb'或者'wb'，读取的数据将不会被做任何处理，产生byte对象。因此在Python3中正确地读取一个文件需要我们指定正确的编码格式。

## Unicode处理指南

### 方法一：Unicode三明治

这一方法既在程序中我们总是仅仅处理unicode字符串，而在程序外仅存储和保留byte字符串。也就是程序内unicode，程序外byte。这要求我们在程序进行IO的入口处解码读取到的数据，在程序进行IO的出口处编码输出的数据。也就是，在程序中我们应当尽可能早地解码数据从而得到unicode对象，同时应当尽可能晚地编码数据从而输出byte对象。这样我们就创造了一个所谓的unicode三明治。

![](https://raw.githubusercontent.com/lc1838228782/pics/master/img/unicode sandwich.png)

同时我们也要注意有一些库本身会进行这些解码/编码过程，例如接收unicode作为输入，或者使用unicode作为输出，且会进行unicode和bytes之间的转化。例如Django和json库。

### 方法二：清楚了解被处理的数据

### 方法三：使用Unicode输入测试我们的代码

## 总结

根据Ned Batchelder在PyCon2012上的讲座，我们将Python处理unicode的五个事实以及三条解决建议总结如下：

五个事实

1. IO操作应当总是处理bytes。
2. 现实世界需要远多于256个字符编码。
3. 程序既需要面对bytes，也需要面对unicodes。
4. 数据编码格式无法通过数据内容本身推测出来。
5. 即使是事先声明的数据编码格式也有可能出错。

三条建议

1. 使用unicode三明治的模式处理文本数据。
2. 明确程序正在使用和处理的数据的类型及编码格式。
3. 使用非ASCII字符串测试程序。



https://blog.csdn.net/Yaokai_AssultMaster/article/details/82823158