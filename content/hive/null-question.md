---
title: "NULL 比较的问题"
layout: page
date: 2019-12-26
---
[TOC]

## 问题
```sql 
where field != 1
```

上述代码在field为NULL的时候，在不同的执行引擎结果不一样，Spark引擎为false