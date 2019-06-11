---
title: "知识图谱与推荐系统"
layout: page
date: 2019-04-28
---
[TOC]

## deepwalk
- DeepWalk: Online Learning of Social Representations, KDD2014
- 解决的问题: 将图中的节点转换为向量,节点之间的关系通过边来刻画
- 解决的方法: 在图中用random walk采样出一些列的序列,得到语料,然后放到word2vec里面训练
- 算法:
    1. 随机初始化每个定点对应的embedding向量
    2. 重复一下步骤gamma次
        1. O = shuffle(V)
        2. for each vi in O do
            1. 使用random walk采样一个从vi出发的顶点序列
            2. 利用skip-gram算法更新模型参数
            
## line
- LINE- Large-scale Information Network Embedding, 2015
- 解决的问题: 大规模,异构网络,有向无向,有权无权; 二阶相似性; 边采样解决权重的高方差
- 一阶近似
    1. 顶点i和j的概率 $( P(v_i, v_j) = \frac{1}{1 + exp(- u_i^T u_j)}  )
    2. 顶点i和j的经验概率 $( \hat{P}(v_i, v_j) = \frac{w_{ij}}{W} )$ wij是边的权重, W是所有边的权重和
    3. 两个概率的KL距离给出目标函数 $(-\sum_{(i,j)\in E} w_{ij} log P(v_i, v_j))$
- 二阶相似: 两个定点有相似的邻居(上下文)
    1. 每个顶点有两个向量,一个作为定点的向量,一个作为上下文的向量
    2. 上下文与定点存在边的概率用softmax表示,用负采样近似。相当于一阶近似中的sigmoid近似,同时考虑了负样本
    3. 优化目标还是用的一阶邻居,那跟一阶有啥本质区别吗??
- 边采样: 没有直接用$(w_{ij})$去乘以梯度,而是用它作为边采样概率, 为什么这样就可以降低梯度方差?梯度方差到底是啥? 梯度的方差是与所有边的梯度加权平均值的方差,如果用加权梯度,确实会增大梯度方差,如果只是做加权采样确实每次梯度都是乘以1就不会有额外的波动

## metapath2vec
- metapath2vec- Scalable Representation Learning for Heterogeneous Networks
- 解决的问题,之前的embedding方案是针对相同实体的方案,本文解决对不同实体的embedding
- 异构网络随机游走的问题:倾向于出现频次高的关系类型;解决方法,限定随机游走只能在给定的meta-path上,meta-path指的是关系路径,比如 用户-item-用户 表明从一个用户触发,下一个智能游走到(看过的,如果限定了关系)item上,不能走到其他类型的节点
- metapath2vec++ 共现概率归一化也限定到一种关系对应的类型的节点上

## Meta-Graph
- Meta-Graph Based Recommendation Fusion over Heterogeneous Information Networks
- 核心思想:
    1. 选出L条meta-path, 得到L个user-item的相似度
    2. 用矩阵分解每个相似度矩阵
    3. 将L个user和item的隐向量拼接得到特征向量,放到FM中学习
    4. FM中的权重可以划分到2L组,对应2L个隐向量(L个用户向量,L个item向量),对每个向量做group lasso,用于筛选meta-path
    

    
