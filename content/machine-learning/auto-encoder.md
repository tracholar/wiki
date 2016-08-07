---
title: "自编码模型"
layout: page
date: 2016-07-21
---
[TOC]

## 关于
自编码模型常用做深度神经网络预训练。


## 历史

## 神经网络预训练方案
多层神经网络直接训练会因为局部最优问题，导致初值敏感，所以随机初始化效果不好。
可以采用局部无监督准则进行逐层初始化预训练，使得后续训练更大可能跳出局部最优。

- RBM：Hinton et al., 2006; Hinton and Salakhutdinov, 2006; Lee et al., 2008
- auto-encoder：Bengio et al., 2007; Ranzato et al., 2007
- semi-supervised embedding：Weston et al.,2008
- kernel PCA：Cho and Saul, 2010

RBM 和 自编码 模型函数形式很像，但是训练和解释都不同。一个很大不同是，确定性的自编码可以用实数值作为隐层的表达，
而随机的RBM采用二进制表达隐层。但是在实际上，应用在深度网络中的RBM还是用实数均值作为表达。
autoencoder 的重构误差可以看做 RBM 的 log-likelihood gradient 一种近似。
RBM 中的 Contrastive Divergence 更新。

如何构造一个好的表达！？采用无监督学习，学出输入中的重要模式！

## 好的特征表达： 交互信息 到 自编码
定义好的表达：对最终我们感兴趣的任务是有用的，相比不采用这种表达，它能够帮助系统更快地达到更高的性能！

> A good representation is one that will yield a better performing classifier

实验表明，一个无监督准则的初始化，可以帮助分类任务得到明显地提升。
人类能够快速地学习新的东西一个重要的原因是已经获得了这个任务的一些先验知识。

学习一个输入$(X)$ 的表达$(Y)$，实际上是学习条件概率$(q(Y|X) = q(Y|X; \theta))$。$(\theta)$是要学习的参数。

一个基本要求是要尽可能保留输入的信息，在信息论里面可以表达为最大化交互信息 $(\matchcal{I}(X; Y))$： Linsker (1989)

### 独立成分分析（ICA）：Bell and Sejnowski (1995)

$$
\arg \max_{\theta} \mathcal{I}(X; Y) = \arg \max_{\theta} - \mathcal{H}(X|Y) \\\\
    = \arg \max_{\theta} \mathcal{E}\_{q(X, Y)} [\log q(X|Y)]
$$

对于任意分布 $(p(X, Y))$，利用 KL 距离的性质可知：

$$
\mathcal{E}\_{q(X, Y)} [\log p(X|Y)] \le - \mathcal{H}(X|Y)
$$

设这个分布通过参数$(\theta')$刻画，那么优化下面这个式子相当于优化条件熵的下界：

$$
\max_{\theta, \theta'} \mathcal{E}\_{q(X, Y; \theta)} [\log p(X|Y; \theta')]
$$

当两个分布相同的时候，可以得到精确的交互信息。infomax ICA 中，特征映射为 $(Y = f_{\theta}(X))$。
那么$(q(X, Y;\theta) = q(X) \approxy q^0(X))$，即用样本集的分布代替总体分布。优化问题变为：

$$
\max_{\theta, \theta'} \mathcal{E}\_{q^0(X)} [\log p(X|Y=f_{\theta}(X); \theta')]
$$

UFLDL里面的[独立成分分析](http://deeplearning.stanford.edu/wiki/index.php/%E7%8B%AC%E7%AB%8B%E6%88%90%E5%88%86%E5%88%86%E6%9E%90)：
找到一组基向量使得变换后的特征是稀疏的。
数据必须ZCA白化，标准正交基维数小于输入维度，是一组不完备基。

$$
\min || W x ||\_1  \\\\
s. t. WW^T = I
$$

优化方法：梯度下降 + 每一步增加投影。

$$
W = W - \alpha \nabla_W || W x ||\_1 \\\\
W = (WW^T)^{-1/2} W
$$

### 传统自编码
在传统 autoencoder(AE) 中，特征变换函数（Encoder）用sigmoid函数来近似：

$$
y = f_{\theta}(x) = s(Wx+b)   \\\\
\theta = \{ W, b \}
$$

特征重构（Decoder）变换也用sigmoid函数

$$
z = g_{\theta'}(y) = s(W' y + b') \\\\
\theta'  = \{ W', b' \}
$$

## Reference
1. 2010, Pascal Vincent, Yoshua Bengio, Stacked Denoising Autoencoders: Learning Useful Representations in a Deep Network with a Local Denoising Criterion.
2.
