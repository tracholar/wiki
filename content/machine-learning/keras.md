---
title: "Keras 深度学习库"
layout: page
date: 2016-07-08
---
[TOC]

## 关于
据说pylearn2停止开发了，当时觉得pylearn2虽然编码少，但是配置和文档使用不便，而且和其他库的融合
也不方便。后来看到有人推荐`keras`，了解了一下，发现很不错。他的底层编译采用`theano`，现在也加入
了`tensorflow`的支持。并且还可以与`scikit-learn`融合，将`keras`的模型包装成`scikit-learn`
里面的模型。基于这两点，决定学习这个库，初试了一下，感觉很不错。

## 快速入门 Step by step
快速入门教程参考官方文档<http://keras.io>

Step1. 创建`Sequential`模型，通过`Sequential.add`方法添加层。
```python
from keras.models import Sequential
model = Sequential()

from keras.layers import Dense, Activation
model.add(Dense(output_dim=64, input_dim=100))
model.add(Activation("relu"))
model.add(Dense(output_dim=10))
model.add(Activation("softmax"))
```

Step2. 编译模型，可以指定优化方法和损失函数等
```python
model.compile(loss='categorical_crossentropy', optimizer='sgd',
             metrics=['accuracy'])
```

Step3. 调用`fit`方法训练模型，这里采用随机生成的数据
```python
x_train = np.random.randn(1000,100)
y_labels = np.random.randint(0,10,size=(1000,))
y_train = np.zeros((1000,10))
y_train[range(1000), y_labels] = 1

model.fit(x_train, y_train, nb_epoch=5, batch_size=20)
```

Step 4. 模型预测，这里采用另一组随机生成的数据
```python
x_test = np.random.randn(1000,100)
y_labels = np.random.randint(0,10,size=(1000,))
y_test = np.zeros((1000,10))
y_test[range(1000), y_labels] = 1

classes = model.predict_classes(x_test, batch_size=20)
proba = model.predict_proba(x_test, batch_size=20)
```
