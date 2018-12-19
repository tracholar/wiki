---
title: "Hive 用户自定义函数开发"
layout: page
date: 2018-08-29
---
[TOC]

## 几个重要概念
- `ObjectInspector` 用于对象描述,实际上应该理解为对SQL一个字段属性的抽象?
- `ConstantObjectInspector` 用于描述常数字段的描述,可以通过方法`getWritableConstantValue`获取到常数的值
    - `WritableConstantIntObjectInspector` 整数


## ObjectInspector 常用方法


- 创建基础类型的OI
```java
PrimitiveObjectInspectorFactory.writableDoubleObjectInspector
```
- 创建复杂的OI
```java
ArrayList<ObjectInspector> foi = new ArrayList<ObjectInspector>();

foi.add(PrimitiveObjectInspectorFactory.writableLongObjectInspector);
foi.add(PrimitiveObjectInspectorFactory.writableDoubleObjectInspector);
foi.add(PrimitiveObjectInspectorFactory.writableDoubleObjectInspector);

ArrayList<String> fname = new ArrayList<String>();
fname.add("count");
fname.add("sum");
fname.add("variance");

ObjectInspectorFactory.getStandardStructObjectInspector(fname, foi);
```
- 利用`ObjectInspector`将 `Object` 转换成基础类型
```java
PrimitiveObjectInspectorUtils.getXXX(obj, oi)
```

- 区分 `javaXXXObjectInspector` 与 `writableXXXXObjectInspector`;

很简单 `javaXXXObjectInspector` 对应的是 Java原生的数据类型, 而 `writableXXXXObjectInspector` 对应 Hive 可序列化的数据类型。
以 `javaStringObjectInspector` 和 `writableStringObjectInspector` 为例,前者对应 `String` 而后者对应 `Text`。
因此,如果你的UDF返回的是java原生类型,那么你的输出 ObjectInspector 必须是前者, 否则将会报错!

    
## UDF

## UDAF

## UDTF
