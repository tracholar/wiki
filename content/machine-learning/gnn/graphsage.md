---
title: "Inductive Representation Learning on Large Graphs"
layout: page
date: 2020-02-10

---

[TOC]

# FAQ

## 本文解决的核心问题是什么
- 传统的Graph embedding 无法泛化到未见过的node（称为 transductive）
- 本文的方法可以泛化到未见过的node（称为inductive），因为node的向量是通过特征学到的
- 模型将node的特征作为输入，模型输出node 的embedding向量
- 这种方式的价值是，提高模型的泛化能力，可以在一定程度上解决冷启动问题


## embedding算法的基本步骤是什么
- 每一层是由一个聚合操作、一个带激活函数的线性变换构成
- 聚合操作是将每个顶点的邻居向量做一个聚合
- 带激活函数的线性变换是将该顶点的上一层向量与邻居聚合向量做一个非线性映射
- 很多这样的层构成了多次邻居聚合和高阶非线性

![基本算法](/wiki/static/images/graphsage-01.png)

## 为什么每一层都要对向量做归一化
- 防止梯度消失和爆炸？

## 为什么要定义一个固定大小的邻居集合，而不是用原始的邻居集合
- 因为有些节点的邻居太多了，影响性能。固定大小的邻居可以减少这个问题
- 这里采样用的是均匀采样，是否在特定应用场景下可以用非均匀采样？

## 模型的参数有哪些？
- 聚合函数的参数，每一层聚合的参数都不一样，类似于多个卷积层的参数也不一样
- 线性变换权重参数，每一层也不一样

## 第0层顶点向量是什么
- 本文是顶点对应的特征向量
- 对于直接embedding，是否也可以是随机初始化的向量？

## Weisfeiler-Lehman Isomorphism Test 是什么
- 没搞清楚，记个 TODO

## 模型的监督信号是什么，参数如何训练
- 监督信号跟random walk 一样，通过随机游走采样，利用负采样损失函数来训练。即监督信号就是节点间的某种距离。
- 如果有具体的任务，应该可以直接用最终的任务来做监督信号

## 聚合函数如何设计
- 均值聚合：直接计算向量的元素均值
- LSTM聚合：将邻居随机排列，输入LSTM来聚合。实际上RNN都可以这样做
- Pooling聚合：先通过一层神经网络，然后按元素取最大值 max-pooling。有参数可以学
- 自己想的，attention聚合。套路都一样
- 文章试验了前三种方式，效果上均值最差，后两者差不太多；计算速度上LSTM最差。总结起来 Pooling 聚合是一个不错的选择

## 代码在哪
<http://snap.stanford.edu/graphsage/>

## 试验如何评估
- 构造了一些顶点的分类任务
- citation数据集：分类一篇文章的类别；特征：顶点的度，文章摘要提取的特征和词向量构造的特征
- Reddit数据集：分类一篇文章属于哪个community；特征：(i) the average embedding of the post title, (ii) the average embedding of all the post’s comments (iii) the post’s score, and (iv) the number of comments made on the post.


## 每一个顶点的聚类系数（clustering coefficient ）是什么
- 即由该顶点已经1跳的顶点构成的子图中，任意三个联通的点构成的三角形中，是封闭三角形的比例
- 参考<https://www.cnblogs.com/startover/p/3141646.html>

## 论文中阐述算法的能力是什么意思
- 算法可以以任意精度近似每个定点的聚类系数

## 如果完全依赖与节点的特征会不会限制模型的表达能力
- 从论文的一些具体的例子来看，如citation数据集和Reddit数据集，用到的特征都是词向量以及基于词向量相关的特征，外加一些辅助特征，所以模型的表达能能力是有的
- 但是如果节点的特征很少，那么理论上应该会限制模型的表达能力。因为模型的参数较少。
- 因此，总结起来，这种方式比较适用于节点本身可抽取的特征信息较多的场景。否则的话，可能还不如直接对节点做embedding效果好。
- 对应辅助特征较少的场景，可以将直接embedding相连跟辅助特征向量concat到一起来表达？

## 这篇文章有落地吗
- 参考 Graph Convolutional Neural Networks for Web-Scale Recommender Systems

## 从这个文章中，可以提出哪些idea
- 在推荐系统中，对于用到item embedding向量的地方，不是直接用embedding向量，而是用通过特征学到的embedding向量

