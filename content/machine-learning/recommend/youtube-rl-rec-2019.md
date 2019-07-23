---
title: "Top-K Off-Policy Correction for a REINFORCE Recommender System"
layout: page
date: 2019-07-16
---
[TOC]


## 论文简介
- 推荐是根据推荐算法曝光展示的样本及反馈数据训练模型,是有偏的
- 主要贡献
    1. 将REINFORCE算法应用到百万级别的动作空间任务
    2. 利用 off-policy 纠正数据的偏差
    3. 展示探索的价值
