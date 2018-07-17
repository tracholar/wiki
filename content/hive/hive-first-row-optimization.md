---
title: "Hive 提取最大值所对应行的操作优化"
layout: page
date: 2018-07-17
---
[TOC]

## 问题背景
设有表A包含3列,分别是c1, c2, c3;其中c1到c2和c3是1对多的关系,需要在每一个相同c1中,提取c3最大(或最小)的那一行的c2值。
即实现如下操作

```sql
select c1, c2
from(
    select c1, c2, row_number() over (partition by c1 order by c3 desc) as r
    from A
)T
where r=1
```

如果不存在某些c1对应大量的c2,即不存在数据倾斜,上述代码非常容易实现上述逻辑。但是当存在数据倾斜,那么这种写法将导致程序卡死在reduce操作上面。
窗函数在实现的时候会将同一个partition放在一个reduce操作,貌似没有什么优化参数可以设置这种情况的数据倾斜。

## 解决方案
方案1:实现一个通用的UDAF, `select_max(c3, c1)`,选出c3最大的c1的值。

方案2:如果c3是日期或者其他满足字典序关系的字段, 比如日期 yyyy-MM-dd。那么可以用一个简单的trick搞定。

```sql
select c1, split(max(concat(c3,'__', c2)))[1] as c2
from A
group by c1
```

即先将c3作为前缀与c2通过一个特殊分隔符拼接成一个字符串,然后里面字符串求最大值的方法求出!
然后设置`set hive.groupby.skewindata=true`可以有效的解决数据倾斜的问题。

通过这种优化之后,能够有效的解决数据倾斜问题,速度有了很大的提升!