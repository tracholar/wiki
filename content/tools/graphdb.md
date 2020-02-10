---
title: "图数据库"
layout: page
date: 2020-02-10

---

[TOC]

# 关于
图数据库是用来存储图结构的非关系型的结构化数据的，相比关系型数据库，在检索和图中游走等操作上有更好的优化。例如，查询朋友的朋友，关系数据库需要两次join，如果更多步查询，需要的join操作就会更多。

# JanusGraph
- 官网：<https://janusgraph.org/>
- Linux基金会支持的分布式图数据库
- 跟Apache hadoop的生态结合的比较好，可以使用Spark，hadoop等分析工具进行分析
- 查询语言：<https://docs.janusgraph.org/basics/gremlin/>
```
gremlin> g.V().has('name', 'hercules').out('father').out('father').values('name')
==>saturn
```

```
g: for the current graph traversal.
V: for all vertices in the graph
has('name', 'hercules'): filters the vertices down to those with name property "hercules" (there is only one).
out('father'): traverse outgoing father edge’s from Hercules.
‘out('father')`: traverse outgoing father edge’s from Hercules’ father’s vertex (i.e. Jupiter).
name: get the name property of the "hercules" vertex’s grandfather.
```

Java client的例子
```java
Traversal t = g.V().has("name", "pluto"); // Define a traversal
// Note the traversal is not executed/iterated yet
Vertex pluto = null;
if (t.hasNext()) { // Check if results are available
    pluto = g.V().has("name", "pluto").next(); // Get one result
    g.V(pluto).drop().iterate(); // Execute a traversal to drop pluto from graph
}
// Note the traversal can be cloned for reuse
Traversal tt = t.asAdmin().clone();
if (tt.hasNext()) {
    System.err.println("pluto was not dropped!");
}
List<Vertex> gods = g.V().hasLabel("god").toList(); // Find all the gods
```


- 图存储的分区
    - 边拆分，每个顶点只存一份，边保存多分
    - 顶点拆分，每条边只存一份，一个定点可能分到多个不同机器。（√）
- 数据存储结构：
    - 邻接表存储，在Hbase中，rowkey为顶点id，value包括顶点属性，边集合，rowkey按字典序排序
    - 每个属性或边都是一个cell，按照列排序来加速索引
