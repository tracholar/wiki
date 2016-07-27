---
title: "Hive"
layout: page
date: 2016-07-08
---
[TOC]


## 关于
学习Hive时的笔记


## Hive SQL
- `row_number()` 函数用法，`partition`用来将数据分区编号，`order by`描述编号顺序
```sql
select uid, row_number() over (partition by uid order by uid)
from
```
- `datediff(d1, d2)`， 其中时间字符串要是这种格式`yyyy-MM-dd`，如果不是，需要先转换
- `from_unixtime(t, 'yyyyMMdd')`, `unix_timestamp(str, 'yyyy-MM-dd')`这两个函数可以实现时间字符串格式转换

### 优化排序
不要使用`order by` <https://stackoverflow.com/questions/13715044/hive-cluster-by-vs-order-by-vs-sort-by>         
- `ORDER BY` 全局排序，但是只能使用一个reducer
- `DISTRIBUTE BY` 采用Hash算法将map处理后的数据分发给reduce，它保证了相同的key是在同一个reducer
- `SORT BY` 不是全局排序，而是在数据进入reduce之前完成排序，只能保证每个reducer的输出是有序的，不能保证全局有序。
- `CLUSTER BY` 相当于先 DISTRIBUTE 然后 sort。也不能保证全局有序。
