---
title: "最大熵模型"
layout: page
date: 2016-07-25
---
[TOC]

## PPT 记录
- conditional or discriminative probabilistic models in NLP , IR, and Speech
- 联合概率 or 条件概率：生成模型 or 判别模型

## 最大熵模型 - 多元逻辑回归
可以证明，多元逻辑回归是在存在约束的条件下，最大化输出的分布 $(\hat{y} = p(y|x))$ 的熵。
[LogisticRegressionMaxEnt.pdf](http://www.win-vector.com/dfiles/LogisticRegressionMaxEnt.pdf)。

对于链接中的证明，有个问题是，那个关键的约束关系是通过逻辑回归的式子，利用极大似然导出的，
后面又用来证明从最大熵原理推导出具体表达式，这不是循环论证么？！

结论：最大熵模型就是多元逻辑回归，他也是建模条件概率，而不是后验概率或者联合概率，是判别模型；
通常我们说建模后验概率（贝叶斯的方法）实际上就是建模联合概率，是生成模型。

## 最大熵模型
给定数据集 $((x, y))$，其经验分布为$(\tilde{p}(x, y))$。$(f(x, y))$ 是样本空间到实数集合的映射，成为特征！
目标是学习得到条件分布$(p(y|x))$！为此，可以让该条件分布满足最大熵原理即

$$
\max H(p) = - \sum_{x,y} \tilde{p}(x) p(y|x) \log p(y|x) \\\\
s.t. p(f_i) = \tilde{p}(f_i), i=1,...,n  \\\\
\sum_{y} p(y|x) = 1     \\\\
$$
这里：
$$
p(f_i) = \sum_{x,y} \tilde{p}(x)p(y|x) f_i(x,y) \\\\
\tilde{p}(f_i) = \sum_{x,y} \tilde{p}(x, y) f_i(x,y)
$$

上面$(p(f_i), \tilde{p}(f_i))$ 分别表示特征$(f_i)$在样本中的期望值和在条件分布$(p)$下的期望值，
约束条件要求这两者相同。如果没有这个约束，那么最大熵的分布就是均匀分布（闭区间）、指数分布（半无限区间，均值约束）和正态分布（无限区间，均值和方差约束）了！

利用拉格朗日对偶原理，可得拉格朗日函数：

$$
L(p, \lambda, \gamma) = - \sum_{x,y} \tilde{p}(x) p(y|x) \log p(y|x) + \sum_i \lambda_i (p(f_i) - \tilde{p}(f_i)) +\gamma (\sum_{y} p(y|x) - 1)
$$

由 KKT 条件，$(\partial L / \partial p(y|x) = 0, \partial L / \partial \gamma = 0)$可得：

$$
p^* (y|x) = \frac{1}{Z_{\lambda}(x)} \exp(\sum_i \lambda_i f_i(x,y))  \\\\
Z_{\lambda}(x) = \sum_y \exp(\sum_i \lambda_i f_i(x,y))
$$

而其对偶问题优化的对象变为

$$
\Phi(\lambda) = -\sum_x \tilde{p}(x)\log Z_{\lambda}(x) + \sum_i \lambda_i \tilde{p}(f_i)
$$

这个目标函数可以看做似然对数！其中条件概率采取上述指数模型建模！（和逻辑回归模型一致）

$$
L_{\tilde{p}}(p) = \sum_{x,y} \tilde{p}(x,y) \log(y|x)
$$

和逻辑回归的关系：取特征映射为$(f_i(x, y) = f_i(x))$即样本的第$(i)$个特征分量！


## reference
1. stanford NLP ppt <http://nlp.stanford.edu/pubs/maxent-tutorial-slides-6.pdf>
