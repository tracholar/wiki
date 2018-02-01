---
title: "近似算法"
layout: page
date: 2018-01-30
---
[TOC]

## 投影算子
点到闭凸集合的投影

$$
prox_C(x) = \arg\min_y I_C(y) + \frac{1}{2}||x - y||^2
$$

其中$(I_C)$是集合C的示性函数

$$
I_C(x) = \begin{cases}
        0, \quad x \in C \\\\
        \infty, \quad x \not\in C
        \end{cases}
$$

几何解释就是点x到集合C的投影就是C中到x最近的点！

把示性函数替换成一般的凸函数f，可以得到一般的投影算子

$$
prox_f(x) = \arg\min_y f(y) + \frac{1}{2}||x - y||^2
$$

不动点方程：如果一个点x*在f的投影下是他自己，那么根据上式，第二项为0，所以

$$
x^ * = prox_f(x^* ) = \arg\min_y f(x)
$$

几何解释就是在f的最小值等高面上的点的投影是它自己！

由于点x到投影点$(prox_f(x))$的方向向量$(x - prox_f(x))$与等高面垂直（投影的几何解释），因此，投影操作可以看做梯度下降的推广！
投影是往函数f的较小值等高面上进行投影！

令$(p = prox_{\lambda f}(x))$，

$$
p = \arg\min_y \frac{1}{2} ||y - x||^2 + \lambda f(y) \\\\
0 \in p-x + \lambda \partial f(p) \\\\
x \in (I + \lambda \partial f) (p) \\\\
p = (I + \lambda \partial f)^{-1} (x)
$$

注意，次梯度的逆有唯一的像！所以投影算子与次梯度关系为

$$
prox_{\lambda f}  = (I + \lambda \partial f)^{-1}
$$

如果$(\lambda)$很小且f存在常规梯度，那么可以近似为

$$
prox_{\lambda f}(v) \approx v -  \lambda \nabla f(v)
$$

也就是说 **投影操作是梯度下降的一种推广**！实际上，可以看做一种前向梯度下降，即下降的梯度不是在当前点v计算得到的，而是在下降的目标点p计算得到的！


如果投影操作计算方便（有简单的解析解），那么用投影操作做优化可以取代梯度下降，并且可以应用到梯度下降没法用的场景——梯度不存在的函数优化！

近似点算法：求函数f的最小值，利用投影算子是压缩算子，且投影算子的不动点是f的最小值点性质可得迭代近似点算法

$$
x_{n+1} = prox_{\lambda f}(x_n)
$$

前面说到投影算子就相当于梯度下降的推广，那么近似点算法可以看做梯度下降求最小值的推广！

## Moreau 分解
点v可以分解为投影和共轭投影之和

$$
v = prox_f(v) + prox_{f^ * }(v) \\\\
f^ * (y) = \sup_x \left(y^T x - f(x)\right)
$$

这个分解可以看做几何中的正交分解！

## 近似梯度法
如果目标函数存在不可微分部分，可以将目标函数分解为两部分，一部分可以微分，另一部分不可微分

$$
\min_x f(x) + g(x)
$$

假设f可微，g是不可微部分。若$(p)$是最优解，那么根据最优条件可知

$$
0 \in \lambda \nabla f(p ) + \lambda \partial g(p ) \\\\
0 \in \lambda \nabla f(p) - p + p+ \lambda \partial g(p ) \\\\
(I - \lambda \nabla f) (p) \in (I + \lambda \partial g) (p ) \\\\
p = (I + \lambda \partial g)^{-1} (I - \lambda \nabla f) (p) \\\\
p = prox_{\lambda g}\left( p - \lambda \nabla f(p)  \right)
$$

上述不动点方程给出了优化迭代步骤！

## 软阈值算法
### L1范数正则
L1正则问题

$$
\min_x f(x) + \lambda |x|_ 1
$$

利用近似梯度法，令$(g=|\cdot| _ 1)$有迭代算法

$$
z_k = x_k - \eta  \nabla f(x_k) \\\\
x_{k+1} = prox_{g} z_k
$$

最后一步是一个投影，根据定义

$$
prox_{\lambda g} (v) = \arg \min_x \lambda |x| _ 1 + \frac{1}{2}||x - v||^2
$$

对上式微分得

$$
v_i - x_i \in \lambda \partial |x_i|, i=1,...
$$

上式表明下降量不超过$(\lambda)$，如果$(v_i)$绝对值大于$(\lambda)$那么下降不会越过不可微分点，可以按照正常的梯度下降，
但是如果小于，那么只能下降到0，才能保证上式成立！

$$
x_i = \begin{cases}
    v_i - \lambda, \quad v_i>\lambda \\\\
    -v_i + \lambda, \quad v_i<-\lambda \\\\
    0, \quad other
    \end{cases}
$$

这表明，加了L1正则项，相当于将阈值为$(\lambda)$以内的分量都置0，以上的都减小$(\lambda)$，图像如下

![软阈值](/wiki/static/images/soft-threshold.png)

### L2范数

### L21范数

## 相关资料
1. 比较全面介绍近似算法的书；Parikh N, Boyd S. Proximal algorithms[J]. Foundations and Trends® in Optimization, 2014, 1(3): 127-239.
2. 近似算法综述论文：Combettes P L, Pesquet J C. Proximal splitting methods in signal processing[M]//Fixed-point algorithms for inverse problems in science and engineering. Springer, New York, NY, 2011: 185-212.
3. 分布式ADMM经典论文：Boyd S, Parikh N, Chu E, et al. Distributed optimization and statistical learning via the alternating direction method of multipliers[J]. Foundations and Trends® in Machine Learning, 2011, 3(1): 1-122.
4. 压缩感知书籍：Foucart S, Rauhut H. A mathematical introduction to compressive sensing[M]. Basel: Birkhäuser, 2013.
