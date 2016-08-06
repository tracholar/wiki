---
layout: page
title: "python 拾遗"
date: 2016-07-10
---
[TOC]

## 基本语法相关
- `*args`，将列表`args`展开为参数列表，`**kwargs`，将字典`kwargs`展开成kv形式的参数列表。

参考 <https://stackoverflow.com/questions/2921847/what-does-the-star-operator-mean-in-python>
和 <https://docs.python.org/3/tutorial/controlflow.html#unpacking-argument-lists>

- 整数除法，在python2.x中`/`代表整数除法（除非左右操作数有一个是浮点数），而在python3.x中，代表浮点数除法，python3.x用`//`代表整数除法。
- 反射函数，`type()`，`isinstance()`，`hasattr()`，`setattr()` 以及属性`__class__`
- Mixin：python通过多重继承实现组合模式。sklearn中很多基础类使用这种模式实现的。<https://github.com/scikit-learn/scikit-learn/blob/51a765a/sklearn/base.py#L281>

## 函数
参考：<https://stackoverflow.com/documentation/python/228/functions>

- 任意参数：`*args, **kwargs`

```python
def func(*args):
    # args will be a tuple containing all values that are passed in
    for i in args:
        print(i)
```

- 字符串函数拾遗：除了`upper, lower` 之外还有：<https://stackoverflow.com/documentation/python/278/string-methods>
    - `capitalize` 将第一个变为大写，其他的变为小写！
    - `title` 将每个单词第一个字母变为大写，其他的为小写！
    - `casefold` @since(3.3) 小写转换的unicode字符版！希腊字母也可以转

    - `translate` 按照替换表进行字母替换
    - `format` 格式化
    - `split, rsplit` 当指定 `maxsplit` 参数的时候，有区别
    - string 模块常量，要使用`import string`导入：`string.ascii_letters` 字母表, `string.ascii_lowercase, string.ascii_uppercase, string.digits, string.hexdigits, string.octaldigits, string.punctuation, string.whitespace, string.printable`
    - unicodedata, 一个有趣的模块
    - `str.count(sub[, start[, end]])` 统计出现的次数
    - `str.replace(old, new[, count])` 简单替换，`re.sub`基于正则式替换
    - `str.isupper(), str.islower() and str.istitle()`
    - `str.ljust and str.rjust` 字符串对齐
    - `'hello'[::-1]` 字符串反转
    - `str.strip([chars]), str.rstrip([chars]) and str.lstrip([chars])` 去掉空白字符
    - `"foo" in my_str` 字符串包含检查
    - `str.startswith() and str.endswith()` 字符串首尾
    - `encode, decode` 字符串和unicode字符串之间的转换
    - 共同前缀！

```python
import os
mylist = ["&abcd", "&abbe", "&ab"]
print (os.path.commonprefix(mylist))
```



## 类
参考 <https://stackoverflow.com/documentation/python/419/classes>

- Bound, unbound, and static methods

Bound 方法就是通过类的实例访问的方法（instancemethod）， unbound 方法就是通过类本身访问的动态方法（Python3.x 已废弃)

```python
a = A()
a.f
# <bound method A.f of <__main__.A object at ...>>
a.f(2)
# 4

# Note: the bound method object a.f is recreated *every time* you call it:
a.f is a.f  # False
# As a performance optimization you can store the bound method in the object's
# __dict__, in which case the method object will remain fixed:
a.f = a.f
a.f is a.f  # True
```

- @classmethod 装饰器，第一个参数代表类，类方法，既可以通过类调用，也可以通过类的实例调用
- @staticmethod ，只能通过类调用
- 无装饰器， 只能通过实例调用


- 类的继承：从3.x开始，`super` 不再需要传入参数了！

```python
class Square(Rectangle):
    def __init__(self, s):
        # call parent constructor, w and h are both s
        super(Rectangle, self).__init__(s, s)

        self.s = s
```

- 抽象类：`abc`模块，需要制定`__metaclass__`变量为`ABCMeta`，子类需要注册！

```python
from abc import ABCMeta

class AbstractClass(object):
    # the metaclass attribute must always be set as a class variable
    __metaclass__ = ABCMeta

   # the abstractmethod decorator registers this method as undefined
   @abstractmethod
   def virtual_method_subclasses_must_define(self):
       # Can be left completely blank, or a base implementation can be provided
       # Note that ordinarily a blank interpretation implicitly returns `None`,
       # but by registering, this behaviour is no longer enforced.

class Subclass:
   def virtual_method_subclasses_must_define(self):
       return

# registration is mandatory to truly create an abstract class
AbstractClass.register(SubClass)
```

- 多重继承：`class FooBar(Foo, Bar)`
- 私有变量：`_non_public` 通过添加一个下划线实现！（哈哈！）
- 属性方法：可以将属性设置为只读，不提供`setter`方法

```python
class MyClass:

    def __init__(self):
       self._my_string = ""

    @property
    def my_string(self):
        return self._my_string

    @my_string.setter
    def my_string(self, new_value):
        self._my_string = new_value

    @my_string.deleter
    def x(self):
        del self._my_string
```

## 装饰器
任何传入一个函数，返回一个函数的函数都可以作为装饰器！(貌似无效！py2.7)

```python
def super_secret_function(f):
    return f

@super_secret_function
def my_function():
    print("This is my secret function.")
```

这里`@` 相当于实现 `my_function = super_secret_function(my_function)`

装饰器类：`__call__` 方法进行装饰。如果需要装饰成员方法，需要指定`__get__`方法！
装饰器可以使用参数，只要在函数内部定义一个类，然后返回即可。


```python
class Decorator(object):
    """Simple decorator class."""

    def __init__(self, func):
        self.func = func

    def __call__(self, *args, **kwargs):
        print('Before the function call.')
        res = self.func(*args, **kwargs)
        print('After the function call.')
        return res

@Decorator
def testfunc():
    print('Inside the function.')

testfunc()
# Before the function call.
# Inside the function.
# After the function call.
```

## 上下文管理器 `with`
- 把文件当做上下文，上下文结束的时候会自动关闭文件！
- 自己的上下文管理器需要实现两个方法 `__enter__() and __exit__()`

## 迭代器相关函数
- `enumerate(iter)` 返回`(idx, value)`的迭代器，可以应用在需要使用index的时候。
- `(x for x ... )` 圆括号生成迭代器的一个语法糖。


## 各种坑
- 不要用可变对象作为函数默认值。字典,集合,列表等等对象是不适合作为函数默认值的.
  因为这个默认值是在函数建立的时候就生成了, 每次调用都是用了这个对象的”缓存”.

```python
In [1]: def append_to_list(value, def_list=[]):
   ...:         def_list.append(value)
   ...:         return def_list
   ...:

In [2]: my_list = append_to_list(1)

In [3]: my_list
Out[3]: [1]

In [4]: my_other_list = append_to_list(2)

In [5]: my_other_list
Out[5]: [1, 2] # 看到了吧，其实我们本来只想生成[2] 但是却把第一次运行的效果页带了进来
```

- 生成器不保留迭代过后的结果

```python
In [12]: gen = (i for i in range(5))

In [13]: 2 in gen
Out[13]: True

In [14]: 3 in gen
Out[14]: True

In [15]: 1 in gen
Out[15]: False # 1为什么不在gen里面了? 因为调用1->2,这个时候1已经不在迭代器里面了,被按需生成过了
```

- 在循环中修改列表项，例如在循环中删除某个元素导致index紊乱

```python
In [44]: a = [1, 2, 3, 4, 5]

In [45]: for i in a:
   ....:     if not i % 2:
   ....:         a.remove(i)
   ....:

In [46]: a
Out[46]: [1, 3, 5] # 没有问题

In [50]: b = [2, 4, 5, 6]

In [51]: for i in b:
   ....:      if not i % 2:
   ....:          b.remove(i)
   ....:

In [52]: b
Out[52]: [4, 5] # 本来我想要的结果应该是去除偶数的列表
```
- IndexError，

```python
In [55]: my_list = [1, 2, 3, 4, 5]

In [56]: my_list[5] # 根本没有这个元素

In [57]: my_list[5:] # 这个是可以的
```

- 全局变量和局部变量重名问题

```python
In [58]: def my_func():
   ....:         print(var) # 我可以先调用一个未定义的变量
   ....:

In [59]: var = 'global' # 后赋值

In [60]: my_func() # 反正只要调用函数时候变量被定义了就可以了
global

In [61]: def my_func():
   ....:     var = 'locally changed'
   ....:

In [62]: var = 'global'

In [63]: my_func()

In [64]: print(var)

global # 局部变量没有影响到全局变量

In [65]: def my_func():
   ....:         print(var) # 虽然你全局设置这个变量, 但是局部变量有同名的, python以为你忘了定义本地变量了
   ....:         var = 'locally changed'
   ....:

In [66]: var = 'global'

In [67]: my_func()
---------------------------------------------------------------------------
UnboundLocalError                         Traceback (most recent call last)
<ipython-input-67-d82eda95de40> in <module>()
----> 1 my_func()

<ipython-input-65-0ad11d690936> in my_func()
      1 def my_func():
----> 2         print(var)
      3         var = 'locally changed'
      4

UnboundLocalError: local variable 'var' referenced before assignment

In [68]: def my_func():
   ....:         global var # 这个时候得加全局了
   ....:         print(var) # 这样就能正常使用
   ....:         var = 'locally changed'
   ....:

In [69]: var = 'global'

In [70]:

In [70]: my_func()
global

In [71]: print(var)
locally changed # 但是使用了global就改变了全局变量
```

- `is` 和 `==` 前者比较是否是同一个对象，后者比较值是否相等

## 工具函数
- 获取对象成员，`getmembers`, `dir`等函数

```python
from inspect import getmembers
for k,v in (getmembers(object)):
    print k,':', v
```
