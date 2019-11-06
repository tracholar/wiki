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
