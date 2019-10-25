---
title: "Java 注解"
layout: page
date: 2019-10-20
---
[TOC]

## 关于
注解是写给编译器看的，在编译时处理，不会影响运行时？

利用注解的好处，将配置和代码放到一起，避免分离！代码编写也方便了！

## 系统内建Annotation
- `@Override` 对方法进行覆盖，如果没有被正确地覆盖，编译器就会报错！
- `@Deprecated` 建议不再使用，编译器会报警
- `@SuppressWarnings` 不要告警！

## 自定义Annotation
```java
public @interface AnnotationName{
    public String key() default "key";
    public String value() default "value";
} 
```

- retention 和 retentionpolicy

- 以下是FastJSON中 `@JSONField` 的注解

```java
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD, ElementType.FIELD, ElementType.PARAMETER})
public @interface JSONField {
    // 以下省略
}
```

- `@Retention(RetentionPolicy.RUNTIME)` 表示保留到什么时候？取值有
    - SOURCE 只保留在源文件中
    - CLASS 保留到类文件中，但不加载到JVM中
    - RUNTIME 一直保留到执行的时候，可以通过反射读取
- `@Target` 表示可以添加注解的位置，包括
```
/** Class, interface (including annotation type), or enum declaration */
TYPE,

/** Field declaration (includes enum constants) */
FIELD,

/** Method declaration */
METHOD,

/** Formal parameter declaration */
PARAMETER,

/** Constructor declaration */
CONSTRUCTOR,

/** Local variable declaration */
LOCAL_VARIABLE,

/** Annotation type declaration */
ANNOTATION_TYPE,

/** Package declaration */
PACKAGE,

/**
 * Type parameter declaration
 *
 * @since 1.8
 */
TYPE_PARAMETER,

/**
 * Use of a type
 *
 * @since 1.8
 */
TYPE_USE
```
- `@Inherited` 表示父类的Annotation可以被子类继承



## 注解的类型
