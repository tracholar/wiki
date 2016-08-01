---
title: "Recurrent neural network based language model"
layout: page
date: 2016-07-31
---
[TOC]

## 历程
- Bengio 采用神经网络做统计语言模型。前馈神经网络 + 固定窗长度
Yoshua Bengio, Rejean Ducharme and Pascal Vincent. 2003. A
neural probabilistic language model. Journal of Machine Learning
Research, 3:1137-1155
- Goodman 在Bengio 的基础上进行发展，发现这种简单模型比混合了其他多种方法的模型都要好。
Goodman Joshua T. (2001). A bit of progress in language modeling,
extended version. Technical report MSR-TR-2001-72.
- Schwenk 发现基于神经网络的模型，能够显著提升语音识别任务，在几个任务中比最好的系统都要好。
Holger Schwenk and Jean-Luc Gauvain. Training Neural Network
Language Models On Very Large Corpora. in Proc. Joint Conference
HLT/EMNLP, October 2005.

这种方法唯一的缺点是，需要采用固定的窗长度，一般在5-10。
递归神经网络理论上能够记忆任意长的信息，解决了这个问题。

另外一种能够实现长期依赖的方法：随机梯度下降？
Yoshua Bengio and Patrice Simard and Paolo Frasconi. Learning
Long-Term Dependencies with Gradient Descent is Difficult.
IEEE Transactions on Neural Networks, 5, 157-166.

## 模型
首先采用一个简单的递归神经网络，也叫Elman网络：
Jeffrey L. Elman. Finding Structure in Time. Cognitive Science,
14, 179-211

输入$(x(t))$为当前词向量$(w(t))$和上一时刻隐层状态$(s(t-1))连接成的新向量，这里用`+`表示链接，不是求和：
用隐层的状态来代表上下文信息。

$$
x(t) = w(t) + s(t-1)  \\\\
s_j(t) = sigmoid(\sum_i x_i(t) u_{ji})  \\\\
y_k(t) = softmax(\sum_j s_j(t) v_{kj})
$$

模型训练：标准的 BP + SGD，一开始学习率$(\alpha=0.1)$，每一个epoch之后，在验证集上检验，如果验证集的对数似然比增加了，
就继续训练，如果没有明显的改善，就将学习率减半$(\alpha_{new} = \alpha / 2 )$。如果之后仍然没有明显的改善，就停止训练。
一般在10-20个epoch就能收敛。

作者的模型没有明显的过拟合，即使在使用正则项的情况下，也没有明显的收益。

误差是基于交叉熵计算的，即交叉熵的导数：

$$
err(t) = desired(t) - y(t)
$$
