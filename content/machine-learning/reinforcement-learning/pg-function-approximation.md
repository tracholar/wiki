---
title: "策略梯度理论"
layout: page
date: 2018-02-05
---
[TOC]

## 关于
策略梯度理论早期经典论文笔记：Sutton R S, McAllester D A, Singh S P, et al. Policy gradient methods for reinforcement learning with function approximation[C]//Advances in neural information processing systems. 2000: 1057-1063.

其中 RS Sutton 就是《Reinforcement Learning - An Introduction》的作者

## 主要结论
1. 策略函数的梯度可以用经验数据中估计出来的动作-值函数(即Q函数)或者优势函数(Advantage function，即A函数)近似。
2. 证明了策略梯度近似可以收敛到局部最优策略，这也是策略梯度相比于值函数近似方法的优势，非线性函数的值函数近似方法无法保证收敛到局部最优！（不过线性函数还是可以的）

## 策略梯度基本理论
用$(\rho)$表示策略的性能测度，即期望回报，用$( \theta )$表示策略函数的参数，即$( \pi = \pi_{\theta}(a| s)   )$。那么策略梯度为

$$
\Delta \theta \approx \alpha \frac{\partial \rho}{\partial \theta}
$$

因此，当策略梯度趋于0时，可以保证$( \rho )$达到极小值！

相比值函数近似方法，策略梯度参数的很小变动只会带来很小的策略上的改变，而值函数近似方法可能带啦跳跃式的改变！

### 几个概念
策略的长期期望单步回报

$$
\rho(\pi) = \lim_{n \rightarrow \infty} \frac{1}{n} E[r_1 + r_2 + ... + r_n| \pi] =  \sum_s d^{\pi}(s )\sum_a \pi(s, a) R(s, a) \\\\
Q^{\pi}(s, a) = \sum_{kt1}^{\infty}E[  r_t -\rho(\pi)|s_0=s, a_0=a, \pi]
$$

- $( R(s, a) = E[r_{t+1} | s_t = s, a_t = a] )$是状态s下采取动作a的单步期望回报。
- $( d^{\pi}(s ) = \lim_{n \rightarrow \infty} Pr(s_t =s |s_0, \pi) )$ 是在该策略下状态s的稳态分布。

这里应该是认为这个马尔科夫链是没有终止态的，所以期望回报会等于稳态分布的单步平均值。

策略的长期折扣回报

$$
\rho(\pi) = E[\sum_{t=1}^{\infty}  \gamma^{t-1} r_t|s_0, \pi] =  \sum_s d^{\pi}(s )\sum_a \pi(s, a) R(s, a) \\\\
Q^{\pi}(s, a) = E[\sum_{k=1}^{\infty}  \gamma^{k-1} r_{t+k}|s_t=s, a_t=a, \pi]
$$

- $( d^{\pi}(s ) = \sum_{t=1}^{\infty} \gamma^{t-1} Pr(s_t =s |s_0, \pi) )$ 是在该策略下状态s的累积折扣次数

这一种定义是比较常用的一种，马尔科夫链可以有终止状态。

### 策略梯度公式
$$
\frac{\partial \rho}{\partial \theta} = \sum_s d^{\pi}(s) \sum_a \frac{\partial \pi(s, a)}{\partial \theta} Q^{\pi}(s, a)
$$
