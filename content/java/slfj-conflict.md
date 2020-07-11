---
title: "log4j冲突问题排查"
layout: page
date: 2020-05-14
---
[TOC]

## 问题
```bash
SLF4J: Class path contains multiple SLF4J bindings.
SLF4J: Found binding in [jar:file:/Users/zuoyuan/.m2/repository/org/slf4j/slf4j-log4j12/1.7.16/slf4j-log4j12-1.7.16.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: Found binding in [jar:file:/Users/zuoyuan/.m2/repository/org/apache/logging/log4j/log4j-slf4j-impl/2.3/log4j-slf4j-impl-2.3.jar!/org/slf4j/impl/StaticLoggerBinder.class]
SLF4J: See http://www.slf4j.org/codes.html#multiple_bindings for an explanation.
SLF4J: Detected both log4j-over-slf4j.jar AND bound slf4j-log4j12.jar on the class path, preempting StackOverflowError. 

SLF4J: See also http://www.slf4j.org/codes.html#log4jDelegationLoop for more details.
```

## 原因

### slf4j 与 log4j 的关系
- log4j 是一个日志框架，是实际执行落日志的角色；而sl4j是一种适配接口，用于支持不同的底层日志框架。
- slf4j 的实现者有： 
    - slf4j-simple
    - logback
    - slf4j-log4j12：使用`log4j`库作为底层日志实现，需要引入的库
- 参考 [Java日志框架：slf4j作用及其实现原理](https://www.cnblogs.com/xrq730/p/8619156.html)

- log4j-over-slf4j 是一个桥接器，是为了使用log4j实现的老代码，在不需要修改一行代码的情况下迁移到slf4j。实现原理是通过将文件`log4j.jar`替换为 `log4j-over-slf4j.jar` 来实现的。参考[log4j-over-slf4j](http://www.slf4j.org/legacy.html)

### 报错的具体原因
- log4j-over-slf4j 与 slf4j 互相调用，陷入了死循环，导致StackOverflow。


## 解法
- 找到 log4j-over-slf4j 是哪个依赖引入的，然后排除掉即可。
- 找到依赖
```bash
mvn dependency:tree
```
- 排除
```xml
<exclusion>
    <groupId>org.slf4j</groupId>
    <artifactId>log4j-over-slf4j</artifactId>
</exclusion>
```

 