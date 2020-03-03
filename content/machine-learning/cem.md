---
title: "CEM: cross entropy method"
layout: page
date: 2020-03-01
---

[TOC]

# 关于
CEM，交叉熵方法，是一种在线求解目标变量最优解的方法。

# 基本原理
- 假设待优化向量为w，可以观察到参数w下的回报S(w)
- S被当做黑盒，不建模
- 最简单的想法是，随机一批w，然后观察S(w)，然后选出最优w即可
- 如果w空间很大，一次迭代难以得到足够的数据来确定最优w，可以利用在线更新的方法（类似汤普森采样）
- 先假设w的一个先验分布，比如高斯分布$(w ~ N(\mu, \sigma))$
- 然后每轮采样n个wi，执行策略并观察回报S(wi)
- 取前K(K<n)个w，重新估计参数$(\mu, \sigma)$，即用这K个w计算新的均值和方差
$$
\mu_{new} = \frac{\sum_{i \in I_K} w_i}{|I_K|} \\\\
\sigma^2_{new} = \frac{\sum_{i \in I_K} (w_i-\mu_{new})^T(w_i-\mu_{new})}{|I_K|} + Z
$$
- Z是一个随机噪声，避免过快收敛

# 具体应用
## 推荐多目标调参
- 假设排序分是pCTR和pCVR的加权和 $(w_1 pCTR + w_2 pCTR * pCVR)$
- 可以认为w1和w2 分别服从高斯分布 N(0, 1)
- 从该分布随机n组w1和w2，观察最终的业务指标，如GMV
- 选择GMV最高的K组w1和w2，重新计算后验分布的均值和方差，并给方差加点噪声
- 重复上述过程

# 参考
- <https://zhuanlan.zhihu.com/p/44623211>
- <https://gist.github.com/kashif/5dfa12d80402c559e060d567ea352c06#file-cem-py-L102>
- <https://github.com/tracholar/ml-homework-cz/blob/master/cem/tracholar/cem.py>

# FAQ
## CEM跟贝叶斯优化的区别是什么

## 为什么CEM最终会收敛到最优目标
- 定性理解：因为每一步都舍弃了一批最差的取值，所以最终分布会不断收敛到最优解
