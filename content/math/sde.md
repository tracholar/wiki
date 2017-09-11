---
title: "随机微分方程"
layout: page
date: 2017-04-10
---
[TOC]

## 关于
随机微分方程的读书笔记. An Introduction with Applications. 6ed.

## 数学基础
### 概率空间、随机变量、随机过程
- 随机变量是联系事件域与欧式空间的桥梁。 $$X: \Omega \rightarrow R^n $$
- 对欧时空间的积分定义在集合测度上，对随机变量的积分定义在概率测度上。

$$
\int_{R^n} f(x) dx \\\\
\int_{w \in \Omega} f(X(w)) dP(w) = \int_{R^n} f(x)d\mu_X(x)
$$

$$dx$$是几何测度，$$d\mu_X(x)$$是概率测度。

- 利用对随机变量的积分定义，可以构造出Lp空间、巴拿赫空间、希尔伯特空间。
