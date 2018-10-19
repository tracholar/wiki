---
title: "强化学习2018年论文阅读笔记"
layout: page
date: 2018-07-22
---

[TOC]

## Robust DQN
- Stabilizing Reinforcement Learning in Dynamic Environment with Application to Online Recommendation, Shi-Yong Chen, Yang Yu etc
- 南京大学跟阿里巴巴合作的论文, KDD18
- 传统RL方法在静态环境中比较适合,本文方法主要解决在动态环境(环境参数分布会随时间变化)中,实现稳定的回报(reward)估计: Robust DQN(Double DQN)。
- 相比于Atari游戏、alpha go,实际的推荐问题中最大的不同是高度动态,环境参数和分布随时间的变化,使得对reward的估计方差很大,并且有偏: 因为reward的增大可能是策略提升带来的,也可能是时间变化带来的。
- 两个关键策略改进
    1. 解决方差较大:用分层采样回放(stratified sampling replay) 代替传统的经验回放。使用一个先验的用户分布,根据这个先验分布采样每一个batch
    2. 解决时变偏差:实现近似的 regret reward
- 分层采样回放
    - 通过长时间的数据统计,选出一些用户分布稳定的属性:性别、年龄、地理信息,通过这些属性对用户分层
    - 在回放的时候,用固定比例的分层采样代替对短期用户的均匀采样
- Approximate Regretted Reward
    - 利用一个离线模型的线上回报估计环境随时间的动态变动, 然后利用估计出来的环境变动补偿线上的DQN
    - 用 $(\tile(r)_t = r_t - r_b)$ 代替 $(r_t)$
    - r_b 估计方法: 随机选择一些用户,利用离线训练好的模型进行决策,然后实时统计这些用户的平均reward作为 r_b
- 应用案例: Tip 推荐
    - Q(S, a) 将动作(20000种Tip,对应动作空间20000)embedding后,作为神经网络的输入
    - $(r = r_1 + \alpha r_2 + \beta r_3)$ 
        - 第一部分回报是点击回报$(r_1 = I * (1 + \rho * e^{-x})$ I代表是否点击,x是当前tip展示的页面数量
        - 第二部分回报是描述用户点击tip的偏好, $(r_2 = I * e^{-y})$ y是用户最近100次PV中对tip的点击次数
        - 第三部分回报是用户交易 $(r_3 = c)$ c代表是否交易
    
