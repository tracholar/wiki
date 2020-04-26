---
title: "Java8的Function API"
layout: page
date: 2020-04-01
---

[TOC]

## 函数式接口
函数式接口(Functional Interface)就是一个有且仅有一个抽象方法，但是可以有多个非抽象方法的接口。

函数式接口可以被隐式转换为 lambda 表达式。

Lambda 表达式和方法引用（实际上也可认为是Lambda表达式）上。

如定义了一个函数式接口如下：

```java
@FunctionalInterface
interface GreetingService 
{
    void sayMessage(String message);
}
```


函数式接口可以对现有的函数友好地支持 lambda。

JDK 1.8 之前已有的函数式接口:

```java
java.lang.Runnable
java.util.concurrent.Callable
java.security.PrivilegedAction
java.util.Comparator
java.io.FileFilter
java.nio.file.PathMatcher
java.lang.reflect.InvocationHandler
java.beans.PropertyChangeListener
java.awt.event.ActionListener
javax.swing.event.ChangeListener
```

JDK 1.8 新增加的函数接口：

<https://www.runoob.com/java/java8-functional-interfaces.html>


