---
title: "MXNET学习"
layout: page
date: 2016-12-24
---
[TOC]




## Rabit
教程地址：<https://github.com/dmlc/rabit/tree/dev/guide>

RABIT：Reliable Allreduce and Broadcast Interface

两个关键的操作：Allreduce 和 Broadcast

**Allreduce**: 和不同的reduce不同，以 max 操作为例，reduce将很多个节点的值变成一个最大的值，
而 allreduce 将每一个节点都变成最大值；这在机器学习操作中经常用到，比如LBFGS同步所有节点
的梯度为全局梯度！

**Broadcast**：和 Spark 的broadcast一样，将一个节点的值广播到所有的节点！

参考 KMEANS 的例子，了解这两个操作：<https://github.com/dmlc/wormhole/blob/master/learn/kmeans/kmeans.cc>


### checkpoint
用于每一次迭代保存模型，以此保证容错能力，API：`LoadCheckPoint`, `CheckPoint`

### Allreduce and Lazy Preparation
```cpp
Allreduce<operator>(pointer_of_data, size_of_data);
```
