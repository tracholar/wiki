---
title: "xgboost"
layout: page
date: 2016-07-23
---
[TOC]

## 关于
xgboost 据说是现在大数据竞赛冠军队的标配！


## xgboost 论文导读
三个关键点
- large-scale
- sparsity-aware algorithm for sparse data
- weighted quantile sketch for approximate tree learning

### introduction
- 机器学习和data-driven方法的成功依赖于两方面的发展：1，有效的（统计）模型 2，scalable 算法
- tree boosting 算法
    -  P. Li. Robust Logitboost and adaptive base class (ABC)
Logitboost. In Proceedings of the Twenty-Sixth Conference
Annual Conference on Uncertainty in Artificial Intelligence
(UAI’10), pages 302–311, 2010.
    - C. Burges. From ranknet to lambdarank to lambdamart:
An overview. Learning, 11:23–581, 2010.
    - X. He, J. Pan, O. Jin, T. Xu, B. Liu, T. Xu, Y. Shi,
A. Atallah, R. Herbrich, S. Bowers, and J. Q. n. Candela.
Practical lessons from predicting clicks on ads at facebook.
In Proceedings of the Eighth International Workshop on
Data Mining for Online Advertising, ADKDD’14, 2014.
    - **Netflix price** J. Bennett and S. Lanning. The netflix prize. In
Proceedings of the KDD Cup Workshop 2007, pages 3–6,
New York, Aug. 2007.
- Kaggle 2015 年29个比赛中，TOP3队伍中有17个用了XGBoost，其中8个用XGBoost直接预测，而另外的用XGBoost和神经网络进行集成。而DNN居然只有11个！
- KDDCup 2015 TOP10 队伍都用了XGBoost！！
- 各种比赛中已解决的任务包括： store sales
prediction; high energy physics event classification; web text
classification; customer behavior prediction; motion detection;
ad click through rate prediction; malware classification;
product categorization; hazard risk prediction; massive online
course dropout rate prediction.
- XGBoost 创新点在于：
    - a novel tree learning algorithm is for handling sparse data;
    - a theoretically justified weighted
quantile sketch procedure enables handling instance weights
in approximate tree learning
    - Parallel and distributed computing
makes learning faster which enables quicker model exploration
    - 还可以使用out-of-core计算，使得在单机就可以处理上亿（hundred million）样本

- 现有的并行的 tree boosting 算法有：
    - B. Panda, J. S. Herbach, S. Basu, and R. J. Bayardo.
Planet: Massively parallel learning of tree ensembles with
mapreduce. Proceeding of VLDB Endowment,
2(2):1426–1437, Aug. 2009.
    - S. Tyree, K. Weinberger, K. Agrawal, and J. Paykin.
Parallel boosted regression trees for web search ranking. In
Proceedings of the 20th international conference on World
wide web, pages 387–396. ACM, 2011.
    - J. Ye, J.-H. Chow, J. Chen, and Z. Zheng. Stochastic
gradient boosted distributed decision trees. In Proceedings
of the 18th ACM Conference on Information and
Knowledge Management, CIKM ’09.

尚未解决的问题是：out-of-core computation,
cache-aware and sparsity-aware learning
- 大神解决的几个方案，后面再膜拜
    - T. Chen, H. Li, Q. Yang, and Y. Yu. General functional
    matrix factorization using gradient boosting. In Proceeding
    of 30th International Conference on Machine Learning
    (ICML’13), volume 1, pages 436–444, 2013.
    - T. Chen, S. Singh, B. Taskar, and C. Guestrin. Efficient
    second-order gradient boosting for conditional random
    fields. In Proceeding of 18th Artificial Intelligence and
    Statistics Conference (AISTATS’15), volume 1, 2015.

### Tree bossting in a nutsell
- 正则化目标函数

回归树的数学表示如下，q是一个将特征向量x映射到树的叶子节点，T是叶子结点个数。
每一个叶子结点对应一个连续值$(w_i)$，输出的是q映射的那个叶子结点的值。

$$
F = \{f(x) = w_{q(x)} \} (q : R^m → {1,2,...,T}, w ∈ R^T)
$$

树ensemble之后的输出是融合每一棵树的结果后的输出(直接求和！！？？)

$$
\hat{y} = \phi(x_i) = \sum_{k=1}^K f_k(x_i), f_k \in F
$$

添加正则项后的目标函数为

$$
L(\phi) = \sum_i l(y_i; \hat{y}_i) + \sum_k \Omega(f_k)  \\\\
where \Omega(f) = \gamma T + \frac{1}{2} \lambda ||w||^2
$$

这个损失函数也在Regularized greedy forest (RGF)  model 出现过，参看这篇文章

T. Zhang and R. Johnson. Learning nonlinear functions
using regularized greedy forest. IEEE Transactions on
Pattern Analysis and Machine Intelligence, 36(5), 2014.

上面的目标函数比 RGF 模型简单，更容易并行处理？！！
传统的GBM模型没有正则项！

- Gradient Tree Boosting，目标函数通过顺序加树进行优化，在第t额颗树，

$$
L^t = \sum_{i=1}^ l(y_i, \hat{y}_i + f_t(x_i)) + \Omega(f_t)
$$

将损失函数展开到二阶项，丢掉常数项后

$$
\hat{L}^t = \sum_{i=1}^ [g_i f_t(x_i) + \frac{1}{2} h_i f_i^2(x_i)] + \Omega(f_t)
$$

例如，损失函数取为

$$
l(y_i, \hat{y}_i) = (y_i - \hat{y}_i)^2
$$

那么，对应的梯度和二阶导为

$$
g_i = -2(y_i - \hat{y}_i) = -2 e_i  \\\\
h_i = 2
$$

定义样本集合$(I_j = \{ i | q(x_i) = j \})$，即到达第j个叶子结点的样本集合。
那么损失函数可以改写为对第t颗树的叶子结点求和，下面的权值w也是指第t颗树的

$$
\hat{L}^t = \sum_{j=1}^T [(\sum_{i \in I_j} g_i) w_j + \frac{1}{2} (\sum_{i \in I_j} h_i + \lambda) w_j^2] + \gamma T
$$

从上式可以求得在给定的q函数下，最佳的权值为

$$
w_j^* = - \frac{\sum_{i \in I_j} g_i}{\sum_{i \in I_j} h_i + \lambda}
$$
对应的最优目标函数为

$$
\hat{L}^t(q) = - \frac{1}{2} \sum_{j=1}^T \frac{(\sum_{i \in I_j} g_i)^2 }{\sum_{i \in I_j} h_i + \lambda} + \gamma T
$$
这个值可以作为q函数的score来评估树的结构，作用和CART的不纯度gini系数一样。
理论上来说需要遍历所有可能的树，实际上用启发式的方法，从单叶子节点的树开始，然后添加分支。
设分裂前的样本集为I，分裂后左右子树的样本集分别为$(I_L, I_R)$，那么分裂带来的损失函数减少量为

$$
L_{split} = \frac{1}{2} ( \frac{(\sum_{i \in I_L} g_i)^2 }{\sum_{i \in I_L} h_i + \lambda}  +  \frac{(\sum_{i \in I_R} g_i)^2 }{\sum_{i \in I_R} h_i + \lambda} - \frac{(\sum_{i \in I} g_i)^2 }{\sum_{i \in I} h_i + \lambda}) - \gamma
$$
就像C4.5 和 CART 的信息增益率和gini系数增加量那样，作为该分裂点的score，用来确定分裂点是否最优。

- Shrinkage and Column Subsampling
这两种技巧用来防止过拟合

shrinkage：
J. Friedman. Stochastic gradient boosting. Computational
Statistics & Data Analysis, 38(4):367–378, 2002.

shrink 将新加入的权值乘上一个系数$(\eta)$，为后面的树提供一定的学习空间。

列采样来自随机森林：
L. Breiman. Random forests. Maching Learning,
45(1):5–32, Oct. 2001

列采样之前没在Boosting里面用过，据说比行采样效果要好。我的理解是，列采样导致每个树学习的多样化，行采样也会有，但是会少很多。
另一方面，也为算法并行化提供了好处。

### 分裂点寻找算法
- Basic Exact Greedy Algorithm

在每一次寻找中，枚举所有可能的分裂点，然后利用score确定最佳分裂点。
代表的实现软件有：sklearn， R的GBM， 单机版的XGBoost。
算法首先对特征进行排序，然后依次访问数据，并以此数据该维特征的值作为分裂点，计算score。

- 近似方法
精确寻找不适用与分布式数据，近似方法通过特征的分布，按照百分比确定一组候选分裂点，通过遍历所有的候选分裂点来找到最佳分裂点。
两种策略：全局策略和局部策略。在全局策略中，对每一个特征确定一个全局的候选分裂点集合，就不再改变；而在局部策略中，每一次分裂
都要重选一次分裂点。前者需要较大的分裂集合，后者可以小一点。论文中对比了补充候选集策略与分裂点数目对模型的影响。
全局策略需要更细的分裂点才能和局部策略差不多。

> 什么意思：    
>
> Notably, it is also possible
> to directly construct approximate histograms of gradient
> statistics [19]    
>
> [19] S. Tyree, K. Weinberger, K. Agrawal, and J. Paykin.
> Parallel boosted regression trees for web search ranking. In
> Proceedings of the 20th international conference on World
> wide web, pages 387–396. ACM, 2011

- Weighted Quantile Sketch 算法
对第k个特征，构造数据集

$$
D_k= \{(x_{1k}, h_1) , (x_{2k},h_2) , ...,(x_{nk},h_n) \}
$$
其中$(h_i)$是该数据点对应的损失函数的二阶梯度。二阶梯度在这里相当于样本的权值，目标函数可以看做一个带权的均方误差(通过近似，将所有凸函数形式的目标函数都变成了和最小均方误差一样了)。
重新改写目标函数为

$$
\sum_{i=1}^n \frac{1}{2} h_i(f_t(x_i) - g_i / h_i)^2 + \Omega(f_t) + constant
$$
定义序函数为带权的序函数

$$
r_k(z) = \frac{1}{\sum_{(x,h) \in D_k } h} \sum_{(x,h) \in D_k, x<z} h
$$
它代表第k个特征小于z的样本比例（带权的）。候选集的目标要使得相邻两个候选分裂点相差不超过某个值$(\epsilon)$。

样本权值相同的时候， quantile sketch 算法可以找到这些分裂点：

1. M. Greenwald and S. Khanna. Space-efficient online
computation of quantile summaries. In Proceedings of the
2001 ACM SIGMOD International Conference on
Management of Data, pages 58–66, 2001.
2. Q. Zhang and W. Wang. A fast algorithm for approximate
quantiles in high speed data streams. In Proceedings of the
19th International Conference on Scientific and Statistical
Database Management, 2007.

对于带权的，目前都是通过对随机抽取的子集进行排序得到的。缺点：没有理论保证，也存在一定错误概率。

作者提出的一种 分布式带权 quantile sketch 算法，有概率上的理论保证。在附录里面有详细介绍。


- Sparsity-aware Split Finding
为了发现稀疏数据里面的模式，为每一个树的节点提供一个默认的方向。如果该特征缺失，就以默认的方向向树的底部移动。
（这不是相当于为空值人为地填充了一个值补全么？）这个方向是学习得到的！（好吧，怎么学的？）
算法让所有缺失值的样本首先全部走到右子树，然后在非缺失值样本上迭代，依次选取不同分裂点求出最佳score，相当于missing value 全部用最大值填充，
接着右让缺失值全部走左子树，然后依次选取不同分裂点求出最佳score，相当于missing value 全部用最小值填充，
经过两次遍历后，选出最佳score，相当于比传统的方式多遍历一次？！（那为什么速度还比传统的快呢？）
算法随非缺失值样本数目现行增长，因为它只在非缺失值样本上迭代。
在Allstate-10K dataset上，比naive的算法快50倍！

算法详细，请看论文。

### 系统设计
- Column Block for Parallel Learning      
最耗时的地方在于对样本排序，为了减少这部分时间，将数据保存在内存单元block中。
在block中，数据以compressed column (CSC) 保存，每一列按照该列对应的特征进行排序。
因此，这种数据只需要计算一次，就可以被反复使用。

此外可以同时对所有的叶子结点执行 split finding 算法，寻找最优分裂点。

> 什么意思：    
> We do the split
> finding of all leaves collectively, so one scan over the block
> will collect the statistics of the split candidates in all leaf branches

这种结构对近似搜索算法也有用，可以使用多个block，每一个block对应一个行的子集，
不同的block还可以在不同的机器上，或者保存在磁盘上实现out-of-core计算。

对每一列的统计可以并行，这导致了split finding 的并行算法。
这种结构也支持列采样。

这个结构还没搞懂，可能需要看一下代码。。。。。待续

- Cache-aware Access       
因为需要访问每行的梯度统计，这种结构导致内存的不连续访问，这会使得CPU cache命中率降低，
而降低算法的运行速度！（靠！这都考虑到了，牛逼），需要合理选择block的大小。

216 examples per block balances the
cache property and parallelization.

- Blocks for Out-of-core Computation        
为了使得核外计算可能，将数据分为多个block，保存到磁盘。
在计算的过程中，并行地用另外的线程将数据从磁盘预取到内存缓存中。
但是由于IO通常会花费更多时间，简单地预取还是不够，我们采用下面两种技巧来优化：
    - Block Compression，block按照列压缩，然后在读取的时候，用另外的线程解压。对于行索引，保存于block初始索引的差值，16bit整数保存。
    - Block Sharding，


## 算法细节




## 源码实现
[dmlc](https://github.com/dmlc/xgboost/blob/master/jvm-packages/xgboost4j-spark/src/main/scala/ml/dmlc/xgboost4j/scala/spark/XGBoost.scala#L61)

- xgboost的spark版本是对每一个分区单独训练？？？？没看懂
```scala
partitionedData.mapPartitions {
      trainingSamples =>
        rabitEnv.put("DMLC_TASK_ID", TaskContext.getPartitionId().toString)
        Rabit.init(rabitEnv.asJava)
        var booster: Booster = null
        if (trainingSamples.hasNext) {
          val cacheFileName: String = {
            if (useExternalMemory && trainingSamples.hasNext) {
              s"$appName-dtrain_cache-${TaskContext.getPartitionId()}"
            } else {
              null
            }
          }
          val trainingSet = new DMatrix(new JDMatrix(trainingSamples, cacheFileName))
          booster = SXGBoost.train(trainingSet, xgBoostConfMap, round,
            watches = new mutable.HashMap[String, DMatrix] {
              put("train", trainingSet)
            }.toMap, obj, eval)
          Rabit.shutdown()
        } else {
          Rabit.shutdown()
          throw new XGBoostError(s"detect the empty partition in training dataset, partition ID:" +
            s" ${TaskContext.getPartitionId().toString}")
        }
        Iterator(booster)
    }.cache()
```

## TODO
- 了解一下Kaggle 2015年TOP3队伍解决方案


## reference
- [XGBoost: A Scalable Tree Boosting System](http://dmlc.cs.washington.edu/data/pdf/XGBoostArxiv.pdf)
- [chen tianqi slide](https://homes.cs.washington.edu/~tqchen/data/pdf/BoostedTree.pdf)
- [github dmlc xgboost](https://github.com/dmlc/xgboost)
- [Greedy function approximation: a gradient boosting machine](http://docs.salford-systems.com/GreedyFuncApproxSS.pdf)
- The present and the future of the kdd cup competition: an outsider’s perspective.
