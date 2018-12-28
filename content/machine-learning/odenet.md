---
title: "ODENet - Neural Ordinary Differential Equations"
layout: page
date: 2018-12-23
---
[TOC]

- 论文: Ricky T. Q. Chen, Yulia Rubanova, Jesse Bettencourt, David Duvenaud. "Neural Ordinary Differential Equations." Advances in Neural Processing Information Systems. 2018. [arxiv](https://arxiv.org/abs/1806.07366)
- 代码: <https://github.com/rtqichen/torchdiffeq>

## 论文笔记
- 用神经网络拟合隐态的梯度, 通过ODE Solver 求解输出

残差网络可以看做一个常微分方程初值问题的差分形式

resnet

$$
h_{t+1} = h_{t} + f(h_t, \theta_t), t=0, 1,2,... \\\\
h_0 = input
$$

ode

$$
\frac{d h(t)}{d t} = f(h(t), \theta, t) \\\\
h_0 = input
$$

相当于在不同的时间上,参数共享!
