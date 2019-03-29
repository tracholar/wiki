---
title: "FAISS"
layout: page
date: 2019-03-27 00:00
---
[TOC]

## 关于
向量最近邻索引在文本、图像、推荐、搜索等领域具有重要意义, Facebook开源的FAISS就是这样一个工具, 提供大规模的向量索引。

## FAISS 简单使用
- 安装 `conda install faiss-cpu`  其他安装方法 <https://github.com/facebookresearch/faiss/blob/master/INSTALL.md>



## 相关文献概述
### Video Google: A Text Retrieval Approach to Object Matching in Videos
- 以文本检索的速度,实现视频中目标检索匹配
- 基本方法
    - 从视频的关键帧图像中提取多个关键点的 SIFT 描述子
    - 将这些描述子做矢量量化, 每一个量化编码的码字可以看做一个 visual word
    - 基于上述 visual word 建立倒排索引, 同时可以考虑stop word, tf-idf权重等和文本检索类似的trick
- 矢量量化 <http://blog.pluskid.org/?p=57>, <https://blog.csdn.net/zouxy09/article/details/9153255>
    - 相当于标量量化的高阶推广
    - 将这些矢量进行聚类, 用聚类中心作为该类的代表

### Product quantization for nearest neighbor search
- 包含了很多ANN的参考文献, 后面可以精读一下
- 矢量量化
    - 码向量c: 指的是代替原始向量的量化后对应的那个中心向量
    - cell: 归属于同一个码的所有点组成的区域
    - 码号i: 码向量的索引
    - Lloyd最优条件
        1. 向量x的码字应该是离x最近的码向量c
            - 推论: cell的分界面是超平面
        2. 码向量应该是cell中的向量的概率平均向量
    - 求解算法: k-means
- 直接使用矢量量化的问题
    - 当码字数目k很多时计算k个中心复杂度高, 而且k个码字的存储空间
- 乘积量化的思想是,将D维向量分解成m个D/m维向量,每个子空间的维度是D/m,在这m个子空间分别进行矢量量化,那么每个子空间的码字数目只要 2^(logk/m) 个,总的码字数目为 m 2^(logk/m)
- 典型的分治法啊,将复杂度降低到 log k
- 将量化误差继续量化,但是是用相同的量化函数
- ADC

###  Searching in one billion vectors: re-rank with source coding
- 量化误差的量化函数重新拟合一个
- 索引阶段:
    1. 将向量y量化函数$(q_c)$和误差量化函数$(q_r)$从数据中学习得到 
    2. 将待索引的向量用$(q_c)$量化后得到码字
    3. 将误差用$(q_r)$量化后,关联上索引向量
- 检索阶段:
    1. 通过压缩域计算距离的方式找到query的最近k个列表L
    2. 对L中每一个向量,利用误差量化结果修正距离计算
    3. 从L中根据修正后的距离排序获取最近邻的若干个向量
    
### The Inverted Multi-Index
- 提出倒排多索引数据结构
- 每个子空间不单独做索引,而是将整个空间做索引,这将导致很多key是空的
- rank的时候不是直接对多个key的结果排序,而是利用multi-sequence算法，高效输出top-k检索结果。

### Optimized Product Quantization
- 



## 其他参考
- 很好的博客文章 <https://yongyuan.name/blog/opq-and-hnsw.html>