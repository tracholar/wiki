---
title: "Pixel RNN & CNN"
layout: page
date: 2017-01-18
---

## 关于
- 参考论文：
    - Den Oord A V, Kalchbrenner N, Kavukcuoglu K, et al. Pixel Recurrent Neural Networks[C]. international conference on machine learning, 2016: 1747-1756.
    - Conditional Image Generation with PixelCNN Decoders

## Pixel RNN
- 对自然图像建模，是无监督学习中的一个重要任务。
- Piexel RNN 将此问题当做一个序列预测的问题，即利用前面的像素(作为上下文 Context)预测后一个像素的离散概率，
建立条件概率模型。
- 采用残差连接，12层
- 预测离散值，直接采用一层 softmax ，作为一个多分类的问题。
- 上下文可以是自左上角到右下角，顺序的上下文；也可以是多尺度的上下文。

图像 $(x_{i,j}, i,j = 1,2,...,n)$ 作为一个序列 $(x_i, i=1,...,n^2)$，
联合概率

$$
p(x_1, .., x_n) = \Pi_{i=1}^{n^2} p(x_i|x_1,...,x_{i-1})
$$

- 并且，不同通道（RGB三个通道）不仅依赖前面的序列，还依赖与其他通道。分布的计算可以并行？！！！

## Pixel CNN
能够建模任意向量的条件概率；如果对图像的 class label 建模，可以生成多样的真实场景下的不同动物，目标，场景等；
如果对图像的CNN embedding向量建模条件概率，可以生成一个没有见到过的人的其他侧面、姿势下的画像！

Pixel CNN 也可以作为图像 autoencoder 的一个强大的解码器！
