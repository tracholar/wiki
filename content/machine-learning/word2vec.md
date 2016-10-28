---
title: "word2vec"
layout: page
date: 2016-10-09
---
[TOC]

## 关于
词向量相关论文学习。

## word2vec 论文
- **Mikolov, T.**, Sutskever, I., **Chen, K.**, Corrado, G. S., & **Dean, J.** (2013). Distributed representations of words and phrases and their compositionality. In Advances in neural information processing systems (pp. 3111-3119).

- 问题：
    - 相比直接通过VSM学习到的词向量，优势是什么？

### 导论
- 创新点：
    1. 针对连续 skip-gram 模型：采用一些介壳提升词向量质量和训练速度
    2. 负采样技术： 对高频词负采样可以显著提升训练时间（2-10倍的提升），同时也能提高低频词的词向量质量？

- recursive autoencoders ：Richard Socher, Cliff C. Lin, Andrew Y. Ng, and Christopher D. Manning. Parsing natural scenes and natural language with recursive neural networks. In Proceedings of the 26th International Conference on Machine Learning (ICML), volume 2, 2011.

### The Skip-gram Model
用中间词预测周围的词，最大化对数似然函数

$$
\frac{1}{T} \sum_{t=1}^T \sum_{-c \le j \le c, j \neq 0} \log p(w_{t+j}| w_t) \\\\
p(w_O| w_I) = \frac{\exp(v^'\_{w_O}^T v_{w_I})}{\sum_{w=1}^W \exp(v^'\_{w}^T v_{w_I}) }
$$

计算代价正比于词典规模 W（10^5-10^7），因此很费时间。

#### Hierarchical Softmax
Frederic Morin and **Yoshua Bengio**. Hierarchical probabilistic neural network language model. In Pro- ceedings of the international workshop on artificial intelligence and statistics, pages 246–252, 2005.

只需要计算$(\log_2W)$个节点！
将条件概率的计算，变成多个分类概率的计算。可以用一个二叉树表示出来，叶子结点对应词，中间节点有一个参数！
每一个内部节点可以看做一个二分类逻辑回归，其参数就是二分类参数。这个参数也要学习！
文中表示用哈夫曼树作为这个二叉树可以简单提升性能。

$$
p\left( w_O | w_I \right) = \prod_{j = 1}^{L(w) - 1} \sigma \left( [n(w,j+1) = ch(n(w,j))] \centerdot v_{n(w,j)}^{\top} v_{w_I} \right)
$$


#### Negative Sampling
- Noise Contrastive Estimation (NCE)：
    1. Michael U Gutmann and Aapo Hyva ̈rinen. Noise-contrastive estimation of unnormalized statistical mod- els, with applications to natural image statistics. The Journal of Machine Learning Research, 13:307–361, 2012.
    2. Andriy Mnih and Yee Whye Teh. A fast and simple algorithm for training neural probabilistic language models. arXiv preprint arXiv:1206.6426, 2012.

负采样解释可以看2014年的文章：<http://cn.arxiv.org/pdf/1402.3722v1.pdf>

可以将每一项理解为一个二元分类问题，正样本是词在中心词的上下文，而负样本是不在中心词上下文的词。
目标函数相当于让正样本出现以及k各负样本不出现的联合概率最大化！

$$
\log \sigma(v_{w_O}'^T v_{w_I}) + \sum_{i=1}^k \mathbb{E}\_{w_i \sim P_n(w)} \left[  \log(-\sigma(v_{w_i}'^T v_{w_I}')) \right]
$$

对于小数据集k取 5-20 即可；对于大数据集k可以取小点2-5.
采样方法 P 取 unigram distribution $(U(w)^{3/4})$最好，即正比于词频的3/4次幂。

#### 高频词的负采样
以概率P丢弃！

$$
P(w_i) = 1 - \sqrt{\frac{t}{f(w_i)}}
$$

t 是阈值，典型值为$(10^{-5})$，f是词频率！
这种方式不但可以加快速度，还能提高低频词的精度！（不是数据越多越好？！）

### 测试数据
- 相似推理任务：
    - 语法相似 syntactic analogies： “quick” : “quickly” :: “slow” : “slowly”
    - 语义相似 semantic analogies：“Germany” : “Berlin” :: “France” : ?
结果如下图所示。

<img src="/wiki/static/images/word2vec.png" style="width:600px; float:left;" />


### 短语学习
将经常出现在一起的，而不经常在其他上下文出现的多个词作为一个token。例如：New York Times；
但是：this is，没有作为一个token！


## CBOW 论文
**Mikolov, T.**, Chen, K., Corrado, G., & Dean, J. (2013). Efficient estimation of word representations in vector space. arXiv preprint arXiv:1301.3781.

周围词加和预测中心词，Hierarchical Softmax 哈夫曼树！

## Glove
Pennington, J., Socher, R., & Manning, C. D. (2014, October). Glove: Global Vectors for Word Representation. In EMNLP (Vol. 14, pp. 1532-43).

- square root type transformation in the form of Hellinger PCA (HPCA) (Lebret and Collobert, 2014)
- Mnih and Kavukcuoglu (2013) also proposed closely-related vector log-bilinear models, vLBL and ivLBL, and Levy et al. (2014) proposed explicit word embed- dings based on a PPMI metric.

核心思想，直接建模共生矩阵！（skip-gram，CBOW是直接建模上下文！不能利用全局统计信息？）
建模概率比率，而不是建模概率本身！

$$
F(w_i, w_j, \hat{w}\_k) = \frac{P_{ik}}{P_{jk}}
$$

希望学习到线性关系？！

$$
F(w_i, w_j, \hat{w}\_k) = F(w_i - w_j, \hat{w}\_k) \\\\
 = F( (w_i - w_j)^T \hat{w}\_k) = \frac{P_{ik}}{P_{jk}}
$$

考虑对称性，即将共生矩阵行列对换，需要保持不变性！那么要F是群(R, +)到(R+, x)的同态映射

$$
F((w_i - w_j)^T \hat{w}\_k) = \frac{F(w_i^T \hat{w}\_k)}{F(w_j^T \hat{w}\_k)}
$$

因此，F是指数函数！

进而要求具有交换对称性，可以增加bias实现

$$
w_i^T \hat{w}\_k + b_i + \hat{b}\_k = \log(X_{ik})
$$
