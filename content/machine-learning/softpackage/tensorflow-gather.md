---
title: "张量索引"
layout: page
date: 2019-03-19
---
[TOC]

## 1维索引 tf.gather
- `tf.gather(tensor, i) = tensor[i]`, i是一个标量
- `tf.gather(tensor, [i1, i2, ..., ik]) = [tensor[i1], tensor[i2], ..., tensor[ik]]`
- `tf.gather(x, ind) = y` 更高维索引, 相当于将ind中每一个元素i替换成x[i], `y[ij...k] = x[ind[ij..k]]`,可以看到,它永远只在x的第一维上索引

```python
In [31]: tf.gather(tf.constant([0,1,2,3,4]), tf.constant(2)).eval()
Out[31]: 2


In [37]: x = tf.constant([[2,3,],[0,1],[3,6]])
In [38]: tf.gather(x, tf.constant(1)).eval()
Out[38]: array([0, 1], dtype=int32)

In [39]: tf.gather(x, tf.constant([1, 2])).eval()
Out[39]:
array([[0, 1],
       [3, 6]], dtype=int32)
```


## 多维索引 tf.gather_nd
- `tf.gather(tensor, i) = tensor[i]`, i至少要是一个向量, 实现一个多维索引
```python
x = tf.constant([[2,3,],[0,1],[3,6]])
tf.gather(x, tf.constant([1, 0])).eval()  # x[1,0] = 0
```

- `tf.gather_nd(x, ind)` 相当于将ind最后一个维度上的向量作为一个多维索引
```python
x = tf.constant([[2,3,],[0,1],[3,6]])
tf.gather(x, tf.constant([[1], [2]])).eval() # [x[1], x[2]] = [[0, 1], [3, 6]]
```

