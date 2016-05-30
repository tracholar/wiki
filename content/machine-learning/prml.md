---
title: "Pattern Recognition and Machine Learning - Bishop"
layout: page
date: 2016-05-30
---
[TOC]

# 关于 
Bishop写得PRML无疑是机器学习领域的权威著作，是加强机器学习理论知识必看的书籍之一。
这里是我在阅读这本书时记录的笔记和自己的一些初步思考。


# 引言
## 曲线拟合
设曲线拟合的目标变量为$(t)$，$(N)$个训练集为
$( \mathbf{x} = (x_1, ..., x_N)^T )$，对应的目标值为
$( \mathbf{t} = (t_1, ..., t_N)^T )$。假设估计误差服从
高斯分布
$$
p(t|x, \mathbf{w}, \beta) = \mathcal{N} (t| y(x, \mathbf{w}), \beta^{-1}) 
$$
这里的$(\mathbf{w})$是待估计的参数，$(\beta^{-1})$对应于
估计误差的方差。此时，按照最大似然准则估计的参数就是最小二乘
法的结果，也就是最小化均方误差。而$(\beta^{-1})$的最大似然估计为
模型预测值与实际值的均方差。
$$
\frac{1}{\beta} = \frac{1}{N} \sum_{n=1}^N [y(x_n,\mathbf{w}_{ML}) - t_n]^2.
$$

如果我们对参数的具有一定先验知识，可以进一步采用最大后验概率估计，
得到更好的结果。作为一个特例，如果认为
$$
p(\mathbf{w}|\alpha) = \mathcal{N}(\mathbf{w} | 0, \alpha^{-1} \mathbf{I})   \\\\
					= (\frac{\alpha}{2\pi})^{(M+1)/2} \exp(-\frac{\alpha}{2} \mathbf{w}^T \mathbf{w}).
$$
那么此时的最大后验概率估计为带$(L_2)$正则项的估计，
它最小化
$$
\frac{\beta}{2} \sum_{n=1}^N [y(x_n, \mathbf{w}) -t_n]^2 + \frac{\alpha}{2} \mathbf{w}^T \mathbf{w}
$$
也就是说正则项是对模型参数的一种先验知识，$(L_2)$正则项代表高斯先验。
那$(L_1)$代表laplace先验(待求证)？





