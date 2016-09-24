---
title: "OWL-QN: Scalable Training of L1-Regularized Log-Linear Models"
layout: page
date: 2016-08-19
---
[TOC]

## 关于
论文导读：Scalable Training of L1-Regularized Log-Linear Models, Galen Andrew, Jianfeng Gao **Microsoft Research**, 2007.

## 问题
优化问题：

$$
f (x) = l(x) + r(x) \\\\
r(x) = \sum |x_i|   
$$

l1 范数优化，在0处不可导。

## 记号
左右偏导

$$
\partial_i^+ f(x) = \lim_{\alpha \rightarrow 0^+} \frac{f(x + \alpha e_i) - f(x)}{\alpha}
$$

- 方向导数 $(f'(x; d))$ 表示在方向d 对应的方向。
- 符号函数 $(\sigma(x))$
- 投影函数

$$
\pi_i(x; y) = \begin{cases}
                x_i & if \sigma(x_i) = \sigma(y_i)  \\\\
                0   & \text{otherwise}
                \end{cases}
$$


## OWL-QN 算法
在一个象限中，二阶近似是成立的，且二阶梯度矩阵与正则项无关。

对任意符号向量 $(\xi \in \\{-1, 0, 1 \\}^n)$，定义

$$
\Omega_{\xi} = \\{ x \in R^n | \pi(x, \xi) = x \\}
$$

在象限$(\Omega_{\xi})$中，目标函数可以写为

$$
f(x) = l(x) + C \xi^T x.
$$

将这个函数扩展到整个空间得到扩展函数$(f_{\xi})$，对于这个函数，可以用拟牛顿法求解。

伪梯度：其实就是将某个方向不可微点的梯度在该方向的分量置0.

<img src="/wiki/static/images/owlqn-algo.png" style="width:500px; " />

与正常L-BFGS算法的改动地方。

- 用伪梯度替代梯度
- 下降方向投影到伪梯度子空间，即和伪梯度不同符号的分量全部置0
- 采用约束线性搜索，以保证搜索不会超过该象限
- 计算向量y只需要损失函数就可以，不用正则项

约束线性搜索：在非0值，约束在原来的象限和0值搜索；在0值约束在负梯度方向！

$$
\xi_i = \begin{cases}
                \sigma(x_i^k) & x_i^k \neq 0  \\\\
                \sigma(- \Diamond_i f(x^k))   & x_i^k =0
                \end{cases}   \\\\
p^k = \pi(d^k; \xi_k)  \\\\
x^{k+1} = \pi(x^k + \alpha p^k; x^k)  \\\\
$$
