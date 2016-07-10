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
如果要对变量作用域里面的所有变量用同一个初始化方法，可以在定义作用域的时候指定。
参考<https://www.tensorflow.org/versions/r0.7/how_tos/variable_scope/index.html#initializers-in-variable-scope>

```python
with tf.variable_scope("foo", initializer=tf.constant_initializer(0.4)):
    v = tf.get_variable("v", [1])
    assert v.eval() == 0.4  # Default initializer as set above.
```

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

tf.placeholder(dtype, shape=None, name=None)
```
注意，这里有个大坑，这个字典的键是一个`op`，而不是一个字符串！！

- 变量作用域，`variable_scope`, `get_variable_scope`, `get_variable`


- word embedding

## 优化
当得到损失函数之后，可以通过`tf.train.Optimizer`优化工具来进行优化，实际优化的时候使用的是他的子类，
如`GradientDescentOptimizer`, `AdagradOptimizer`, or `MomentumOptimizer`。
优化obj通常有以下几个重要方法可以使用。

- `train_op = minimize(loss)`，直接最小化损失函数
- `compute_gradients(loss)`，计算梯度
- `train_op = apply_gradients(grad)`，应用梯度更新权值

## TIPS
- `tf.reshape(some_tensor, (-1, 10))`将数据重新划分为10列的元素，第一维自适应
- `tf.device` 指定CPU或者GPU
- `tf.add_to_collection(name, value)`将value保存为名字为name的共享集合中，供后面使用.
  `tf.get_collection(name)`，获取存储的值

## ADAM 随机梯度下降算法


[1] <https://www.tensorflow.org/>
[2] <http://cs224d.stanford.edu/lectures/CS224d-Lecture7.pdf>
