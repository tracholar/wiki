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
- 每一个单词有两个向量$(u, v)$. 最终的词向量是 $(u+v)$?
- 词向量的线性关系
	- $( X_{apple} - X_{apples} \approx X_{car} - X_{cars} \approx X_{family} - X_{families})$

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

> WHY？
>
> 一般而言，这种方式上的区别使得CBOW模型更适合应用在小规模的数据集上，能够对很多的分布式信息进行平滑处理；而Skip-Gram模型则比较适合用于大规模的数据集上。
>



## 问题
- 为什么每一次SGD后需要对参数向量进行标准化？
- 一般的交叉熵能够理解为最大似然估计么？


## Multitask learning
共享网络前几层的权值，只针对不同任务改变最后一层的权值。
总的代价函数是各代价函数（如交叉熵）之和。

## 神经网络TIPS
- 对词向量的监督训练的重新调整，对任务也有提升。 C&W 2011
- 非线性函数
	- sigmoid
	- tanh ： 对很多任务，比sigmoid好，初始值接近0，更快的收敛，与sigmoid一样容易求导
	- hard tanh : -1, if < -1; x, if -1 <= x <= 1; 1, if x > 1.
	- softsign(z) = z/(1 + |z|)
	- rect(z) = max(0, z)
ref: Glorot and	Bengio,	AISTATS 2011

- MaxOut network (Goodfellow et al. 2013)
- 梯度下降优化建议，大数据集采用SGD和mini-batch SGD，小数据集采用L-BFGS或者CG。
  大数据集L-BFGS Le	et	al.	ICML	2011。
- SGD的提升，动量
$$
v = \mu v - \alpha \nabla_{\theta} J_t(\theta)   \\
\theta^{new} = \theta^{old} + v
$$
- 学习率：adagrad， adam
- 防止过拟合：
	- 减少模型大小，隐藏节点数目等
	- L1 or L2正则化
	- 提前停止，选择在验证集合上最好的结果
	- 隐藏节点的稀疏约束，参考UFLDL教程
$$
KL(1/N \sum_{n=1}^N a_i^{(n)|0.001})
$$
	- dropout，输入以一定概率随机置0
	- denoise
- 超参数的搜索：随即搜索。
Y.	Bengio	(2012),	“Practical	Recommendations	for	GradientBased
Training	of	Deep	Architectures”		
- Xavier initialization 初始化策略

## Language Models
所谓语言模型就是建立单词的联合概率模型$(P(w_1,...,w_T))$.

### 神经网络语言模型 Bengio 2003
一个直接连接部分和一个非线性变换部分。输入为前n个词的词向量
$$
y = b + Wx + U tanh(d + Hx) .  \\
P(w_t|w_{t-1},...,w_{t-n+1}) = \frac{e^{y_{w_t}}}{\sum_i e^{y_i}}.
$$
缺点是窗口是固定的。记忆能力有限？

### 递归神经网络
基于之前见到的所有单词（理论上有无限长的时间窗）
> Condition	the	neural	network	on	all	previous
> words	and	tie	the	weights	at	each	time	step

设词向量列表为 $(x_1, x_2, ..., x_t, ..., x_T)$。L矩阵中的列向量。
$$
h_t = \sigma(W^{(hh) h_{t-1}} + W^{hx} x_{t}). \\
\hat{y}\_t = softmax(W^{(S)} h_t). \\
P(x_{t+1}=v_j|x_t, ..., x_1) = \hat{y}\_{t, j}.
$$
所有时刻的权值都是相同的。损失函数为交叉熵
$$
J^{(t)}(\theta) = -\sum_{j=1}^{|V|} y_{t,j} \log \hat{y}\_{t,j}. \\
J = - \frac{1}{T} \sum_t J^{(t)}
$$
Perplexity ???

- 训练困难，梯度容易衰减或者很大。Bengio et	al	1994
- 初始化策略
	- $(W^{(hh)})$ 初始化为单位阵
	- 非线性函数用rect函数替换
> Parsing	with	Compositional
> Vector	Grammars,	Socher	et	al.	2013
>
> A	Simple	Way	to	Initialize	Recurrent	Networks	of	Rectified	Linear
> Units,	Le	et	al.	2015
>
> On	the	difficulty	 of	training	Recurrent	Neural	Networks,	Pascanu et	al.	2013
>

- 梯度消减 Mikolov，如果梯度的范数超过阈值，就将梯度归一化到范数等于该阈值的向量或矩阵。

### 实现细节
- dropout正则化，在TensorFlow里面，可以使用`tf.nn.dropout`来实现。


## Deep-learning package zoom
- Torch
- Caffe
- Theano(Keras, Lasagne)
- CuDNN
- Tensorflow
- Mxnet


## Project
- 利用deeplearning去解决kaggle上的NLP问题。


## NLP benchmark tasks
### tasks
- Part-Of-Speech tagging
- chunking
- Named Entity Recognition (NER)
- Semantic Role Labeling (SRL)

### models
- CRF conditional random field

- 词语分布式假说：词的上下文相似，那么这两个词也相似

## Reference
1. <http://cs224d.stanford.edu/syllabus.html>
2. An	Improved	Model	of	Seman4c	Similarity	Based	on	Lexical	Co-Occurrence Rohde	et	al.	2005