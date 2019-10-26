---
title: "利用反射实现自动配置和加载"
layout: page
date: 2019-10-25
---
[TOC]

## 关于
最近在用Java写一个简单的推荐引擎，实现了一个可配置的引擎[ConfigurableSimpleRecEngine](https://github.com/tracholar/recsys-proj/blob/master
/service/src/main/java/com/tracholar/recommend/engine/ConfigurableSimpleRecEngine.java)。并且，顺便实现了一个从JSON
文件来初始化配置的一个引擎[JsonConfigRecEngine](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com
/tracholar/recommend/engine/JsonConfigRecEngine.java)。有了这个类，就可以通过配置文件，快速实现一个推荐引擎了！配置文件格式参考[engine.json](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/resources/article-rec-sys/engine.json)

```json
{
  "name" : "文章推荐引擎",

  "abtest" : {
    "name" : "abtest",
    "class" : "com.tracholar.articlerecsys.ABTest"
  },

  "detailFetcher" : {
    "name" : "detail fetcher",
    "class":"com.tracholar.articlerecsys.ArticleFetcher"
  },

  "recalls" : [
    {
      "name": "time-recall",
      "class": "com.tracholar.articlerecsys.recall.TimeRecall",
      "abtestKey": {
        "layerKey" : "1",
        "flowKey" : "0,1,2,3,4,5,6,7,8,9"
      }
    }
  ]
  /** 以下省略 */
}
```

使用配置文件的好处是可以统一管理，但是当配置的模块特别多的时候，特指文件会特别长，阅读困难，另外在多人合作开发的时候，仍然存在多人会修改同一个文件的问题，在合并代码的时候，存在一定的风险，导致一个人配好的配置被另外的开发者无意中修改的问题。

那么，有没有一种方案，可以实现不需要配置文件，自动加载呢？答案是肯定的，spring boot 就实现了这样一套方案，方法是通过「注解」Annotation。对java的注解不了解的同学可以参考 [Annotation
](annotation.html)。


## 定义两个注解`@Autoload`和`@ABTestConf`
`@Autoload` 用于标识组建可以自动加载，避免加载不需要的组建。由于要在运行时获取到这个注解，所以需要指定`@Retention(RetentionPolicy.RUNTIME)`，这个注解只注解类型，所以需要标注`@Target({ElementType.TYPE})`。
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE})
public @interface Autoload {
}
```

`@ABTestConf` 用于配置AB测试的key，对应于JSON配置中的`abtest`配置项。它有两个必须参数`layerKey` 和 `flowKey`。

```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.TYPE})
public @interface ABTestConf {
    String layerKey();
    String flowKey();
}
```

## 实现自动加载
有了这两个注解之后，我们还是只能知道哪些类被标注了，但是还需要把这些信息转换成`RecEngineConfig`对象。这需要我们通过反射的方式，获取这些类的配置信息。

首先，让我们引入一个有用的反射库[reflections](https://github.com/ronmamo/reflections)，它可以简化很多操作。
```xml
<dependency>
    <groupId>org.reflections</groupId>
    <artifactId>reflections</artifactId>
    <version>0.9.11</version>
</dependency>
```

为了实现自动加载，第一步，我们可以假设所有的模块都在某个特定的包下面，减少我们搜索类的难度。然后我们需要找到某个包下面的所有类，判断是否标记了`@Autoload`。以加载`Recall`召回组件为例，我们可以通过`.getSubTypesOf`获取所有`Recall`的子类，并通过`.isAnnotationPresent()`判断一个类是否有某个特定的注解，从而筛选出标记了自动加载的`Recall`组件。

另外，我们还需要通过`@ABTestConf`注解来获取ab测试的配置信息，这可以通过 `.getAnnotation` API获取类的注解，从而得到注解的参数。

综合这两步，我们可以实现`Recall`组件的自动加载和自动加载AB测试配置了。

```java
Reflections reflections = new Reflections(packageName);
Set<Class<?>> classes = reflections.getSubTypesOf(Recall.class);

List<ComponentConfig> conf = classes.stream()
                .filter(c -> c.isAnnotationPresent(Autoload.class))
                .map(e -> {
                    ComponentConfig c = new ComponentConfig();
                    c.setClassName(e.getName());
                    c.setName(e.getSimpleName());
                    if(e.isAnnotationPresent(ABTestInfo.class)){
                        ABTestConf info = e.getAnnotation(ABTestConf.class);
                        ABTestKey key = new ABTestKey(info.layerKey(), info.flowKey());
                        c.setAbTestKey(key);
                     }

                    
                    return c;
                }).collect(Collectors.toList());
```

我们可以将这个功能写成一个函数，用于加载多个不同类型的组件。
```java
private List<ComponentConfig> getComponentConfig(Reflections reflections,
                                                     Class cls) {

        Set<Class<?>> classes = reflections.getSubTypesOf(cls);
        List<ComponentConfig> conf = classes.stream()
                .filter(c -> c.isAnnotationPresent(Autoload.class))
                .map(e -> {
                    ComponentConfig c = new ComponentConfig();
                    c.setClassName(e.getName());
                    c.setName(e.getSimpleName());
                    if(e.isAnnotationPresent(ABTestConf.class)){
                        ABTestConf info = e.getAnnotation(ABTestConf.class);
                        ABTestKey key = new ABTestKey(info.layerKey(), info.flowKey());
                        c.setAbTestKey(key);
                    }
                    return c;
                }).collect(Collectors.toList());
        return conf;
    }
```

利用这个函数，可以很快写出自动加载推荐引擎的所有组件，这些代码比较琐碎，就不再叙述了，可以参考[AutoloadRecEngine.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/AutoloadRecEngine.java)的具体实现。
