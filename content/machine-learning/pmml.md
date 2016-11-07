---
title: "PMML: 用XML描述机器学习模型"
layout: page
date: 2016-10-28
---
[TOC]

## 关于
PMML是用XML来描述数据挖掘模型的一种通用可交换格式，利用PMML可以将各种工具生成的模型很方便的发布到生产环境！
目前著名的 sklearn 和 R中的模型，都支持导出为PMML格式！
<http://dmg.org/pmml/>

## 通用结构
- 采用XML格式，只有一个根节点

```xml
<PMML version="4.1"
    xmlns="http://www.dmg.org/PMML-4_1"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
</PMML>
```

- 一个PMML文件可以包含多个模型，一个应用系统可以通过名字选择模型，否则选择第一个模型


## Header
可以包含 `copyright, description, Applilcation, name, version, Annotation`。
例子：

```xml
<Header copyright="www.tracholar.com"/>
```

## MiningBuildTask
包含任意XML值，描述训练模型的配置，不是 PMML 必须。


## DataDictionary
定义用于模型输入的数据的类型，范围等。
可以用在多个模型当中。

### `DataField`
- `name` 必须唯一！
- `opType` 操作类型，表明是连续，离散特征
- `dataType` 数据类型 <https://www.w3.org/TR/xmlschema-2/>，外加几个新类型 `timeSenconds, dateDaysSince[aYear], dateTimeSecondsSince[aYear]`
- `Value`



## TransformationDictionary
对数据变换:

- Normalization：归一化，输入可以是连续或离散值
- Discretization：将连续值变成离散值
- Value mapping：将离散值映射为离散值
- Text Indexing：对给定的term赋予一个频率值？
- Functions：函数映射
- Aggregation：聚合函数，例如求和
- Lag：使用之前的值？
## MODEL-ELEMENT

### MiningModel
#### functionName
PMML 定义了5中挖掘函数，每个模型可以有一个属性 `functionName` 用来指定functionName。
可以取值为：

- associationRules
- sequences
- classification
- regression
- clustering
- timeSeries
- mixed

#### Segmentation
- `multipleModelMethod` 多个模型融合方法：
    - `majorityVote` 投票
    - `modelChain` 模型链，前一个模型的结果作为后一个模型的输入
    - `average` 平均
    - selectFirst, selectAll, majorityVote, modelChain, etc
- Segment， id 1-based index：为什么要有一个 `<True/>`

### RegressionModel
回归模型

- MiningSchema：定义建模的字段
- Output：定义一个模型的输出结果值，使用OutputField定义名字，predictedValue字段表明输出的是 raw predicted value.
- Target

## Extension

## 其他


### modelName
用来指定模型民资


### 基本数据类型
NUMBER, INT-NUMER, REAL-NUMBER, PROB-NUMBER, PERCENTAGE-NUMBER, FIELD-NAME, ARRAY
