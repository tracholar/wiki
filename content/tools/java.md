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


## 并发相关
- `synchronized` 和 `Lock` 来保证原子性, `synchronized` 修饰的代码,能够保证任一时刻只有一个线程执行该代码块。
- `@volatile` 可见性。当一个共享变量被volatile修饰时，它会保证修改的值会立即被更新到主存，当有其他线程需要读取时，它会去内存中读取新值。
- 优质博客 <https://www.cnblogs.com/dolphin0520/p/3920373.html>


## lombok 库
可以通过注解的方法，实现`get`, `set`, 构造函数等JAVA中常用的功能。举例

```java
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
@AllArgsConstructor
public class User {
    private String uuid;
    private Long userId;
}

```
这样就可以直接使用get，set和构造函数

```java
public class SomeClass {
    public void someFunc(){
        User u1 = new User("uuid", 12345);
        User u2 = new User();
    }
}

```


