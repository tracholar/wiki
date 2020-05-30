---
layout: page
title: "python 内置函数"
date: 2020-05-29
---
[TOC]

## 内置变量
- `__debug__` 默认为true

## 内置函数
- `abs(number)` 数学函数abs
- `all(iterable)` 每个元素都是true就返回true
- `any(iterable)` 任意一个元素为true就行
- `bin(number)` 返回数字的二进制表示
- `callable(p_object)` 判断是否能当做函数调用，即是否实现 `__call__`方法
- `chr(i)` 返回assic码为i的字符
- `cmp(x, y)` 比较数字大小函数
- `coerce(x, y) -> (x1, y1)` 将数字x和y转成相同类型
```python
>>> coerce(3,4.5)
(3.0, 4.5)
```
- `compile` 编译源代码
- `copyright()` 返回版权信息
- `credits()` 返回感谢信息
- `delattr(p_object, name)` 删除对象的属性
- `dir(p_object=None)`  `__dir__`
- `divmod(x, y)` 带余数除法
- `eval(source, globals=None, locals=None)` 执行源代码
- `execfile(filename, globals=None, locals=None)` 执行文件
- `exit(i)` 退出
- `filter(function_or_none, sequence)` 函数式编程filter
- `format(value, format_spec=None)` `value.__format__(format_spec)`
- `getattr(object, name, default=None)` 获取属性
- `globals()` 全局变量
- `hasattr(p_object, name)` 是否有属性
- `hash(p_object)` hash函数
- `help(with_a_twist)` 帮助函数
- `hex(number)` 16进制
- `id(p_object)` 返回id
- `input(prompt=None)` 执行输入，`eval(raw_input(prompt))`
- `intern(string)` 将string常量化
- `isinstance` 是否实例
- `issubclass(C, B)` 是否子类
- `iter(source, sentinel=None)` 返回迭代器
```python
iter(collection) -> iterator
iter(callable, sentinel) -> iterator
```





