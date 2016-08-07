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
### `base.py`

- `LinearModel` 线性模型的基础类
    - 继承 `BaseEstimator`，提供抽象方法 `fit` 需要子类实现。
    - 它实现了 `predict` 方法，用来预测，该方法直接输出 `_decision_function` 的结果！
    - 它的核心就是这个 `_decision_function`，代码很简单 `safe_sparse_dot(X, self.coef_.T, dense_output=True) + self.intercept_`。
就是对特征向量`X`做一个仿射变换 `y = W X + b`。此外，还有一个 `_set_intercept` 用来设置偏置$(b = \bar{y} - w \bar{x})$。
    - 在实现的时候，会对数据做中心化处理，有一个私有的中心化函数 `_conter_data`。

- `LinearClassifierMixin` 线性分类器 Mixin，只处理预测
    - 核心的函数是 `decision_function` 返回样本到超平面的有向距离，和上面的函数一样（？为什么分开写）。
    - `predict` 如果是两分类，返回 `(scores > 0).astype(np.int)`，多分类则返回距离最大的那个index `scores.argmax(axis=1)`，
最后会将结果映射会label标签的值。
    - `_predict_proba_lr` LR的私有方法，输出概率！代码采用 inplaced 优化空间，可以看[这个代码](https://github.com/scikit-learn/scikit-learn/blob/51a765acfa4c5d1ec05fc4b406968ad233c75162/sklearn/linear_model/base.py#L284)


- `SparseCoefMixin` 稀疏系数 Mixin 类。稀疏稀疏和普通系数互相转换。L1 正则化需要继承这个类
    - `densify` 方法将稀疏系数转为普通向量。
    - `sparsify`  转为稀疏系数

- `LinearRegression` 线性回归模型，继承 `LinearModel, RegressorMixin`。
    - `residues_` 采用 `@property` 修饰实现只读属性！0.19要废弃这个属性
    - [`fit` 方法](https://github.com/scikit-learn/scikit-learn/blob/51a765acfa4c5d1ec05fc4b406968ad233c75162/sklearn/linear_model/base.py#L440)
    是核心。对于稀疏数据，
### 逻辑回归



## 特征选取


## 参考
1. [官方文档](http://scikit-learn.org/stable/modules/classes.html)
2. [github源码](https://github.com/scikit-learn/scikit-learn)
