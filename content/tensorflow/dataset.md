---
title: "Tensorflow 数据读取"
layout: page
date: 2018-10-22
---
[TOC]

TensorFlow 导入数据的基本机制有两种,一种是利用 `placeholder`,一种是将数据读取也作为一个OP。
如果在GPU上执行计算图的计算,那么前一种方式会因为从CPU拷贝数据到GPU而堵塞,后一种方式可以利用多线程提升性能。


## TFRecordDataset
`TFRecordDataset` 是Tensorflow中标准格式,

### TFRecordDataset的生成
如果利用Spark可以利用 [spark-tensorflow-connector](https://github.com/tensorflow/ecosystem/tree/master/spark/spark-tensorflow-connector),
可以直接下载编译好的jar包,通过 `--jars path-to-spark-tensorflow-connector.jar` 导入这个包,然后就可以直接将`DataFrame`转换成分布式TFRecord文件。
不过这个包要 Spark2.0 以上才支持。

```scala
import org.apache.commons.io.FileUtils
import org.apache.spark.sql.{ DataFrame, Row }
import org.apache.spark.sql.catalyst.expressions.GenericRow
import org.apache.spark.sql.types._

val path = "test-output.tfrecord"
val testRows: Array[Row] = Array(
new GenericRow(Array[Any](11, 1, 23L, 10.0F, 14.0, List(1.0, 2.0), "r1")),
new GenericRow(Array[Any](21, 2, 24L, 12.0F, 15.0, List(2.0, 2.0), "r2")))
val schema = StructType(List(StructField("id", IntegerType), 
                             StructField("IntegerCol", IntegerType),
                             StructField("LongCol", LongType),
                             StructField("FloatCol", FloatType),
                             StructField("DoubleCol", DoubleType),
                             StructField("VectorCol", ArrayType(DoubleType, true)),
                             StructField("StringCol", StringType)))
                             
val rdd = spark.sparkContext.parallelize(testRows)

//Save DataFrame as TFRecords
val df: DataFrame = spark.createDataFrame(rdd, schema)
df.write.format("tfrecords").option("recordType", "Example").save(path)

//Read TFRecords into DataFrame.
//The DataFrame schema is inferred from the TFRecords if no custom schema is provided.
val importedDf1: DataFrame = spark.read.format("tfrecords").option("recordType", "Example").load(path)
importedDf1.show()

//Read TFRecords into DataFrame using custom schema
val importedDf2: DataFrame = spark.read.format("tfrecords").schema(schema).load(path)
importedDf2.show()
```

提交命令

```bash
## jar包在本地
$SPARK_HOME/bin/spark-shell --jars target/spark-tensorflow-connector_2.11-1.10.0.jar

## jar包在HDFS上
$SPARK_HOME/bin/spark-shell --jars viewfs://xxxx/path/to/spark/tensorflow/connector.jar
```

## Dataset读取

```python
# 定义转换函数,输入时序列化的
def parse_tfrecords_function(example_proto):
    features = {
        'label' : tf.FixedLenFeature([1], tf.float32),
        'idx' : tf.VarLenFeature(tf.int64),
        'value' : tf.VarLenFeature(tf.float32)
    }
    
    # tf.parse_single_example 将序列化的数据解码成张量
    parsed_features = tf.parse_single_example(example_proto, features)
    return parsed_features["idx"], parsed_features["value"], parsed_features['label']

dataset = tf.data.TFRecordDataset(filenames)
# map, repeat, prefetch, shuffle, batch 都是可选的,可以直接 make_initializable_iterator
it = dataset.map(parse_tfrecords_function).repeat(10).prefetch(10240).shuffle(10240).batch(128).make_initializable_iterator()
idx_op, value_op, label_op = it.get_next()

# 需要初始化
sess.run(it.initializer)  
```