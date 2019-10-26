---
title: "推荐引擎设计"
layout: page
date: 2019-10-26
---
[TOC]

## 关于
本文通过自顶向下设计，实现一个可扩展的推荐引擎。主要特性：

- 实现一个基于JSON配置的推荐引擎，和自动加载配置的引擎
- 各组件可自由定制
- 新增功能，不需要修改原有代码，只需要扩展现有的组件类型

## 系统设计
### 设计思想
采用自顶向下抽象的方法，设计整个推荐系统。每一层抽象只实现该层次公共的代码逻辑，而将可变的部分交给子类型来实现。

### 推荐引擎接口及其基础数据类型的定义
在设计推荐引擎接口时，首先我们要深刻理解「什么是推荐引擎」？有人说，推荐引擎要实现召回、排序，那么，问题来了，没有召回和排序就不是推荐引擎了吗？举个例子，我调用一个外部一个服务来获取推荐结果，一次搞定，没有召回也没有排序，那么它不是一个推荐引擎吗？显然也是。所以召回、排序不是推荐引擎的必须特性。我认为，在最初的抽象层次，推荐引擎有且只有一个接口，就是传入用户，上下文信息，返回推荐的item列表！因此，推荐引擎接口应该是
```java
import com.tracholar.recommend.data.IContext;
import com.tracholar.recommend.data.IItem;
import com.tracholar.recommend.data.IUser;

import java.util.List;

/**
 * @author: tracholar.github.io
 * 从外部来看，推荐引擎就是传入1个用户和一个上下文信息，你给我返回一个item的推荐列表。
 */
public interface RecEngine<U extends IUser, I extends IItem, C extends IContext> {
    List<I> recommend(U user, C ctx);
}
```

要理解这一点，有一个很简单的视角，就是从引擎的调用者来看，它并不需要知道你这个引擎如何实现，只要知道要传给你什么（用户，上下文），然后可以从你这获得什么（推荐的item列表）！至于如何实现这个引擎，你到底是外包给一个外部服务，还是自己实现两阶段推荐（召回、排序），甚至更多阶段的推荐流程（例如可以再细化为：触发、检索、粗排、精排、重排机制），那应该是下一层抽象该做的事情。

这个引擎同时定义了3个基础数据类型 `IUser` `IItem` `IContext`。同样，我们可以问一句，这三个数据类型有什么是必须的吗？看起来除了ID之外，其他都不见的是必须的，因为只要有ID，就可以对每个实际的user或者item有了标识，因此只有ID是必须的。

- `HasId` 接口定义
```java
public interface HasId<T> {
    T getId();
}
```

- `IUser` 接口定义
```java
public interface IUser<ID> extends HasId<ID> {
}
```

- `IItem` 接口定义
```java
public interface IItem<ID> extends HasId<ID> {
}
```
- `IContext` 接口定义
```java
public interface IContext<ID> extends HasId<ID> {
}
```

### 实现一个6阶段推荐引擎
有了第一层抽象之后，我们只知道对使用引擎的人来说，该如何使用这个引擎。但是对实现引擎的人来说，有一些共同的东西可以抽象，这就是第二层的抽象。这一层我们可以抽象出几种不同的推荐模式。比如一阶段推荐（查外部接口、或者简单的策略推荐）、两阶段推荐（按照主流的推荐模式拆分为召回、排序）等等。这里我已一个比较主流的6阶段推荐模式为例，实现一个`SimpleRecEngine`。其他类型的推荐范式可以参考这个例子对`RecEngine`进行扩展。

所谓6阶段推荐是指：召回、合并、过滤、排序、重排、取详情 这六个阶段。在这一层次的抽象中，我们该实现哪些不该实现哪些以保证可扩展性呢？跟前面一样，我们要对6阶段推荐流程有深刻的理解，即问一句「六阶段推荐引擎哪些是必须要有的」，但还得多问一句「所有的六阶段推荐引擎，哪些逻辑是相同的」？

对第一个问题，都有的就是这六个阶段所用到的模块，我们称之为组件。其次，我们还得有AB测试模块，因为标准的推荐引擎需要通过试验的方式进行迭代。为什么AB测试模块要放到这一层呢？因为我们假定所有的6阶段推荐引擎都会有AB测试，但如果你硬要抬杠说，我就要一个不需要AB测试的6阶段推荐引擎，那我只能说「万万没想到」！这个设计就取决于一种平衡，如果现实中确实有这种情况，那确实不应该不把AB测试这个模块放到这一层，但实际上这种情况几乎不可能，所以没必要过度设计。是否抽象层次过多和抽象层次过少，就需要设计系统者对该业务有深入理解才能把握！这里，我就拍个板了，现实中没有不需要AB测试的六阶段推荐引擎，所以我就不考虑了。

对第二个问题，相同的逻辑应该有：

1. 通过AB测试模块判断当前推荐中命中了哪些模块？例如，通常有很多排序组件，分别对应不同的排序策略，但一次推荐中，应该只会命中一个一个排序策略。所以需要AB测试模块来判断。
2. 将这六个阶段串联起来，实现推荐。显然，我们可以假定这六阶段都是按照固定的流程串联起来的。如果有其他串联方式，可以从`RecEngine`再派生一种子推荐类型即可。

在这一层，由于我们需要实现特定的逻辑，所以不能再用java的`interface`了，接口是不能有实现逻辑的，因此应该用抽象类来实现。

```java
import com.tracholar.recommend.data.*;
import com.tracholar.recommend.abtest.ABTestKey;
import com.tracholar.recommend.abtest.ABTestProxy;
import com.tracholar.recommend.abtest.ABTestable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author tracholar.github.io
 * SimpleRecEngin 抽象了整个推荐系统的流水线，将各个模块（如召回、排序、AB测试等）组装起来。
 * 但是它并不知道如何获取这些模块，而将这些模块的获取留给子类实现。
 */
public abstract class SimpleRecEngine implements RecEngine {
    abstract protected List<Recall> getRecalls();
    abstract protected List<Merge> getMerges();
    abstract protected List<Filter> getFilters();
    abstract protected List<Ranker> getRankers();
    abstract protected List<ReRanker> getReRankers();

    abstract protected ABTestProxy getAbTestProxy();
    abstract protected DetailFetcher getDetailFetcher();


    private <T> List<T> filterByABTest(IUser user, IContext ctx, List<T> arr){
        List<T> strategies = new ArrayList<>();
        for(T s : arr){
            if(s instanceof ABTestable){
                ABTestKey key = ((ABTestable) s).getAbTestKey();
                if(! getAbTestProxy().match(user, ctx, key)){
                    continue;
                }

            }
            strategies.add(s);
        }
        return strategies;
    }
    private <T> T getByABTest(IUser user, IContext ctx, List<T> arr)
            throws ComponentNotFoundException{
        List<T> filtered = filterByABTest(user, ctx, arr);

        if(filtered.size() == 0){
            throw new ComponentNotFoundException();
        }
        return filtered.get(0);
    }


    private List<RecallResult> doRecall(IUser user, IContext ctx)
        throws ComponentNotFoundException{
        Map<Recall, List<RecallResult>> results = new HashMap<>();
        for(Recall strategy : filterByABTest(user, ctx, getRecalls())){
            try {
                List<RecallResult> res = strategy.recall(user, ctx);
                if (res == null) {
                    continue;
                }
                results.put(strategy, res);
            }catch (Exception e){
            }
        }

        Merge merge = getByABTest(user, ctx, getMerges());
        return merge.merge(results);
    }

    private List<RecallResult> doFilter(IUser user, List<RecallResult> results, IContext ctx){
        for(Filter f : filterByABTest(user, ctx, getFilters())){
            int begine = results.size();

            results = f.filter(user, results, ctx);

        }
        return results;
    }

    @Override
    public List<IItem> recommend(IUser user, IContext ctx){
        try {
            // recall
            List<RecallResult> results = doRecall(user, ctx);

            // filter
            results = doFilter(user, results, ctx);

            // rank
            Ranker ranker = getByABTest(user, ctx, getRankers());
            List<RankResult> rankResults = ranker.rank(user, results, ctx);

            // re-rank
            rankResults = getByABTest(user, ctx, getReRankers())
                    .reRank(user, rankResults, ctx);

            // fetch details
            return getDetailFetcher().fetch(rankResults);
        }catch (ComponentNotFoundException e){
        }
        return null;
    }

}
```

为简单期间，这里的代码略去了一些必要的日志记录，原始版本可以参考[SimpleRecEngine.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/SimpleRecEngine.java)。

这个抽象类实现了推荐引擎的接口`recommend`，将6个阶段串联了起来，其中`doRecall`将多路召回和融合组装到一起，`doFilter`将多个filter串联执行，便于`recommend`的逻辑清晰。`filterByABTest`和`getByABTest`实现通过AB测试模块对注册的组件进行过滤，筛选出匹配的组件。

在这个层次，并不知道如何获得这些组件，所以留出了几个未实现的获取对应组件接口，让下一层来实现。

在这一层，我们新引入了几个组件的抽象

1. AB测试组件：`ABTestProxy`，它有一个必须的方法`boolean match(IUser user, IContext ctx, ABTestKey key)`，即判断用户在当前请求下是否命中AB测试的key。具体实现参考[ABTestProxy.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/abtest/ABTestProxy.java)
    - 这个组件派生出一个新的接口`ABTestable`，如果组件实现了这个接口，那么说明支持AB测试，否则就不支持AB测试，需要全部命中！接口定义参考[ABTestable.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/abtest/ABTestable.java)
    - 这个组件同样需要一个`ABTestKey`对象，通常来说分层AB测试是一个共识，所以这部分也没有继续抽象，直接定义这个对象有两个属性，layerKey和flowKey。
2. 召回组件：`Recall`，它必须有一个方法`List<R> recall(U user, C ctx)` 即输入user跟ctx，输出`RecallResult`。这里三个类型参数分别是（下同）：U代表`IUser`接口的子类，C代表`IContext`的子类，`R`是`RecallResult`的子类。定义参考[Recall.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/Recall.java)
3. 合并组件：`Merge`，它必须有一个方法`List<R> merge(Map<Recall, List<R>> results)`即将多个召回的结果，合并到一个召回结果列表中！定义参考[Merge.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/Merge.java)
4. 过滤组件：`Filter`，它必须有一个方法`List<R> filter(U user, List<R> results, C ctx)` 即对合并的结果进行过滤。过滤掉某些不合适的召回结果，可能是一些人工策略和业务需要！定义参考[Filter.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/Filter.java)
5. 排序组件：`Ranker`，它必须有一个方法`List<RR> rank(IUser user, List<R> results, IContext ctx)`，对过滤后的召回列表进行排序，得到排序结果`RankerResult`，RR是泛型参数，是`RankerResult`的子类。定义参考[Ranker.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/Ranker.java) [RankResult](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/data/RankResult.java)
6. 重排组件：`ReRanker`，它必须有一个方法`List<R> reRank(IUser user, List<R> results, IContext ctx)`将排序的结果重新排序，主要是为了满足一些策略上的要求，一般在排序阶段只会对每个item计算匹配得分，但不太会考虑多样性和业务特定规则以及EE的一些问题，所以这一层主要是为了干这些事情。输入输出都是`RankResult` 本质上它也是排序，但是输入不一样，`Ranker`输入是召回结果列表，而`ReRanker`输入是排序结果列表！定义参考[ReRanker.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/ReRanker.java) 
7. 取详情组件：`DetailFetcher`，它有一个方法`List<I> fetch(List<ID> arr)`。输入是一个有ID的记录列表，这里输入是`RankResult`，输出是有详情的结果`IItem`。但是在涉及这个接口时，做了一些扩展，把泛型参数上限都设为`HasId`，便于用到其他地方，比如取user的详情。定义参考[DetailFetcher.java](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/DetailFetcher.java)

### 可配置的6阶段推荐引擎
前一节我们实现了一个6阶段推荐引擎，但是并不知道这个引擎所需要的那些组件怎么创建和获取。而这一层，我们就来解决这个问题。这些组件可以通过代码写死来创建，也可以通过更灵活的配置方式来创建。因此，不同的创建方式可以得到不同的子类型。这一层我只实现配置的方式来创建组件的引擎 `ConfigurableSimpleRecEngine`。

那个问题又来了，对于可配置的引擎来说，什么是必须有的呢？首先，它要实现上一层次要求它实现的接口，即获取组件的接口。其次，它要从配置来创建这些组件！考虑到配置可以是不同的，比如可以来自MySQL数据库，也可以来自配置文件，甚至可以像后面要说的通过java注解的方式实现的配置！因此，这一部分不应该设计到具体的配置方式，所以可以定一个配置结构[RecEngineConfig](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/config/RecEngineConfig.java)。

总结起来，这一阶段要干的事有：从配置对象初始化各组件，实现父类获取组件的接口。不该干的事情是：从某种配置类型（文件、数据库等）创建配置对象。从而实现配置变化带来的重构问题，屏蔽配置类型的变化！

```java
import com.tracholar.recommend.abtest.ABTestKey;
import com.tracholar.recommend.abtest.ABTestProxy;
import com.tracholar.recommend.abtest.ABTestable;
import com.tracholar.recommend.engine.config.ComponentConfig;
import com.tracholar.recommend.engine.config.Configable;
import com.tracholar.recommend.engine.config.RecEngineConfig;
import lombok.AccessLevel;
import lombok.Getter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.List;

/**
 * @author tracholar.github.io
 *
 * <p>
 * ConfigurableSimpleRecEngine 抽象了通过配置生成推荐引擎所需要的模块。
 * 已经是一个独立的推荐引擎了！ 使用示例
 *
 * <pre>{@code
 * ConfigurableSimpleRecEngine engine = new ConfigurableSimpleRecEngine()
 * // 构造一个 RecEngineConfig 对象
 * RecEngineConfig config = ...;
 * engine.init(config);
 * }
 * </pre>
 *
 * {@code RecEngineConfig} 可以参看 {@link com.tracholar.recommend.engine.config.RecEngineConfig}
 *
 *
 *
 */
@Getter(AccessLevel.PROTECTED)
public class ConfigurableSimpleRecEngine extends SimpleRecEngine {
    @Getter
    private String name;
    private ABTestProxy abTestProxy;
    private DetailFetcher detailFetcher;
    private List<Recall> recalls = new ArrayList<>();
    private List<Merge> merges = new ArrayList<>();
    private List<Filter> filters = new ArrayList<>();
    private List<Ranker> rankers = new ArrayList<>();
    private List<ReRanker> reRankers = new ArrayList<>();

    public void init(RecEngineConfig conf) throws EngineInitialException{
        try {
            //通过配置文件构造一个推荐引擎
            name = conf.getName();

            //abtest
            abTestProxy = (ABTestProxy) Class.forName(conf.getAbtest().getClassName()).newInstance();
            if(abTestProxy instanceof Configable) {
                ((Configable) abTestProxy).init(conf.getAbtest().getArgs());
            }
            detailFetcher = (DetailFetcher) Class.forName(conf.getDetailFetcher().getClassName()).newInstance();
            if(detailFetcher instanceof Configable) {
                ((Configable) detailFetcher).init(conf.getDetailFetcher().getArgs());
            }

            //recall
            loadComponents(recalls, conf.getRecalls());

            //filters
            loadComponents(filters, conf.getFilters());

            //merges
            loadComponents(merges, conf.getMerges());

            //rankers
            loadComponents(rankers, conf.getRankers());

            //re-rankers
            loadComponents(reRankers, conf.getReRankers());
        }catch (Exception e){
            throw new EngineInitialException(e);
        }
    }

    private <T> void loadComponents(List<T> arr, List<ComponentConfig> compConfs)
            throws Exception{
        for(int i=0; i<compConfs.size(); i++) {
            ComponentConfig c = compConfs.get(i);
            T strategy = (T) Class.forName(c.getClassName()).newInstance();

            if(strategy instanceof ABTestable && c.getAbTestKey() != null) {
                ABTestKey key = c.getAbTestKey();
                ((ABTestable) strategy).setAbTestKey(key);
            }
            if(strategy instanceof Configable) {
                ((Configable) strategy).init(c.getArgs());
            }
            arr.add(strategy);
        }
    }
}
```

这个类的核心代码是两个函数`init` 和`loadComponents`，前者是从配置对象初始化的入口，要创建一个引擎最简单的方式是，构造一个`RecEngineConfig`，然后传给`init`方法，就搞定了！`loadComponents`是从组件的配置创建组件的函数，在这个函数中，会通过反射`Class.forName`创建类的实例对象，并判断组件是否是`ABTestable`，和是否是`Configable`，从而进行必要的设置！

另外，这里使用了`lombok`这个库，通过`@Getter`注解实现了get方法，从而实现了父类要求实现的获取组件的方法！

### 两种配置方式构造的引擎
这一节我们介绍实现的两种配置方式，一种是JSON配置文件，另一种是自动加载注解的方式。上一节已经可以从配置初始化组件了，但是不知道配置怎么来的，不同的配置方法实现不同的配置类型推荐引擎。这就是在这一层我们要干的事情。

#### JSON配置
JSON配置比较简单，直接将`RecEngineConfig` JSON序列化就行，然后修改对应的配置项。反序列化也简单，利用FastJSON `JSON.parseObject(is, RecEngineConfig.class)`就能实现。

```java
public class JsonConfigRecEngine extends ConfigurableSimpleRecEngine {
    private RecEngineConfig config;

    private JsonConfigRecEngine(){};

    public JsonConfigRecEngine(RecEngineConfig config) throws EngineInitialException{
        super.init(config);
    }

    public static JsonConfigRecEngine load(InputStream is) throws Exception {
        RecEngineConfig config = JSON.parseObject(is, RecEngineConfig.class);
        return new JsonConfigRecEngine(config);
    }
    public static JsonConfigRecEngine load(String json) throws Exception {
        RecEngineConfig config = JSON.parseObject(json, RecEngineConfig.class);
        return new JsonConfigRecEngine(config);
    }
}
```

#### 自动加载
自动加载的原理是java的反射，通过`@Autoload`注解判断组件是否需要加载，通过`@ABtestConf`获取AB测试的配置信息。具体原理解析参考[利用反射实现自动配置和加载](autoload.html)，代码参考[AutoloadRecEngine](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/engine/AutoloadRecEngine.java)。



## 教程与演示案例
### 演示案例
在[com.tracholar.recommend.demo](https://github.com/tracholar/recsys-proj/tree/master/service/src/main/java/com/tracholar/recommend/demo)包中，实现了一个个简单的推荐系统，JSON配置版的[DemoSimpleRecEngine](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/demo/DemoSimpleRecEngine.java)，自动加载版的[DemoAutoloadRecEngine](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/recommend/demo/DemoAutoloadRecEngine.java)。各类组件都分别实现了至少一个实例。下面是一个随机排序的Ranker组件

```java
package com.tracholar.recommend.demo;

import com.tracholar.recommend.abtest.ABTestInfo;
import com.tracholar.recommend.data.IContext;
import com.tracholar.recommend.data.IUser;
import com.tracholar.recommend.data.RankResult;
import com.tracholar.recommend.engine.Ranker;
import com.tracholar.recommend.data.RecallResult;
import com.tracholar.recommend.engine.config.Autoload;
import com.tracholar.recommend.model.SimpleScore;

import java.util.ArrayList;
import java.util.List;

@Autoload
@ABTestInfo(layerKey = "1", flowKey = "234")
public class DemoRanker implements Ranker<RecallResult, RankResult> {
    @Override
    public List<RankResult> rank(IUser user, List<RecallResult> results, IContext ctx){
        List<RankResult> rs = new ArrayList<>();
        for(int i=0; i<results.size(); i++){
            rs.add(new DemoRankResult((String) results.get(i).getId(), new SimpleScore(0.1f), i));
        }

        return rs;
    }
}
```

### 一个简单的文章推荐系统
在`com.tracholar.articlerecsys`包中实现了一个简单的文章推荐系统[ArticleRecEngine](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/articlerecsys/ArticleRecEngine.java)，文章的数据来自之前实现的一个推荐系统，通过爬虫爬取到文章，加工后存入数据库中。同时，还是先了一个基于spring boot的推荐系统web api，参考类[ArticleRecEngineWebAPI](https://github.com/tracholar/recsys-proj/blob/master/service/src/main/java/com/tracholar/articlerecsys/ArticleRecEngineWebAPI.java)。

### 自己动手实现一个推荐系统
在`com.tracholar.tutorial`包中实现了一个组件基本框架，但是没有实现代码，可以作为启动代码，自己动手实践一个推荐系统。可以选择以下两个任务中的一个作为实践。

#### 作业一：实现一个新闻推荐系统
- 新闻需要有：标题，作者，发布时间，页面的URL等信息
- 需要过滤掉用户已经看过的新闻列表


#### 作业二：实现一个图片推荐系统
- 图片需要有：URL，尺寸，大小，标签等信息
