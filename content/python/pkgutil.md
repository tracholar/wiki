---
title: "pkgutil使用"
layout: page
date: 2020-04-29 20:00
---
[TOC]

## 动态包加载
```python
import pkgutil
__path__ = pkgutil.extend_path(__path__, __name__)
```

上述代码的目的是将`__path__`加到模块`__name__`的搜索路径中。这样就可以在两个不同的目录中扩展同一个模块了

## python 单模块多路径问题


