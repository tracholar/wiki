---
title: "Graph Embedding"
layout: page
date: 2019-04-28
---
[TOC]

## 关键文献
- DeepWalk 2014
    - 关键想法: 通过random walk从Graph中采样语料(类似于NLP中的句子),然后利用word2vec学习node向量
- LINE 2015
    - 关键想法: 从embedding向量建模1阶相似和2阶相似,然后优化得到向量, 没有采样语料, 借鉴了word2vec思想的精髓, 而是不是将问题转化为word2vec能解决的问题
- node2vec 2016
    - 关键想法: 改进deepwalk的采样方法,既考虑深度有考虑宽度
- metapath2vec 2017
    - 关键想法: 处理异质网络, 有多种类型的节点, random walk考虑节点和关系的类型,只能沿着指定的节点-关系路线(即meta-path)进行采样
- Billion-scale Commodity Embedding for E-commerce Recommendation in Alibaba, 阿里2018
    - 关键想法: 将side-information引入embedding改进冷启动的问题,模型还是浅层,是否可以用深层(如DNN,原则上是可以的)
- NetSMF WWW2019, 微软、清华
    - 
