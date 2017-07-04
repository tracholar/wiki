---
title: "知识图谱"
layout: page
date: 2017-05-07
---
[TOC]

## AAAI tutorial
### 知识图谱简介
- 知识图谱：将知识表达为图的形式
- 获取实体、属性、关系
- 图的顶点代表实体

- 应用：QA，决策
- 工业界产品：
    - Google Knowledge Vault
    - Amazon Product Graph

- 数据来源：结构数据和非结构化数据，图片、视频？
- 知识表达：
    - RDF：<rdf:subject, rdf:predicate, rdf:object> : r(s,p,o)
    - ABox (assertions) versus TBox (terminology)
    - Common ontological primitives
        - rdfs:domain, rdfs:range, rdf:type, rdfs:subClassOf, rdfs:subPropertyOf, ...
        - owl:inverseOf, owl:TransitiveProperty, owl:FunctionalProperty, ...
- 语义网
- 从文本中获取知识的方法
    - chunking
    - polysemy/word sense disambiguation 消歧义
    - entity coreference 实体共止：通过判定两个实体是否存在共指关系？如“IBM”和“IBM Inc.”
    - relational extraction：关系抽取

- 基本问题
    - 实体识别
    - 实体的属性和标签
    - 实体关系

### NLP基础
- Entity resolution
- Entity linking
- Relation extraction
- Coreference Resolution
- Dependency Parsing
- Part of speech tagging
- Named entity recognition

- 抽取方法：
    - 规则：高精度低召回
    - 监督学习

- 定义 domain


论文：Toward an Architecture for Never-Ending Language Learning，


## 跨语言知识图谱构建：李涓子
- 语义搜索 Semantic Search
- RDF ： 资源描述框架 W3C 标准
    - 要素：资源、属性和属性值
    - RDF陈述：主体、谓语和客体
- 语义网：数据用一个 directed labeled graph 描述，每一个顶点对应一个资源，每一个边标注了一个属性类型。
- 语义网描述：RDF，RFDS
- 语义网查询：SOAP
-  
