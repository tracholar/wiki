---
title: "Asynchronous Methods for Deep Reinforcement Learning"
layout: page
date: 2018-07-17
---
[TOC]

## 简介
强化学习经典论文,异步实现方法。Deepmind & Google。

## 要点
- 之前的 Deep RL 算法需要经验回放(experience replay)解决样本时间相关性的问题, 例如DQN;
  经验回放有两个问题: 1是需要大内存存放经验(通常非常大), 2是用经验回放,那么线上策略与学习策略不一致,也就是要用off-policy学习方式。
- 异步并行的学习方式可以很好的解决样本的时间相关性问题,因为同时运行很多个episode,每一个都是独立的,不存在时间相关性。
- 使用了单机多卡(GPU)的并行方案
- asynchronous advantage actor-critic (A3C)效果最优
- The General Reinforcement Learning Architecture (Gorila)
    - 多机分布式异步训练
    - 每个actor独立训练和使用经验回放
    - 每一个learner从所有的经验中采样,计算DQN loss
    - 梯度异步与PS交互,更新
    - 模型参数定期同步到每一个 actor-learner
    - 100 actor-learner, 30 parameter server, 130台机器
- 使用n步TD的方法更新参数