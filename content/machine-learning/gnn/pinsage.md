---
title: "Graph Convolutional Neural Networks for Web-Scale Recommender Systems"
layout: page
date: 2020-02-10

---

[TOC]

# 简介
- 本文是GraphSAGE的落地版本

# FAQ
## 模型的主要思路是什么
- 跟GraphSAGE一样，其中聚合函数取的是Pooling聚合函数。
- 邻居的定义通过随机游走，到达的归一化次数最高的T个顶点
- 损失函数用的是max-margin 损失函数


