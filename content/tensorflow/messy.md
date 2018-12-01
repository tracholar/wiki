---
title: "Tensorflow 未归档知识点"
layout: page
date: 2018-11-01
---
[TOC]


## GFile

TensorFlow 提供GFile API,用于操作文件系统,包括本地文件,google云存储 `gs://`,以及HDFS `hdfs://`。

> File I/O wrappers without thread locking

```python
text_dir = 'hdfs://user/xxx/data/dir'
file = tf.gfile.Open(text_dir, 'w')
s = 'hello world!'
file.write(s)
file.close()
```


## AUC计算
Tensorflow 提供AUC计算的方法`tf.contrib.metrics.streaming_auc(predict, label)` 或者 `tf.metrics.streaming_auc(label, predict)`


## tf.nn.embedding_lookup_sparse
如果输入样本有空值,那么会出现shape不匹配错误。当输入存在特征idx和val都是空的样本,那么就会出现这个问题。此时应该补0!

```
InvalidArgumentError (see above for traceback): Incompatible shapes: [17,1] vs. [32,1]
```