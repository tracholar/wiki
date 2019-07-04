---
title: "Flink"
layout: page
date: 2019-07-04
---
[TOC]


## 关于
实时数据处理利器。

## 基本概念
### 编程模型
- 不同的抽象级别
    - stateful streaming: 通过 process function 嵌入到 `DataStream` API 中。提供event自由处理能力。
    - `DataStream` 和 `DataSet` API。绝大多数应用关注这个级别的API即可,关注数据的变换,聚合,JOIN
    - `Table API` 
![抽象级别](/wiki/static/images/levels_of_abstraction.svg)


### DataStream API

## 快速入门


## 参考链接
- <https://ci.apache.org/projects/flink/flink-docs-stable/>
- <https://ci.apache.org/projects/flink/flink-docs-stable/concepts/programming-model.html>
