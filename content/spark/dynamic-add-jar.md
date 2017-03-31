---
title: "Spark动态加载JAR包的问题"
layout: page
date: 2017-03-31
---
[TOC]

## 问题
在开发Spark程序时，有时候需要动态加载jar包到系统的 classpath 。
例如，file:///xxx.jar 包中存在一个类A的子类AA，A类在当前Spark
程序中存在，但是没有子类AA，

```scala
val jarPath = lines(1)
val myJar = Array(new Path(jarPath).toUri.toURL)

log.info(s"Load jar $jarPath")
val parentLoader = Thread.currentThread().getContextClassLoader
val classLoader = new URLClassLoader(myJar, parentLoader)

val a = classLoader.loadClass("AA").newInstance().asInstanceOf[A]
```

如果AA操作要用到SparkContext，那么将会报错 `java.lang.ClassNotFoundException`。
可以通过将jar包增加到上下文中，解决此问题！

```scala
sc.addJar(jarPath)
```
