---
title: "【2018-Airbnb】Customized Regression Model for Airbnb Dynamic Pricing"
layout: page
date: 2018-12-23
---
[TOC]

## 摘要与间接
- 3个模型组成
    - 每个listing订购概率, 二分类
    - 自定义损失函数的回归模型,拟合最优价格
    - 额外的个性化逻辑
- 产品:
    - Price Tips 价格建议
    - Smart Pricing 智能价格

![smart price](/wiki/static/images/smart-pricing.png)

## 动态价格模型

### 需求估计
- 如果能够估计出需求曲线(需求函数) $(F(P) )$ P是价格,那么,最大化 $(P F(P))$ 即可得到最优价格P。
- 在 Airbnb场景下,需求还跟其他因素有关,所以作者估计的是这样一个需求函数 $( F(P, t, id) )$ t是时间,id是listing id。
- 时变因素:
    - 季节因素
    - 订购时间与入住时间的距离,距离越短,订购成功的概率越低
- Listing-varying, 也就是listing 是唯一的,可替代性比酒店弱
- 预估的价格只有部分被采用


- 需求估计相关论文:
    1. PatrickBajari,DenisNekipelov,StephenP.Ryan,andMiaoyuYang.2015.Demand estimation with machine learning and model combination. National Bureau of Economic Research (2015).
    2. H. Varian. 2014. Big data: New tricks for econometrics. Journal of Economic Perspectives 28, 2 (2014), 3 – 28.

### 价格策略模型
- 模型: GBM
- 目标函数: 自定义函数, 借鉴 SVR, $(\epsilon)$ 提供一个范围区间, 认为最优价格是在这个区间里面,因为实际上观察不到最优价格


## 定价系统

Booking Probability Model -> Strategy Model -> Personalization

### Booking Probability Model
- 预估函数 Pro(id, t, ctx, P) 
- 特征
    - Listing Features: listing price per night, room type, person capacity, the number of bedrooms/bathrooms, amenities, locations, reviews, historical occupancy rate, instant booking enabled, etc.
    - Temporal Features: seasonality (the day of the year, day of the week, etc), the calendar availability (e.g. the gap between check in and check out), distance between ds and ds_night, etc.
    - Supply and demand dynamics: number of available listings in the neighborhood, listing views, searches/contacts rates, etc.
- 相比所有市场用一个GBM,每个市场单独使用一个,并采用不同的采样率会更好
- 预估需求曲线的问题
    - 数据稀疏: 很多listing价格不怎么变, 价格变化的listing变动也很少,导致探索困难。 也就是数据分布不到所有空间。 这个也是我们在做的时候遇到的问题
    - 样本的唯一性: 不存在同时处于两种价格的listing, 不同的listing又难以互相替代
    - 特征依赖: 我们希望价格P和其他特征是独立的,然而很多其他特征是依赖价格的, 比如订购率。 这导致需求曲线预估的精度很差
    
### 评估建议价格
- 无法观测到最优价格
- 但是可以观察到订购的样本的下界(即最优价格不低于订购价格)和未订购样本的上界(即最优价格不高于未订购价格)
- 订购样本的上界可以估计为订购价格 P 的c1倍,c1>1是超参数
- 未订购样本的下界可以估计为未订购价格 P 的 c2 倍, c2<1是超参数
- 损失函数为区间损失,即在这个区间,损失为0,否则损失为距离这个区间的距离

$$
L = \min \sum_i (L(P_i, y_i) - f_{\theta}(x))^+ + (f_{\theta}(x) - U(P_i, y_i))^+
$$

L 和 U 分别代表下界和上界

### Strategy Model

## 参考
1. <https://www.kdd.org/kdd2018/accepted-papers/view/customized-regression-model-for-airbnb-dynamic-pricing>
2. <https://zhuanlan.zhihu.com/p/46288070>
