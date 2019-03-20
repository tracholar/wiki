---
title: "优化相关论文摘要"
layout: page
date: 2019-03-18
---
[TOC]


## 参考
- 论文: DON’T DECAY THE LEARNING RATE, INCREASE THE BATCH SIZE
- 论文: A Bayesian Perspective on Generalization and Stochastic Gradient Descent
- 如果要减少学习率, 那么可以替换为同等比例地增加batch size
- 梯度下降可以表述为一个随机微分方程, w是模型参数,C是损失函数,t是步长,$(\eta)$是随机噪声
- 随机噪声的的均值为0, 方差幅度 $(g = \epsilon (N/B - 1))$, epsilon 是学习率,N是总训练样本,B是batch size, 因此, 减少学习率也可以通过增加B来实现。
$$
\frac{d w}{d t} = - \frac{d C}{d t} + \eta(t)
$$