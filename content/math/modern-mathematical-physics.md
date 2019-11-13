---
title: "现代数学物理教程"
layout: page
date: 2019-11-10
---
[TOC]

# 10 拓扑
## 欧几里得拓扑
- 实线 R 和欧式平面 $(R^2)$
- 对实数集合R，x的领域是指存在$(\epsilon > 0)$， 开区间 $(  (x - \epsilon, x + \epsilon) \subset R )$
- 序列$( \{ x_n \} )$ 收敛到 x （记作 $(x_n \rightarrow x)$），是指对任意$( \epsilon >0 )$，存在足够大的 N >0, 使得对所有 n > N 都有 $( |x - x_n| < \epsilon )$
- 连续定理：函数 $(f : \mathcal{R} \rightarrow \mathcal{R})$ 在$(\mathcal{R})$中连续，当且仅当对任意开集 $( U \subset R )$ 的原像 $(V = f^{-1}(U))$ 也是 $(\mathcal{R})$ 中的开集

## 广义拓扑空间
- 给定集合X，X中的拓扑是一个子集族$(\mathcal{O})$，也叫开集，满足
    1. 空集$(\Phi)$是开集，全集X是开集，$(\{ \Phi, X \} \subset \mathcal{O})$
    2. 交运算封闭：如果$(U,V \in \mathcal{O} )$是开集，那么交集$( U \cap V \in \mathcal{O} )$
    3. 并运算封闭：如果$(\\{V_i|i \in I \\})$ 是开集族，那么并集$(\cup_{i \in I} V_i)$ 也是开集。
- 集合X和拓扑$(\mathcal{O})$共同构成了拓扑空间 $((X, \mathcal{O}))$, X中的元素叫做点
- 欧式空间$(R^n)$的拓扑，定义开集为：对子集$(U \subset R^n)$，如果U中的任意点x，存在一个开球
$$
B_r(x) = \\{y\in R^n| |y - x| < r \\} \subset U
$$
距离按照正常欧式空间的定义。这些所有的开集组成开集族，构成欧式空间中的拓扑，这种拓扑也就是标准拓扑！
- 闭集：如果V的补X - V 是开集，那么V是闭集。闭集的有限并是闭，无限并就可能是开集了
- 如果A是X的子集，A上的相对拓扑是指开集族
$$
\mathcal{O}_A = \\{ A \cap U | U \in \mathcal{O}  \\}
$$

## 距离空间
- 如果在拓扑空间$( (M, \mathcal{O}) )$ 中定义距离函数 $(d : M \times M \rightarrow R)$。距离函数要满足，半正定、对称、三角不等式。那么这个空间就构成一个距离空间。

## Hausdorff空间
- 如果X中的点是可分的，那么就是一个Hausdorff空间。可分是指，对任意 $(x, y \in X)$，存在不相交的开邻域$(x \in U)$ 和 $(y \in V)$，$(U \cap V = \Phi)$。也就是存在两个包含这两个点的互不相交的开集。

## 紧空间 





# 15 微分几何
- 微分流形，局部欧式空间性质的拓扑空间
- 一个局部欧式空间或者n维拓扑流形M：
    1. M是Hausdorff拓扑空间
    2. 对每一个点x存在一个邻域同胚于$(R^n)$的一个开子集