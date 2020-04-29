## 环境配置

```sh
$ sudo apt install python3
$ python3 -m pip install pip -U
$ python3 -m pip install virtualenv
```

`virtualenv`使用

```sh
$ virtualenv envname			# using directly in some directory
$ virtualenv -p /usr/bin/python2.7 venv	# assign a python version
$ . envname/bin/activate
$ deactivate
```

## 内建类型

标准整型 长整型

布尔类型和布尔对象

双精度浮点型

十进制浮点型 `Decimal(.1)`

复数

ASCII转换 `ord`和`chr`转换字符串与数字。

字符串， 列表， 元组， 字典， 集合

## 函数

`docstring`

```python
def printMax2(x, y):
    '''Prints the maximum of two numbers.
 
    The two values must be integers.'''
    x = int(x) # convert to integers, if possible
    y = int(y)
 
    if x > y:
        print(x, 'is maximum')
    else:
        print(y, 'is maximum')
 
printMax2(3, 5)
print(printMax2.__doc__)
```

## 异常

```python
try:
    f = open('integers.txt')
    s = f.readline()
    i = int(s.strip())
except (IOError, ValueError):
    print("An I/O error or a ValueError occurred")
except:
    print("An unexpected error occurred")
    raise
```

自定义异常

```python
class ShortInputException(Exception):
    '''A user-defined exception class.'''
    def __init__(self, length, atleast):
        Exception.__init__(self)
        self.length = length
        self.atleast = atleast
 
try:
    s = raw_input('Enter something --> ')
    if len(s) < 3:
        raise ShortInputException(len(s), 3)
    # Other work can continue as usual here
except EOFError:
    print('\nWhy did you do an EOF on me?')
except ShortInputException as x:
    print('ShortInputException: The input was of length %d, \
          was expecting at least %d' % (x.length, x.atleast))
else:
    print('No exception was raised.')
```

`finally`

```python
def test1():
   try:
      print('to do stuff')
      raise Exception('hehe')
   except Exception:
      print('process except')
      print('to return in except')
      return 'except'
   finally:
      print('to return in finally')
      return 'finally'
 
test1Return = test1()
print('test1Return : ' + test1Return)
```

运行结果

```
to do stuff
process except
to return in except
to return in finally
test1Return : finally
```

## 类

```python
class SchoolMember:
    def __init__(self, name, age):
        self.name = name
        self.age = age
    def tell():
        print(self.name, self.age)
    
class Teacher(SchoolMember):
    def __init__(self, name, age, salary):
        SchoolMember.__init__(self, name, age)
        self.salary = salary
        
    def tell():
        print(self.name, self.age, self.salary)
```

## 文件操作

`open`, `read`, `write`

`os.remove`

`os.rename` `os.renames` `os.listdir` `os.chdir` `os.walk` `os.rmdir` `os.removedirs`

`stat`





