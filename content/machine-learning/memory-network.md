---
title: "Memory Network"
layout: page
date: 2017-02-22 00:00
---

[TOC]

## 关于
RNN相当于给神经网络加了记忆单元，做个类比，普通的前馈神经网络（DNN，CNN）就好像是组合逻辑电路；
而增加了记忆单元的 RNN (LSTM etc)就好像是时序逻辑电路。有了记忆单元的神经网络就能够像图灵机
一样记忆、推断等高级能力，而不仅仅是像组合逻辑那样只能学一个数学函数！


## Memory Network
- [1] Memory Network, Jason Weston, Sumit Chopra & Antoine Bordes, Facebook AI Research 2015.
- [2] End-To-End Memory Networks, Sainbayar Sukhbaatar, Arthur Szlam, Jason Weston, Rob Fergus 2015.

RNN 的缺点在于记忆单元太小，所以记忆能力很弱，比如最简单的问题：输入一串单词，输出刚看到的单词，RNN都难以实现(Zaremba & Sutskever, 2014)

结构：一个记忆单元集合 $(   m = (m_1,m_2, ...)   )$，4个模块：

1. I : input feature map, 将输入转换为中间特征表达。
2. G : generalization, 根据新的输入更新存储单元的内容。
3. O : output feature map, 根据当前的输入和存储单元内容，生成输出结果的特征空间表达。
4. R : response, 将输出的特征空间表达转换为目标形式，例如一个文本，或者一个动作。

给定一个输入 x,例如一个句子，一个视频，

1. 首先将输入转换为中间表达 $( I(x) )$
2. 对新的输入x，更新内存 $(m_i = G(m_i, I(x), m), \forall i)$
3. 对新的输入x 和内存m，计算输出向量 $( o = O(I(x), m) )$
4. 最后解码输出序列 r = R(o)


## A MemoryNN Implement for Text
如果上述四个组件是用神经网络来实现，就称作 Memory NN，这些组件可以用任何机器学习的方法！

### 基本模型
输入是一个句子(I)，文本被存在下一个空的记忆单元，不做任何处理(G)，O 模块根据输入 x 找到k个有关的记忆单元，

$$
o_j = O_j(x, m) = \arg\max_{i=1,...,N} s_O([x, m_{o_1}...,m_{o_{i-1}}], m_i)
$$

最终输出给 R 的是 $( [x, m_{o_1},..,m_{o_k}]  )$，在这个例子中，R根据这些记忆单元和输入，
输出一个最匹配的单词

$$
r = \arg\max_{w \in W} s_R([x, m_{o_1},..,m_{o_k}], w)
$$

这个任务可以用来实现单个单词的问答问题。
两个匹配函数$(s_O, s_R)$都可以用下列方式建模

$$
s(x, y) = \phi_x(x)^T U^T U \phi_y(y)
$$

U 是要学习的参数，$(\phi)$是对输入的 embedding 函数。

### 训练
学习最优的参数 $(U_O, U_R)$ 最小化损失函数，损失函数包含2部分(k=2)，

1. 选择最匹配的记忆单元带来的风险
2. 选择最佳response带来的风险，如果用RNN就直接用RNN的损失函数就行

上述风险都用大间隔损失函数来刻画！

### 词序列作为输入
没有区分 statements 和 questions，因此需要学习一个分割函数，将词序列分割为两部分。

$$
seg(c) = W_{swg}^T U_S \Phi_{seg}(c)
$$

上述三个参数分别是：分割模型的线性权重参数，将特征空间映射到embedding空间的线性变换矩阵（即查找表），输入词的特征向量（高维稀疏），c是词袋词典。


### EFFICIENT MEMORY VIA HASHING
利用 hash trick 加速内存查找：将输入 I(x) hash到一个或多个桶，只将输入和同一个桶内的 memory 进行打分计算。
两种 hash 策略：

1. 直接 hash 词，因此对于一个句子，只需比较至少共享一个词的 memory
2. 对词向量做 k-means 聚类，对一个句子，比较的是和至少一个词在同一个类别的memory，这种方法考虑了词的语义。

### MODELING WRITE TIME
将 memory 写的时间也记录下来。 对内存 $(m_j)$ 将索引 j （假定没有内存更新，索引和时间一一对应）也编码进 $(\Phi_x, \Phi_y)$。具体做法，将 $(\Phi)$ 扩充三维，都设置为0，对输入 x ，当前两个内存 y, y', 计算分数

$$
s(x, y) = \phi_x(x)^T U_{O_t}^T U_{O_t} (\phi_y(y) - \phi_y(y') + \phi_t(x, y, y') )
$$

其中$(\phi_t(x, y, y'))$只有扩充的三维不为0，其他都为0，扩充的三维分别用于指示

- whether x older than y,
- whether x older than y',
- whether y older than y',

实际上将原来的 poinwise 变为 pairwise了，并加入了时间先后关系信息。选出比较完胜的y作为输出。

### 对于unseen词的处理
利用语言模型，用周围的词预测这个词可能是什么词，那么这个unseen的词就和这个预测的结果的词相似。
将词特征表达从 3|W| 增加到 5|W|，增加的两个部分分别表达这个词的左上下文和后上下文。

### 精确匹配
embedding 因为将词降维到低维连续向量，所以无法做精确匹配。
解决的办法是为 score 增加一项精确匹配score。

$$
s(x, y) = \phi_x(x)^T U^T U \phi_y(y) + \lambda \phi_x(x)^T  \phi_y(y)
$$


### 结果
<img src="/wiki/static/images/memnn.png"  style="width:600px;" />

- 可以看出 只用 embedding 特征和之前最好结果差不多，稍低，但是加上 BOW 特征，效果马上就提升了9个点。
- 不同hash策略的影响，word hash 实现了1000倍的加速但是精度减少很明显，cluster hash 在精度只减少1个点的情况下，实现了80倍的加速。

<img src="/wiki/static/images/memnnqa.png"  style="width:600px;" />

- 可以看到时间特征对提升非常显著
- 输出选取的内存个数k对效果提升也非常显著
- MemNN 完胜 RNN，LSTM

## End-To-End Memory Networks


## Neural Turing Machine
图灵机三个基本机制：元素操作（算数计算），逻辑流控制（分支），外部存储。
现代机器学习模型侧重建模复杂数据，缺乏流控制和存储。
