---
title: "Hive full join 的优化"
layout: page
date: 2017-11-21
---
[TOC]

## 问题背景
有多个表A,B,...,Z，主key是用户ID，需要full join 到一个表中。

```sql
select coalesce(A.ID, B.ID, C.ID) as ID, A.col1, B.col2, C.col3
from A
full outer join B on A.ID=B.ID
full outer join C on coalesce(A.ID, B.ID)=C.ID
```

如果有很多个表，由于上述多个连接操作的key中并没有一个固定的key，所以HIVE无法优化到一个MR，只能顺序的join，导致速度较慢。

## 优化方案
可以通过 UNION ALL 优化

```sql
select ID, sum(col1) as col1, sum(col2) as col2, sum(col3) as col3
from (
    select ID, col1, NULL as col2, NULL as col3 from A

    UNION ALL

    select ID, NULL as col1, col2, NULL as col3 from B

    UNION ALL

    select ID, NULL as col1, NULL as col2, col3 from C
)T
group by ID
```

对于数值类型，用 sum 聚合， 如果是字符串类型，可以用 concat_ws('', collect_list(col)) 聚合。

在一个任务上实测优化前需要20min，优化后减少到10min！
