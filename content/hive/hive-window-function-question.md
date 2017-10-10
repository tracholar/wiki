---
title: "Hive中窗函数的一个问题"
layout: page
date: 2017-10-10
---
[TOC]

## 问题背景
现有源表A，包含两列，id和score

id  |   score
-------------
1   |   0.3
2   |   0.5
8   |   0.9

A表大约有3亿条记录，现在需要根据score将记录分为10个等级。
于是，很容易用聚合函数 `percentile_approx()` 通过转换为窗函数实现

```sql
select id, array_search(score, splits)
from (
    select id, score, percentile_approx(score, array(0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9)) over () as splits
    from A
)t
```

`array_search` 是自己写的一个UDF，寻找score所在的分段！

## 现象
运行的时候发现，多个reduce最后一个运行特别慢，而且当运行到100%后会卡死，直到大约20分钟结束，
在输出的数据分片中，发现所有的数据都集中到一个分片了。

## 解释
问题在 `over ()` 将聚合函数转换为窗函数上，窗函数会将同一个partition的数据分到一个reduce上，
如果是 `over (partition by xx)`，那么xx字段相同的值会在同一个reduce上，然而这里是将所有的记录当做同一个分片，
因而出现了所有数据集中到一个reduce上面的现象，进而导致写入数据的时候，只有一个reduce在写入3亿条记录。
导致看起来就像卡死在100%这个状态了！

解决办法：不要用窗函数，用join

```sql
select id, array_search(score, splits) from A
join (
    select percentile_approx(score, array(0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9)) as splits
    from A
)t
```

经过改造后，运行时间从之前接近1小时减少到5分钟！
