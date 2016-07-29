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

## reference
1. stanford NLP ppt <http://nlp.stanford.edu/pubs/maxent-tutorial-slides-6.pdf>
