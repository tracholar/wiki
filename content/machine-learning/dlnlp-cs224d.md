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


## word2vect
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
	- $( X_{apple} - X_{apples} \approxy X_{car} - X_{cars} \approxy X_{family} - X_{families})
	


## Reference
1. <http://cs224d.stanford.edu/syllabus.html>
2. An	Improved	Model	of	Seman4c	Similarity	Based	on	Lexical	Co-Occurrence Rohde	et	al.	2005
