---
title: "FFM"
layout: page
date: 2016-09-27
tags: ["论文导读", "Machine Learning"]
---
[TOC]

## 关于
相比于逻辑回归，FM是一个二阶模型。

## 原理
线性模型：逻辑回归

$$
\phi(w, x) = w^T x \\\\
y = sigmoid(\phi(w, x))
$$

考虑所有二阶组合

$$
\phi(w, x) = x^T W x
$$

上式计算量为O(n^2)，通过将交叉项变换为平方差，然后合并同类项，可以将计算量减少到O(n)线性时间复杂度。

FM：将 W 分解为两个低秩矩阵的乘积！

$$
\phi(w, x) = x^T W^T W x, W \in \mathbb{R}^{k \times n} \\\\
W = (w_1, w_2, ..., w_n), w_i \in \mathbb{R}^{k}
$$

FFM：对每一个特征，将它划分到某个feild，一个特征和不同field的特征交叉时，使用不同的 $(w)$。

$$
W_{i,j} = w_{i, f_j}^T w_{j, f_i}
$$
