---
title: "CS224d: Deep Learning for Natural Language Processing"
layout: page
date: 2016-06-28
---
[TOC]

# 关于
cs224d这门课是将深度学习应用到自然语言处理上面的课程，十分推荐。


## 一些疑惑
<https://www.quora.com/How-is-GloVe-different-from-word2vec>
对于word2vec与GloVe的比较的见解。


## word2vec
- 单词的表达： Word-Net， ONE-HOT
- 文档-单词 共生矩阵， SVD提取， LSA
	- 潜在问题：SVD计算复杂度高当词典或者文档数目很大时，对新词和新的文档难以处理，与其他DL不同的学习体制。
- 直接学习低维词向量：word2vect
	- Learning	representa4ons	by	back-propaga4ng	errors.	 Rumelhart	et	al.,	1986
	- A	neural	probabilis4c	language	model	(Bengio	et	al.,	2003)
	- NLP	(almost)	from	Scratch	(Collobert	&	Weston,	2008)
	- A	recent,	even	simpler	and	faster	model:	word2vec	(Mikolov	et	al.	2013)	à	intro	now
- 不是直接统计共同发生的次数，而是预测每一个单词周围的单词；速度快，易于应用到新词和新文档
- 目标函数
$$
J(\theta) = \frac{1}{T} \sum_{t=1}^T  \sum_{-m \le j \le m, j \neq 0} \log p(w_{t+j} | w_t)
$$
其中条件概率采用如下指数形式
$$
p(o|c) = \frac{\exp(u_o^T v_c)}{\sum_{w=1}^W \exp(u_w^T v_c)}
$$
- 每一个单词有两个向量$(u, v)$.
- 词向量的线性关系
	- $( X_{apple} - X_{apples} \approxy X_{car} - X_{cars} \approxy X_{family} - X_{families})$

### 负采样近似
单个输入词向量与单个输出词向量的损失函数
$$
J(u_o, v_c, U) = - \log(\sigma(u_o^T v_c)) - \sum_{k \sim P} \log(\sigma(- u_k^T v_c)).
$$
其中求和是对总体的一个采样?

### skip-gram模型
设由$(w_c)$预测$(w_o)$的单个损失函数为$(F(w_o, w_c))$，那么skip-gram模型可以表示为
由中心单词预测周围的单词，损失函数为
$$
J = \sum_{-m \le j \le, j \neq 0} F(w_{c+j}, v_c).
$$

### CBOW模型
CBOW模型使用周围单词的词向量之和来预测中心单词$w_c$。
$$
\hat{v} = \sum_{-m \le j \le, j \neq 0} v_{c+j}
$$
他的损失函数为
$$
J = F(w_c, \hat{v})
$$

## 问题
- 为什么每一次SGD后需要对参数向量进行标准化？


## Multitask learning
共享网络前几层的权值，只针对不同任务改变最后一层的权值。

## Project
- 利用deeplearning去解决kaggle上的NLP问题。

## Reference
1. <http://cs224d.stanford.edu/syllabus.html>
2. An	Improved	Model	of	Seman4c	Similarity	Based	on	Lexical	Co-Occurrence Rohde	et	al.	2005
