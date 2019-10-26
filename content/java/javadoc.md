---
title: "Java文档自动生成"
layout: page
date: 2019-10-25
---
[TOC]

## 关于
文档自动生成 <https://docs.oracle.com/javase/7/docs/technotes/tools/windows/javadoc.html>

## 命令
```shell script
javadoc [ options ] [ packagenames ] [ sourcefilenames ] [ -subpackages pkg1:pkg2:... ] [ @argfiles ]
```

## IDEA中自动生成工具
Tool -> Generate JavaDoc

## 包的说明文件
package-info.java

```java
/**
 * Provides the classes necessary to create an  
 * applet and the classes an applet uses 
 * to communicate with its applet context.
 * <p>
 * The applet framework involves two entities:
 * the applet and the applet context.
 * An applet is an embeddable window (see the
 * {@link java.awt.Panel} class) with a few extra
 * methods that the applet context can use to 
 * initialize, start, and stop the applet.
 *
 * @since 1.0
 * @see java.awt
 */
package java.lang.applet;
```

## 几个重要的语法
- `@link：{@link 包名.类名#方法名(参数类型)}` 快速链接到对应代码
- `@code： {@code text}` 标记该文本是一段代码
- `@param` 参数描述
- `@author` 作者
- `@see` 另见，跳转到其他地方
- `@since` 从哪个版本开始
- `@version` 版本号
- `@return` 返回值

另注： 支持所有的HTML标签



