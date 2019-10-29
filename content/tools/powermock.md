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


### Mock 外部调用
- 待测试的类 [ClassUnderTest](https://github.com/tracholar/ml-homework-cz/blob/master/testing/tracholar/src/main/java/com/tracholar/testing/ClassUnderTest.java) 

```java
public class ClassUnderTest {
    public void methodToTest(){
        final long id = IdGenerator.generateNewId();
        System.out.println(id);
    }
}
```
这个待测试的类有一个外部调用 `IdGenerator.generateNewId()`，为了测试，我们必须把这个外部调用mock掉，返回我们想要的值才能测试。可以通过 `mockStatic` 实现静态方法的mock，通过`when(IdGenerator.generateNewId()).thenReturn(2L)` 将静态方法返回的值设为给定的值，[代码如下](https://github.com/tracholar/ml-homework-cz/blob/master/testing/tracholar/src/test/java/com/tracholar/testing/TestClassUnderTest.java)
```java
import org.junit.Test;
import org.junit.runner.RunWith;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import static org.powermock.api.mockito.PowerMockito.mockStatic;
import static org.powermock.api.mockito.PowerMockito.verifyStatic;
import static org.powermock.api.mockito.PowerMockito.when;

@RunWith(PowerMockRunner.class)
@PrepareForTest(IdGenerator.class)
public class TestClassUnderTest {
    @Test
    public void demoStaticMethodMocking() throws Exception {
        mockStatic(IdGenerator.class);
        when(IdGenerator.generateNewId()).thenReturn(2L);
        new ClassUnderTest().methodToTest();

    }
}
```

### Mock非静态方法和对象（例如文件IO）
- 假设要测试的方法需要创建一个文件，这是一个非静态方法
```java
import java.io.File;

public class DirectoryStructure {
    public boolean create(String directoryPath) {
        File directory = new File(directoryPath);

        if(directory.exists()){
            throw new IllegalArgumentException(directoryPath + " already exists.");
        }

        return directory.mkdirs();
    }
}
```
- 可以利用API `whenNew(File.class).withArguments(directoryPath).thenReturn(directoryMock)` 实现创建对象的mock

```java
import org.junit.Test;
import org.junit.runner.RunWith;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;

import java.io.File;

import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.mock;
import static org.powermock.api.mockito.PowerMockito.verifyNew;
import static org.powermock.api.mockito.PowerMockito.when;
import static org.powermock.api.mockito.PowerMockito.whenNew;

@RunWith(PowerMockRunner.class)
@PrepareForTest(DirectoryStructure.class)
public class TestDirectoryStructure {
    @Test
    public void createDirectoryWhenNotExists() throws Exception {
        final String directoryPath = "mocked path";
        File directoryMock = mock(File.class);

        whenNew(File.class).withArguments(directoryPath).thenReturn(directoryMock);
        when(directoryMock.exists()).thenReturn(false);
        when(directoryMock.mkdirs()).thenReturn(true);

        assertTrue(new DirectoryStructure().create(directoryPath));
        verifyNew(File.class).withArguments(directoryPath);
    }
}
```
- 代码：
    - <https://github.com/tracholar/ml-homework-cz/blob/master/testing/tracholar/src/main/java/com/tracholar/testing/DirectoryStructure.java>
    - <https://github.com/tracholar/ml-homework-cz/blob/master/testing/tracholar/src/test/java/com/tracholar/testing/TestDirectoryStructure.java>
    
### Mock 私有字段
- 下面的例子有一个私有字段引用了外部对象
```java
public class LocalServiceImpl {
    private ServiceA remoteService;

    public Node getRemoteNode(int num) {
        return remoteService.getRemoteNode(num);
    }
}
```
- 可以通过`Whitebox.setInternalState(obj, field, mockObj);`来实现设置内部私有字段
```java
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.junit.MockitoJUnitRunner;
import org.powermock.reflect.Whitebox;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

/**
 * Created by zuoyuan on 2019/7/19.
 */
@RunWith(MockitoJUnitRunner.class)
public class TestLocalServiceImplMock {
    private LocalServiceImpl localService;

    private ServiceA remoteService;

    @Before
    public void setUp(){
        localService = new LocalServiceImpl();
        remoteService = Mockito.mock(ServiceA.class);
        Whitebox.setInternalState(localService, "remoteService", remoteService);

    }

    @Test
    public void testMock(){
        Node target = new Node(1, "target");
        Mockito.when(remoteService.getRemoteNode(1)).thenReturn(target);
        Node result = localService.getRemoteNode(1);

        assertEquals(target, result);
        assertEquals(1, result.getNum());
        assertEquals("target", result.getName());

        Node result2 = localService.getRemoteNode(2);
        assertNull(result2);
    }
}
```
### Mock 静态field
- 下面的例子有一个静态的字段 `logger`，一般可能是一个远程的logger需要mock，这里以log4j为例
```java
package com.tracholar.testing;

import java.util.logging.Logger;

public class StaticFieldTest {
    private static Logger logger = Logger.getLogger(StaticFieldTest.class.getName());

    public void doSomething(){
        logger.info("Hello");
    }
}
```
- 可以通过`@SuppressStaticInitializationFor`和`@PrepareForTest`api来mock类的静态字段的初始化
```java
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.reflect.Whitebox;
import java.util.logging.Logger;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.tracholar.testing.StaticFieldTest")
@PrepareForTest(StaticFieldTest.class)
public class TestStaticFieldTest {
    @Test
    public void test1(){
        Whitebox.setInternalState(StaticFieldTest.class, "logger", Mockito.mock(Logger.class));
        StaticFieldTest test = new StaticFieldTest();
        test.doSomething();
    }
}
```

## 作业
- 纸上得来终觉浅，绝知此事要躬行，做个作业行不行。启动代码已经帮你写好，放在[getstart](https://github.com/tracholar/ml-homework-cz/tree/master/testing/getstart) 目录中

