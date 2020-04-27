---
title: "NBDT神经支持决策树：决策树+神经网络"
layout: page
date: 2020-04-24
---
[TOC]

# 关于
- 可解释性+效果
- BAIR 博客地址：<https://bair.berkeley.edu/blog/2020/04/23/decisions/>
- 论文地址：<https://arxiv.org/abs/2004.00221>
- 开源项目地址：<https://github.com/alvinwan/neural-backed-decision-trees>
- 解读 <https://mp.weixin.qq.com/s/CL1Dvx70TxsQP3KrjT8HCg>


# 主要方法
- 利用NN将每个样本变成一个d维向量（例如用VGGNet最后一层向量）
- 决策树的每个节点（叶子节点和内部节点）都有一个向量r
- 利用内积法则决策，决策路径根据样本向量与节点向量的内积
- 最后一层全连接层(softmax)的权重实际上就对应每一个类别(决策树叶子节点)的向量
- hard 推断：从root开始，每一步都走最大的子节点
- soft 推断：遍历所有路径，每个叶子节点的概率是路径上的决策概率乘积
- 对softmax权重层级聚类，得到决策树

![NBDT](/wiki/static/images/nbdt01.png)

- Training with Tree Supervision Loss：中间节点的向量通过启发式方式构建（如子节点平均值）
- 