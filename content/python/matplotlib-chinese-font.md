---
title: "Matplotlib 中文字体问题"
layout: page
date: 2017-10-17
---
[TOC]

## 问题背景
用 matplotlib 画图时，经常遇到中文(非ASCII)字体乱码问题，导致这些字符全部变成 □□！

## 问题原因
找不到中文字体

## 解决方法
首先下载 msyh.ttf 即微软雅黑字体文件，放到 matplotlib 的字体目录里面，或者系统字体中。
字体目录通过如下命令获取，不同系统不一样。

```python
import matplotlib
print(matplotlib.matplotlib_fname())
```
    /Library/Python/2.7/site-packages/matplotlib/mpl-data/matplotlibrc

/Library/Python/2.7/site-packages/matplotlib/mpl-data/ 为配置目录，在该目录下 fonts/ttf/ 目录就是存放字体文件的目录。


最简单的方法是在运行程序前，动态设置

```python
import matplotlib
matplotlib.rcParams[u'font.sans-serif'] = ['Microsoft YaHei'] + matplotlib.rcParams[u'font.sans-serif']

import numpy as np
import matplotlib.pyplot as plt
plt.plot(np.random.randn(100))
plt.title(u'测试雅黑字体')
```

也可以通过设置 matplotlibrc 文件，编辑该文件，编辑下述两项，去掉注释并将值设为下述值

```
font.family         : sans-serif

font.sans-serif      : Microsoft YaHei
```
可以通过下述代码查看系统支持的字体，MAC默认中文字体是 `STHeiti`

```python
fm = matplotlib.font_manager.FontManager()
for f in fm.ttflist:
    print f.name.decode('utf-8')
```

注意，要使得配置生效，还需要删除字体缓存！MAC 中字体缓存在 `~/.matplotlib/fontList.cache` ，删除该文件即可！
