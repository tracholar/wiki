---
title: "Lambda Rank"
layout: page
date: 2017-12-10
---
[TOC]

## 背景
排序问题和分类回归问题本质差异在于优化的目标不同，除此之外，分类回归问题的模型应该都能用。
但是排序问题存在一些非连续优化目标，如NDCG，直接优化该目标并不容易。Lambda Rank是一种近似优化方法。

排序问题归根结底也是对每个样本打分，目标是使得应该排在前面的样本得分更高！
通常排序会有一个GROUP，GROUP之间的序关系没有意义，有意义的是GROUP内部的序关系。
例如搜索排序中，query就是一个GROUP，对给定的一个GROUP，将另外一种实体e进行排序。

一般将二元组(q, e)看做一个样本，如果考虑个性化，还要加上用户这个实体。总而言之，将这些实体多元组看做一个样本，
提取一些列特征$(x_i)$，然后构建一个模型$(f)$对它进行打分$(f(x))$。

## Rank Net
将样本$(U_i)$排在$(U_j)$前面的概率$(P(U_i \succ U_j))$建模成得分差值的sigmoid函数！

$$
P_{ij} = P(U_i \succ U_j) = \frac{1}{1 + e^{-\sigma(s_i - s_j)}} \\\\
s_i = f(x_i)
$$

通过对所有可比较的样本对计算此概率值，然后进行极大似然估计得到损失函数。
由于一个样本$(x_i)$会同时出现在多个样本对中，损失函数对$(s_i)$的梯度$(\lambda_i)$会跟多个样本对有关，

$$
\lambda_i = \frac{\partial l}{\partial s_i} = \sum_k \frac{\partial l_k}{\partial s_i}
$$




## 参考
1. From RankNet to LambdaRank to LambdaMART: An Overview
