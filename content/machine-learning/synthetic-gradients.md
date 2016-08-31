---
title: "Decoupled Neural Interfaces using Synthetic Gradients"
layout: page
date: 2016-08-28
---
[TOC]

## 论文导读
这篇文章是 Google Deepmind 团队的一片文章，改进了BP算法在Leayer 层面上的串行计算结构。

## 导言
神经网络 BP 算法的几个 Lock：

- Forward Locking：每一模块（层）必须等到前面的层全部计算结束才能得到输入数据。
- Update Locking：每一个模块都要等到forward模式下依赖的模块计算完成才能更新。
- Backwards Locking：所有模块要等到forward模式和backward模式依赖的所有模块计算完成后才能更新。

现有神经网络更新权值的问题：Update Locking

- 多个异步模块构成的系统
- 分布式模型

本论文主要工作是移除了 Update Locking 依赖。权值更新方法：

$$
\frac{\partial L}{\partial \theta_i} = f_{Bprop}((h_i,x_i,y_i,\theta_i),(h_{i+1},x_{i+1},y_{i+1},\theta_{i+1}),...) \frac{\partial h_i}{\partial \theta_i} \\\\
\approx \hat{f}\_{Bprop}(h_i) \frac{\partial h_i}{\partial \theta_i}
$$

这里下标表示层数（或者模型序号），$(x, y)$表示输入和监督？h表示层的输出，该方法用一个神经网络对第一部分建模，而且只采用该层输出的局部信息！

Decoupled Neural Interface (DNI). 预测误差仅依赖与该层的输出，因而一旦该层计算完毕，就可以马上得到BP误差！
该梯度仅跟输出h有关，成为 synthetic gradients (合成梯度)，可以利用这个合成梯度立即更新该层网络权值！
从而移除了 Update Locking 和 Backwards Locking！

利用同样思想，移除Forward Locking?预测输入？

1. J. Baxter and P. L. Bartlett. Direct gradient-based reinforcement learning. In Circuits and Systems,
2000. Proceedings. ISCAS 2000 Geneva. The 2000 IEEE International Symposium on, volume 3, pages
271–274. IEEE, 2000.
2. **Y. Bengio**. How auto-encoders could provide credit assignment in deep networks via target propagation.
arXiv preprint arXiv:1407.7906, 2014.
