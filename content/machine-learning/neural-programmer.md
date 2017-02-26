---
title: "神经程序员"
layout: page
date: 2016-12-25
---
[TOC]

## 关于
用神经网络做编程！！

1. Neural Programmer: Inducing Latent Programs with Gradient Descent, Arvind Neelakantan, Quoc V Le, Ilya Sutskever, ICLR 2016

## 摘要
神经网络虽然在很多领域如语音识别，图像识别etc等领域取得了巨大的成功，但是在基本的算术和逻辑代数运算上，
神经网络的精确学习却很困难！ Neural Programmer 增加了一部分基本的算术和逻辑操作，解决了这一点。

> A major limitation of these models is in their inability to learn even simple arithmetic and logic operations.
> recurrent neural networks (RNNs) fail at the task of adding two binary numbers even when the result has less than 10 bits

往梯度中增加高斯噪声，可以提升训练效果，增加泛化能力。

## 模型
Neural Programmer 由3个部分构成：

1. question Recurrent Neural Network (RNN) 处理用自然语言输入的问题
2. selector 生成两个概率分布，用于（soft select）选择数据分片和操作
3. history RNN 记住历史选择的数据分片和操作

<img src="/wiki/static/images/neural-programmer.png" style="width:500px; float:left" />

除了操作列表，其他的都可以通过梯度下降，由数据 (question, data source, answer) 三元组样本训练得到！

data source 以表格形式存在 $(table \in \mathbb{R}^{M \times C})$

QUESTION MODULE 是一个简单的 RNN 模块，将输入的词序列（分布是表达）编码成一个 d 维的向量q。
如果问题包含长句子，采用一个双向 RNN。

预处理将数字单独拿出来，放到一个列表中。

SELECTOR 生成两个分布，一个是operator的概率分布，一个是数据列的概率分布（问题：数据列是变动的，怎么办？）
输入是问题的编码向量q（d维)和输入历史的向量h[t]（d维）。

每一个 operator 编码为一个d维向量！所有的 operator 构成一个矩阵 U。 operator 选择表达式为：

$$
\alpha_t^{op} = softmax(U tanh(W^{op} [q; h_t]))
$$

数据列名采用问题编码RNN中的词向量表达！或者 RNN phrase embedding。
所有的列名构成一个矩阵 P！列选择表达为

$$
\alpha_t^{col} = softmax(P tanh(W^{col} [q; h_t]))
$$

将出现的数字单独拿出来，对于比较操作，需要知道比较的列，即 pivot，

$$
\beta_{op} = softmax(Z U(op))  \\\\
pivot_{op} = \sum_{i=1}^N \beta(i) qn_i
$$
