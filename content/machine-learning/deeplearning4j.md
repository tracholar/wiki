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


## 在 Spark 上运行训练任务
### 注意事项
- 需要指定内存，否则会报各种错误，方法和参数设置见官方文档 <https://deeplearning4j.org/spark>，
从官网上抄下来的一个例子：

```
--class my.class.name.here --num-executors 4 --executor-cores 8 --executor-memory 4G --driver-memory 4G --conf "spark.executor.extraJavaOptions=-Dorg.bytedeco.javacpp.maxbytes=5368709120" --conf "spark.driver.extraJavaOptions=-Dorg.bytedeco.javacpp.maxbytes=5368709120" --conf spark.yarn.executor.memoryOverhead=6144
```
