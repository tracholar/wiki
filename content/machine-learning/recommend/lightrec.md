---
title: "LightRec: a Memory and Search-Efficient Recommender System"
layout: page
date: 2020-04-27
---
[TOC]

## 关于
- 优化推荐系统中embedding的参数数目，减少系统内存，基于乘积量化
- 将item向量$(q_i)$表示成码字之和
$$
q_i = \sum_{b=1}^B c_{w_i^b}^b, s.t. w_i^b = \arg\max_w s(q_i, c_w^b)
$$
- 显示建模item向量与码字的关系？
- 用户向量与item向量的内积为
$$
r_{ui} = \sum_{b=1}^B <p_u, c_{w_i^b}^b>
$$

- 循环残差编码，第b个码字是从item的残差中算出来的
