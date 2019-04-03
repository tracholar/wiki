---
title: "少样本学习"
layout: page
date: 2019-04-01
---
[TOC]

## 关于
> 人类非常擅长通过极少量的样本识别一个新物体，比如小孩子只需要书中的一些图片就可以认识什么是“斑马”，什么是“犀牛”。在人类的快速学习能力的启发下，研究人员希望机器学习模型在学习了一定类别的大量数据后，对于新的类别，只需要少量的样本就能快速学习，这就是 Few-shot Learning 要解决的问题。 
> <https://mp.weixin.qq.com/s/sp03pzg-Ead-sxm4sWyaXg>

## 问题
- flow-shot学习和one-shot学习本质上的问题是,给你C个类别,但是每个类别只有K个样本,现在有个新的样本,要你识别这个样本是哪个类别, 这种就叫C-way K-shot学习
- 一般的分类问题,每个类别的样本数目K一般很大,几百到几万, 这里一般只有几个,所以很难正常建立一个分类模型去识别是哪个类别
- 这个类似于人脸匹配的任务,数据库中只有人脸的1张图片,有个新的人脸来了,要识别是谁

## 基于模型的方法
- 没看懂,比较复杂

## 基于度量的方法
- 对样本间距离建模,同类靠近,不同类的远离

### 孪生网络
- 论文: Siamese Neural Networks for One-shot Image Recognition
- 训练好的网络不但可以预测未见过的样本但是见过的类别的数据,还可以预测没有见过类别的数据?
- 问题: 预测那些我们只见过一次的类别 one-shot learning
    - 区别于 zero-shot learning, 一次都不见
- 相关论文:
    - Fe-Fei, Li, Fergus, Robert, and Perona, Pietro. A bayesian approach to unsupervised one-shot learning of object categories. In Computer Vision, 2003. Proceedings. Ninth IEEE International Conference on, pp. 1134– 1141. IEEE, 2003.
    - Fei-Fei, Li, Fergus, Robert, and Perona, Pietro. One-shot learning of object categories. Pattern Analysis and Ma- chine Intelligence, IEEE Transactions on, 28(4):594– 611, 2006.
    - Lake, Brenden M, Salakhutdinov, Ruslan, Gross, Jason, and Tenenbaum, Joshua B. One shot learning of simple visual concepts. In Proceedings of the 33rd Annual Con- ference of the Cognitive Science Society, volume 172, 2011.
- 基本方法
    1. 不是降图片作为多分类来做,而是将**图片对**当做二分类问题
    2. 对每一对有标注的图片(a, b) 用同一个CNN抽取特征向量va, vb
    3. 计算加权距离  |alpha(va - vb)| (L1, L2 距离), alpha是这个模型的参数,用来给不同维度加权
    4. 用 sigmoid(加权距离) 作为这两个图片属于同一类别的概率
    5. 利用二分类损失函数进行训练
- 数据集的构造
    1. 有标注的同一类别的图片不能太少,否则无法构造相同类别的图片对
    2. 成对构造同类和不同类别的图片对,也就是最终模型预测的正负例是1:1,其他比例影响是什么?
    3. 测试集的构造,选取一类图片的两张,把其中一张用来预测,其他类别抽出一张,除了用来预测的那1张外,剩下的C张用作支撑集,让模型预测那一张是跟支撑集里面的哪个类比一致!
    
$$
y \sigma(\alpha^T(v_a - v_b)) + (1-y)\sigma(-\alpha^t(v_a - v_b))
$$

- 其他损失函数, 相似损失直接用距离, 不像似损失要求距离大于某个数值
$$
y D(a, b) + (1-y) (max(0, m - D(a, b)))^2
$$


