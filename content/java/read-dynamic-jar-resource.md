---
title: "读取动态加载的jar包中的资源文件"
layout: page
date: 2019-10-25
---
[TOC]

## 问题描述
java的某些框架设计时，往往会动态加载一些开发者开发的jar包，对系统进行扩展。这会导致在jar包中的代码读取自己包中的资源文件比较困难。

例如系统engine动态加载A.jar，在A.jar中某个类 Module 中正常加载资源文件 `resources/conf.json` 可以这样写

```java
class Module {
    public Module() throws Exception{
        InputStream is = getClass().getResourceAsStream("/conf.json");
        
        // do something

    }
}
```

但是当这个jar包是动态加载的时候，会找不到这个资源文件，因为动态加载的时候会读取框架的资源文件目录，当然找不到用户自己的包中的资源。因此，要在Module类中读取资源文件需要直接操作jar文件来实现。

```java
class Module {
    public Module() throws Exception{
        URL url = getClass().getProtectionDomain().getCodeSource().getLocation();
    
        JarFile file = new JarFile(new File(url.toURI()));
        is = file.getInputStream(file.getEntry("conf.json"));
        
        // do something

    }
}
```

将两种情况都考虑进来，可以通过下述代码实现兼容

```java
class Module {
    public Module() throws Exception{
        URL url = getClass().getProtectionDomain().getCodeSource().getLocation();

        if(url.getFile().endsWith(".jar")) {
            JarFile file = new JarFile(new File(url.toURI()));
            is = file.getInputStream(file.getEntry("conf.json"));
        }else{
            is = getClass().getResourceAsStream("/conf.json");
        }
        
        // do something

    }
}
```