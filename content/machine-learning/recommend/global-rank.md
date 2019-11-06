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


# DPP：行列式点过程
## 行列式点过程
- 一个点过程P是行列式点过程是指，如果Y是一个采样自P的随机子集，那么对任意 $(S \subset Y)$，有
$$
P(S \subset Y) = \det(K_S)
$$
其中$(K)$是一个实对称半正点相似矩阵，$(K_S)$代表子集S作为下标集合表示的子矩阵。
- 由于K的任意子矩阵的行列式表示一个概率所以，K的特征值应该都在[0,1]区间，因此有$(0 \le K \le 1)$
- $(P(e_i \in Y)= K_{ii})$
- $(P(e_i, e_j \in Y)= K_{ii}K_{jj} - K_{ij}^2 = P(e_i \in Y) P(e_j \in Y) - K_{ij}^2)$，也就是两个元素同时出现的概率小于分别出现概率的乘积！！这表明这两个元素是互斥的！Kij越大，表示这两个元素同时出现的概率越小！
- 当没有交叉项，也就没有斥力项，此时K是对角阵，不同元素之间互相独立！

### 例子
- <https://arxiv.org/pdf/0904.3740.pdf>
- 长度为N的序列，每个元素是从集合I中选出，从这个序列的第二个元素开始，如果当前元素小于前面一个数，那么就把这个元素的下标选出，这些下标的分布构成一个行列式点过程。




## Google YouTube
- Practical Diversified Recommendations on YouTube with Determinantal Point Processes
- rerank的目标，最大化总交互数目
$$
G' = \sum_{u \sim user} \sum_{i \sim item} y_{ui}
$$
- 为了刻画rerank的收益，rerank的目标是把交互的用户和item对排到最前面，可以用下述累积收益来刻画
$$
G = \sum_{u \sim user} \sum_{i \sim item} \frac{y_{ui} }{j}
$$
j 是rerank后的排序！上述收益可以刻画rerank的排序效果！
- 两个item是相似的，如果他们放在一起会导致效用下降
$$
P(y_i=1, y_j=1) < P(y_i=1)P(y_j=1)
$$
如果feed流中有两个item是相似的，那么排序策略不是最优策略了！

![DPP](/wiki/static/images/dpp-serving.png)

- 有N个item，用0表示用户没有点，1表示点击，那么N个item对应的用户的行为向量为[0,..,1,..,0]，其中点击的下标服从行列式点过程！！因为下标是「当前元素小于前一个数」（假设1小于0），所以刚好是DPP中的那个例子！
- 因此用户点击的item下标服从DPP，点击下标集合Y的概率分布可以用一个矩阵的行列式来表示
$$
P(Y) = \frac{\det(L_Y)}{\sum_{Y ' \subset S} \det(L_{Y'})}
$$
S = {1,2,3,...,N}是全量下标集合。上式的分母可以简化为
$$
\sum_{Y ' \subset S} \det(L_{Y'}) = \det(L + I)
$$

- DPP核矩阵的定义，假设第i个video的Pointwise得分为$(q_i)$，sparse embedding向量为$(\phi_i)$。假设排序完有N个video，定义如下核矩阵
$$
L_{ii} = q_i^2 \\\\
L_{ij} = \alpha q_i q_j \exp(- \frac{D_{ij}}{2\sigma^2}), i \neq j
$$
$(D_{ij})$是i和j的距离，通过embedding向量计算得到。
- 当$(\alpha)$较大的时候，代表斥力很大，但是就无法保证核矩阵的半正定要求。作者通过一个投影操作，将核矩阵强行半正定化。投影的方法是，将核矩阵对角化，然后将负特征值强行置0！
