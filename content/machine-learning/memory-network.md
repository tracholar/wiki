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
[1] Memory Network, Jason Weston, Sumit Chopra & Antoine Bordes, Facebook AI Research 2015.
[2] End-To-End Memory Networks, Sainbayar Sukhbaatar, Arthur Szlam, Jason Weston, Rob Fergus 2015.

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
s(x, y) = \phi_x(x)^T U^T T \phi_y(y)
$$

U 是要学习的参数，$(\phi)$是对输入的 embedding 函数。

### 训练
学习最优的参数 $(U_O, U_R)$ 最小化损失函数，损失函数包含2部分(k=2)，

1. 选择最匹配的记忆单元带来的风险
2. 选择最佳response带来的风险，如果用RNN就直接用RNN的损失函数就行

上述风险都用大间隔损失函数来刻画！
