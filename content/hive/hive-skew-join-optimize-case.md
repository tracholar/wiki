---
title: "Hive join 数据倾斜的一个case优化"
layout: page
date: 2018-04-18
---
[TOC]


## 问题
设有一个关于用户信息表A，用户ID字段为uid，存在少量(<1000)uid有大量记录(>100000)，使得表A和其他表以uid为key做JOIN的时候，存在数据倾斜。

```
uid |  f1  | f2
1   |  xx  | yy
1   |  xx  | yy
2   |  xx  | yy
```

## 解决方案

思路一：在WHERE条件中过滤掉那些导致数据倾斜的uid。

可以解决，但是如果这种uid太多，或者每天会随时间变化，那么这种硬编码的方法并不通用。

思路二：利用skewjoin的优化选项

```
set hive.optimize.skewjoin=true;
```

将一个join操作变为两个，第一个会将同一个key分散到不同的reduce，从而解决JOIN时数据倾斜问题。原则上这个方案没问题，但是我实验的时候发现，HIVE没有做这个转化，可能是还有其他参数要设置，没设置对。

思路三：利用 JOIN 过滤掉那些导致倾斜的uid

根据上述思路，写下如下代码

```SQL
select A.* from A
JOIN (
    select uid as guid from A
    group by uid
    having sum(1) < 1000
) B
ON A.uid=B.guid
```

B表用于选择满足条件的uid，然后通过JOIN过滤！问题来了，这个过滤也是JOIN，所以仍然会有数据倾斜的问题！！

由于这样的uid总体上来说比较少，而问题的关键出在不能有JOIN的REDUCE操作，所以可以利用MAPJOIN来解决

```SQL
select A.* from A
LEFT OUTER JOIN (
    select uid as guid from A
    group by uid
    having sum(1) > 1000
) B
ON A.uid=B.guid
WHERE B.guid is null
```

注意这里的B表是一个小表，可以通过MAPJOIN优化使得这个LEFT OUTER JOIN没有reduce操作，实现在map端过滤，完美解决！
