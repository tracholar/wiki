---
title: "Hivemall源代码阅读"
layout: page
date: 2019-10-30
---
[TOC]

# 概述
Hivemall 是一个可以运行在Hive和Spark上用SQL来训练机器学习模型的一个开源库。


# 总体设计

## 分类
- `UDTFWithOptions` 实现了`parseOptions`处理命令行参数，将`getOptions`留给子类自己处理。`showHelp` 通过获取注解的UDF描述信息，生成帮助文本
    - `LearnerBaseUDTF`
        - `BinaryOnlineClassifierUDTF`
        