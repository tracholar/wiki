---
title: "Tensorflow 相关问题和答案"
layout: page
date: 2020-05-09
---
[TOC]


## MetaGraphDef、GraphDef、Graph 三者之间的关系是什么
- `Graph` 是由python构建的计算图数据结构，包含算子和张量，它序列化之后就是 `GraphDef`
- `GraphDef` 定义在 [graph.proto](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/framework/graph.proto)
- 但是，直接将 `GraphDef` 反序列化后是得不到 `Graph` 的，需要利用 `importer` 模块来将图导入到session中。[import_pb_to_tensorboard.py](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/python/tools/import_pb_to_tensorboard.py)
- 从 `GraphDef` 的定义来看，它只包含了`NodeDef`，而没有其他的信息，[`MetaGraphDef`](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/protobuf/meta_graph.proto) 扩展了`GraphDef`，记录了更多信息，包括`SaverDef`, `MetaInfoDef`,`CollectionDef` 等重要信息
- 参考<https://zhuanlan.zhihu.com/p/31308381>


## opname中的:0是什么意思
- `<op>:0`, `<op>:1` 表示的是第几个输出，试一下以下代码
- <https://stackoverflow.com/questions/40925652/in-tensorflow-whats-the-meaning-of-0-in-a-variables-name>
```python
import tensorflow as tf
a,b=tf.nn.top_k([1], 1)
print a.name # => 'TopKV2:0'
print b.name # => 'TopKV2:1'
```
