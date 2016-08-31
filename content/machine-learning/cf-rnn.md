---
title: "Collaborative Filtering with Recurrent Neural Networks"
layout: page
date: 2016-07-21
---
[TOC]

## 论文导读
Collaborative Filtering with Recurrent Neural Networks， IRIDIA，2016


### 协同过滤作为一个序列预测问题
传统方法，用t时刻之前，用户消费的item集合预测t时刻之后用户消费的集合。
没有考虑顺序！

新方法：用t时刻之前的消费序列$(x_1, x_2, x_3)$，预测t之后消费$(x_4,x_5)$。
考虑顺序！预测结果也考虑顺序！

- 短期预测：预测下一个消费的item
- 长期预测：预测最终消费的item

序列预测方法适合短期预测！短期预测可以提高推荐的多样性！
而不管顺序的静态方法适合长期预测！

作者任务隐藏在序列里面的信息很重要！静态方法没有充分考虑。
可以找到用户兴趣变化，帮助辨别哪些item与当前用户兴趣是无关的，或者哪些商品导致用户兴趣的消失。
也能了解哪些item对用户变化的兴趣更有影响力？！
