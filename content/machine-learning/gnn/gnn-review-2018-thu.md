---
title: "GNN综述：Graph Neural Networks: A Review of Methods and Applications"
layout: page
date: 2019-11-04
---
[TOC]

## 摘要
- GCN，卷积
- GAT，attention
- GGNN，gated

## 导言
- 图的应用
    - 社会科学
        - Inductive representation learning on large graphs， NIPS2017
        - Semi-supervised classification with graph convolutional networks， ICLR 2017
    - 自然科学
        - Graph networks as learnable physics engines for inference and control，arXiv 2018
        - Interaction networks for learning about objects, relations and physics，NIPS 2016
        - Protein interface prediction using graph convolutional networks，NIPS 2017
    - 知识图谱：Knowledge transfer for out-of-knowledge-base entities : A graph neural network approach，ICJAI 2017
    - 组合优化：Learning combinatorial optimization algorithms over graphs
- 图节点分类，链接预测，聚类
- CNN的三个要点：局部连接、权值共享、多层堆叠；但是只能应用到欧式空间的数据：图像（二维），文本（一维序列）

![图片和非欧数据](/wiki/static/images/non-euclidean.png)

- Graph embedding: node2vec, LINE, TADW
    - 参数跟节点数线性增长，参数没有共享？？？
    - 不能处理动态图，泛化到新的图
- GNN：能建模节点依赖；能够建模散射过程
    - GNN的输出跟节点的输入顺序无关
    - GNN将图的结构信息作为传播关系，而不是直接作为特征
    
## 模型
- F. Scarselli, M. Gori, A. C. Tsoi, M. Hagenbuchner, and G. Monfar- dini, “The graph neural network model,” IEEE TNN 2009, vol. 20, no. 1, pp. 61–80, 2009
- GNN的目标是学习一个隐向量，包含了邻居节点的信息
- 输出的结果依赖这个隐向量和当前节点的信息
$$
H = F(H, X) \\\\
O = G(H, X_N)
$$
H 是所有节点的状态，X是所有的特征，X_N是所有节点的特征，O是所有的输出。F是global transition function，G是global output function。H是F的不动点，可以通过迭代求解。
- 传统GNN的不足
    - 迭代求解不动点是低效的
    - 不同层的参数是相同的
    - 存在一些边相关的特征无法建模

### GNN的变体
