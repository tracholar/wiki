---
title: "sklearn 源码阅读计划"
layout: page
date: 2016-07-21
---
[TOC]

## 关于
sklearn是python的机器学习库，来源于Google summer 的一个项目，个人认为是一个设计地非常好的软件，
对模型的抽象很不错。另一方面，在学习理论的同时，自己写代码是一个加深理解的好方式，而阅读源码也是
一种很好的方式，而且还能学习别人的实现和优化技巧。

这个源码阅读计划可能持续较长，内容会总结在wiki中，最后形成文章在我的博客中出现。

## 基础类 base.py
sklearn使用5个基础类，对模型的功能进行抽象。用户可以基于这些类实现自己的模型，使得可以像操作sklearn
里面的其他模型那样调用。基础类放在包`sklearn.base`中

- BaseEstimator，要求构造方法能够设置模型参数，并且实现 `get_params` 和 `set_params` 两个方法。
- ClassifierMixin，实现`score`方法，返回 accuracy_score，所有的分类器的Mixin类
- RegressorMixin，实现`score`方法，返回 r2_score，所有回归器的Mixin类
- ClusterMixin，实现`fit_predict`方法，返回 聚类标签，所有聚类器的Mixin类
- BiclusterMixin，两聚类Mixin类
- TransformerMixin，实现`fit_transform`方法，转换器Mixin类
- MetaEstimatorMixin，MetaEstimator Mixin类

estimator有个属性`_estimator_type`，用来表示他是分类器还是回归器。这是通过两个Mixin类来实现的。

在`base`模块中，还提供一个`clone`函数，用于克隆一个现有的estimator。



## 线性模型


### 逻辑回归



## 特征选取


## 参考
1. [官方文档](http://scikit-learn.org/stable/modules/classes.html)
2. [github源码](https://github.com/scikit-learn/scikit-learn)
