---
title: "最优控制简介"
layout: page
date: 2018-02-03
---
[TOC]

## 关于
An introduction to Mathematical Optimal Control Theory 读书笔记


## 最优控制问题
受控微分动力系统

$$
\begin{align}
\dot{x}(t) &= f(x(t), \alpha(t)), t>0 \\\\
x(0) &= x^0
\end{align}
$$

Payoffs 函数

$$
P[\alpha(\cdot)] = \int_0^T r(x, \alpha) dt + g(x(T))
$$

g是终态回报，最优控制就是寻找是上式最大的控制函数！

当f是线性函数时，即系统是线性控制系统，存在bang-bang控制是最优控制！即$(|\alpha| = 1)$

最优控制实际上相当于系统模型已知的强化学习问题！并且是确定性系统，该系统用微分方程描述！

## 动态规划
值函数$(V(x, t))$

$$
V(x, t) = \sup_{\alpha \in A} P_{x, t} = \int_t^T r(x, \alpha) ds + g(X(T))
$$

值函数满足哈密顿-雅克比-贝尔曼方程HJB

$$
V_t + \max_{a \in A}\{ f(x, a) \cdot \nabla_x V + r(s, a)  \} = 0
$$

解释：即在最优控制下，单位时间值函数的减小量等于回报！

动态规划求解步骤是，先根据HJB方程求出值函数，然后根据值函数设计控制策略！
