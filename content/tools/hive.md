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
