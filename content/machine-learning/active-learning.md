---
title: "Active Learning 主动学习"
layout: page
date: 2019-02-27
---
[TOC]

## 关于
- 主动学习算法草图
- 给定一个模型M和未标注数据U
    1. 使用当前模型M预测第c类的概率 P(c|U) = M(U)
    2. 根据概率P(c|U)从U中采样出一个集合L进行标注
    3. 将标注的集合L加到训练集中,重新训练并更新模型M
- 重复上述步骤知道提升/标注成本<某个阈值
- 不确定性采样方法
    1. query synthesis
    2. selective sampleing
    3. pool-based active learning
- 不确定性采样最简单的方法
    - 熵: 每个类别上的预测概率对应的熵越大,表明越不确定。对于二分类,认为在0.5附近的具有很大不确定性
    - least confident: 1 - max_c P(c|x) 代表样本x的不确定性
    - margin sampling: P(c_1|x) - P(c_2|x) 即最大的概率减去第二大的概率作为样本x的不确定性
- Query by Commitee:
    - 版本空间不确定性最小化
    - 训练很多个模型, 如果这些模型在某个未标注数据上预测的结果差异很大,表明这个样本的不确定性很大
- 综述论文: Active Learning Literature Survey

## Active Deep Learning to Tune Down the Noise in Labels
- KDD2018
- 通过主动学习,消除标注样本中的噪声
- 通过众包平台标注的样本会包含噪声
- 主动学习
    - Query by Commitee: 如果先验分布已知,就能找到决策面附近的样本进行标注; 但是如果位置的话,就会有偏
    - 通过 ee搜索,可以避免这种偏差
    - 