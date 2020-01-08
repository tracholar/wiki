---
title: "RecSys2019 比赛获奖论文"
layout: page
date: 2020-01-03
---


## 第一名 方案
- Boosting algorithms for a session-based, context-aware recommender system in an online travel domain
- 数据集
    - 16M 用户点击事件
    - 数据
        - user
        - session
        - timestamp
        - action type
        - location
        - device
        - filter preference
    - 候选集最多25个
    - 目标：按照点击率排序
- 评估指标： MRR <https://en.wikipedia.org/wiki/Mean_reciprocal_rank>
$$
MRR = \sum_{r_i} \frac{1}{r_i}
$$

- 作者选用了LightGBM，没有优化MRR的目标函数，作者选择了NDCG来做优化目标
- 问题：如何直接优化MRR？lambdaMart不行？
- 
