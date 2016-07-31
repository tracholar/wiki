---
title: "机器学习中的基本概念"
layout: page
date: 2016-07-27
---
[TOC]

## 关于
记录机器学习中的基本问题和概念。



## 基础统计分布
### 几种常见的分布
包括正态分布、泊松分布、指数分布等。略，后面可能会写。

### 正态分布的和
两个服从正太分布的随机变量 $(X_1, X_2)$， 只要其联合分布为正态分布，那么和也为正态分布。

### 卡方分布
设$(X_1,...,X_n)$ iid，服从标准正态分布，那么平方和$(X_1^2+...+X_n^2)$服从自由度为n的卡方分布。

卡方分布的和： $(X_1,X_2)$独立，$(X_1 \sim \chi_m^2, X_2 \sim \chi_n^2)$，那么 $(X_1+X_2 \sim \chi_{n+m}^2)$
### student-t 分布
$(X_1,X_2)$独立，且$(X_1\sim\chi_n^2, X_2 \sim N(0,1))$，那么$(X_2 / \sqrt{X_1/n})$服从自由度为n的t分布。
一个例子，从正态分布总体采样的n+1个样本均值对样本标准差归一化后的值服从自由度为n的t分布。

### F分布
$(X_1\sim\chi_n^2, X_2\sim\chi_m^2)$且统计独立，那么$(m^{-1} X_2 / (n^{-1} X_1))$服从自由度为$((m,n))$的F分布

### 卡方分布、t分布、F分布的联系
设 $(X_1,...,X_n,Y_1,...,Y_m)$独立同分布(iid)，服从标准正态分布，记 $(\bar{X} = (X_1+...+X_n)/n)$，
$(S^2=\sum_i (X_i - \bar{X})^2/(n-1))$，则：       

1. $((n-1)S^2)$服从自由度为n-1的卡方分布
2. $(\sqrt{n}\bar{X}/S)$服从自由度为n-1的t分布
3. $([S_Y^2/(m-1)]/[S_X^2/(n-1)])$服从自由度为(m-1,n-1)的F分布


### 参考
- 陈希孺，概率论与数理统计，中国科学技术大学出版社
- [Cochran theaream](https://en.wikipedia.org/wiki/Cochran%27s_theorem#Sample_mean_and_sample_variance)

## 统计检验
### 方差统计
去掉方差太小的特征。

### 卡方统计
卡方检验，也称独立性检验，拟合优度检验。使用要求，自变量为正值。
`sklearn`中用来检验正值特征与目标是否独立，从而进行特征选择。

例如变量 X 为性别（男0，女1），变量 Y 为是否为左撇子（否 0, 是 1) 。对于某个样本，
有列联表：


|   | 男 | 女 | 总计 |
|---|----|----|-----|
否  | 43 | 44 | 87  |
是  | 9  | 4  | 13  |
总计| 52 | 48 | 100 |

从数据中，可以看到几个边缘分布：

$$
P(男) = 0.52, P(女) = 0.48 \\\\
P(否) = 0.87, P(是) = 0.13
$$

如果两个变量是独立的，那么列联表里面的分布应该由式

$$
P(X, Y) = P(X) P(Y)
$$

得到，我们将这个值作为期望值，记作$(E_{i,j})$，而将实际值记作$(O_{i,j})$，例如，男性不是左撇子的期望值为
$(E_{1,1} = 100 * 0.52*0.87 = 45)$，而观测值为$(O_{1,1} = 43)$。

利用上述符号，定义统计量       
$$
\chi^2 = \sum_{i=1}^r \sum_{j=1}^c \frac{(O_{i,j} - E_{i,j})^2}{E_{i,j}}
$$

则它近似服从自由度为$((r-1)(c-1))$的卡方分布（理论呢？）。该统计量越小，说明越符合独立分布，因此，变量间越独立。

如果其中一个是连续值，`sklearn`中是将连续值求和，然后用类的分布概率乘以该值作为期望值，而实际不同类求和的值作为观测值，然后求卡方值。

```python
## Y是类别的one-hot编码，X是特征
observed = safe_sparse_dot(Y.T, X)          # n_classes * n_features
feature_count = X.sum(axis=0).reshape(1, -1)
class_prob = Y.mean(axis=0).reshape(1, -1)
expected = np.dot(class_prob.T, feature_count)
```

### F classif
ANOVA F-value

连续变量与类别变量之间的独立性检验，只要求为正态分布。

统计检验量是样本在各个分组之间的差异(between-group mean square value)与组内的差异之和(within-group mean of squares)的比值。
组间差异是指每个样本用它所在组的均值替换，然后汇总每个样本与样本均值的差的平方，最后除以自由度。
而组内差异是指直接计算每个样本与该组样本均值的差的平方，最后除以自由度。

如果自变量对因变量没有显著影响，那么这个比值应该接近于1，反之将远大于1.

$$
MSB = \frac{1}{K-1}\sum_{i,j} (\bar{Y_i} - \bar{Y})^2  \\\\
MSW = \frac{1}{K(N-1)}\sum_{i,j} (Y_{i,j} - \bar{Y_i})^2 \\\\
F = MSB / MSW
$$

这里$(K,N)$分别是分组数目（或者分类类别数目）和每组样本数目，分母其实是自由度。
最后得到的统计检验量服从自由度为(K-1,K(N-1))的F分布。

参考[wikipedia](https://en.wikipedia.org/wiki/F-test#One-way_ANOVA_example)

### F regression
Univariate linear regression tests

构建很多个单因素的线性回归检验.

- SST 总的平方差之和 $(\sum (Y - \bar{Y})^2)$
- SSM 模型的平方差之和 $(\sum (Y_{pred} - \bar{Y})^2)$
- SSR 残余的平方差之和 SST - SSM.
- MSM 模型均方差 SSM / SSM的自由度
- MSR 残余均方差 SSR / SSR的自由度


$$
R^2 = SSM / SST  \\\\
F =  MSM / MSR
$$

参考 <http://homepages.inf.ed.ac.uk/bwebb/statistics/Univariate_Regression.pdf>
