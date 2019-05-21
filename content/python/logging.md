---
title: "日志模块"
layout: page
date: 2019-05-19
---

[TOC]


## 基本配置

```python
logging.basicConfig(
    format='%(asctime)s %(levelname)-8s %(message)s',
    level=logging.INFO,
    datefmt='%Y-%m-%d %H:%M:%S')
```
