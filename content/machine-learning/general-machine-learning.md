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

由于F分布是两个卡方分布的比值，而卡方分布是正态分布的平方和，常出现在方差之中，所以F分布在方差分析之中被大量使用。

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

如果自变量与因变量没有明显的关系，那么F值应该很小。

参考 <http://homepages.inf.ed.ac.uk/bwebb/statistics/Univariate_Regression.pdf>

## 特征变换
### 特征Hash

一种快速的空间效率高的向量化特征的方法。Yahoo 研究院的Weinberger与ICML2009提出。
[Feature Hashing for Large Scale Multitask Learning](http://www.machinelearning.org/archive/icml2009/papers/407.pdf)

Hash 算法是一种有效的特征降维的方法，非参数估计。可以用来做 multitask learning with hundreds of
thousands of tasks！！！

核方法 kernel trick：$(x_1,...,x_n \in X)$，半正定核

$$
k(x_1, x_2) = (\phi(x_1), \phi(x_2))
$$

这种方法可以将原空间非线性决策边界变成变换后空间的线性可分的界。（SVM）
相反的问题是，如果问题在原空间是现行可分的（通常是通过人工的非线性特征工程实现），
但是特征的维度很高。作者提出一种和 kernel trick 互补的方法 hash trick，
将高维空间$(R^d)$特征映射到低维空间$(R^m)$，$(\phi(x) \to R^m)$（Langford et al., 2007; Shi et al., 2009）。
并且有$(m << d)$。
不同于 **随机投影？** ， hash trick 解决了稀疏性，也不需要额外的存储空间存储核矩阵。

hash 内积

hash trick 在 multi-task learning 场景下很有用，这里原始特征是数据集的交叉积 cross product。
每一个任务 U 可以采用不同的hash 函数$(\phi_1(x),...,\phi_{|U|}(x))$，另外共享一个通用的hash函数$(\phi_0(x))$。

以垃圾邮件分类为例，每一个用户需要有他自己的个性化偏好，这里的任务集 U 是针对所有用户（对于 Yahoo mail 和 Gmail数目很大）。
特征空间是大量的语言的词汇集合。

论文的主要贡献：

1. 专门实现的任意内积hash函数，可以应用到许多 kernel method
2. 指数界解释
3. 独立hash子空间使得大规模 multi-task learning 空间使用很小
4. 实际的协同垃圾邮件过滤系统


Hash 函数：

hash 函数 $(h: \mathbb{N} \to \\{1,...,m\\})$，另一个hash函数$(\xi : \mathbb{N} \to \\{\pm 1\\})$.
对于向量 $(x,x' \in l_2)$，定义hash 特征映射

$$
\phi_i^{(h, \xi)} = \sum_{j: h(j)=i} \xi(j) x_j \\\\
\langle x, x' \rangle_{\phi} = \langle  \phi^{(h, \xi)}(x), \phi^{(h, \xi)}(x') \rangle
$$

这里的hash函数定义在自然数集上，实际上对字符串也成立，因为有限长字符串都可以表示为一个自然数。

个人理解：举例，假设h是一个求余函数，那么特征映射后的低维空间的每一个分量相当于将向量等间隔的分量交替求和，从而实现降维。

引理： hash kernel 是无偏的，如果 $(E_{\phi} [\langle x, x' \rangle_{\phi}] = \langle x, x' \rangle)$。
并且方差 $(\sigma_{x,x'}^2 = \frac{1}{m} \sum_{i\neq j} x_i^2 x_j^{'2} + x_i x_i' x_j x_j')$，如果原始向量
都是标准化的，即二范数为1，那么方差为$(O({\frac{1}{m}}))$。

略去若干理论和证明，后面有需要再来看。

Multiple Hashing

近似正交性：对于multi-task learning，需要学习不同的权值，当映射到同一个空间时，需要参数空间交叉部分尽可能少。
设$(w \in R^m)$是$( U \ \{u\})$中的某个任务的参数向量，对任意特征$(x \in u)$，$(w)$ 和 $(\phi_u(x))$的内积很小。


应用：减少存储，避免矩阵向量乘法运算 Locality Sensitive Hashing ？

个性化：每一个task更新局部(local)权值和全局(global)权值。正交性让我们可以将这些特征hash到同一个空间，而没有太多的重叠部分。

邮件过滤问题：每个用户有一些标记数据，但是很少，如果对每一个用户单独训练一个模型$(w_u)$（模型参数？），将缺乏数据而使模型不可用。
可以通过一个全局模型$(w_0)$使得数据能够在多个分类器中共享。存储所有分类器需要$(O(d (|U| + 1)))$的空间复杂度。
简单的方法是排除低频的token。但这回导致恶意错误拼写的单词被丢弃，而hash到一个低维空间，使得这些低频词也能对模型有所贡献。
而且大规模的邮件过滤器对内存和时间有严格要求，因为用户量太大。为此，我们将权值向量$(w_0, ..., w_{|U})$ 通过不同的映射$(\phi_0,...,\phi_{|U|})$，
映射到低维特征空间$(R^m)$。最终的权值为：

$$
w_h = \phi_0(w_0) + \sum_{u \in U} \phi_u(w_u)
$$

实际上，$(w_h)$可以在低维空间直接学习，重来不需要去计算高维向量。对于用户u新的文档或者邮件x，预测任务变成计算内积$(\langle \phi_0(x) + \phi_u(x), w_h \rangle)$。hash操作将会带来内积的计算的失真$(\epsilon_d)$，其他hash函数带来的重叠$(\epsilon_i)$。

$$
\langle \phi_0(x) + \phi_u(x), w_h \rangle = \langle x, w_0 + w_u \rangle + \epsilon_d + \epsilon_i
$$

大规模协同过滤：$(M = U^T W)$，U和W $(\in R^{nd})$，存储需要大量空间，利用hash只需要两个m维向量$(u, w)$

$$
u_i = \sum_{j,k:h(j,k)=i} \xi(j,k) U_{jk},\\\\
w_i = \sum_{j,k:h'(j,k)=i} \xi'(j,k) W_{jk}
$$

这里$((h,\xi), (h',\xi'))$是两组独立的hash 函数。近似计算$(M_{ij})$的方法是：

$$
M_{ij}^{\phi} = \sum_k \xi(k,i)\xi'(k,j) u_{h(k,i)} w_{h'(k,j)}
$$
