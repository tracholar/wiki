---
title: "tensorflow google 开源机器学习库"
layout: page
date: 2016-07-01
---
[TOC]

## TensorFlow

## 核心图数据结构
class tf.Graph

## 基本数据类型
- constant
```python
a = tf.constant(5.0)
b = tf.constant(6.0)
c = a * b
with tf.Session() as sess:
    print sess.run(c)
    print c.eval()      # just syntactic sugar for sess.run(c) in the currently active session!
```
- Session
    - `tf.InteractiveSession()`
    - default session
- Variables，用来表示模型参数。
> “When you train a model you use variables to hold and
> update parameters. Variables are in-memory buffers
> containing tensors” - TensorFlow Docs.

```python
W1 = tf.ones((2,2))
W2 = tf.Variable(tf.zeros((2,2)), name="weights")
R = tf.Variable(tf.random_normal((2,2)), name="random_weights")
```
使用函数`tf.initialize_all_variables()`参数初始化，初始值在定义的时候给出。

变量的更新，使用方法`tf.add`, `tf.assign`等方法
```python
state = tf.Variable(0, name="counter")
new_value = tf.add(state, tf.constant(1))
update = tf.assign(state, new_value)
with tf.Session() as sess:
    sess.run(tf.initialize_all_variables())
    print sess.run(state)
    for _ in range(3):
        sess.run(update)
        print sess.run(state)

0
1
2
3
```

利用`tf.convert_to_tensor`方法可以将数值变量转换为张量。

- placeholder用来输入数据，通过`feed_dict`字典将输入数据映射到placeholder.
```python
input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)
output = tf.mul(input1, input2)
with tf.Session() as sess:
    print sess.run([output], feed_dict={input1:[7.], input2:[2.]})
```

- 变量作用域，`variable_scope`, `get_variable_scope`, `get_variable`




## TIPS
`data.reshape(-1, 10)`将数据重新划分为10列的元素，第一维自适应


## ADAM 随机梯度下降算法


[1] <https://www.tensorflow.org/>
[2] <http://cs224d.stanford.edu/lectures/CS224d-Lecture7.pdf>
