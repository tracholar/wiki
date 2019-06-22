---
title: "softmax近似"
layout: page
date: 2019-04-04
---
[TOC]

## 背景
- 问题:多分类如果类别太多要对softmax近似
- 方法
    - importance sampling: sampled softmax [1]
    - noise contrastive estimation [2]
    - 经验分布采样
    
    
1. Bengio, Yoshua and Senecal, Jean-Se ́bastien. Adaptive importance sampling to accelerate training of a neu- ral probabilistic language model. IEEE Trans. Neural Networks, 19(4):713–722, 2008.
2. Gutmann, Michael and Hyva ̈rinen, Aapo. Noise- contrastive estimation of unnormalized statistical models, with applications to natural image statistics. Journal of Machine Learning Research, 13:307–361, 2012.