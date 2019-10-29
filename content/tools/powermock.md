---
title: "PowerMock在单元测试中的应用"
layout: page
date: 2019-07-19
---
[TOC]

## 关于
- 参考IBM教程, <https://www.ibm.com/developerworks/cn/java/j-lo-powermock/index.html>

## 配置环境
```xml
<properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <powermock.version>2.0.2</powermock.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.powermock</groupId>
            <artifactId>powermock-module-junit4</artifactId>
            <version>${powermock.version}</version>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>org.powermock</groupId>
            <artifactId>powermock-api-mockito2</artifactId>
            <version>${powermock.version}</version>
            <scope>test</scope>
        </dependency>

        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
            <scope>test</scope>
        </dependency>
    </dependencies>
```

## 应用
- 模拟静态方法: 模拟静态方法返回指定的值
- 模拟构造函数: 模拟构造函数返回的结果
- 模拟私有以及final方法
- `mock` 对象对于未指定处理规则的调用会按照方法返回值类型返回该类型的默认值(int,long返回0, boolean返回false,对象则返回null,void则什么都不做)
- `spy`对象对于未指定处理规则的时候会调用真实方法
- 初始化静态field, `SuppressStaticInitializationFor` 
    - <http://codyaray.com/2012/05/mocking-static-java-util-logger-with-easymocks-powermock-extension>
    - <https://github.com/powermock/powermock/wiki/Mock-Policies>
- 个人DEMO例子参考：<https://github.com/tracholar/ml-homework-cz/tree/master/testing>
