---
title: "Flink"
layout: page
date: 2019-07-04
---
[TOC]


## 关于
实时数据处理利器。

## 基本概念
### 编程模型
- 不同的抽象级别
    - stateful streaming: 通过 process function 嵌入到 `DataStream` API 中。提供event自由处理能力。
    - `DataStream` 和 `DataSet` API。绝大多数应用关注这个级别的API即可,关注数据的变换,聚合,JOIN
    - `Table API` 扩展关系数据库, 类似于HIVE表
    - `SQL` 和table api类似,但是是用SQL来表达
    
![抽象级别](/wiki/static/images/levels_of_abstraction.svg)

- 编程和数据流
    - 基本block是 `stream` 和 `transformations`。`transformations`输入一个或多个流,输出一个或多个流,类似于Spark的变换算子。
    - 每一个数据流从`source`开始, 到 `sink` 结束

![program_dataflow](/wiki/static/images/program_dataflow.svg)

- Parallel Dataflows
    - stream可以有两种变换数据的模式: one-to-one, redistributing
        - one-to-one: 从source到`map()`操作之间,保持数据分片和时间顺序
        - redistributing: 从`map()`到`keyBy/window`操作之间。不保持分片和时间顺序,类似于Spark的shuffle
        
![parallel_dataflow](/wiki/static/images/parallel_dataflow.svg)
        
- Windows
    - 时间驱动: 每30s
    - 数据驱动: 每300个样本
    - 窗的类型:
        - tumbling windows, 没有重叠,滚动窗
        - sliding windows, 有重叠,滑动窗
        - session windows, 用没有行为的时间区间来截断

- Time
    - Event Time, 事件创建的时间
    - Ingestion time, 事件进入到flink dataflow的时间
    - Processing Time, 事件被处理的时间点
    
![event_ingestion_processing_time](/wiki/static/images/event_ingestion_processing_time.svg)
    

- Stateful Operations 有状态的操作
    - 需要cache多个事件才能操作
    - 通过`keyBy`实现, 实际上是维持一个key/value存储
    - 保证后续操作只跟这个`key`有关,不用考虑其他`key`数据,让flink分配key的操作是透明的
    
![state_partitioning](/wiki/static/images/state_partitioning.svg)

- Checkpoints for Fault Tolerance 容错
    - stream replay 和 checkpointing
    - 保留操作的状态,可以从checkpoint通过回放的方式,重新消费数据
- Batch on Streaming
    - 有限的流
    - `DataSet` API
    
### DataStream API

## 快速入门


## 参考链接
- <https://ci.apache.org/projects/flink/flink-docs-stable/>
- <https://ci.apache.org/projects/flink/flink-docs-stable/concepts/programming-model.html>
