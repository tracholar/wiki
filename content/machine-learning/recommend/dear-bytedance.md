---
title: "基于深度强化学习的广告推荐模型 DEAR"
layout: page
date: 2020-04-24
---
[TOC]


# 关于
- 论文：Deep Reinforcement Learning for Online Advertising in Recommender Systems
- 创新点
    1. 广告收益最大化，同时考虑用户体验
    2. DQN改进模型DEAR，自适应调整策略：推荐列表中是否需要插入广告、插入什么广告、在什么位置插入广告
- action：推荐列表中是否需要插入广告、插入什么广告、在什么位置插入广告；
    - 不插入广告作为一个特殊的动作
    - 错误的广告将导致用户流失，忠实用户可以高概率插广告
- reward：收益，用户体验
    - 收益部分：广告收入
    - 用户体验部分：继续看+1，离开-1
    
- REF：<https://mp.weixin.qq.com/s/lAF4epLjuMgbxZc96PCcZw>