---
title: "Hive"
layout: page
date: 2016-07-08
---
[TOC]


## 关于
学习Hive时的笔记


## 配置
环境要求
    - Hive 1.2 需要 java 1.7+， 0.14-1.1 可以工作在 java1.6 版本上。
    - Hadoop 2.x
    - 可以运行在 Linux 和 Windows 环境， Mac 通常用来做开发的！

添加 `$HIVE_HOME` 环境变量，并将`$bin`目录添加到`$PATH`变量中。

`hive.metastore.warehouse.dir` 配置指明数据仓库的目录，默认是 `/user/hive/warehouse` 和 `/tmp`（临时文件目录）


## Hive 基本概念
### Hive 的定位
Hive 用来做数据仓库，非实时数据处理。

### 数据单元
- Database： 用来做名字空间，防止表名字冲突，也可以用于用户权限管理。
- Tables：表，就是传统数据库意义上的表
- Partitions：分区，一个表通常由多个分区组成。获取某个特定分区的数据比全表扫描快！每个分区一个目录！
- Buckets (or Clusters)：分桶或分簇，在一个分区里面，可以按照某些字段的hash值进行分桶，便于采样。例如PV表按照userid分桶
`clustered by (userid) into 100 buckets`。

## 类型系统
- Integers
    - TINYINT—1 byte integer
    - SMALLINT—2 byte integer
    - INT—4 byte integer
    - BIGINT—8 byte integer
- Boolean type
    - BOOLEAN—TRUE/FALSE
- Floating point numbers
    - FLOAT—single precision
    - DOUBLE—Double precision
- Fixed point numbers
    - DECIMAL—a fixed point value of user defined scale and precision
- String types
    - STRING—sequence of characters in a specified character set
    - VARCHAR—sequence of characters in a specified character set with a maximum length
    - CHAR—sequence of characters in a specified character set with a defined length
- Date and time types
    - TIMESTAMP— a specific point in time, up to nanosecond precision
    - DATE—a date
- Binary types
    - BINARY—a sequence of bytes

隐式类型转换，只能从低精度到高精度。也允许从 STRING 到 DOUBLE。
显示类型转化可以用内置函数实现。

### 复杂类型
- Structs 结构体
- Maps key-value
- Arrays 索引list

结构体类型的操作！怎么用？！

### 操作
除了常规的比较操作，还支持正则式比较：

`A RLIKE B, A REGEXP B`，字符串A是否匹配Java正则式B。注意有个坑，正则式B中的`\`需要转义字符！！
例子：`lat rlike '\d+\.\d+'`是错误的，应该是 `lat rlike '\\d+\\.\\d+'`


### 内置函数
- 数值类型函数：`round, floor, ceil, rand`
- 字符串函数： `caoncat, substr, upper, ucase, lower, lcase, trim, ltrim, rtrim, regexp_replace`
- 时间函数： `from_unixtime, to_date, year, month, day`
- 复杂类型函数：`size, get_json_object`, `reflect,java_method`可以用来调用所有java内置的函数！！
- 其他：`cast`
- 内置聚合函数：`count, sum, avg, min, max`

count 会自动去掉NULL值，这在条件count的时候很有用，例如分别统计在a>1的情况下和a<0情况下的uid，可以用一个查询搞定，不用join

```sql
select count(distinct if(a>1, uid, null)) as cnt1,
        count(distinct if(a<0, uid, null)) as cnt0
from some_table
```

## Hive SQL
- `row_number()` 函数用法，`partition`用来将数据分区编号，`order by`描述编号顺序
```sql
select uid, row_number() over (partition by uid order by uid)
from
```
- `datediff(d1, d2)`， 其中时间字符串要是这种格式`yyyy-MM-dd`，如果不是，需要先转换
- `from_unixtime(t, 'yyyyMMdd')`, `unix_timestamp(str, 'yyyy-MM-dd')`这两个函数可以实现时间字符串格式转换

- JOIN：支持常规的内连接 inner join，外链接 outer join。还支持 left semi join，用来从左边表过滤出满足join条件的记录，
相当于 where exists subquery 方式，也可以替代in，效率比 inner join高。（此时要不要将大表放右边？！哈哈）

join优化：将大表放右边！

> Also it is best to put the largest table on the rightmost side of the join to get the best performance.

- 聚合操作不能 distinct 两个不同的列

- HAVING @since(0.7.0) 可以将聚合函数放在 WHERE 中当做条件使用，
- 多表/文件插入操作：还是看代码吧！

```sql
FROM pv_users
INSERT OVERWRITE TABLE pv_gender_sum
    SELECT pv_users.gender, count_distinct(pv_users.userid)
    GROUP BY pv_users.gender

INSERT OVERWRITE DIRECTORY '/user/data/tmp/pv_age_sum'
    SELECT pv_users.age, count_distinct(pv_users.userid)
    GROUP BY pv_users.age;
```

- 动态分区插入：@since(0.6.0)，能够减少调度时间，显著提升性能！但是注意几个问题。

```sql
insert overwrite table pv partition (dt='2010-01-01', country)  ----- 动态决定country分区，dt分区值固定
insert overwrite table pv partition (dt, country='US')          ----- 将所有dt分区下 country='US' 子分区都覆盖，一般不要能用这种写法！
```

如果分区字段为NULL，会写入到默认分区HIVE_DEFAULT_PARTITION中。

影响动态分区的一些配置：

- hive.exec.max.dynamic.partitions.pernode (default value being 100) 每个 mapper 或者 reducer 能够创建的最大分区数目。
- hive.exec.max.dynamic.partitions (default value being 1000) 一个表能够创建的最大分区数目。
- hive.exec.max.created.files (default value being 100000) 所有的 mapper 和 reducer 能够创建的全部文件数目最大值
- hive.exec.dynamic.partition.mode=strict 禁止动态分区 nostric 使用动态分区。只能使用静态分区。
- hive.exec.dynamic.partition=true/false 彻底禁止动态分区。


- 写入本地文件：`INSERT OVERWRITE LOCAL DIRECTORY '/tmp/pv_gender_sum'`
- 快速采样：`TABLESAMPLE(BUCKET x OUT OF y)`，需要在建表的时候`CLUSTERED BY`支持。例如选出第3个bucket。

```sql
TABLESAMPLE(BUCKET 3 OUT OF 64 ON userid)
```

- UNION ALL：
- Array 操作：

```sql
CREATE TABLE array_table (int_array_column ARRAY<INT>);

SELECT pv.friends[2]
FROM page_views pv;
```

相关UDAF函数`percentile_approx, histogram_numeric, collect_set, collect_list`
- Map 操作：

- Custom Map/Reduce Scripts： `MAP, REDUCE`（是`TRANSFORM`的语法糖而已），或者`TRANSFORM` 函数（是否只能实现UDF的功能，UDAF和UDTF呢？）

```sql
FROM (
     FROM pv_users
     MAP pv_users.userid, pv_users.date
     USING 'map_script'
     AS dt, uid
     CLUSTER BY dt) map_output

INSERT OVERWRITE TABLE pv_users_reduced
     REDUCE map_output.dt, map_output.uid
     USING 'reduce_script'
     AS date, count;
```

脚本：

```python
import sys
import datetime

for line in sys.stdin:
  line = line.strip()
  userid, unixtime = line.split('\t')
  weekday = datetime.datetime.fromtimestamp(float(unixtime)).isoweekday()
  print ','.join([userid, str(weekday)])
```

- `CLUSTER BY, DISTRIBUTE BY, SORT BY`

- 分组：`CLUSTER BY` 相当于先按列`DISTRIBUTE BY`，然后`SORT BY`



### 优化排序
不要使用`order by` <https://stackoverflow.com/questions/13715044/hive-cluster-by-vs-order-by-vs-sort-by>         
- `ORDER BY` 全局排序，但是只能使用一个reducer
- `DISTRIBUTE BY` 采用Hash算法将map处理后的数据分发给reduce，它保证了相同的key是在同一个reducer
- `SORT BY` 不是全局排序，而是在数据进入reduce之前完成排序，只能保证每个reducer的输出是有序的，不能保证全局有序。
- `CLUSTER BY` 相当于先 DISTRIBUTE 然后 sort。也不能保证全局有序。

## HIVE 命令
- `set <key>=<value>` 设置参数
- `add FILE[S] <filepath> <filepath>*`, `add JAR[S] <filepath> <filepath>* `, `add ARCHIVE[S] <filepath> <filepath>*` 添加文件



## 文件格式
### Avro 格式
- 要求：Hive 0.9.1+
- 不同版本要求的语法还不同，具体参看<https://cwiki.apache.org/confluence/display/Hive/AvroSerDe>。
0.14 以后的版本可以在创建表的时候这样写：`STORED AS AVRO` 即可。

### ORC 格式
Optimized Row Columnar 格式，采用这种格式可以提升HIVE读写性能。

- 输出是单个文件，减少 NameNode 的负载
- 支持Hive所有格式，包括复杂格式
- 文件中存储了轻量级的索引，便于以行为单位移动指针
- 压缩：游程编码（Int）字典码（String）
- 同时读同一个文件
- split 文件，而不需要scanning for markers
- 限制了读写内存


文件结构：以Strip为单位（默认250MB）。

cli读取命令 `hive --orcfiledump`

创建表的时候这样写：`STORED AS ORC` 即可。


### Parquet 格式
Hadoop 生态中的一个！

### 压缩文件格式
直接从 gzip 等格式中存取为text格式表格。

```sql
CREATE TABLE raw (line STRING)
   ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t' LINES TERMINATED BY '\n';

LOAD DATA LOCAL INPATH '/tmp/weblogs/20090603-access.log.gz' INTO TABLE raw;
```

### LZO
略

## UDF
UDF，UDAF，UDTF

HIVE数据类型与 java 数据类型对应关系：

```
hive   java
map    HashMap
array  ArrayList<?>
```
## JOIN
- 多表 JOIN 的时候，当JOIN条件都包含同一个Key的时候，会用同一个 Map/Reduce 处理，例如

```sql
SELECT a.val, b.val, c.val FROM a JOIN b ON (a.key = b.key1) JOIN c ON (c.key = b.key1)
```

只有一个 Map/Reduce 任务。而下面这个会有2个 Map/Reduce 任务(JOIN a,b; JOIN * ,c)。

```sql
SELECT a.val, b.val, c.val FROM a JOIN b ON (a.key = b.key1) JOIN c ON (c.key = b.key2)
```

> In every map/reduce stage of the join, the last table in the sequence is streamed through the reducers where as the others are buffered. Therefore, it helps to reduce the memory needed in the reducer for buffering the rows for a particular value of the join key by organizing the tables such that the largest tables appear last in the sequence.

将大表放后面，大表会以streaming的方式进入reducer，而其他的一buffer的方式存在（内存？），可以减少内存的需求。
默认让最后的表以streaming方式进入reducer，也可以手动指定。

```sql
SELECT /*+ STREAMTABLE(a) */ a.val, b.val, c.val FROM a JOIN b ON (a.key = b.key1) JOIN c ON (c.key = b.key1)
```

当存在这个hint的时候，会将表b，c缓存，而让a以流的方式进入reducer。不存在的时候，则会将最后的表以流的方式进入reducer。

- JOIN 逻辑发生在 WHERE 之前！对于 inner join，条件放在 ON 还是 WHERE 都是一样的，但是如果是其他 JOIN， 则会有区别。
<https://stackoverflow.com/questions/354070/sql-join-where-clause-vs-on-clause>

但是在实现的时候，会先用WHERE里面的条件过滤吧？！否则性能不是很差！？

- JOIN 是不可交换的！

- 左半连接更有效！ 要求右表的字段只在 ON 条件中出现！在map端过滤掉不会参加join操作的数据，则可以大大节省网络IO。

> LEFT SEMI JOIN implements the uncorrelated IN/EXISTS subquery semantics in an efficient way.


- MAPJOIN，JOIN 小表的时候，可以减少 reducer，提升性能！但是右链接和全连接中不能用！

```sql
SELECT /*+ MAPJOIN(b) */ a.key, a.value
FROM a JOIN b ON a.key = b.key
```

上述代码不需要 reducer！

> The restriction is that a **FULL/RIGHT OUTER JOIN** b cannot be performed.

- 如果在 JOIN 列上，进行分桶了，并且其中一个表的桶数目是另一个的倍数，那么就可以采用 MAPJOIN 优化了。
在 MAP 的时候，左表的第一个桶只会去取右表的第一个桶，而不是所有的数据！这个行为不是默认的，需要设置参数：

```sql
set hive.optimize.bucketmapjoin = true
```

- 如果两个表 JOIN 的字段分桶且排序的，并且分桶数目相同，那么可以采用 sort-merge。例如满足上述条件的两个表的join可以
只需要 Map 阶段！

```sql
SELECT /*+ MAPJOIN(b) */ a.key, a.value
FROM A a JOIN B b ON a.key = b.key
```

同时需要设置参数：

```sql
set hive.input.format=org.apache.hadoop.hive.ql.io.BucketizedHiveInputFormat;
set hive.optimize.bucketmapjoin = true;
set hive.optimize.bucketmapjoin.sortedmerge = true;
```

- MAPJOIN 可以用来 JOIN 小表，实现优化，但是下列情况是不行的！！！我晕！
    - Union Followed by a MapJoin
    - Lateral View Followed by a MapJoin
    - Reduce Sink (Group By/Join/Sort By/Cluster By/Distribute By) Followed by MapJoin
    - MapJoin Followed by Union
    - MapJoin Followed by Join
    - MapJoin Followed by MapJoin

可以设置 `hive.auto.convert.join=true` 让hive自动帮你转为 MAPJOIN。从 Hive 0.11.0  开始，默认值就是true。
MAPJOIN 将小表放到内存，保存为一个 HASH MAP。工作流程是：<https://cwiki.apache.org/confluence/display/Hive/LanguageManual+JoinOptimization>

```
Local work:
read records via standard table scan (including filters and projections) from source on local machine
build hashtable in memory
write hashtable to local disk
upload hashtable to dfs
add hashtable to distributed cache

Map task
read hashtable from local disk (distributed cache) into memory
match records' keys against hashtable
combine matches and write to output
```

MAPJOIN 的限制：

- 一次只能一个KEY
- chain of MAPJOINs 是不可以的，除非写成子查询形式。`mapjoin(table, subquery(mapjoin(table, subquery....)`
- 每一次 MAPJOIN 都需要重新建立 HASH 表，包括上传和下载

- 优化链式 MAPJOIN

```sql
select /*+ MAPJOIN(time_dim, date_dim) */ count(*) from
store_sales
join time_dim on (ss_sold_time_sk = t_time_sk)
join date_dim on (ss_sold_date_sk = d_date_sk)
where t_hour = 8 and d_year = 2002
```

通过两个值设置 MAPJOIN

```sql
set hive.auto.convert.join.noconditionaltask = true;
set hive.auto.convert.join.noconditionaltask.size = 10000000;
```

SMB Map Join: Sort-Merge-Bucket (SMB) joins

表已经是分桶并且排序好的， JOIN 过程通过顺序 merge 已经排序好的表即可。（效率比普通 JOIN 高）

> However, if the tables are partitioned, there could be a slow down as each mapper would need to get a very small chunk of a partition which has a single key.

```sql
set hive.auto.convert.sortmerge.join=true;
set hive.optimize.bucketmapjoin = true;
set hive.optimize.bucketmapjoin.sortedmerge = true;

------ 大表自动转化设置
set hive.auto.convert.sortmerge.join.bigtable.selection.policy
    = org.apache.hadoop.hive.ql.optimizer.TableSizeBasedBigTableSelectorForAutoSMJ;
```
大表选择策略会自动决定哪个表被 streaming，而不是 hash 并且 streaming。可选策略有

```sql
org.apache.hadoop.hive.ql.optimizer.AvgPartitionSizeBasedBigTableSelectorForAutoSMJ (default)
org.apache.hadoop.hive.ql.optimizer.LeftmostBigTableSelectorForAutoSMJ
org.apache.hadoop.hive.ql.optimizer.TableSizeBasedBigTableSelectorForAutoSMJ
```

如果表有不同数量的keys（SORT 列），会发生异常！

SMB 存在的目的主要是为了解决大表与大表间的 Join 问题，分桶其实就是把大表化成了“小表”，然后 Map-Side Join 解决之，这是典型的分而治之的思想。

这个Blog写的不错，<https://my.oschina.net/leejun2005/blog/178631>

## 编写自己的UDF
TRANSFORM 貌似不能实现 UDAF。可以用java写UDF或UDAF，UDTF等。需要jdk1.7版本。

```sql
ADD JAR hdfs:///user/hadoop-data/user_upload/hive-kv-udaf_2.10-0.0.1.jar;
CREATE TEMPORARY FUNCTION kv as 'KV';
```

## UNION
- UNION ALL：不去重融合 1.2.0 以前只支持这个
- UNION DISTINCT：去重，1.2.0 以后默认（UNION）是这个融合
- UNION 常需要对列名重命名，使得UNION的时候，列名是相同的

## Lateral View
用在 UDTF 中。对于输入的一行，输出是多行。0.12.0 版本开始，列名可以不用写，会自动采用UDTF输出的StructObjectInspector对象自动得到列名。参考<https://cwiki.apache.org/confluence/display/Hive/LanguageManual+LateralView>

一个例子，adid_list是一个Array，`explode()`函数会将这个list输出为多行。

```sql
SELECT pageid, adid
FROM pageAds LATERAL VIEW explode(adid_list) adTable AS adid;

SELECT adid, count(1)
FROM pageAds LATERAL VIEW explode(adid_list) adTable AS adid
GROUP BY adid;

SELECT * FROM src LATERAL VIEW OUTER explode(array()) C AS a limit 10;
```

FROM 语句里面可以包含多个 Lateral View。通过 `OUTER` 关键字可以让 `explode` 输出为NULL的时候，
记录至少存在一行！（没有这个关键字，结果中将不会出现记录）



## 子查询
子查询放在 FROM 里面，在 0.13 版本后，可以放在 IN 和 EXISTS 之中，但是存在一些限制。

- 只能放在表达式右边
- IN/NOT IN 子查询只支持单列
- EXISTS/NOT EXISTS 必须有一个或者多个 correlated predicates （where 条件？）
- 对父查询字段的引用只支持在 WHERE 中。



## 采样
```sql
TABLESAMPLE (BUCKET x OUT OF y [ON colname])
```

colname 可以是非分区字段以外的字段 或者 `RAND()`。
表采样是很慢的，如果建表的时候采用`CLUSTERED BY `创建，
那么可以加快采样速度，因为只要简单地取出对应的BUCKET就可以了，而不用全表扫描。

更多信息参考：<https://cwiki.apache.org/confluence/display/Hive/LanguageManual+Sampling>

block sampling @since(0.8)

```sql
TABLESAMPLE (n PERCENT)
TABLESAMPLE (ByteLengthLiteral)   ---- 例如 100M
TABLESAMPLE (n ROWS)

----- 例子
SELECT *
FROM source TABLESAMPLE(100M) s;
```

这个是在 HDFS block level 上进行的采样，
所以一些压缩格式表数据不支持这个特性。复现可以通过设置种子来实现`set hive.sample.seednumber=<INTEGER>;`。

## 虚拟列
`INPUT__FILE__NAME, BLOCK__OFFSET__INSIDE__FILE` 在 Mapper 里面分别指输入文件名 和 全局文件位置

简单例子

```sql
select INPUT__FILE__NAME, key, BLOCK__OFFSET__INSIDE__FILE from src;
select key, count(INPUT__FILE__NAME) from src group by key order by key;
select * from src where BLOCK__OFFSET__INSIDE__FILE > 12000 order by key;
```

## 窗函数和分析函数
@since(0.11)
### 窗函数(没搞懂)
- LEAD， LEAD 开窗函数返回位于分区中当前行的下方（之后）的某个给定偏移量位置的行的值。如果超过窗的末尾了，返回NULL。例子 <https://docs.aws.amazon.com/zh_cn/redshift/latest/dg/r_Examples_of_LEAD_WF.html>
- LAG，LAG 开窗函数返回位于分区中当前行的上方（之前）的某个给定偏移量位置的行的值。如果超过窗的开头，返回NULL。例子<https://docs.aws.amazon.com/zh_cn/redshift/latest/dg/r_Examples_of_LAG_WF.html>
- FIRST_VALUE，返回分区第一个值
- LAST_VALUE，返回分区最后一个值

### OVER
可以将聚合函数的返回值应用到每一列（窗函数的功能），就像分析函数那样！！

- 标准聚合函数 `COUNT, SUM, AVG, MAX, MIN`
- `PARTITION BY` 一个或多个分区列，`ORDER BY` 一个或多个字段
- 可以在 OVER 里面指定窗 `ROWS ((CURRENT ROW) | (UNBOUNDED | [num]) PRECEDING) AND (UNBOUNDED | [num]) FOLLOWING`

```sql
SELECT a, SUM(b) OVER (PARTITION BY c, d ORDER BY e, f)
FROM T;

SELECT a, SUM(b) OVER (PARTITION BY c ORDER BY d ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)
FROM T;
```


### 分析函数
- RANK，返回序数，可能存在相同的序号，因为同时排第一之类的
- ROW_NUMBER，返回行号
- DENSE_RANK，返回序号，不存在相同的序号，即使相同，也会让后面一个序号+1
- CUME_DIST，返回累积分布 0-1 之间的值。
- PERCENT_RANK，百分比排名 0-1 之间的值。计算公式 (row_number - 1) / (count - 1)
- NTILE，对排名进行分组，尽可能保证每组数目均匀，返回分组编号。

### 其他细节
- `DISTINCT` 关键词使用在这些函数中要到 2.1.0 版本之后。
- 在 OVER 中使用聚合函数也要到 2.1.0 版本之后

```sql
COUNT(DISTINCT a) OVER (PARTITION BY c)

SELECT rank() OVER (ORDER BY sum(b))
FROM T
GROUP BY a;
```

## Enhanced Aggregation, Cube, Grouping and Rollup
- GROUPING SETS：等价于多个 GROUP BY 然后 UNION

```sql
SELECT a, b, SUM(c) FROM tab1 GROUP BY a, b GROUPING SETS ( (a,b) , a)

---- 等价于
SELECT a, b, SUM( c ) FROM tab1 GROUP BY a, b
UNION
SELECT a, null, SUM( c ) FROM tab1 GROUP BY a
```

- Grouping__ID


- Cubes and Rollups
`WITH CUBE/ROLLUP` 关键字，只能用在 `GROUP BY` 之中。
`GROUP BY a, b, c WITH CUBE` 会组合所有的可能 `(a, b, c), (a, b), (b, c), (a, c), (a), (b), (c), ( )`。
而 `GROUP BY a, b, c, WITH ROLLUP` 等价于  `GROUP BY a, b, c GROUPING SETS ( (a, b, c), (a, b), (a), ( ))`.

设置 `hive.new.job.grouping.set.cardinality` 的值，当候选分组数目（上面分别是8和4）超过这个值时，将开启额外的 Mapper Reducer 任务来处理。


## EXPLAIN 命令
用来显示 query  的执行计划的，例如对于这个query有几个stage等。

`EXPLAIN [EXTENDED|DEPENDENCY|AUTHORIZATION] query`

## HIVE 权限管理
略

## MORE
HIVE on spark!


## UDF 开发
支持 JAVA（或Scala） 写UDF UDAF UDTF！依赖：

- Java UDF：<https://github.com/apache/hive/tree/master/ql/src/java/org/apache/hadoop/hive/ql/udf/generic>
- Scala UDF及单元测试的例子：<https://github.com/sharethrough/hive-udfs>

### UDF
实现普通函数 (v1, ...) -> (w1, ...)。
需要继承`org.apache.hadoop.hive.ql.exec.UDF`这个类，并实现 `evaluate` 方法。

[UDFTrim 模板](https://issues.apache.org/jira/secure/attachment/12542020/UDFTrim.java)


## MAC 切换不同的JDK
参考：<https://stackoverflow.com/questions/20974607/can-java-7-and-java-8-co-exist-on-osx>

```bash
use-java () {
    export JAVA_HOME=`/usr/libexec/java_home -v 1.$1`
}
```

然后使用 `use-java 7` 就可以切换到 `jdk 1.7` 了。



## ERROR 汇总
- metainfo 超大：  

```
org.apache.hadoop.yarn.exceptions.YarnRuntimeException: java.io.IOException: Split metadata size exceeded 10000000. Aborting job job_1469532484579_647683
```
解决方法：`set mapreduce.jobtracker.split.metainfo.maxsize=-1;`

## 问题
- HIVE 中使用 VIEW 视图！


## TIPS
- HIVE 中上传本地csv文件作为表格的简单方法：利用hive建表命令，创建一个表格，然后将本地csv文件通过 hadoop shell 上传到
表对应的HDF文件夹即可！注意建表的时候要用文本格式，注意分隔符要匹配。

- SQL 将列重命名不要命名为已存在的列的名字！否则将会取存在的列的值，而不是你想要的值！
- group by 和 sort by func(col)，可以是一个函数
- 设置mapreduce阶段的mapper和reducer数目

```bash
# YARN: Hadoop 2
set mapreduce.job.maps=<num>;
set mapreduce.job.reduces=<num>;
```

- 设置每个reducer最大处理数据量 `set hive.exec.reducers.bytes.per.reducer=<number>`
- 用 `distribute by` 控制进入reducer的样本
- 不要用 `count(distinct id)` 因为只能用一个reducer！可以用`sum(1) + group by id`，多一个job但是快很多，因为`group by`可以用多个reducer。
一个数据，约3亿不同的id，第一种用时40分钟，其中reducer耗时35分钟！后一种5分钟！
<https://stackoverflow.com/questions/19311193/why-is-countdistinct-slower-than-group-by-in-hive>

另一种去重的方法

```sql
select sum(if(r=1, 1, 0)) as distinct_num,
    sum(1) as num
from (
    select
        id,
        row_number() over (partition by id) as r
    from tableA
)
```

条件
- `SET hive.exec.parallel=true;` 让不同job并发执行！
- skew join 优化： `set hive.optimize.skewjoin=true;`将一个join操作变为两个，第一个会将同一个key分散到不同的reduce
- skew groupby 优化：`set hive.groupby.skewindata= true ;`
- 输入是否融合小文件：

```sql
set hive.input.format=org.apache.hadoop.hive.ql.io.HiveInputFormat;   --- 不融合
set hive.input.format=org.apache.hadoop.hive.ql.io.CombineHiveInputFormat;  --- 融合
```
