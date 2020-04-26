---
title: "DGL深度图神经网络实战教程 WWW2020"
layout: page
date: 2020-04-24
---
[TOC]


## 图神经网络概述
- 应用
    - 欺诈检测
    - 推荐系统
    - 社交网络营销
- 图神经网络任务
    - node classification：欺诈检测
    - link prediction：推荐，关系建模
    - Graph classification：预测化学成分的性质！制药！！
- 传统图学习方法
    - 人工特征工程
    - 无监督embedding：svd，random walk
        - 只利用了节点关系信息，没有利用节点属性信息。实际上我理解可以稍作改造就能利用属性信息
        - 不能end-to-end，我理解这个才是关键
- GNN：end-to-end embedding学习，有监督
- 通用GNN结构，message-passing
$$
m_v^(l) = \sum_{w \in neibor(v)} M^(l)(h_v^{l-1}, h_w^{l-1}, e_{v,w}) \\\\
h_v = U^(l) (h_v(l-1), m_v^(l))
$$
- GCN：聚合函数用均值/sum-pooling，变换函数在节点间不变（卷积特性）
- GAT：Attention加权聚合
- RGCN：多种关系的图，每种关系对应一种投影矩阵
- 下游任务用hv作为输入
    - 节点分类，hv做输入，交叉熵损失
    - 链接预测，负采样近似损失函数
    - 图分类，将多个节点的向量，readout成一个向量(聚合一类的操作)，然后分类损失
- 训练技巧
    - min-batch，邻接表表示图
    - 邻居采样，不算全部邻居

## DGL
- <https:www.dgl.ai>
- 核心抽象
    - DGLGraph
    - `g.ndata['h']`
- message passing APIs
    - send
    - recv
    - 可扩展的函数：
        - message function：对边和顶点做变换
        - reduce function：将邻居聚合的函数
        - update function：利用上一步隐向量和邻居聚合结果更新隐向量
    - 异构图支持
- transformer是gat，在一个完全图上
- NLP中的图：知识图谱



## REF
- <https://mp.weixin.qq.com/s/l98ZYOpDKdI9bWOE_2NN2w>
- <https://github.com/dglai/WWW20-Hands-on-Tutorial>