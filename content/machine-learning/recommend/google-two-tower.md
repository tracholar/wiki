---
title: "Sampling-Bias-Corrected Neural Modeling for Large Corpus Item Recommendations"
layout: page
date: 2020-04-22
---
[TOC]


# 关于
Google Youtube 的双塔召回实战经验


# 简介
- 召回的一些难点
    1. 数据规模大
    2. 表示学习对长尾item方差大
    3. 数据分布会随时间变化
- 优化点
    1. batchsoftmax
    2. 流失训练
- 固定词表：通过hash，hash冲突怎么办？
- 估计流式训练过程中item的频率
