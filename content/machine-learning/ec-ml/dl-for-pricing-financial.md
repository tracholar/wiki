---
title: "A Deep Learning Framework for Pricing Financial Instruments"
layout: page
date: 2019-09-13
---

[TOC]

## 摘要
- 机器学习是一种非常重要的资产管理工具
- 已有方法的局限性
    - alpha的来源没有被确定。NLP任务通过阅读新闻来预测涨跌,实际上是一个动量策略,但是动量策略通常是低alpha策略
    - 异构数据源没有被利用上。新闻,价量指标
    - 交叉组合效应没有考虑。Facebook和google股价并不独立
- 作者的方法
    - 多个数据源
        1. 技术指标,即股票的价量指标
        2. 财经新闻,突发事件
        3. 交叉组合信号。刻画相似股票的共同变动
    - 方法
        1. 低过拟合
        2. 不同类别信号交叉
        3. 低计算成本。交叉通过因子模型

## 相关工作
- 将机器作为一个黑盒预测模型,输入专家构造的因子
- 将股票价格作为时间序列预测问题。用DNN自动抽取特征
- 新闻: 从新闻中抽取特征或者端到端利用DNN来预测股票价格
- 因子模型: 交叉组合的回报, 两个或多个股票价格的变动可以分解为几个因子


## 我们的模型
- 问题设置, n个股票, 股票i在第t天的对数回报
$$
r_i^t = \log\left( \frac{p_i^t}{p_i^{t-1}} \right) 
$$

$(p_i^t)$ 代表第t天第i只股票的开盘价。
$(f_i^t)$ 代表第t天第i只股票的因子向量(特征向量)
$(c_i^t)$ 代表第t天第i只股票相关的新闻语料

将问题建模为一个回归问题,拟合目标是$(r_i^t)$, 输入的特征是第t-1天到t-T天的特征向量$(f_i^j)$ 和 新闻 $(c_i^j)$

- 包含的模块
    - stock2vec
    - 技术因子表示模块
    - 新闻embedding模块
    - 序列建模

### stock2vec
- stock embedding
    1. 利用Glove构建stock vector,共现关系从新闻中统计得到。如果两只股票在同一个新闻中被提到,那么这两只股票就算共现一次
    2. 最近邻图,构建有向图G={V, E},只有k近邻才有边
- Stock Attention Mechanism
    - 最终的某只股票的向量$(c_i)$是k近邻的加权和,权重
    $$
    \alpha_{ij} = \frac{exp(f(e_i,e_j))}{\sum_{k\in S(i)}   exp(f(e_i,e_k)) }
    $$
    f是一个单隐层神经网络
    - Q:自己节点的权重是多少?貌似是统一用MLP决定的
- 技术指标embedding: 直接拿个MLP将300多个技术指标映射到m维 $(g_i^t)$
- 新闻embedding
    - 直接平均词向量作为新闻的embedding向量 $(o_i^t)$
- 序列模型
    - 序列的状态向量构造 $(h_t = [c_i, g_i^t, o_i^t])$
    - 序列建模,双向RNN: BiLSTM
    - 时域attention: 跟RNN的attention机制没啥区别,最终得到的是隐状态向量的加权和$(v_{t+1})$作为输出
- 最后将$(v_{t+1})$ 作为MLP的输入,输出score通过softmax归一化???? lossfunction是均方误差

    
    
    

