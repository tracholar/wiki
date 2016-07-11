---
layout: page
title: "python 拾遗"
date: 2016-07-10
---
[TOC]

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
