---
title: "GAN: 生成对抗网络"
layout: page
date: 2017-04-15
---
[TOC]

## GAN
论文：Generative Adversarial Nets，an J. Goodfellow, Jean Pouget-Abadie∗, Mehdi Mirza, Bing Xu, David Warde-Farley, Sherjil Ozair†, Aaron Courville, Yoshua Bengio‡，2014

- 生成模型G，对于随机noise z，分布$(p_z(z))$，生成一个样本 $(G(z; \theta_g))$
- 判别模型D，是一个常规的二分类模型，输出是样本来自真实数据的概率，$(D(x; \theta_d))$
- 极大似然估计，来自样本的认为是正例，来自生成模型的认为是负例
- minmax value function $(V(D, G))$

$$
\min_G \max_D V(D, G) = \mathbf{E} _ {x \in p_{data}(x)}[\log D(x)] + \mathbf{E} _ {x \in p_{z}(z)}[\log(1-D(G(z)))]
$$

训练算法：

<img src="/wiki/static/images/gan-algo.png" alt="GAN-GLAORITHM"/>

### 理论结论
- 全局最优：$(p_g = p_{data})$
- 对固定的G，

$$
V(G,D) = \int_x dx p_{data}(x) \log(D(x)) + p_g(x) \log(1 - D(x))
$$

容易验证，最优的D满足

$$
D^* _ G(x) = \frac{p_{data}(x)}{p_{data}(x) + p_g(x)}
$$

最优 value function为

$$
C(G) = \mathbf{E} _ {x \in p_{data}(x)}[\log \frac{p_{data}(x)}{p_{data}(x) + p_g(x)}] + \mathbf{E} _ {x \in p_{z}(z)}[\log(\frac{p_{g}(x)}{p_{data}(x) + p_g(x)})] \\\\
= - \log 4 + 2 JSD(p_{data} || p_g)
$$

JSD 是 Jensen–Shannon divergence. 上式最优的结果是 $(-\log4)$，当 $(p_g = p_{data})$ 取得。

- 算法 Algorithm 1 的收敛性
