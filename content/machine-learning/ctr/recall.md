---
title: "召回方法的调研"
layout: page
date: 2019-03-27
---
[TOC]

## 关于
本文收集了在搜索、广告、推荐系统中的一些召回策略和算法


## 协同过滤
- 主要是基于行为的协同过滤
- userCF
- itemCF
- 矩阵分解 SVD/SVD++/LFM
- 基于行为的方法的问题在于冷启动, 因为它只用到行为, 而新的item是没有行为的

## 基于内容的召回
- 计算用户的画像与内容的画像之间的距离(相似度)

## 基于用户群
- 对用户分群
    1. 获得user embedding向量, 方法可以是主题模型中的主题向量, 用户画像等数据用SVD,autoencoder降维后的向量等
    2. 利用user向量做聚类: k-means、层级聚类、GMM
- 对用户群中的用户推荐在这个用户群上的点击率高的item


## 倒排链
- 召回实现的时候是从标签反查item, 所以需要建立倒排索引
- 在拿到用户标签后, 通过标签反查item, 倒排链放到分布式索引数据库中加快检索速度

```javascript
{ 
    'tag_1': 
        [ { itemID: '13', weight: 0.7 }, { itemID: '2', weight: 0.53 } ],
    'tag_2': 
        [ { itemID: '1', weight: 0.37 } ],
    ...
}
```

## 向量召回
- hash
- KNN 
- faiss 

## 参考资料
1. <https://cloud.tencent.com/developer/article/1174893>
2. J. Weston, A. Makadia, and H. Yee. Label partitioning for sublinear ranking. In S. Dasgupta and D. Mcallester, editors, Proceedings of the 30th International Conference on Machine Learning (ICML-13), volume 28, pages 181–189. JMLR
3. T. Liu, A. W. Moore, A. Gray, and K. Yang. An investigation of practical approximate nearest neighbor algorithms. pages 825–832. MIT Press, 2004