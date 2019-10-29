---
title: "反射"
layout: page
date: 2019-10-25
---
[TOC]

## 有用的库
```xml
<dependency>
    <groupId>org.reflections</groupId>
    <artifactId>reflections</artifactId>
    <version>0.9.11</version>
</dependency>
```
- <https://github.com/ronmamo/reflections>

## 类加载
### 基本加载
`Class.forName(String name)`

### 从jar包加载类
```java
URLClassLoader classLoader = new URLClassLoader(new URL[]{
        new URL("")
});
Class cls = classLoader.loadClass("com.tracholar.tutorial.ArticleRecEngine");
cls.newInstance();
```

### 加载包下所有的类
```java
Reflections reflections = new Reflections("my.project");

Set<Class<? extends SomeType>> subTypes = reflections.getSubTypesOf(SomeType.class);

Set<Class<?>> annotated = reflections.getTypesAnnotatedWith(SomeAnnotation.class);
```



### 动态代理
<https://www.cnblogs.com/haitaofeiyang/p/7724263.html>