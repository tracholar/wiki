---
title: "Deeplearning4j"
layout: page
date: 2016-09-27
tags: ["优化", "Machine Learning"]
---
[TOC]

## 分布式训练
DL4J 支持分布式训练，加快训练速度，目前仅支持数据并行，参数平均方式的训练！

```java
TrainingMaster tm = new ParameterAveragingTrainingMaster.Builder(int dataSetObjectSize)
            ... (your configuration here)
            .build();
```
