---
title: "Spark"
layout: page
date: 2016-07-05
---
[TOC]


## 安装
从Spark官网下载安装包，然后解压即可。非常简单


## 启动主机和worker
进入spark目录，然后运行脚本
```bash
./sbin/start-master.sh
```
即可。进程会在后台运行，你可以通过 <http://localhost:8080> 进行监控。


启动worker的脚本是
```bash
./bin/spark-class org.apache.spark.deploy.worker.Worker spark://IsP:PORT
```
其中IP和PORT可以在监控页面看到。


关闭worker很简单，直接关闭worker运行的shell或者ctr + c中断即可。
关闭主机需要运行脚本
```bash
./sbin/stop-master.sh
```

### Spark shell
启动scala版的shell命令为`./bin/spark-shell`，python版的命令为`./bin/pyspark`


## SparkContext
sc是spark的入口，通过`SparkConf`来创建它。
```scala
val sparkConf = new SparkConf().setAppName("FromPostgreSql")
  .setMaster("local[4]")
  .set("spark.executor.memory", "2g")
val sc = new SparkCsontext(sparkConf)
```
对了，目前spark只支持的scala版本是2.10.x，所以用2.11.x版本可能会出错。

使用`sc.stop()`方法停止SparkContext。貌似不执行stop，本地用`sbt run`运行时会出现错误信息，
但是提交jar方式运行没问题。
参考<https://stackoverflow.com/questions/28362341/error-utils-uncaught-exception-in-thread-sparklistenerbus>.

- issue
    - 使用`sbt run`方式运行任务，如果涉及到`saveAsTextFile`操作时，会出错，原因未知。

## RDD
- RDD，全称为Resilient Distributed Datasets，是一个容错的、并行的数据结构，可以让用户显式地将数据存储到磁盘和内存中，并能控制数据的分区。
- in-memory cache. `cache()`
- RDD 常用操作
    - `count()`
    - `foreach`, `map`, `flatMap`, `filter`,
- 并行化容器，可以通过`SparkContext.parallelize` 方法创建分布式便于并行计算的数据结构。也可以用来将scala的容器转换为RDD结构的tips
```scala
val data = Array(1,2,4,5,6,7)
val distData = sc.parallelize(data)
```
- 从外部数据库创建，支持本地文件系统，HDFS，Cassandra， HBase， Amazon S3， 等。
  支持的文件格式包括文本文件， SequenceFiles，其他Hadoop输入格式。
  其中文本格式可以通过`SparkContext.textFile(URI [, partition_number])`方法创建RDD。
    - 支持本地文件和网络文件的URI，"/home/user/path-to-file", "hdfs://path-to-file"
    - 支持文件夹，压缩文件，通配符等方式。例如"/path-to-file/\*.gz", "/path-to-file/directory"
    - 指定分区数目，每一个分区是64MB，默认创建一个分区。
    - 也可以通过 `SparkContext.wholeTextFiles` 读取一个目录下的所有文本文件，返回的是 (filename, content)，
      而`textFile` 则返回所有的行
    - 其他Hadoop输入格式可以使用 `SparkContext.hadoopRDD` 方法。
    - 其他基于 `org.apache.hadoop.mapreduce` API 的输入格式可以通过  `SparkContext.newAPIHadoopRDD` 方法创建
    - `RDD.saveAsObjectFile` 和 `SparkContext.objectFile` 支持保存RDD为简单的序列化java对象。

### RDD 操作
- 支持两种操作 map， reduce
- 变换：从一个已经存在的数据创建新的数据，如 `map`, `reduce`, `reduceByKey`。所有的变换操作都是惰性求值，而且不保存
  中间结果。如果重新计算，中间结果也会重新计算。如果需要保存中间结果可以通过`RDD.persist()`方法指明保存该RDD。
- 传递函数给spark，不同的语言不同
    - scala中可以通过以下几种方式
        - 匿名函数
        - 单例模式对象的一个静态方法
        - 一个类的实例对象的一个成员方法，这种情况需要传递整个对象过去。同样，如果函数应用了外部的对象的一个域，那么也需要传递整个对象。
          为了避免这个问题，可以创建该域的一个本地拷贝。

```scala
class MyClass {
  val field = "Hello"
  def doStuff(rdd: RDD[String]): RDD[String] = { rdd.map(x => field + x) }
}

// 修改后的doStuff 函数
def doStuff(rdd: RDD[String]): RDD[String] = {
  val field_ = this.field
  rdd.map(x => field_ + x)
}
```

    - java,  `org.apache.spark.api.java.function` 对象，或者java 8 的lambda表达式
    - python， lambda表达式，本地函数，模块的顶级函数，对象的方法

- 重新分区，`repartition`会重新分配所有数据，如果是降低分区数目，可以用`coalesce`，它会避免移动所有数据，
  而只是移动丢弃的分区的数据，参考[stackoverflow的讨论](https://stackoverflow.com/questions/31610971/spark-repartition-vs-coalesce)。

### RDD持久化
持久化的两个方法 `.cache()`和`.persist(StorageLevel.SOME_LEVEL)`，存储级别有：

- MEMORY_ONLY ： 默认级别，以 deserialized Java objects 保存在内存（JVM），内存放不下的部分每次也是重新计算
- MEMORY_AND_DISK ： 保存在内存，放不下的放在磁盘
- MEMORY_ONLY_SER ： 序列化后再保存在内存，放不下重新计算
- MEMORY_AND_DISK_SER ：与上一个术语差异在于放不下的放磁盘
- DISK_ONLY ： 只放磁盘
- MEMORY_ONLY_2, MEMORY_AND_DISK_2, etc. ： 多保存一个备份
- OFF_HEAP (experimental) ： Store RDD in serialized format in Tachyon

在python中都是用pickle序列化，只有这一种。
手动移除cache的方法是 `RDD.unpersist()`，如果不手动移除，Spark 也会自动处理cache的。

### 理解闭包
- 在RDD的foreach中，对外部变量的引用实际上是复制了该对象到executor中，然后引用executor中的那个对像，所以不会改变本想引用的那个对象。
  可以使用`Accumulator`来实现改变主对象。
- 输出RDD到stdout，同样存在一个问题，在foreach和map中的prinln是输出到executor的stdout。可以通过`RDD.collect().foreach(println)`方法实现，
  如果该只是打印一部分，可以通过`RDD.take(100).foreach(println)` 来实现。

### KV值操作
- 由于KV类型可以是很多不同类型，通用的操作不多，最常用的是 `shuffle` 操作，例如 grouping 和 aggregating by key。
- 在spark中通过创建Tuple2对象实现K-V，例如在下述代码中

```scala
val lines = sc.textFile("data.txt")
val pairs = lines.map(s => (s, 1))
val counts = pairs.reduceByKey((a, b) => a + b)
```

注意，在使用自定义的对象作为key的时候，需要确保`.equals()`方法与`hashCode()`方法兼容。

### 通用的变换
- map(func)
- filter(func)
- flatMap(func), 相当于先做map，然后做flat操作
- mapPartitions(func)，map到每一个分区
- mapPartitionsWithIndex(func)， 带有index的版本
- sample， 采样
- union，并集
- intersection，交集
- distinc, 去重
- groupByKey，输入(K,V)，输出(K, Iter<V>)
- reduceByKey(func)，输入(K,V)
- aggregateByKey
- sortByKey
- `join(otherDataset [, numTasks])`,  (K,V), (K,W) -> (K, (V,W))  
- cogroup
- cartesian 笛卡尔积？
- pipe
- coalesce
- repartition
略
### Action
- reduce
- collect
- count
- first
- take(n)
- takeSample
- takeOrdered
- saveAsTextFile(path)
- saveAsSequenceFile(path), java and scala
- countByKey，对每一个key单独计数
- foreach(func)


## 共享变量
- broadcast变量，不同的executor共享
```scala
val broadcastVar = sc.broadcast(Array(1, 2, 3))
broadcastVar.value
```
- Accumulator,
```scala
val accum = sc.accumulator(0, "My Accumulator")
sc.parallelize(Array(1, 2, 3, 4)).foreach(x => accum += x)
accum.value
```

一般需要实现自己的AccumulatorParam子类，
```scala
object VectorAccumulatorParam extends AccumulatorParam[Vector] {
  def zero(initialValue: Vector): Vector = {
    Vector.zeros(initialValue.size)
  }
  def addInPlace(v1: Vector, v2: Vector): Vector = {
    v1 += v2
  }
}

// Then, create an Accumulator of this type:
val vecAccum = sc.accumulator(new Vector(...))(VectorAccumulatorParam)
```

## 提交spark任务
使用 bin/spark-submit 脚本提交，语法

```bash
./bin/spark-submit \
  --class <main-class> \
  --master <master-url> \
  --deploy-mode <deploy-mode> \
  --conf <key>=<value> \
  ... # other options
  <application-jar> \
  [application-arguments]
```

> For Python applications, simply pass a .py file in the place of <application-jar> instead of a JAR, and add Python .zip, .egg or .py files to the search path with --py-files.


## Spark Streaming
简单地说，就是用来从其他地方拉数据的。
输入数据流 => Spark streaming => batches of input data => Spark engine => batches of processed data

## Spark SQLContext，
- 从SparkContext创建
```scala
org.apache.spark.sql.SQLContext
val sc: SparkContext // An existing SparkContext.
val sqlContext = new org.apache.spark.sql.SQLContext(sc)
```

- 使用`.sql`函数进行SQL查询，Spark SQL支持的语法
```sql
SELECT [DISTINCT] [column names]|[wildcard]
FROM [kesypace name.]table name
[JOIN clause table name ON join condition]
[WHERE condition]
[GROUP BY column name]
[HAVING conditions]
[ORDER BY column names [ASC | DSC]]
```
如果使用join进行查询，则支持的语法为：
```sql
SELECT statement
FROM statement
[JOIN | INNER JOIN | LEFT JOIN | LEFT SEMI JOIN | LEFT OUTER JOIN | RIGHT JOIN | RIGHT OUTER JOIN | FULL JOIN | FULL OUTER JOIN]
ON join condition
```

-
### DataFrame
Spark DataFrame的设计灵感正是基于R与Pandas。
我们通过外部Json文件创建一个DataFrame：
```scala
val dataFrame = sqlContext.load("/example/data.json", "json")
dataFrame.show()
```

> With a SQLContext, applications can create DataFrames from an existing RDD, from a Hive table, or from data sources.

```scala
// Create the DataFrame
val df = sqlContext.read.json("examples/src/main/resources/people.json")

// Show the content of the DataFrame
df.show()
// age  name
// null Michael
// 30   Andy
// 19   Justin

// Print the schema in a tree format
df.printSchema()
// root
// |-- age: long (nullable = true)
// |-- name: string (nullable = true)

// Select only the "name" column
df.select("name").show()
// name
// Michael
// Andy
// Justin

// Select everybody, but increment the age by 1
df.select(df("name"), df("age") + 1).show()
// name    (age + 1)
// Michael null
// Andy    31
// Justin  20

// Select people older than 21
df.filter(df("age") > 21).show()
// age name
// 30  Andy

// Count people by age
df.groupBy("age").count().show()
// age  count
// null 1
// 19   1
// 30   1
```

- 直接在文件上运行SQL！
```scala
val df = sqlContext.sql("SELECT * FROM parquet.`examples/src/main/resources/users.parquet`")
```

- 注册UDF
```scala
sqlContext.udf.register("strLen", (s: String) => s.length())
```




## MLlib
- 不同的包的特点，推荐`spark.ml`
    - `spark.mllib` contains the original API built on top of RDDs.
    - `spark.ml` provides higher-level API built on top of `DataFrames` for constructing ML pipelines.

### spark.ml 包

#### 基础类
- 基于DataFrame，借助于抽象，将模型抽象为三个基本类，estimators（实现fit方法）, transformers（实现transform方法）, pipelines
- 一个正常的模型应该同时实现 `fit` 和 `transform` 两个方法
- `transform` 将生成一个新的DataFrame，包含了预测的结果
- `fit` 的DataFrame需要包含两列 featuresCol 和 labelCol 默认名字为 label
- `transform` 之前的DataFrame需要包含一列 featuresCol，默认名字为features，输出三列（依赖于参数），三列有默认名字，都可以通过setter函数进行设置。
    - predictedCol 预测的标签，默认名字为 `prediction`
    - rawPredictedCol 预测的裸数据？向量？逻辑回归是`wx`貌似，默认名字为 `rawPrediction`
    - probabilityCol 预测的概率，默认名字为 `probability`

- 模型参数封装类 `Param`，他的一个常用子类是 `ParamMap`，实现了Map接口，可以通过 `get, put`进行操作

```scala
val paramMap = ParamMap(lr.maxIter -> 20)
  .put(lr.maxIter, 30) // Specify 1 Param.  This overwrites the original maxIter.
  .put(lr.regParam -> 0.1, lr.threshold -> 0.55) // Specify multiple Params.
```

```python
paramMap = {lr.maxIter: 20}
paramMap[lr.maxIter] = 30 # Specify 1 Param, overwriting the original maxIter.
paramMap.update({lr.regParam: 0.1, lr.threshold: 0.55}) # Specify multiple Params.
```

- `pipeline` 将不同模型（transform）堆叠起来，类似于sklearn里面的pipeline。
pipeline保存了一个Array[PipelineStage]，可以通过`.setStage(Array[_ <: PipelineStage])`函数进行设置。
pipeline实现了estimator的fit接口和transformer的transform接口。

- `PipelineStage`抽象类，啥也没干？？？？？？？？！！！！！`transformer`还是它的子类！！
- `UnaryTransformer` 单列转换对象，是transformer的子抽象类，也实现了pipelinestage接口。
  有两个变量`inputCol`和`outputCol`代表输入输出列的名字。
  有几个常用的实例，例如Tokenizer，HashingTF等。

- 模型的保存和加载，利用类的静态方法`.load`加载(MLReader的实现)，而用实例的`.save`方法（MLWriter的实现）保存模型到文件。

- 模型评估 `Evaluator`(实现`evaluate(dataFrame)`方法)， `RegressionEvaluator`回归， `BinaryClassificationEvaluator`二元分类，
  `MulticlassClassificationEvaluator` 多元分类。
    -  `BinaryClassificationEvaluator` 除了`evaluate`方法之外，还有几个重要的属性和属性setter。标签列名`labelCol`，度量名称 `metricName` 默认为areaUnderROC，即AUC。`rawPredictionCol` 预测结果列名。以及相应的setter和getter。
    - `MulticlassClassificationEvaluator`，三个属性 `labelCol`，`metricName` （supports "f1" (default), "precision", "recall", "weightedPrecision", "weightedRecall"），`predictionCol`
    - `RegressionEvaluator`，三个属性  `labelCol`，`metricName` （"rmse" (default): root mean squared error， "mse": mean squared error， "r2": R2 metric， "mae": mean absolute error），`predictionCol`

- 交叉验证选择模型超参数。交叉验证 `CrossValidator` 类，有4个基本方法
    - `.setEstimator`
    - `.setEvaluator`
    - `.setEstimatorParamMaps(paramGrid)` 参数网络
    - `.setNumFolds(k)` k-fold交叉验证的参数k
同是他也是一个estimator，调用它的`fit`方法训练模型，返回训练好的模型CrossValidatorModel或模型序列。
他也是一个transformer，调用`transform`方法直接执行多个transform。

- 训练集和测试集的分割 `TrainValidationSplit`与交叉验证类类似，取代`.setNumFolds`的是函数`.setTrainRatio(ratio)`。

- 参数网格可以通过 `ParamGridBuilder`对象创建，他有三个方法，`addGrid(param, values:Array)`添加一个参数网格，
  `baseOn(paramPair)`设置指定参数为固定值，`build()`方法返回一个`Array[ParamMap]`数组

#### 特征变换
- `ml.feature`包中的模型 `IndexToString, StringIndexer`。
    - `StringIndexer` 将字符串类型的变量（或者label）转换为索引序号，序号会按照频率排序，不是字典序
    - `IndexToString` 和`StringIndexer`配合使用可以让字符串类型的变量的处理变得透明，这个是将index变成原来的字符串

```scala
val labelIndexer = new StringIndexer()
    .setInputCol("label")
    .setOutputCol("indexedLabel")
    .fit(training)
val rf = new RandomForestClassifier()
    .setPredictionCol("indexedPrediction")
    .setLabelCol("indexedLabel")
setRFParam(rf, param)
val labelConverter = new IndexToString()
    .setInputCol("indexedPrediction")
    .setOutputCol("prediction")
    .setLabels(labelIndexer.labels)
val pipeline = new Pipeline()
    .setStages(Array(labelIndexer, rf, labelConverter))
```

#### DataFrame
DataFrame相当于 RDD[Row]，而Row相当于一个可以包含各种不同数据的Seq。
DataFrame通过collect函数之后就是Array[Row]

通过工厂方法`SQLContext.createDataFrame`创建DataFrame，可以从一下几个数据源创建

- 从`List(label, FeatureVector)`序列创建
- 从 `JavaRDD`创建
- 从 `RDD` 创建
- 从 `List[Row]` 创建
- 从 `RDD[Row]` 创建

```scala
val training = sqlContext.createDataFrame(Seq(
  (1.0, Vectors.dense(0.0, 1.1, 0.1)),
  (0.0, Vectors.dense(2.0, 1.0, -1.0)),
  (0.0, Vectors.dense(2.0, 1.3, 1.0)),
  (1.0, Vectors.dense(0.0, 1.2, -0.5))
)).toDF("label", "features")
```

spark的DataFrame每一列可以存储向量！甚至图像！任意值都行！！

- SQL操作
    - select(col1, col2, ...) 选取部分列
    - sample 采样
    - sort 排序
    - unionAll 融合其他表
    - orderBy
    - limit
    - join 内连接
    - groupyBy
    - filter(sql表达式)

- lazy val rdd 对象，可以通过RDD接口操作
- df.sqlContext 可以访问创建该DataFrame 的SQLContext对象，rdd.sparkContext 可以访问创建RDD的SparkContext对象。


- 保存到磁盘
```scala
df.rdd.map { 转换操作 } .saveAsTextFile(filepath)
```



### spark.mLlib
- LogisticRegressionWithLBFGS
- LogisticRegressionModel， 要`model.clearThreshold` predict才会输出概率，否则输出的是判决后的值

### 基本数据结构
- Vector, 可以通过工厂对象`Vectors`创建，普通向量`Vectors.dense`，稀疏向量`Vectors.sparse`，通过`.toArray`方法转换为`Array[Double]`
- LabeledPoint, 二元组 `(label:Double, features: Vector)`
- Matrix， 可以通过工厂对象`Matrices`创建，普通矩阵 `Matrices.dense`，稀疏矩阵`Matrices.sparse`
- RowMatrix，前面的向量和矩阵都是存在单机中，这种和下面的矩阵是分布式存储的。
- IndexedRowMatrix，indexedrow是(long, vector)的包装使得index是有意义的
- CoordinateMatrix，
- BlockMatrix


```scala
val rows: RDD[Vector] = ... // an RDD of local vectors
// Create a RowMatrix from an RDD[Vector].
val mat: RowMatrix = new RowMatrix(rows)


val rows: RDD[IndexedRow] = ... // an RDD of indexed rows
// Create an IndexedRowMatrix from an RDD[IndexedRow].
val mat: IndexedRowMatrix = new IndexedRowMatrix(rows)


```

### 模型评估
包名`org.apache.spark.mllib.evaluation`

- 两分类 BinaryClassificationMetrics
- 多分类 MulticlassMetrics


## TIPS
### 使用log4j
```scala
package org.apache.log4j;

  public class Logger {

    // Creation & retrieval methods:
    public static Logger getRootLogger();
    public static Logger getLogger(String name);

    // printing methods:
    public void trace(Object message);
    public void debug(Object message);
    public void info(Object message);
    public void warn(Object message);
    public void error(Object message);
    public void fatal(Object message);

    // generic printing method:
    public void log(Level l, Object message);
}

// 例子
import org.apache.log4j.Logger
val log = Logger.getLogger(getClass.getName)
log.info("info")
```


1. Spark in Action [BOOK] <https://zhangyi.gitbooks.io/spark-in-action>
2. Spark Programming Guide <https://spark.apache.org/docs/latest/programming-guide.html>
