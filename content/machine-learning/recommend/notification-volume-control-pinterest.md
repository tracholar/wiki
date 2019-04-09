---
title: "Notification Volume Control and Optimization System at Pinterest"
layout: page
date: 2019-04-05
---

[TOC]

## 论文摘要
- 通知系统的几个关键问题:
    - 什么时候投放
    - 通过哪个通道投放
    - 投放的频率和投放量
- 挑战
    - 目标函数应该包含长期效应,不仅仅是短期效应
    - 模型对容量增长的非线性效应
    - 算法必须对大规模可扩展
- 系统
    - Weekly Notification Budget
    - Notification Service
        - 每天轮询每一个用户,决定是否给他投放,生成投放内容
        - Budget Pacer, 一个KV系统,userid做key,value是用户预算额度
            - even pacing, 最小化用户疲劳, 比如一周预算为3, 那么就隔一天投放一次。本质上是均匀分配预算
            - 更智能的pace: 周末投放更多一些?
        - RANKER, 对于可以投放的用户, 从一系列的通知中选择最好的一个投放。ML模型在这个环节预测CTR等业务关键指标
        - delivery, 投放时间策略决定什么时候投放, 投放后跟踪用户响应反馈
    - Volume Optimization Workflows
        - 之前的工作: 如果一个用户有很高的概率会和某种类型的通知交互,那么就给他头更高频地投放这种类型的通知
            - 如果CTR排名前10%,可以每3天发一次这种类型的push;如果20%,则延长到14天发一次; 间隔区间通过手动AB测试选择
        - 问题: 不能从指标上区分是通知质量变好带来的,还是通知的数量变多带来的?

- 建模概率 $(p(a|u,k_u))$, a代表这一天是否活跃, u代表某个用户, $(k_u)$代表用户的配额, 优化
$$
\max_{k_u} p(a|u, k_u) \\
s.t. \sum_{u} k_u \le K
$$
- 效用建模, $(p(a|u) = p(a_o|u) + (1-p(a_o|u))p(a_n|u))$, 第二项表示用户没有通过其他途径打开APP,而是通过通知打开APP的的概率,可以近似认为是通知的效用(因为这个公式假定了不同通道之间是独立的,然而并不是这样)
- 建模长期效用
    - 将用户是否退订考虑进去,将活跃的概率分解为两部分,即退订后未来长时间的活跃概率,加上没有退订当前的活跃概率;这里将退订和不退订的活跃概率分别用不同时期的活跃概率是为了即兼顾长期利益,又能考虑短期收益,是个混血儿
$$
p(a|u, k_u) = p(s_{unsub}|u, k_u) p(a_L|u, k_u, s_{unsub}) + (1-p(s_{unsub}|u, k_u)) p(a|u, k_u, s_{sub})
$$
- 三个模型都是个二分类问题
- 无偏数据收集, 随机对一组用户设定投放次数,收集反馈作为数据
- 前两个模型的样本构建都没啥问题,注意构建退订模型的时候,k要选择分配的通知预算,而不是实际收到的数目,a代表活跃,而不是点击
- 第3个模型是长期活跃,作者选了第4周之后的一周是否活跃构造label
- 

## 相关文献
- Rupesh Gupta, Guanfeng Liang, and Rómer Rosales. 2017. Optimizing Email Volume For Sitewide Engagement. In Proceedings of the 2017 ACM on Conference on Information and Knowledge Management, CIKM 2017, Singapore, November 06 - 10, 2017. 1947–1955
- Rupesh Gupta, Guanfeng Liang, Hsiao-Ping Tseng, Ravi Kiran Holur Vijay, Xi- aoyu Chen, and Rómer Rosales. 2016. Email Volume Optimization at LinkedIn. In Proceedings of the 22nd ACM SIGKDD International Conference on Knowledge Discovery and Data Mining, San Francisco, CA, USA, August 13-17, 2016. 97–106
    