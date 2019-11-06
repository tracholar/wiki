---
title: "Rerank机制调研"
layout: page
date: 2019-11-05
---
[TOC]

# 问题
- 现有的推荐模型基本上都是预测一个得分f(u, i)，是一个pointwise模型。而实际上排序时，对i的打分还依赖前后的item，所以应该是listwise模型。rerank就是为了解决这个问题



# 传统方法
- 在排序中，可以利用lambdaMart，优化list的损失函数来实现

# 微软2009
- Diversifying Search Results
- 基本思路是在排序得到一个文档集合之后，先初始化一个空集，然后贪心地将边际收益最大的文档加入集合中
- 边际收益的定义。假设有很多个类别，query和document都有一定概率属于某个类别。用$( V(d| q, c) )$ 表示在query为q，类别c的条件下，出文档d的价值（比如相关性）；用$( U(c | q, S) )$ 表示在集合S都不属于c类别条件下，q属于类别c的概率。那么在列表S中加入文档d带来的边际收益（额外增加的价值）可以表示为：现有的S都不是c类别但query的意图是c类别，此时文档d带来的收益为V(d|q,c)。对所有类别求期望有
$$
g(d|q, c,S) = \sum_{c \in C(d)} U(c|q,S) V(d|q,c)
$$
C(d)是文档d的所属类别集合（文档可以属于多个类别）。
- 当在集合S中增加文档d后，条件概率$(U(c | q, S \cup {d}))$的更新表达式为
$$
U(c|q, S \cup {d}) = (1 - V(d|q,c))U(c | q, S)
$$

# 多臂老虎机-ICML2008
- Learning diverse rankings with multi-armed bandits
- 

# MMR
- The use of mmr, diversity-based reranking for reordering documents and producing summaries
- 最大化边际相关性
- 对于给定的文档集合S，往里面加一个新的文档d的边际相关性为
$$
\lambda Sim_1(d, q) - (1-\lambda) \max_{d_i \in S} Sim_2(d, d_i)
$$
即为文档d与q的相关性减掉d与S中所有文档中最大相关性！

# xQuAD
- 预定义效用函数，对相关性和多样性加权，多样性的衡量是改写的qi和现有集合S中的文档都不相关但与文档d相关的概率。
$$
(1-\lambda P(d|q)) + \lambda \sum_{q_i \in Q}P(q_i|q)P(d|q_i) \Pi_{d_j \in S} (1-P(d_j|q_i))
$$

# SVMDiv


# 多样性
- Learning for search result diversification

# MDP-2017 
- Adapting markov decision process for search result diversification

# 优化整个页面的rank
- Beyond ranking: Optimizing whole-page presentation

# 阿里：Globally Rank
- Globally Optimized Mutual Influence Aware Ranking in E-Commerce Search
- 将问题建模为估计$(p(i|c(o, i)))$ c是上下文中的item，上下文是指排序展示结果中有的item
- 关键是构造global特征，global特征构造方法是算一个全局分位点值
$$
f' = \frac{f - f_{min}}{f_{max} - f_{min}}
$$
- 最后将rerank建模成一个序列生成模型，用RNN来做decoder，用beam search来做搜索
- lambdaMART用了listwise的损失函数，但是模型没有使用展示的list中的信息，这里的一个创新点应该是还利用到了list中的信息来做特征，建模这个得分。




# 阿里：个性化rerank
- Personalized Re-ranking for Recommendation
![PRM](/wiki/static/images/prm.png)

- recsys2019年，阿里发表的文章
- 将问题建模为对排序后的每一个item计算一个得分 $(P(y_i | X, PV; \theta))$，其中X是该list所有item的特征矩阵，PV是一个正常pointwise排序模型（比如W&D）最后一个隐层输出向量，作为用户和这个item的个性向量
- 本质上是建模一个序列(排序后的item列表)到另一个序列(每一个item的打分序列)的问题
- 将item特征向量和pv向量拼接，在加上position Encoder向量，通过一个transformer来建模这个序列到序列的变换模型；最后一层是一个softmax，监督信号是点击之类的信号


# DPP：行列式点过程-YouTube
- Practical Diversified Recommendations on YouTube with Determinantal Point Processes
- 两个item是相似的，如果他们放在一起会导致效用下降
$$
P(y_i=1, y_j=1) < P(y_i=1)P(y_j=1)
$$
如果feed流中有两个item是相似的，那么排序策略不是最优策略了！

