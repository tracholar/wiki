---
title: "Adaptive Gradient Methods with Dynamic Bound of Learning Rate"
layout: page
date: 2019-02-27
---
[TOC]


## 关于
- 参考: Adaptive Gradient Methods with Dynamic Bound of Learning Rate

## 摘要与间接
- 关键点: 解决Adam的不收敛问题,动态地调整学习率的界,实现平滑地从Adam和AMSGRAD演变为SGD
- 历史进程
    1. Robbins & Monro 1951年发明了SGD
    2. SGD在所有方向的缩放是相同的,对于稀疏优化不太适合
    3. 所以,人们发明了自适应方法, 在不同的方向上以累积梯度的均方根成反比的缩放学习率
        - ADAM
        - ADAgrad
        - RMSProp
- 自适应学习率方法在初始的时候, 学习的很快, 但是很快在测试集上的效果出现饱和, 反而是最原始的SGD收敛的更好
- AMSGRAD

![generic-optimize-algorithm](/wiki/static/images/generic-optimize-algorithm.png)

- $(\phi_t: \mathcal{F}^t \rightarrow \mathbf{R}^d)$ 相当于梯度的一阶矩,它是历史上t个梯度的函数
- $(\psi_t: \mathcal{F}^t \rightarrow \mathbf{S} _ +^d)$ $(\mathbf{S} _+^d)$是d阶正定方阵, 相当于二阶矩, 不过是一个方阵, 常见的那种二阶矩是一个向量,可以看做对角方阵, 那么向量的元素除法就可以看做对对角矩阵的除法了
- $(\Pi_{\mathcal{F}, M}(y) = \arg\min_{x \in \mathcal{F}} ||M^{\frac{1}{2}}(x - y)||)$ 即点y向空间$(\mathcal{F})$中的投影,M可以理解为空间的度规。




