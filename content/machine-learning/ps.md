---
title: "Parameter Server for Distributed Machine Learning"
layout: page
date: 2016-11-21
---
[TOC]

## 关于
论文：Parameter Server for Distributed Machine Learning，NIPS2014。

## 一个优化问题
近似梯度法：

最小化下述问题

$$
f(w) + h(w)
$$

定义近似操作：

$$
\text{Prox}\_{\gamma}(x) = \arg \min_{y \in \mathbb{X}} h(y) + \frac{1}{2 \gamma} || x - y ||^2
$$

f是可微的，h有可能不可微，那么，对于学习率 $(\gamma_t > 0)$ 迭代方程是：？

$$
w(t + 1) = \text{Prox}\_{\gamma_t} [w(t) - \gamma_t \nabla f(w(t))]  \text{for } t \in \mathbb{N}
$$

一个博客： <http://blog.csdn.net/lanyanchenxi/article/details/50448640>
