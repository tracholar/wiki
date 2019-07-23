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

- Time Characteristic
    - 是否指定时间戳
    - 使用哪种时间
    
- Event Time and Watermarks
    - event time 事件时间, 数据不一定按照事件时间顺序过来
    - watermark 告诉operator到接受到当前watermark的时候,之后不再有该事件之前的数据了
    - Periodic Watermarks `AssignerWithPeriodicWatermarks`
        - 
- source functions with Timestamps and Watermarks
    - 源中直接设置时间戳和发送watermark
    - 使用 timestamp assigner 设置时间戳和水印
    
- Timestamp Assigners / Watermark Generators
    - 在数据源后面增加
    - 预定义的assigner  
        - `stream.assignAscendingTimestamps( _.getCreationTime )`
        - 固定延迟 `stream.assignTimestampsAndWatermarks(new BoundedOutOfOrdernessTimestampExtractor[MyEvent](Time.seconds(10))( _.getCreationTime ))`


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

### 分布式运行时环境
- JobManagers(maters) : 规划任务、协调checkpoint和恢复。
    - 至少有一个, 可以有多个来备份
- TaskManagers(workers) : 执行具体任务, 缓存状态, 改变数据流
- Client : 类似于Spark的client,用于汇报进展
- 部署模式
    - standalone cluster
    - YARN, Mesos
    
![processes](/wiki/static/images/processes.svg)

- task slots : 一个slot是一个线程

![tasks slots](/wiki/static/images/tasks_slots.svg)

- slot sharing

![slots sharing](/wiki/static/images/slot_sharing.svg)

- State Backends: 
    - 内存中的一个hash map
    - rocksDB
    
- Savepoints: 用户手动触发, checkpoint 是自动触发


### DataStream API
- `aggregate` 函数
    - 


## 快速入门
- wordcount

```scala
object WikipediaAnalysis {
	def main(args: Array[String]) {
		val env = StreamExecutionEnvironment.getExecutionEnvironment
		val edits = env.socketTextStream("localhost", 1025)
		val result = edits.flatMap(line => line.split("\\s+"))
			.map((_, 1)).keyBy(0)
		    .timeWindow(Time.seconds(5))
		    .sum(1)
		result.print()

		env.execute()

	}
}
```

创建流 `nc -l 1025` netcat

- 数据源
    - `sourceFunction`, `ParallelSourceFunction`, `RichParallelSourceFunction`
    - 文件源
        - `readTextFile(path)`
        - `readFile(fileInputFormat, path)`
        - `readFile(fileInputFormat, path, watchType, interval, pathFilter)`
    - socket
        - `socketTextStream`
    - 集合
        - `fromCollection(Seq)` iterator
        - `fromElements(elements: _*)`
        - `fromParallelCollection(SplittableIterator)`
        - `generateSequence(from, to)`
    - Custom
        - `FlinkKafkaConsumer08`

- 流的变换


- sinks
    - `writeAsText`
    - `writeAsCsv`
    - `print/printToErr`
    - `writeUsingOutputFormat() / FileOutputFormat`
    - `writeToSocket`
    - `addSink` 自定义的sink

- iterators, `IterativeStream`
- Execution Parameters。 <https://ci.apache.org/projects/flink/flink-docs-release-1.8/dev/execution_configuration.html>
    - `env.getConfig`

- Fault Tolerance
- Controlling Latency
    - `env.setBufferTimeout(timeoutMillis)` 默认是100ms, 即使没有满也会将buff发送出去

### Debugging
- 本地执行环节, 可以直接从IDE执行,设置断点。`StreamExecutionEnvironment.createLocalEnvironment`
- Collection Data Sources。利用 `env.fromElements` `env.fromCollection` 从序列创建流,测试
- Iterator Data Sink. `DataStreamUtils.collect`

```scala
import org.apache.flink.streaming.experimental.DataStreamUtils
import scala.collection.JavaConverters.asScalaIteratorConverter

val myResult: DataStream[(String, Int)] = ...
val myOutput: Iterator[(String, Int)] = DataStreamUtils.collect(myResult.javaStream).asScala
```


## scala API
- 增加依赖

```xml
<!-- https://mvnrepository.com/artifact/org.apache.flink/flink-scala -->
<dependency>
    <groupId>org.apache.flink</groupId>
    <artifactId>flink-scala_2.11</artifactId>
    <version>${flink.version}</version>
    <scope>provided</scope>
</dependency>

<!-- https://mvnrepository.com/artifact/org.apache.flink/flink-streaming-scala -->
<dependency>
    <groupId>org.apache.flink</groupId>
    <artifactId>flink-streaming-scala_2.11</artifactId>
    <version>${flink.version}</version>
    <scope>provided</scope>
</dependency>
```

- 导入基础包

```scala
import org.apache.flink.api.scala._
import org.apache.flink.streaming.api.scala._

// 偏函数支持
import org.apache.flink.streaming.api.scala.extensions._
```

- scala 偏函数支持, 偏函数支持flink提供了额外的API, `xxxWith`。 <https://ci.apache.org/projects/flink/flink-docs-stable/dev/scala_api_extensions.html>

## flink vs storm
- flink 有状态, 而storm无状态,需要自己管理状态
- flink 支持窗口, 而storm不支持。统计特征啊


## 算子
- `map`
- `flatMap`
- `filter`
- `keyBy`   DataStream → KeyedStream
- `reduce` KeyedStream → DataStream
- `fold` KeyedStream → DataStream, `keyedStream.fold("start")((str, i) => { str + "-" + i })`
- `Aggregations`, `min`, `max`, `sum` KeyedStream → DataStream
- `window`, KeyedStream → WindowedStream
- `windowAll`, DataStream → AllWindowedStream
- `Window Apply` `apply`, WindowedStream → DataStream; AllWindowedStream → DataStream	
- `Window Reduce`, `reduce`, WindowedStream → DataStream
- `Window Fold`, WindowedStream → DataStream
- `Aggregations on windows`, WindowedStream → DataStream
- `Union` DataStream* → DataStream
- `Window Join` DataStream,DataStream → DataStream
- `Window CoGroup`, DataStream,DataStream → DataStream	
- `Connect`, 两个流共享状态。DataStream,DataStream → ConnectedStreams	
- `CoMap, CoFlatMap`, ConnectedStreams → DataStream
- `Split`, 将一个流分为多个, DataStream → SplitStream
- `Select`, 从splitstream 中选择一个。SplitStream → DataStream
- `Iterate`, 实现迭代计算, DataStream → IterativeStream → DataStream
- `Extract Timestamps`, 抽取时间戳, DataStream → DataStream
- `Project`, 从tuple中提取部分字段, scala可以用模式匹配,但要加上扩展 <https://ci.apache.org/projects/flink/flink-docs-release-1.8/dev/scala_api_extensions.html>

### Physical partitioning
- 自定义分区, `dataStream.partitionCustom`
- 随机分区, `dataStream.shuffle()`
- 重平衡,用于解决不同分区数据倾斜的问题, `dataStream.rebalance`
- 在单机上增加或缩小分片 `dataStream.rescale`
- 广播变量到所有分片 `dataStream.broadcast`

### Task chaining and resource groups
- 手动维护任务链,构建任务分组(slot)
- 创建新链, `someStream.filter(...).map(...).startNewChain().map`
- 禁止任务链, `someStream.map(...).disableChaining()`
- 设置slot分组, `someStream.filter(...).slotSharingGroup("name")`, 默认的分组名是`default`

## Windows
- 窗的生命周期
    - 窗口创建于第一个事件到来的时候,结束时间是 窗口结束时间 + 指定的时延
- 每一个窗都有一个 `trigger` 和一个窗函数`ProcessWindowFunction, ReduceFunction, AggregateFunction or FoldFunction` 与之关联
    - `trigger` 指定什么时候应用窗函数
    - `Evictor` 可以在触发之后,移除窗口中的一些元素
- Keyed vs Non-Keyed Windows
    - 通过 `keyBy` 来划分key,变成逻辑上的分key的流。(逻辑上??)
    - 所有相同key的数据被发送到同一个task, 而不分key会使所有的数据发送到同一个task
- Window Assigners
    - 定义如何将元素关联到window。在 `window(...)` 和 `windowAll()` 中指定
    - 预定义 window assigner
        - tumbling windows 滚动窗,时间无重叠切分
            - 固定大小, 没有重叠 
            - `TumblingEventTimeWindows.of(Time.seconds(5))`
            - `TumblingEventTimeWindows.of(Time.days(1), Time.hours(-8))` 第二个参数是offset,中国位于东8区,所以加8小时offset
        - sliding windows 滑动窗
            - 固定大小, 有重叠
            - `SlidingEventTimeWindows.of(Time.seconds(10), Time.seconds(5))` 大小为10s,滑动距离为5s的滑动窗
            - `SlidingProcessingTimeWindows.of(Time.hours(12), Time.hours(1), Time.hours(-8))`
        - session windows 用两次事件时间间隔超过某个阈值进行划分
            - 不重叠, 没有固定的起止时间。通过固定时间间隔(session gap)没有收到数据来划分窗口
            - `EventTimeSessionWindows.withGap(Time.minutes(10))` 使用event时间
            - `ProcessingTimeSessionWindows.withGap(Time.minutes(10))` 使用处理时间
            - `EventTimeSessionWindows.withDynamicGap` 自定义gap
        - global windows
            - 需要自己定义 `trigger`, 否则这个窗口不会结束, 因此也不会做任何事情
            - `GlobalWindows.create()`
        - 自定义: 实现 `WindowAssigner` 类
- Window Functions
    - ReduceFunction 增量聚合
        - `.reduce { (v1, v2) => (v1._1, v1._2 + v2._2) }`
    - AggregateFunction 增量聚合
        - 通用的reduce函数, 3个类型, 输入类型IN,累积类型ACC,输出类型OUT
        - AggregateFunction 接口的函数用途
            - `createAccumulator` 创建ACC,返回ACC的初始值
            - `add` 将新的元素加到ACC中
            - `getResult` 获取最终结果
            - `merge` 合并两个中间的ACC
            
        ```scala
        class AverageAggregate extends AggregateFunction[(String, Long), (Long, Long), Double] {
          override def createAccumulator() = (0L, 0L)
        
          override def add(value: (String, Long), accumulator: (Long, Long)) =
            (accumulator._1 + value._2, accumulator._2 + 1L)
        
          override def getResult(accumulator: (Long, Long)) = accumulator._1 / accumulator._2
        
          override def merge(a: (Long, Long), b: (Long, Long)) =
            (a._1 + b._1, a._2 + b._2)
        }
        ```
    - FoldFunction 高阶函数
        - `.fold("") { (acc, v) => acc + v._2 }`
    - ProcessWindowFunction
        - 这个函数会先缓存所有, 尽量用其他聚合函数,可以增量聚合
    - windowfunction
    
- triggers
    - 每一个 `WindowAssigner` 都有一个默认的trigger
    - 自定义trigger `trigger(...)`
    - `onElement()` 在每个元素被加到窗口后调用
    - `onEventTime()` 当注册一个事件时间触发的时候调用
    - `onProcessingTime()` 当注册处理时间的时候调用
    - `onMerge` 合并状态
    - `clear()` 窗口被移除的时候调用
    
- evictor
    - `evictBefore` 在窗口函数之前调用
    - `evictAfter` 在窗口函数之后调用
    - `CountEvictor` 保留用户定义数目的元素,丢弃其他的
    - `DeltaEvictor` 到最后一个元素的时间区间,只保留低于阈值的原始
    - `TimeEvictor` 移除超过一定区间的
- Allowed Lateness

## JOIN
- 实现流的JOIN操作, 实现 inner-join, 除了key相同,还要求在同一个窗口内

```scala
stream.join(otherStream)
    .where(<KeySelector>)
    .equalTo(<KeySelector>)
    .window(<WindowAssigner>)
    .apply(<JoinFunction>)
```

- Interval Join
    - JOIN A和B两个流,除了key相同,还要求相对时间间隔在一定范围内
    - `.intervalJoin(greenStream.keyBy(elem => /* select key */)).between(Time.milliseconds(-2), Time.milliseconds(1))`
    - 指定上限和下限

## ProcessFunction
- low-level 流处理操作
    - events 流的元素
    - state 容错、一致性、只有keyedstream有
    - timers 事件时间、处理时间、只有keyedstream有
    - 可以看做 `FlatMapFunction` + 可以访问keyed state 和 timers
    - 每一个事件接受到的时候被调用
    - 通过 `RuntimeContext` 访问 keyed state
    - `stream.keyBy(...).process(new MyProcessFunction())`
- Low-level Joins
    - `CoProcessFunction`
    
- Timer Coalescing
    - 降低时间分辨率
    
## 异步IO操作
- 访问外部系统(例如外部存储), 原始的访问方式如在 `MapFunction` 中访问将会发送同步请求,这导致接口调用占用大量的处理时间
- 异步实际上是并发请求
- KV存储一般存在异步请求客户端
- 需要实现
    - 实现`AsyncFunction`发送请求
    - `callback`处理返回的结果
    - 应用异步IO操作到 DataStream
        - `Timeout` 超时时间, 默认会抛出异常, 任务重启, 通过重载 `AsyncFunction#timeout` 方法处理异常
        - `Capacity` 并发量
- 结果顺序
    - 无序, 通过 `AsyncDataStream.unorderedWait` 调用,低延时,低overhead
    - 有序, 通过 `AsyncDataStream.orderedWait` 调用

        
```scala
/**
 * An implementation of the 'AsyncFunction' that sends requests and sets the callback.
 */
class AsyncDatabaseRequest extends AsyncFunction[String, (String, String)] {

    /** The database specific client that can issue concurrent requests with callbacks */
    lazy val client: DatabaseClient = new DatabaseClient(host, post, credentials)

    /** The context used for the future callbacks */
    implicit lazy val executor: ExecutionContext = ExecutionContext.fromExecutor(Executors.directExecutor())


    override def asyncInvoke(str: String, resultFuture: ResultFuture[(String, String)]): Unit = {

        // issue the asynchronous request, receive a future for the result
        val resultFutureRequested: Future[String] = client.query(str)

        // set the callback to be executed once the request by the client is complete
        // the callback simply forwards the result to the result future
        resultFutureRequested.onSuccess {
            case result: String => resultFuture.complete(Iterable((str, result)))
        }
    }
}

// create the original stream
val stream: DataStream[String] = ...

// apply the async I/O transformation
val resultStream: DataStream[(String, String)] =
    AsyncDataStream.unorderedWait(stream, new AsyncDatabaseRequest(), 1000, TimeUnit.MILLISECONDS, 100)
```

## Streaming Connectors
- 当前支持的系统
    - Apache Kafka (source/sink)
    - Apache Cassandra (sink)
    - Amazon Kinesis Streams (source/sink)
    - Elasticsearch (sink)
    - Hadoop FileSystem (sink)
    - RabbitMQ (source/sink)
    - Apache NiFi (source/sink)
    - Twitter Streaming API (source)
- Connectors in Apache Bahir
    - Apache ActiveMQ (source/sink)
    - Apache Flume (sink)
    - Redis (sink)
    - Akka (sink)
    - Netty (source)
- 容错
- Kafka
    - `FlinkKafkaConsumer08`
    - 构造函数
        - topic name 或 topic name列表
        - 序列化方式 DeserializationSchema / KafkaDeserializationSchema
            - 解析从kafka中来的数据
            - `DeserializationSchema`
            - `T deserialize(byte[] message)`
            - 预定义的schema
                - `TypeInformationSerializationSchema`, `TypeInformationKeyValueSerializationSchema` 基于flink的 `TypeInformation`
                - `JsonDeserializationSchema` 和 `JSONKeyValueDeserializationSchema`
                - `AvroDeserializationSchema`
        - 属性
            - bootstrap.servers
            - zookeeper.connect
            - group.id
    - 指定消费的起始位置
        - myConsumer.setStartFromEarliest()      // start from the earliest record possible
        - myConsumer.setStartFromLatest()        // start from the latest record
        - myConsumer.setStartFromTimestamp(...)  // start from specified epoch timestamp (milliseconds)
        - myConsumer.setStartFromGroupOffsets()  // the default behaviour
- Kafka Consumers and Fault Tolerance
    - 对源进行checkpoint
    - `env.enableCheckpointing(5000) // checkpoint every 5000 msecs`
    
- Kafka Consumers Topic and Partition Discovery
    - 设置配置属性 `flink.partition-discovery.interval-millis`
    - 创建 consumer 时用正则表达式匹配, `new FlinkKafkaConsumer08[String](Pattern.compile("test-topic-[0-9]"), new SimpleStringSchema, properties)`
- Kafka Producer

    
```scala
val stream: DataStream[String] = ...

val myProducer = new FlinkKafkaProducer011[String](
        "localhost:9092",         // broker list
        "my-topic",               // target topic
        new SimpleStringSchema)   // serialization schema

// versions 0.10+ allow attaching the records' event timestamp when writing them to Kafka;
// this method is not available for earlier Kafka versions
myProducer.setWriteTimestampToKafka(true)

stream.addSink(myProducer)
```
            
    
```
<dependency>
  <groupId>org.apache.flink</groupId>
  <artifactId>flink-connector-kafka_2.11</artifactId>
  <version>1.8.0</version>
</dependency>
```

- side outputs
    - 在主要的结果stream中filter出满足一定规则的stream
     
- 测试
    - 单元测试
    - 集成测试: 端到端测试flink pipeline
        - `flink-test-utils_2.11`


## Tabel API
- 在stream上定义table
- 连续query
    - 将启动的时候收到的第一条记录到当前记录看做一个有限表,聚合操作在这个有限表中执行
        - `SELECT user, count(1) as cnt FROM clicks GROUP BY user`
    - 窗聚合
        - `SELECT user, TUMBLE_END(cTime, INTERVAL '1' HOURS) as endT, COUNT(1) as cnt FROM clicks GROUP BY user, TUMBLE_END(cTime, INTERVAL '1' HOURS)`




## 参考链接
- <https://ci.apache.org/projects/flink/flink-docs-stable/>
- <https://ci.apache.org/projects/flink/flink-docs-stable/concepts/programming-model.html>
