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
