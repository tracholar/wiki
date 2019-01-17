---
title: "Java 挖坑记录"
layout: page
date: 2019-01-08
---
[TOC]

## 字符串相关

`String.split` 默认会去掉收尾的多余字符
```java
String args = "0.035635684\t\t13001002926\t\t";
System.out.println(args.split("\t").length); //结果是3

System.out.println(args.split("\t", -1).length); //结果是5
```

