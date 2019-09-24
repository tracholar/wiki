---
title: "λOpt: Learn to Regularize Recommender Models in Finer Levels"
layout: page
date: 2019-09-16
---

[TOC]


## 简介
- KDD2019,微软出品的论文。代码 <https://github.com/LaceyChen17/lambda-opt>
- 根据验证集上的效果, 更新正则系数;
- 不同的参数采用不同的正则

## 相关工作
- SGDA, 用训练集迭代模型参数$(\Theta)$, 用验证集迭代正则参数$(\lambda)$, 交替迭代。
    - Steffen Rendle. 2012. Learning recommender systems with adaptive regulariza- tion. In WSDM.
    - 迭代正则参数时, 优化任务是让$(\lambda)$ 最小化下一次模型迭代的损失函数。因为下一次迭代的参数是$(\lambda)$的函数,所以这个优化问题有非平凡解
    $$
    \lambda^* | \Theta^t = \arg\min_{\lambda} l(\hat{y}(x|\Theta^{t+1}), y)
    $$
- BPR: 
    - Steffen Rendle, Christoph Freudenthaler, Zeno Gantner, and Lars Schmidt-Thieme. 2009. BPR: Bayesian personalized ranking from implicit feedback. In UAI.
- 相比利用贝叶斯优化, 不用训练多次模型
- 利用自动微分, lambdaOPT可以应用到更复杂的推荐模型中
- 可以利用更先进的优化器,比如 Adam
- lambda正则项可以是user wise, item wise, etc
- Bilevel Optimization
