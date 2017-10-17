---
title: "PMML 预测模型"
layout: page
date: 2016-07-21
---
[TOC]

## 什么是PMML
PMML 是一种基于XML的标准语言，用于表达数据挖掘模型，可以用来在不同的应用程序中交换模型。
一种非常有用的应用场景是在生产环境中部署用各种建模工具训练出来的模型。
目前最新的标准是4.3 <http://dmg.org/pmml/pmml-v4-3.html>。

PMML 文件的结构遵从了用于构建预测解决方案的常用步骤，包括：

1. 数据词典，这是一种数据分析阶段的产品，可以识别和定义哪些输入数据字段对于解决眼前的问题是最有用的。这可以包括数值、顺序和分类字段。
2. 挖掘架构，定义了处理缺少值和离群值的策略。这非常有用，因为通常情况，当将模型应用于实践时，所需的输入数据字段可能为空或者被误呈现。
3. 数据转换，定义了将原始输入数据预处理至派生字段所需的计算。派生字段（有时也称为特征检测器）对输入字段进行合并或修改，以获取更多相关信息。例如，为了预测停车所需的制动压力，一个预测模型可能将室外温度和水的存在（是否在下雨？）作为原始数据。派生字段可能会将这两个字段结合起来，以探测路上是否结冰。然后结冰字段被作为模型的直接输入来预测停车所需的制动压力。
4. 模型定义，定义了用于构建模型的结构和参数。PMML 涵盖了多种统计技术。例如，为了呈现一个神经网络，它定义了所有的神经层和神经元之间的连接权重。对于一个决策树来说，它定义了所有树节点及简单和复合谓语。
5. 输出，定义了预期模型输出。对于一个分类任务来说，输出可以包括预测类及与所有可能类相关的概率。
6. 目标，定义了应用于模型输出的后处理步骤。对于一个回归任务来说，此步骤支持将输出转变为人们很容易就可以理解的分数（预测结果）。
7. 模型解释，定义了将测试数据传递至模型时获得的性能度量标准（与训练数据相对）。这些度量标准包括字段相关性、混淆矩阵、增益图及接收者操作特征（ROC）曲线图。
8. 模型验证，定义了一个包含输入数据记录和预期模型输出的示例集。这是非常重要的一个步骤，因为在应用程序之间移动模型时，该模型需要通过匹配测试。这样就可以确保，在呈现相同的输入时，新系统可以生成与旧系统同样的输出。如果实际情况是这样的话，一个模型将被认为经过了验证，且随时可用于实践。

一个通用的PMML文件结构如下(参考<http://dmg.org/pmml/v4-3/GeneralStructure.html>)：

```xml
<?xml version="1.0"?>
<PMML version="4.3"
  xmlns="http://www.dmg.org/PMML-4_3"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

  <Header copyright="Example.com"/>
  <DataDictionary> ... </DataDictionary>

  ... a model ...

</PMML>
```

## 使用PMML发布预测模型
下面以目前应用广泛的 XGBoost 模型为例，介绍使用PMML发布预测模型。


## 参考
1. https://www.ibm.com/developerworks/cn/opensource/ind-PMML1/index.html