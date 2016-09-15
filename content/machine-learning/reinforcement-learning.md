---
title: "强化学习"
layout: page
date: 2016-09-04
---
[TOC]

## 关于
强化学习是未来很重要的方向！参考 Alpha Go！

## Markov Decision Process
- A set of states $(s \in S)$
- A set of actions $(a \in A)$
- A transition function $(T(s, a, s'))$
    - 转移概率 $(P(s'| s, a))$
    - 也叫 model 或者 dynamics
- A reward function $(R(s, a, s'))$
    - 通常只是状态的函数$(R(s), R(s'))$
- 一个初始状态
- 一个终止状态

MDP 不是一个确定性的搜索问题，一种方法是 **期望值最大搜索** ？！

马尔科夫性说的是未来的决策与过去无关，至于当前状态和策略有关！

（最优）策略：$(\pi^* : S \rightarrow A)$

状态转移函数 $(T(s, a, s') = p(s'| s, a))$

discounting reward: $(r_0 + \gamma r_1 + \gamma^2 r_2 + ...)$

当 $(\gamma < 1)$时，discounting reward 有界 $(R_{max}/(1-\gamma))$！

或者采用 finite horizon，当迭代到 T 步时，停止！

- MDP quantities:
    - Policy = 对每一个状态选择一个action
    - Utility = sum of (discounted) rewards

- 状态 value function： $(V^* (s))$
- q-state (s, a), $(Q^* (s,a))$
- 最优策略 $(\i^* (s))$

- 递归定义：
    - $(V^* (s) = \max_a Q^* (s, a))$
    - $(Q^* (s, a) = \sum_{s'} (s, a, s') \left[ R(s, a, s') + \gamma V^* (s') \right])$
    - $(V^* (s) = \max_a \sum_{s'} (s, a, s') \left[ R(s, a, s') + \gamma V^* (s') \right])$

- Time-limited value: 定义$(V_k(s))$ 为状态s下，最多k步下的最优value
- Value iteration:
    1. $(V_0(s) = 0)$
    2. $(V_{k+1}(s) \leftarrow \max_a \sum_{s'} (s, a, s') \left[ R(s, a, s') + \gamma V_k^* (s') \right]) )$
    3. 重复1-2直到收敛！
    4. 复杂度，每次迭代 $(O(S^2A))$
    5. 收敛到唯一最优值！

## Reinforcement Learning
仍然是一个MDP过程，仍然寻找最优决策！
但是不知道状态转移函数T 和 回报函数 R！

- MDP：Offline
- RL：Online

### model-based Learning
通过经验，学习一个近似模型，然后求解！

- Step1：学习经验MDP模型：
    1. 对每一个个(s, a)统计s'
    2. 归一化得到$(\hat{T}(s, a, s'))$
    3. 


## 深度强化学习tutorial@ICML2016
AI = RL + DL

- 值函数
- 策略
- 状态转移模型

使用深度学习建模值函数，策略和模型！

### Value-Based Deep RL
Q-Networks

$$
Q(s, a, w) \approx Q^* (s, a)
$$

Q-Learning：

最优 Q-value 应该遵循 Bellman 方程

$$
Q^* (s, a) = \mathbb{E}\_{s'} \left[ r + \gamma \max_{a'} Q(s', a')^* |s, a  \right]
$$

其中 s 表示状态，a 表示agent对环境做出的 action！
将方程右边当做目标，用神经网络学习！
损失函数：

$$
l = (r + \gamma \max_{a'} Q(s', a', w) - Q(s, a, w) )^2
$$

问题：1. 训练样本不是 iid； 2. 目标不稳定！

DQN：利用 agent 自身经验构建样本！

$$
l = (r + \gamma \max_{a'} Q(s', a', w^-) - Q(s, a, w) )^2
$$

$(w^-)$是固定的！


Double DQN

- 当前的 Q-network w 用来选择 action
- 老的 Q-network w- 用来评估 action

$$
l = \left( r + \gamma Q( s', \arg\max_{a'} Q(s', a', w), w^- ) - Q(s, a, w)  \right)^2
$$

Duelling network

### Deep Policy Networks
用神经网络建模策略函数

$$
a = \pi(a| s, \mathbf{u}) = \pi( s, \mathbf{u})
$$

目标函数为total discounted reward

$$
L(u) = \mathbf{E}(r_1 + \gamma r_2 + \gamma^2 r_3 + ... | \pi(., u))
$$



## 相关资料
- 强化学习书籍：<https://webdocs.cs.ualberta.ca/~sutton/book/ebook/the-book.html>
- Tutorial: Deep Reinforcement Learning， ICML 2016. David Silver, Google Deepmind.
- Berkeley 课程：Deep RL. <http://rll.berkeley.edu/deeprlcourse/>
- <http://ai.berkeley.edu/course_schedule.html>
