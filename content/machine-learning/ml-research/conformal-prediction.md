---
title: "Conformal Prediction"
layout: page
date: 2019-05-08
---

[TOC]

## 参考
- A Tutorial on Conformal Prediction

## 摘要
- 解决的问题:用机器学习模型预测的时候,预测结果为$(\hat{y})$,它离真实值$(y)$的距离是多少?
- 预测一个区域 $(\Gamma)$ 而不是一个点 $(\hat{y})$, 这个区域以95%的概率包含了真实值$(y)$
- $(\Gamma^{\epsilon})$ 表示该区域以概率$(1-\epsilon)$包含了真实值
- 对于分类问题,$(\epsilon)$ 越大, 那么预测集合$(\Gamma^{\epsilon})$包含的类别数目越少,当$(\epsilon)$增大到$(\Gamma^{\epsilon})$恰好只有一个类别的时候,那么可以将$(1-\epsilon)$作为这个类别的预测置信概率
- 在线预测配置, 预测第n个样本时,必须知道前n-1个的真实值??
- 