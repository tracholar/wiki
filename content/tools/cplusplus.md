---
title: "C/C++拾遗"
layout: page
date: 2019-08-24
---
[TOC]

## 拾遗
- 可变参数宏 
```c++
#define showUpdate(...) { fprintf(stderr, __VA_ARGS__ ); }
```

