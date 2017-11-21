---
title: "Hive 行列互转"
layout: page
date: 2017-11-21
---
[TOC]

## 行转列
比如表A有三个字段如下

ID, key, value
1,  A,  1
1,  B,  0.4
2,  A,  0.3

需要将同一个ID的多条记录转为一列，显然需要通过聚合操作实现。
对于value是数值类型的，聚合函数可以选为`sum`，对于字符串类型，需要使用聚合函数`collect_list`。

```sql
select ID, sum(if(key='A', value, NULL)) as A_value, sum(if(key='B', value, NULL)) as B_value
group by ID
```

字符串类型

```sql
select ID, concat_ws('', collect_list(if(key='A', value, NULL))) as A_value, concat_ws(collect_list(if(key='B', value, NULL))) as B_value
group by ID
```
