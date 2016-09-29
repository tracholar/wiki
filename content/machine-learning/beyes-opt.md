---
title: "Beyes optimization"
layout: page
date: 2016-09-27
tags: ["优化", "Machine Learning"]
---
[TOC]

## 关于
贝叶斯优化用来调参！
论文：Practical Bayesian Optimization of Machine Learning Algorithms, Jasper Snoek, Hugo Larochelle, Ryan P. Adams.

## 导论
- 关键词：高斯过程（Gaussian Process）
- 关键要素：type of kernel，treatment of its hyperparameters
- 贝叶斯优化原始文献：
    - Jonas Mockus, Vytautas Tiesis, and Antanas Zilinskas. The application of Bayesian methods for seeking the extremum. Towards Global Optimization, 2:117–129, 1978.
    - D.R. Jones. A taxonomy of global optimization methods based on response surfaces. Journal of Global Optimization, 21(4):345–383, 2001.
    - Niranjan Srinivas, Andreas Krause, Sham Kakade, and Matthias Seeger. Gaussian process optimization in the bandit setting: No regret and experimental design. In Proceedings of the 27th International Conference on Machine Learning, 2010.
    - Adam D. Bull. Convergence rates of efficient global optimization algorithms. Journal of Machine Learning Research, (3-4):2879–2904, 2011.
- 假定连续函数采样自高斯过程，并保存一个后验概率，根据观测值来调整。
- expected improvement (EI)； Gaussian process upper confidence bound (UCB)


- random search is beter than grid search: James Bergstra and Yoshua **Bengio**. Randomsearch for hyper-parameter optimization. Journal of Machine Learning Research, 13:281–305, 2012.

## Bayesian Optimization with Gaussian Process Priors
- 优化函数 $(f(x), x \in \mathcal{X})$，$(\mathcal{X})$ 是 $(\mathbb{R}^n)$ 上的闭集。
- 贝叶斯优化基本思想：为函数 $(f(x))$ 建立概率模型，然后根据贝叶斯法则决定下一个搜索点！
- 贝叶斯优化 **综述**：Eric Brochu, Vlad M. Cora, and Nando de Freitas. A tutorial on Bayesian optimization of ex- pensive cost functions, with application to active user modeling and hierarchical reinforcement learning. pre-print, 2010. arXiv:1012.2599.

### 高斯过程
- 在有限集 $(\\{ x_n \in \mathcal{X}  \\}\_{n=1}^N)$，第n个点的值为 $(f(x_n))$。
- 均值函数 $(m: \mathcal{X} \rightarrow \mathbb{R})$
- 正定协方差函数（covariance function） $(K: \mathcal{X} \times \mathcal{X} \rightarrow \mathbb{R})$
- 高斯过程综述：Carl E. Rasmussenand Christopher Williams. Gaussian Processes for Machine Learning. MIT Press, 2006.

### Acquisition Functions
- 假定函数 $(f(x))$ 来自高斯先验，观测量 $(\\{x_n, y_n \\}\_{n=1}^N, y_n \sim \mathcal{N}(f(x_n), v))$，v是观测噪声。
- Acquisition function: $(a: \mathcal{X} \right \mathbb{R}^+)$，下一个最优搜索点通过该函数得到：$(x_{next} = \arg\max_x a(x))$。跟之前的观测值有关，也写作 $(a(x; \\{x_n, y_n \\}, \theta))$， $(\theta)$是超参数。


## In Action
### Covariance Functions and Treatment of Covariance Hyperparameters
