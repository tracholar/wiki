---
title: "局部敏感哈希"
layout: page
date: 2019-08-24
---
[TOC]

## 简介
LSH是一种利用hash的方法,对向量进行快速近邻检索的方法。

## minhash
- 最小化hamming距离
- 向量的每一维是0/1变量
- 每一次hash的步骤是
    - 随机shuffle向量的维度顺序
    - 为1的最小下标作为hash值
    - 多次hash得到hash向量
- hash(x) = shuffle(x) 的不为0的下标最小值

## 随机投影超平面
- 最大化余弦相似度
- 连续向量
- 每一次hash的步骤是
    - 随机生成一个向量v
    - 计算投影 x·v 的符号
    - hash值是:大于0则为1,小于0则为0
    - 多次hash生成多个比特

## 投影取整
- 最小化欧式距离
- 连续向量
- 每次hash的步骤是
    - 随机生成一个向量v
    - hash = floor(x·v / r) r是尺寸缩放因子
    - 多次hash生成hash向量

## 内积
- 最大化内积
- 连续向量
- 在建索引的时候,给每个向量x增加一维, $(\sqrt{\phi^2 - x^2})$ , $(\phi^2)$是所有向量的最大模
- 在检索时, 给检索向量q增加一维0. 
- 那么新向量的欧式距离 
$$
||\hat{x} - \hat{q}||^2 = \phi^2 + q^2 - 2 x\dot q
$$
- 因为前两项是与待检索向量无关的常数,所以最小化新向量的欧式距离就等价于最大化原始向量内积


## 工程实现
- 每一次hash相当于对原始集合的划分,多次划分实际上就生成一棵树
- 由于划分没有重叠,所以单棵树精度不高,可以建多棵树来提高精度
- 软件工具包:
    - annoy, C++ 库 <https://github.com/spotify/annoy>
        - 我实现了一个简单的工具,可以在HIVE和Spark中使用 <https://github.com/tracholar/annoy/tree/hive-dev/src>
