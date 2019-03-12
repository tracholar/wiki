---
title: "catboost"
layout: page
date: 2019-03-12
tags: ["Machine Learning"]
---
[TOC]

## 简介
- 论文: CatBoost: unbiased boosting with categorical features, Liudmila Prokhorenkova1,2, Gleb Gusev1,2, Aleksandr Vorobev1,Anna Veronika Dorogush1, Andrey Gulin1
- 用于解决决策树模型对类别特征,尤其是高维稀疏特征拟合的不足
- 现有基于onehot将cat特征转化为数值特征的问题
    - 如果cat取值过多(高维稀疏),那么决策树需要拟合深度很深,泛化能力也不足。
    - 如果使用hash trik降维到几百维,那么会导致效果下降明显
    - 不能泛化到未见过的取值
- 基于 label-encode 的方法,将类别特征转化为基于label 的统计量的数值特征
    - 例如,对于item的某个类别特征x,不是直接用x作为特征,而是使用概率 P(y=1|x) 的数值作为特征,例如将广告id转化为广告id对于的点击率特征,注意,这里和历史点击率的区别,这里是直接用样本标签计算的统计量,而不是用历史数据统计出来的
    - 但是这种方法容易过拟合
    - 使用两个技巧, 贝叶斯先验估计, 在线更新 P(y=1|x)
- 解决特征交叉的问题
    
## 论文简述
- prediction shift
- target leakage

### 类别特征
- high cardinality features
    - 聚类后在做onehot
    - target statistic 方法
- LightGBM
    - 每一次都要计算每一个取值的统计信息
    - 为了节省空间和时间,将长尾取值放到一个类别中

### Target Statistic
- 对于类别特征 $(x^i)$, 其中某个取值 $(x^i_k)$ 可以用target的统计量来替代

$$
\hat{x}^i_k = \frac{\sum \mathbf{1} _ {x^i_j=x^i_k} y_j +a p}{\sum \mathbf{1} _ {x^i_j=x^i_k} + a}
$$

- 即用target平滑后的平均值来替代这个类别特征
- 但是这种方法存在一个问题,在训练集和测试集中, 相同的类别特征的target statistic 不相等,存在所谓的condition shift和target泄露问题
- 解决方法: 不在全部数据集中计算TS,而是对不同的样本用不同的集合计算TS, 例如对样本 $(x_k)$, 用排除该样本的其他集合计算的TS作为$(x_k)$的类别特征的替代值
- Holdout TS: 将数据集划分为两部分, 一部分用来计算TS,另一部分用来训练
- Leave-one-out TS: 对训练样本 $(x_k)$用排除了$(x_k)$的剩下样本计算TS,对测试样本则用全量计算TS。但是并没有解决target泄露的问题
- Ordered TS: 借鉴自在线学习, 在离线训练时,先打散,得到一个认为的时间顺序,在对每个样本计算TS时,只用时间在之前的那些样本。对测试集,就用全量训练集来计算TS
    - 如果只用一次随机排列,那么前面的树的方差将比后面的树大, Catboost每一轮都重新使用一次随机排列
    
### LightGBM
- 通过采样减少计算增益时的计算量
    - 基于梯度的单边采样GOSS, 出发点: 梯度大的样本对增益影响大,所以只对梯度小的样本下采样
    - Exclusive Feature Bundling: 对于稀疏特征,求解最优分组问题(很复杂),将它转化为图着色问题
- GBDT复杂度
    - 找到特征分割点
        - 预排序: 先将特征值排序,然后枚举所有可能的分割点
        - 直方图算法: 将连续特征离散分桶,构造直方图,只在直方图的交界点尝试分割,并用直方图计算Gain。对稀疏特征兼容
        
       
- Exclusive Feature Bundling: 将稀疏特征分组,从而将复杂度从 O(#data x #feature) 减少到 O(#data x #bundle),在稀疏特征存在的时候,特征数量会远多于分组的数量

- 稀疏特征的分割
    - 将稀疏特征的取值按照累积的 (sum_gradient / sum_hessian) 排序,然后在排序后的直方图中找到最优分割点

    

### 参考
- D. Micci-Barreca. A preprocessing scheme for high-cardinality categorical attributes in classifi- cation and prediction problems. ACM SIGKDD Explorations Newsletter, 3(1):27–32, 2001.
