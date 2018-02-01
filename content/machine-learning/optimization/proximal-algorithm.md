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

### forward-backward splitting
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

如果两步都采用投影来做

$$
p = prox_{ g}\left( prox_f (p) \right)
$$

就是 backward-backward splitting.

### 投影梯度算法
求解约束优化问题

$$
\min_{x \in C} f_2(x)
$$

迭代算法

$$
x_{k+1} = P_C\left(x_k - \gamma \nabla f_2(x_k)\right)
$$

$(P_C)$是到凸集的投影，该算法的集合意义是先沿着f2的梯度方向下降，让后将结果点投影到集合C中，以保证解不会离开约束区域！

### 交替投影算法

求解约束优化问题

$$
\min_{x\in C} \frac{1}{2} d_D^2(x)
$$

$(d_D(x))$是点x到凸集D的距离，采 backward-backward 分割，可得迭代算法

$$
x_{k+1} = P_C(P_D(x_k))
$$

该算法的几何图像是，交替像两个凸集C和D进行投影，直到收敛！

## 投影算子计算
常规的计算方法是直接根据定义，求解优化问题：

$$
\min_y f(y) + \frac{1}{2\lambda}||y - x||^2
$$

|  | $(dom f)$有限 | $(dom f = R^n)$ |
---|---------------|-----------------|
f光滑|   投影梯度法、内点法  |   梯度下降        |
f不光滑|  投影次梯度法  | 次梯度法        |

### 二次函数
$(f(x) = 1/2x^T A x + b^T x +c )$，

$$
prox_{\lambda f} (v)= (I + \lambda A)^{-1}(y - \lambda b)
$$

如果 A = I, 且只有二次项，即$(f = || \cdot || _ 2^2)$，那么投影算子表现为shrinkage operator，直观来看就是把做了一个衰减！

$$
prox_{\lambda f}(v) = \frac{v}{1 + \lambda}
$$

### 标量函数
如果f是标量函数，自变量是单变量，那么很容易求得

$$
v \in \lambda \partial f(x) + x \\\\
x =prox_{\lambda f}(v) = (1 + \partial f)^{-1} v
$$

当$(f(x) = |x|)$时，有

$$
prox_{\lambda f}(v) = \max(|v| - \lambda , 0) sgn(v)
$$

即软阈值（soft thresholding）操作！

### 纺射集投影
集合$(C = \\{x| Ax = b \\})$,投影操作为

$$
\Pi_C(v) = v - A^\dagger(Ax - b)
$$

$(\dagger)$是[伪逆](https://en.wikipedia.org/wiki/Moore%E2%80%93Penrose_inverse)。

当A是一个向量时，相当于投影到超平面，

$$
\Pi_C(v) = v - \frac{a^Tx-b}{||a||_ 2^2} a
$$

### 半空间
集合$(C = \\{x| a^Tx \le b \\})$,投影操作为

$$
\Pi_C(v) = v - \frac{(a^Tx-b) _ + }{||a|| _ 2^2} a
$$

即要减掉向量在另外一边的分量！

### 集合的支持函数
集合C的支持函数定义为

$$
S_c(x) = \sup_{y\in C}y^T x
$$

结合解释是，以x为外法向量的点就是支持点！支持函数与示性函数共轭$(S_C^ * = I_C )$。
根据Moreau分解，知

$$
prox_{\lambda S_c}(v) = v - \lambda \Pi_C(v/\lambda)
$$

注意最后一个式子的几何理解！

### 范数
如果函数$(f = || \cdot ||)$，那么对偶函数$(f^ * = I_B)$， B是对偶范数的单位球！

$$
prox_{\lambda f}(v) = v - \lambda \Pi_B(v/\lambda)
$$

#### L2范数
L2范数的对偶范数还是L2范数，L2单位球就是欧式空间的单位球！所以

$$
\Pi_B(v) = \begin{cases}
            v/||v|| _ 2, \quad ||v|| _ 2 > 1, \\\\
            v, \quad ||v|| _ 2 \le 1
            \end{cases}
$$

所以L2范数的投影为

$$
prox_{\lambda f}(v) = \begin{cases}
            (1 - \lambda/||v|| _ 2) v, \quad ||v|| _ 2 > \lambda, \\\\
            0, \quad ||v|| _ 2 \le \lambda
            \end{cases}
$$

也就是将原始向量v沿着v方向减少$(\lambda)$长度，除非减到0向量！也叫做 block soft thresholding操作！

#### L1范数
L1范数的对偶范数是$(L_{\infty})$，对应的单位球是单位立方体，投影为

$$
\Pi_B(v) = \begin{cases}
            sgn(v_i), \quad |v _ i| > 1, \\\\
            v_i, \quad |v_i| \le 1
            \end{cases}
$$

所以

$$
v_{i+1} = \begin{cases}
    v_i - \lambda, \quad v_i>\lambda \\\\
    -v_i + \lambda, \quad v_i<-\lambda \\\\
    0, \quad other
    \end{cases}
$$


<img alt="软阈值" src="/wiki/static/images/soft-threshold.png" style="width:400px;float:left;" />


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

这表明，加了L1正则项，相当于将阈值为$(\lambda)$以内的分量都置0，以上的都减小$(\lambda)$。


### L2范数

### L21范数

## 相关资料
1. 比较全面介绍近似算法的书；Parikh N, Boyd S. Proximal algorithms[J]. Foundations and Trends® in Optimization, 2014, 1(3): 127-239.
2. 近似算法综述论文：Combettes P L, Pesquet J C. Proximal splitting methods in signal processing[M]//Fixed-point algorithms for inverse problems in science and engineering. Springer, New York, NY, 2011: 185-212.
3. 分布式ADMM经典论文：Boyd S, Parikh N, Chu E, et al. Distributed optimization and statistical learning via the alternating direction method of multipliers[J]. Foundations and Trends® in Machine Learning, 2011, 3(1): 1-122.
4. 压缩感知书籍：Foucart S, Rauhut H. A mathematical introduction to compressive sensing[M]. Basel: Birkhäuser, 2013.
