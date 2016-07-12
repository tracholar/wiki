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

## Spark SQLContext
Spark SQL支持的语法
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

### DataFrame
Spark DataFrame的设计灵感正是基于R与Pandas。
我们通过外部Json文件创建一个DataFrame：
```scala
val dataFrame = sqlContext.load("/example/data.json", "json")
dataFrame.show()
```


1. Spark in Action [BOOK] <https://zhangyi.gitbooks.io/spark-in-action>
