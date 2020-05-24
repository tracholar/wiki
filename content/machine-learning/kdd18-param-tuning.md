---
title: "Online Parameter Selection for Web-based Ranking Problems"
layout: page
date: 2020-02-19
---
[TOC]

# 论文简介

## 基本问题
- 多个模型预测分融合排序，线性加权的权重调整是个问题，目标是多个业务目标的最大化。（这是个多目标优化的问题）
- 传统的方法是，在线利用AB测试来找到一组最优参数。文章的做法是利用贝叶斯优化的方法更快地找到最优参数（帕累托最优）。
- 其次，文章的方法自适应地调参，可以解决最优参数随时间漂移的问题。
- 问题重定义：最大化某个指标，在另一个指标最多降低一个特定阈值的情况下

## Linkin优化的目标
1. 病毒动作（Viral Actions），喜欢、评论、分享，因为这些动作会触发其他用户的更多动作，所以叫做病毒动作。包含这些动作的session占比，作为一个优化目标
2. Engaged Feed Sessions(EFS)，除了喜欢、评论、分享，还加入了点击和滑动超过10个feed，计算这些session占比
3. job apply(JA)。申请了工作的session占比

- Feed-Mixer：分层rank
    1. 对每种类型的feed检索TOP-ranked 的items
        - 工作推荐
        - 新闻推荐
        - 相关文章
        - 好友分享
    2. 多多种类型进行混排
- 最终的得分
$$
score = P_{VA}(m,u) + x_{EFS}P_{EFS}(m,u) + x_{JA}P_{JA}(m ,u)
$$
- 将VA作为优化目标，其他两个作为约束条件
$$
max VA(x)
s.t. EFS(x) \ge c_{EFS}, JA(x) \ge c_{JA}
$$
- $(c_{EFS}, c_{JA})$ 系数是产品给定的值
- 本文的方法可以极大节省人工调参的成本

- 将约束优化问题转化为无约束优化问题
$$
max U_1(x) + \exp(B) \sum_{i=2}^n \sigma_{\epsilon}(U_i(x) - c_i)
$$

B 是U的上界，Ui就是第i个目标，ci是第i个约束常数，x是超参数向量。

- 在linkin场景下，$(x=(x_{EFS}, x_{JA}))$
- **效应函数通过数据来估计，而不是直接观测**
- 设在超参数x下，用户i在session j中，发生动作k=VA的随机变量为$(Y_{i,j}^k(x)) \sim Bernoulli(\sigma(f_k(x)))$，fk建模成高斯过程
- 假设不同session 是互相独立的，那么对用户i有
$$
Y_{i}^k(x) \sim Binomial(n_i(x), \sigma(f_k(x)))
$$
n是session数目，我们可观测量是Y和n
- 因为效应函数小于1，所以拉格朗日乘子取大于1的数即可保证约束条件成立

- 建模超参数到最终的业务指标的映射
- 通过贝叶斯方法，实时（隔一段时间）更新模型参数
- 在当前参数下，采样一组超参数去收集数据


- 最简单的调参方法可以这样
    - 首先，上述多目标优化问题通过转化变成无约束优化问题，待优化的参数就是超参数，目标函数是一个有最终业务指标合成的目标函数。注意，建模的目标跟业务目标是不同的
    - 在一段时间中，将超参数可能取值分成若干组（离散值），在线上跑一段时间
    - 收集线上每组参数的业务效果，就可以得到多组数据(xi, fi)；理论上，如果我们分得足够细，而且参数空间遍历地足够充分，收集的数据足够多，以致于fi的方差很小。那么我们直接取fi最大的xi作为最优参数，问题就解决了
    - 实际上，fi的方差不能忽略，所以需要继续探索搜集数据，知道f(x)的不确定性基本没有，就可以得到最优参数了
    - 进一步，在探索过程中，可以利用f的连续性，将一些不太可能的区域提前减枝 
## 连续臂汤普森采样




# FAQ

## 当多个业务目标存在冲突，调参是如何处理的

## 无约束优化问题跟原问题的解是相同的吗？