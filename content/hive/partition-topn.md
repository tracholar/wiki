---
title: "分区取TOP N问题"
layout: page
date: 2019-05-10
---
[TOC]

## 问题背景
设想你对用户在不同品类上的行为打分聚合后得到这样一个表 user_cate_score

|uid  |   cate |  score  |
|----|---------|---------|
| 1   |    1   |   0.3  |
| 2   |    2   |   0.5  |
| 8   |   3    |   0.9  |

现在,你想将每个品类的TOP 100W 用户取出来,这种场景在推荐、营销中很常见。

## 基本方法
这个很容易通过窗函数写出如下解法

```sql
select uid, cate, score
from(
    select uid, cate, score, row_number() over (partition by cate order by score desc) as r
    from user_cate_score
)T 
where r <= 1000000
```

通过窗函数`row_number()`在每个cate分区上按照score排序,得到row_number,然后取TOP 100W即可。

问题: HIVE在执行的时候由于每个partition会分到一个reducer上,这导致单个reducer处理的数据量非常大,严重拖慢执行时间,没法充分利用到分布式的效率。

## 改进方法
近似取TOP 100W

```sql
select uid, cate, score
from(
    select uid, cate, score, row_number() over (partition by cate,rnd  order by score desc) as r
    from(
        select *, cast(rand() * 100 as int) as rnd
        from user_cate_score
    )Ta
)Tb
where r <= 10000
```

思路: 加上一个0-100的随机数进行partition,可以将一个cate分散到100个reducer上执行,然后对每个cate的每个reducer取TOP 1W,就近似实现了对每个cate取TOP 100W。好处,速度快了很多。
