---
title: "第1.1讲：python快速入门"
layout: page
date: 2018-02-05
---
[TOC]

## 关于
在本讲中，你讲快速了解python的基本编程语法和工具。本教程是为了给机器学习算法学习提供支持的，除了讲解python语法外，还会涉及做算法相关的工具。

## 安装
### WINDOWS系统
对于windows系统，可以从[官网下载](https://www.python.org/downloads/)，安装过程可以参考[这个教程](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/0014316090478912dab2a3a9e8f4ed49d28854b292f85bb000)

### MAC
MAC自带有python，如果没有可以通过下述两种方式安装

1. 和window系统一样，通过官网下载安装
2. 通过brew安装 `brew install python`

### Linux
linux一般自带python，如果没有可以通过下述两种方式安装

1. 通过官网下载安装
2. 通过包管理器安装，Ubuntu `sudo apt-get install python`, CentOS `yum install python`

## 编码工具
用常用的带语法高亮的代码编辑器就行，windows推荐 [notepad++]()，mac推荐 atom。

如果喜欢IDE可以用VS或者IDEA。IDE的好处是有代码提示，写起来方便。

Python还有一个神奇，叫做IPython、Jupyter，愿意折腾的可以试试。

## Hello word
和所有编程语言一样，首先来实现一个 Hello word程序，将下面的代码保存到 hello.py 文件中

```python
print("Hello word!")
```

然后打开终端，进入 hello.py 所在目录，执行命令`python hello.py`就可以看到输出结果了！

    Hello word!

## 数据类型
和很多编程语言一样，python有很多基础数据类型：整型、浮点数、字符串、布尔值(True, False)、空型(None)。

```python
# 整数
v = 123

# 浮点数
pi = 3.14159

# 字符串，可以用单引号和双引号，两者是等价的
s = 'Hello word'
s = "hello word"

# 字符串支持格式化, python2.7版本用百分号，python3用format函数
s = 'v = %d' % v
s = 'v = {0}'.format(v)

# 布尔值
v = True
if v:
    print('True')

# 空型
v = None
```

## 列表、集合和字典
python原生支持3种常用的集合类型：列表、集合和字典。列表就是很多元素的集合，集合和列表一样，只是集合要求元素互不相同，字典是key-value结构。

```python
# 一个列表
arr = [1,3,5,7,9, 3, 5]

# 列表遍历
for i in arr:
    print(i) # 1 3 5 7 9 3 5

# 集合
s = set(arr)
for i in s:
    print(i) # 1 3 5 7 9

# 字典
d = {'A' : 1, 'B' : 2, 'C' : 4}
for k in d:
    print(k, d[k])

```

### 列表推导与字典推导
python对集合的遍历支持更方便的语法，叫做列表推导和字典推导。

```python
# 列表推导
arr_new = [i*2 for i in arr] # 将arr中每个元素乘以2

# 字典推导
d_new = {k : v*2 for k,v in d.items()}
```


## 函数和类
python通过关键字`def`定义函数，通过关键字class定义类，self代表类自己，类似于其他编程语言中的this。

```python
# 乘以2的函数
def times_two(x):
    return x * 2

# 定义一个类
class TimesTwo(object):
    def __init__(self):
        pass
    def times_two(self, x):
        return x*2
```

## 科学计算库：numpy
利用numpy可以方便地进行向量和矩阵操作。

```python
import numpy as np
x = np.array([1.0,2.0,3.0,5.0])
y = np.array([2.0,3,4,5])
print(np.dot(x, y)) # 计算向量x和y的内积
```

## 绘图库：matplotlib
利用matplotlib会图库可以方便地画出各种图形。

```python
import matplotlib.pyplot as plt
x = np.array([1.0,2.0,3.0,5.0])
y = np.array([2.0,3,4,5])

plt.plot(x, y)
plt.show()
```


## 思考与实践
对比for循环方式对数组迭代和用numpy的速度。

```python
import numpy as np
import time

def for_loop(arr):
    """实现for循环将数组arr里面所有元素平方

    arr: np.array
    """
    pass

def np_loop(arr):
    """实现numpy将arr所有元素平方
    arr: np.array
    """
    pass



arr = np.random.rand(100000000)
start = time.clock()
for_loop(arr)
print('time for function for_loop: {:.3f}'.format(time.clock() - start))

start = time.clock()
np_loop(arr)
print('time for function np_loop: {:.3f}'.format(time.clock() - start))
```



<img src="/wiki/static/images/support-qrcode.png" alt="支持我" style="max-width:300px;" />
