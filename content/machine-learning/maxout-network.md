---
title: "MaxOut 网络"
layout: page
date: 2016-08-04
---
[TOC]

## 关于
2013年， Ian Goodfellow, Yoshua Bengio 提出maxout网络，号称只需要两个隐层，就可以逼近任意分片连续函数。

## Maxout networks 论文导读
论文地址 [maxout networks](http://jmlr.org/proceedings/papers/v28/goodfellow13.pdf)

### 导言
Dropout 是 Hinton 2012 年提出的一种简单的方法， 模型平均？！

> We argue that rather than using dropout as a slight
> performance enhancement applied to arbitrary models,
> the best performance may be obtained by directly
> designing a model that enhances dropout’s abilities as
> a model averaging technique

### Review of Dropout
对于给定的输入向量$(v)$，预测输出$(y)$，一系列的隐层 $(\vec{h} = \{ h^{(1)}, ..., h^{(L)} \})$。
Dropout 用变量$(v, \vec{h})$ 的一部分变量，训练一组模型（这是随机置0的高级理解！！？）。
这些模型采用同一个模型参数 $(\theta \in \mathcal{M})$，和一个二元 mask 变量 $(\mu)$。
对于每一个样本，对不同的随机参数$(\mu)$，我们通过$(log p(y|v; \theta, \mu))$ 的梯度训练一个
子模型。

Dropout 很像 bagging 的方法，在数据集的很多子集上训练一组模型。
不同的是， Dropout 每一个模型只训练一步，并且不同模型共享一个参数。
因此，在训练的时候，每一步需要比较大的影响（把步长调大？）。

### Maxout 模型
Maxout 模型也是一个多层的前馈结构，它的基本单元是 Maxout unit。
对给定的 $(x \in \mathbb{R}^d)$，一个 maxout 隐层为：

$$
h_i(x) = \max_{j \in [1, k]} z_{i,j} \\\\
z_{i, j} = x^T W_{.ij} + b_{ij}, W \in \mathbb{R}^{d \times m \times k}, b \in \mathbb{R}^{m\times k}
$$

相当于$(k)$个常规的纺射变换（一个线性变化$(W)$ 加上平移 $(b)$），然后取每一个模型的最大值作为最终的输出。

单个 Maxout 单元可以理解为，对凸的分片连续函数的近似。

- Stone-Weierstrass 函数逼近理论说，紧集（闭区间的推广）上的连续函数可以用连续的分片线性函数(PWL) **一致逼近**。
- Wang 2004 的理论表明，任意连续分片线性函数可以表达为两个凸的分片线性函数之差。

个人总结：Maxout 模型很简单，理论也容易理解，就是要逼近效果好，需要$(k)$值足够大，这将会导致参数个数随$(k)$增大，而线性增长。

### 效果
- MNIST 手写数字数据集两层 Conv. maxout + dropout 得到最佳效果 0.45%
