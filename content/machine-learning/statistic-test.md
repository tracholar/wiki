---
title: "统计检验"
layout: page
date: 2016-10-09
---
[TOC]

## 关于
汇总统计检验相关知识点。


## 独立性检验
- 卡方检验
- F检验


## KS（Kolmogorov–Smirnov）检验
检验数据拟合优度。[KS test](https://en.wikipedia.org/wiki/Kolmogorov%E2%80%93Smirnov_test)

检验统计量

$$
F_n(x) = \frac{1}{n} \sum_{i=1}^n I_{[-\infty, x]}(X_i) \\\\
D_n = \sup_{x} |F_n(x) - F(x)|
$$

- Wiener过程 $(W_t)$
    1. $(W_0 = 0)$
    2. 独立增量过程 $(W_{t+u} - W_t, u \ge 0)$ 独立于 $(W_s: s \le t)$
    3. 高斯增量 $(W_{t+u} - W_t \sim \mathcal{N}(0, u))$
    4. 连续路径，以概率1在t空间连续

Wiener过程作为 random walk 的极限！设 $(\xi_1, \xi_2, ...)$ iid，均值为0，方差为1的随机变量。对任意正整数n，定义连续时间随机过程

$$
W_n(t) = \frac{1}{n} \sum_{1 \le k \le nt} \xi_k, n \rightarrow \infty
$$

由中心极限定理可知，对于充分大的n，$(W_n(t) - W_n(s) \rightarrow \mathcal{N}(0, t-s))$。

- Brownian bridge，定义随机过程 $(B_t)$ 为

$$
B_t := (W_t | W_T = 0), t \in [0, T]
$$
其中 $(W_t)$是 Wiener 过程。

- Kolmogorov 分布，定义随机变量

$$
K = \sup_{t \in [0, 1]}|B(t)|
$$
其中 $(B(t))$ 是 Brownian bridge.

KS检验的检验统计量 $(\sqrt{n} D_n \rightarrow \sup_t |B(F(t))|)$，即近似服从 Kolmogorov–Smirnov 分布。
