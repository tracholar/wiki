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

## keras的layer相关注记
- call中的所有操作会自动置于 self.name 的name_scope下面，不用手动指定；但是创建的变量还是需要手动指定variable_scope
- 手动执行build方法时，还需要将模块的`built`属性设置为True
```python
dnn.build(input_shape[0])
dnn.built = True
```

## name_scope 与 variable_scope 的区别
- name_scope 只会加到op上，而 variable_scope 会加到op和variable上
- 只用 name_scope 时，创建了新变量，会导致变量在顶层scope中，不利于计算图可视化

