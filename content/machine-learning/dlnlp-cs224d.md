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
v = \mu v - \alpha \nabla_{\theta} J_t(\theta)   \\\\
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
y = b + Wx + U tanh(d + Hx) .  \\\\
P(w_t|w_{t-1},...,w_{t-n+1}) = \frac{e^{y_{w_t}}}{\sum_i e^{y_i}}.
$$
缺点是窗口是固定的。记忆能力有限？

### 递归神经网络
基于之前见到的所有单词（理论上有无限长的时间窗）
> Condition	the	neural	network	on	all	previous
> words	and	tie	the	weights	at	each	time	step

设词向量列表为 $(x_1, x_2, ..., x_t, ..., x_T)$。L矩阵中的列向量。
$$
h_t = \sigma(W^{(hh) h_{t-1}} + W^{hx} x_{t}). \\\\
\hat{y}\_t = softmax(W^{(S)} h_t). \\\\
P(x_{t+1}=v_j|x_t, ..., x_1) = \hat{y}\_{t, j}.
$$
所有时刻的权值都是相同的。损失函数为所有时刻交叉熵的平均值
$$
J^{(t)}(\theta) = -\sum_{j=1}^{|V|} y_{t,j} \log \hat{y}\_{t,j}. \\\\
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
- 补充对RNN求梯度的理论推导

- 利用RNN学到的语言模型，生成词

### 实现细节
- dropout正则化，在TensorFlow里面，可以使用`tf.nn.dropout`来实现。

### bidirectionl RNNS 双向RNN
每一个隐层存在两个变量，$(h^L, h^R)$。

$$
h_t^R = f(W^R x_t + V^R h_{t-1}^R + b^R) \\\\
h_t^L = f(W^L x_t + V^L h_{t+1}^L + b^L) \\\\
y_t = g(U [h_t^R; h_t^L] + c)
$$

问题：$(h_{t+1}^L)$的值怎么来？

数据集 MPQA	1.2	corpus

## Deep-learning package zoom
- Torch
- Caffe
- Theano(Keras, Lasagne)
- CuDNN
- Tensorflow
- Mxnet


## RNN 机器翻译
### 传统统计机器翻译：
参考CS224n

- 翻译模型 p(f|e) 和 语言模型 f(e)，然后得到目标语 Decoder : $(argmax_e p(f|e) p(e))$
- alignment

### RNN 模型
- 最简单的encoder + decoder 模型：
	- Encoder：利用RNN将句子变成一个向量  $(h_t = f(W^{hh} h_{t-1} + W^{hx} x_t))$
	- Decoder：将句子向量变成一句话  $(h_t = W^{hh} h_{t-1}, y_t = softmax(W^S h_t))$
	- 损失函数：最小化所有输出结果的交叉熵， $(\max_{\theta} \frac{1}{N} \sum_{n=1}^N \log p_{\theta} (y^{(n)}| x^{n}))$
用句子向量作为中间桥梁。

- 对上述模型的改进措施
	- 对Encoder和Decoder训练不同的权值
	- 对Decoder，采用三个变量计算隐层，上一个时间的隐层$(h_{t-1})$，Encoder最后的状态c，上一个输出结果$(y_{t-1})$：$(h_t = \phi(h_{t-1}, c, y_{t-1}))$
	- 多层网络训练
	- 双向RNN Encoder
	- 反过来训练？
	- 门限递归单元GRU

论文：2014年，Kyunghyun Cho, Yoshua Bengio, Learning Phrase Representations using RNN Encoder-Decoder for Statistical Machine Translation

### GRU
门限RNN单元

- 更新门$(z_t)$，基于当前输入和上一时刻隐层状态
- 重置门$(r_t)$，如果重置门接近于0，那么当前隐层状态将忘记之前的隐层状态，只依赖当前输入
- 新的隐层$(\widetilde{h}_t)$，另外一个的隐层状态，最终的隐层状态是基于更新门组合这个新隐层和上一时刻的隐层

$$
z_t = \sigma(W^{(z)} x_t + U^{(z)} h_{t-1})  \\\\
r_t = \sigma(W^{(r)} x_t + U^{(r)} h_{t-1})  \\\\
\widetilde{h}\_t = tanh(W x_t + r_t \circ U h_{t-1})	\\\\
h_t = z_t \circ h_{t-1} + (1 - z_t) \circ \widetilde{h}_t
$$

当重置门接近0，允许模型忘记历史，实现短期依赖。
当更新门接近1，简单复制上一时刻的隐层，导致更少的vanishing gradients，实现长期依赖。

### LSTM
跟多的门，每一个门都是当前输入和上一时刻的隐层的函数，只是权值不同。

- 输入门 $(i_t)$
- 忘记门 $(f_t)$
- 输出门 $(o_t)$

- 新的存储单元：$(\widetilde{c}\_t = tanh(w^c x_t + U^c h_{t-1}))$
- 最终的存储单元：$(c_t = f_t \circ c_{t-1} + i_t \circ \widetilde{c}\_t)$
- 新的隐层：$(h_t = tanh(c_t))$

存储单元可以保存输入信息，除非输入让它忘记或者重写它；它可以决定是否输出信息或者只是简单地保存信息。

论文：2014, Sutskever, Sequence to Sequence Learning with Neural Networks, Google inc

比赛：WMT	2016 competition

### 更多的门 GRUs
Gated Feedback Recurrent Neural	Networks, Chung	et	al, Bengio.	2015

更多的门来控制多个隐层之间互相连接。
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


## NLP 综述
论文： Natural Language Processing (almost) from Scratch

文章提出一种统一的神经网络结构，可以用在很多自然语言处理任务当中：POS tagging，chunking，NER，semantic role labeling。
这种方案可以不用针对特定任务进行特征工程和先验知识。

### POS tagging：数据集：Wall Street Journal (WSJ) data
- Toutanova et al. (2003） 最大熵 + bidirectional dependency network => 97.24%
- Gim ́enez and Marquez (2004) SVM + 双向维特比译码 => 97.16%
- Shen et al. (2007) 双向序列分类 => 97.33%

### Chunking
句法成分标记

- Kudoh and Matsumoto (2000)： 93.48%
- (Sha and Pereira, 2003; McDonald et al., 2005; Sun et al., 2008)：random fields


### Named Entity Recognition
- Florian et al. (2003)： F1 => 88.76%
- Ando and Zhang (2005): 89.31%

### Semantic Role Labeling
[John]ARG0 [ate]REL [the apple]ARG1

预测关系词？

### 神经网络方法
1. 将词映射到特征向量
2. 在一个窗口中，将词对应的特征向量拼接成一个大的向量，作为下一层的输入
3. 一个正常的神经网络，线性层+非线性层 的多次堆叠！

#### Transforming Words into Feature Vectors
将词 embedding 到一个低维的词向量，也可以理解为一个查找表 W，输入词的索引 idx，输出W(:, idx)，
而参数 W 通过BP算法学习！

如果加入其它离散特征，把每一个特征做同样的 embedding 操作，每一个特征都有一个查找表 $(W_k)$，这些参数都要通过后续学习。
最终输出的特征向量是这些离散特征 embedding 后的特征拼接而成！

经过这一层后，每一个词输出为一个 $(d_{wrd})$ 维的向量。

#### Extracting Higher Level Features from Word Feature Vectors
- Window Approach： 加窗，只使用词的邻居词，将窗内的词对应的向量拼接起来，成为一个大的向量，设窗口为 $(k_{sz})$，
那么，加窗后的向量长度为 $(d_{wrd} \times k_{sz})$。将这个固定长度的向量输入到一个正常的多层全连接神经网络。

边界效应：对于句子起始和结束的词，在前后补充半个窗口的特殊词“PADDING”，这个词对应的词向量也是通过学习得到的。等价于学习序列的开始和结束！

- Sentence Approach：利用时间一维卷积层 Waibel et al. (1989) ，Time Delay Neural Networks (TDNNs)。
卷积层可以学习局部特征

卷积层，Max pooling层。最后得到的固定长度的特征向量进入一个标准的全连接神经网络。
边界通过相同的 PADDING 方法解决不同长度的问题！

### 训练
#### Word-Level Log-Likelihood
每一个词是独立的，最大化极大似然函数等价于最小化交叉熵损失函数。

#### Sentence-Level Log-Likelihood
词的标注之间是不独立的！从一个TAG到另一个TAG可能是不允许的。
设TAG之间转移用score矩阵 A 表示，最终对一个句子$([x]\_1^T)$ 标注序列$([i]\_1^T)$ 的score是
标注序列转移score和神经网络输出的score之和！

$$
s([x]\_1^T, [i]\_1^T, \hat{\theta}) = \sum_{t=1}^T ([A]\_{[i]\_{t-1}, [i]\_t} + [f_{\theta}]\_{[i]\_t,t}) \\\\
\hat{\theta} = \theta \union \{A_{ij}, \forall i,j \}
$$

最后输出的是score对所有路径的softmax归一化后的值，解释为路径的条件概率。
因为路径数目随句子长度字数增长，所以分母上的求和项也有指数个。
幸运的是，可以在线性时间复杂度内求得。

优化算法：动态规划，Viterbi algorithm ！

- 其他方法：
	- Graph Transformer Networks
	- Conditional Random Fields

#### Stochastic Gradient
stochastic gradient (Bottou, 1991)

目标函数的可微性，因为 max 层的引入，导致在某些点不可微，但是随机梯度下降仍然可以找到极小值点！

### 结果
很不幸，神经网络的结果都不如 baseline！无监督学习加入可能有用？！

### 更多的未标注数据
利用更多未标注数据，学习词向量，然后初始化 embedding 权重！

- 数据集：
	- English Wikipedia
	- Reuters RCV1 (Lewis et al., 2004) dataset

- 排序准则和熵准则

### Training Language Models
训练一系列词典增大的神经网络，每一个网络用之前的网络初始化 embedding 层！(Bengio et al., 2009)


## Sequence to Sequence Learning with Neural Networks
序列到序列学习！经典文献。来自 Google。

DNN 只能对固定长度的输入，进行建模，但是很多时候需要实现序列到序列的学习：语音识别，机器翻译！

序列学习的方法：用一个RNN（通常是LSTM）将一个序列编码成一个大的固定长度的向量（Encoder），
然后再用一个RNN将该向量解码成一个新的序列（Decoder）。
译码方案：Beam-search decoder。

评估指标：BLEU？

一个 trick： 将序列（句子）反序后，加入训练集。？

### 模型
标准的RNN需要将输入和输出对齐才能用。
利用标准的 RNN 做编码和译码的方案： Cho et al. [5]
这种方案的问题在于标准的RNN难以学到长期依赖。

LSTM可以学到长期依赖，编码器将输入序列编码到一个固定长度的向量，解码器是一个标准的 LSTM-LM 形式的解码结构，
用上述向量初始化该结构的初始隐层状态！解码序列直到输出 **结束标记** 才停止。

- 这篇文章的创新点：
	- 编码器和解码器采用不同的 LSTM，参数不同，同时学习两种序列的结构
	- 深层 LSTM 比浅层好，用了4层
	- 将输入反序而输出不反序，再进行训练， LSTM学得更好！

### 实验
WMT’14 English to French MT task

解码通过一个从左到右的简单 beam search 方案，每次保存 B 个最可能的前缀！
每次将这B个前缀扩展，然后再保存最可能的 B 个新的前缀，直到都碰到结束符！
B=1 就很好了！增加B的值，收益不是很大！？

将输入反序进行训练带来的收益很大！

- 正序：1,2,3 => a,b,c
- 反序：3,2,1 => a,b,c

> the LSTM’s test perplexity dropped from 5.8 to 4.7, and the test BLEU scores of its decoded translations increased from 25.9 to 30.6

作者给了一个简单的解释：不反序，第一个词..... 没看懂

比 baseline 好，但比最好的结果还是稍微差点。
baseline：phrase-based SMT system？

H. Schwenk. University le mans. http://www-lium.univ-lemans.fr/~schwenk/cslm_joint_paper/,
2014. [Online; accessed 03-September-2014]

## RNN for QA
- 一般的 RNN 应用: 词序列（句子） => 连续值或者有限的离散值
- QA： 词序列（句子） =>  富文本

### Matching Text to Entities: Quiz Bowl
对同一个结果，需要有冗余的样本进行学习。

模型：dependency-tree rnn (dt-rnn)，对语法变化具有鲁棒性，同时训练问题和答案，映射到同一个向量空间。

问题：词之间的这个树结构怎么得到的？！De Marneffe et al., 2006

- 每一个关系的终止节点（词）通过矩阵 $(W_v \in \mathbb{R}^{d \times d})$ 映射到隐层。
- 中间节点也关联一个词，通过下式将该词和子节点映射到隐层。

$$
h_n = (W_v w_n +b + \sum_{k \in K(n)} W_{R(n, k)} h_k)
$$

权重 $(W_{R(n, k)})$ 描述当前词n与子节点隐层 $(h_k)$ 之间的组合关系。

设S是所有的节点，给定一个句子，设词c是正确结果，Z是所有不正确的结果集合。那么对这一个样本，损失函数为

$$
C(S, \theta) = \sum_{s \in S} \sum_{z \in Z} L(rank(c, s, Z)) \max(0, 1- x_c h_s + x_z h_s) \\\\
L(r) = \sum_{i=1}^r 1/i
$$

## Image retrieval by Sentences
论文：Grounded Compositional Semantics for Finding and Describing Images with Sentences, 2013, Richard Socher, **Andrej Karpathy**, Quoc V. Le*, Christopher D. Manning, **Andrew Y. Ng**

DT—RNN：CT-RNN, Recurrent NN

将图像和句子映射到同一个空间，这样就可以用一个来查另外一个了。

zero shot learning

最简单的将词向量变成句子或短语的方式是，简单地线性平均这些词向量，（词向量中的 bag of word）。
RNN 的方法就没有这些问题。

句子 parsed by the dependency parser of de Marneffe et al. (2006)

每一个句子被表达为一个词，词向量序列， $(s = ( (w_1, x_{w_1}), (w_2, x_{w_2}), ..., (w_n, x_{w_n}) ))$。
parse后得到树状结构，可以用(孩子，父亲)对来表示 $(d(s) = \{ (i, j) \})$。
最后输入 DT-RNN 的样本是两者组成的 (s, d)

图像特征提取是用 DNN(Le et al., 2012)，利用未标注的web图片和标注的 ImageNet 训练学出来的，dim=4096。
输入：200x200，使用了三个层：滤波（CNN），pooling(L2)，local contrast normalization.

local contrast normalization: 将输入的子图块（文章中5x5）减去均值，除以方差进行归一化。 有点像 layer nomalize.

- Multimodal Mappings
	- 固定图片特征4096维
	- 联合训练图片特征向量映射到联合空间矩阵和DT-RNN参数。

损失函数：大间隔损失函数，略。


## Deep Visual-Semantic Alignments for Generating Image Descriptions
论文：Deep Visual-Semantic Alignments for Generating Image Descriptions, **Andrej Karpathy**, **Li Fei-Fei**

- 图像特征： RCNN
- 文本特征： 双向RNN

将文本特征和图像特征映射到同一个空间，并学习图像块和文本的对齐向量。


## RNN语言模型
Recurrent neural network based language model

动态模型：在训练的时候，每个样本在多个epoch出现，测试的时候，也更新模型，不过一个样本只出现在一个epoch中

cache techiques。

### 优化技巧
1. 将出现频率低于某个阈值的词映射为同一个词，称作rare token。条件概率变为：

$$
p(w_i(t+1)| w(t), s(t-1)) = \begin{cases}
							y_{rare}(t)/C_{rare}, w_i(t+1) is rare. \\\\
							y_i(t), otherwise
							\end{cases}
$$

s 是隐层，即上下文向量。因为 rare 是多个词概率之和，所以对某个词来说，它的概率就要把 rare 的概率除以rare词的数目。
对这些词来说，概率都是一样的。

RNN LM： 6小时； Bengio：几天，24小时 sampling

## A Neural Probabilistic Language Model
Yoshua Bengio，2003.

传统的 n-gram 模型的问题，维数灾难。随着n增大，测试集中的 n-gram 是训练集中没有的概率越来越大。
解决之道：神经网络模型，词的分布式表达。

1. 学习到了词的分布是表达
2. 基于这种词的表达的条件概率模型，语言模型


维数灾难：建模离散随机变量的联合分布时，10个变量就有 |V|^10 个可能的状态（参数）。
而建模连续变量就容易一些，可以用神经网络，等等。（连续函数一般具有局部光滑，即局部可微）。

传统n-gram的问题：考虑的近邻词数目太少，最好的结果也就是trigram；
没有考虑到词之间的相似性。

维数灾难解决办法：词的分布式表达。

n-gram 语言模型，也会通过所有的gram进行平滑。

perplexity， 条件概率的倒数的集合平均值！

impotance sampling: Quick training of probabilistic neural nets by importance sampling， Bengio, 2003.

## EXTENSIONS OF RECURRENT NEURAL NETWORK LANGUAGE MODEL
Toma ́sˇ Mikolov。问题，计算复杂度太高。要计算 softmax

BPTT， 4-5步就可以达到不错的精度了。

将输出层分解，减少计算量？？？

## Opinion Mining with Deep Recurrent Neural Networks
将观点挖掘作为序列标注的问题。

- BIO tagging scheme：
	- B，表达观点的开始位置
	- I，表达观点的词中
	- O，词外

3-4层后，性能也上不去了！

## Gated Feedback Recurrent Neural Networks
1. 多层 RNN（简单RNN， LSTM， GRU都可以）
2. 不同层RNN互相连接，通过全局 rest 门控制不同层之间的交互

其中，第i层到第j层的全局 reset 门，由当前时刻第 j-1 层的输入（对于第一层，是x），
以及上一时刻所有的隐层共同决定，如下式：

$$
g^{i \rightarrow j} = \sigma(w_g^{i \rightarrow j} h_t^{j-1} + u_g^{i \rightarrow j} h_{t-1}^{\*})
$$

隐层的更新，将单层隐态更新方程中上一时刻隐层项，变成对所有隐层通过全局 reset 门的组合。
所有类型的 RNN如LSTM，GRU都适用，详细见论文。

- 试验任务：
	- character-level 语言模型，评估指标 bits-per-character
	- python 程序结果预测：输入一段python程序，要求预测输出结果。every input script ends with a print statement。 属于 sequence to sequence 的问题。通过调节程序的难度，可以在不同难度上评估不同模型的优劣。

## Recursive Deep Models for Semantic Compositionality Over a Sentiment Treebank
作者：Richard Socher, Alex Perelygin, Jean Y. Wu, Jason Chuang, Christopher D. Manning, **Andrew Y. Ng** and Christopher Potts

Semantic vector spaces：？
