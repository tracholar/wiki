---
title: "用神经网络做风格迁移"
layout: page
date: 2017-05-05
---
[TOC]

## 关于
风格迁移就是将图片变成某种风格的图片，例如为你的照片加上梵高的画的风格。

## 神经网络风格迁移
用神经网络做风格迁移的方法最早由 Leon A. Gatys 提出： Image Style Transfer Using Convolutional Neural Networks，2016

![风格迁移](/wiki/static/images/nnstyle01.png)

基本思想是用一个神经网络，对于原始照片 p ，用神经网络中的某一层特征 $(F_{ij}^l)$ (这里第一个下标表示特征通道，第二个下标表示空间维度，将二维空间压缩为一维便于表述，下同)作为内容的表达，
对于生成的照片 x，用神经网络中的同一层特征表达生成的图片的内容 $(P_{ij}^l)$，要求新生成的照片内容和原始照片内容接近，
即损失函数

$$
L_{content}(p, x, l) = \frac{1}{2} \sum_{ij}(F_{ij}^l P_{ij}^l)^2
$$

较小。

另一方面，需要生成的照片的风格和图片 a 相似，风格可以通过特征空间的 Gram 矩阵来表达。

$$
G_{ij}^l = \sum_{k} F_{ik}^l F_{jk}^l
$$

通过图片x不同层的Gram矩阵和图片 a 相似，实现风格的相似。

$$
E_l = \frac{1}{4N_l^2M_l^2} \sum_{ij} (G_{ij}^l - A_{ij}^l)^2 \\\\
L_{style}(a, x) = \sum_{l=1}^L w_l E_l.
$$

$(N_l, M_l)$分别是第l层特征数目和空间维度！

通过内容和风格损失函数最小化，实现内容和风格的 tradeoff。

$$
L = \alpha L_{content}(p, x) + \beta L_{style}(a, x)
$$

注意上述两个损失函数的特征都是做了max-pooling后的特征。

- 一些讨论：
    - 内容匹配如果选用约高层的表征，保留的细节越少，感受约平滑
    - 太慢了：用 K40 GPU，一张512x512图片的风格迁移也需要1小时！

![风格迁移](/wiki/static/images/nnstyle02.png)


## 实时风格迁移
论文：Perceptual Losses for Real-Time Style Transfer and Super-Resolution，Justin Johnson, Alexandre Alahi, and Li Fei-Fei，2016.

特点： perceptual loss functions， three orders of magnitude faster，real-time

- 超分辨 pixel loss function：不能很好的刻画语义的不同（例如两个只是平移了一点点的图片，像素的差异很大，但是语义差异较小）
- 解决之道：利用训练好的深度神经网络的高层语义空间的特征表达！

![实时风格迁移模型架构](/wiki/static/images/nnstyle03.png)

- 解决风格迁移test速度慢的方法是，用一个神经网络建模这种变换关系，目标用前者的损失函数！
- 包含两个网络： image trans- formation network $(f_W)$ ， loss network $(\phi)$；前者是一个深度残差网络，后者是一个预训练的 VGG 网络。

$$
W^* = \arg \min _ W \mathbf{E} _ {x, \{ y_i \}} \sum_{i} \lambda _ i l _ i (f_W (x), y_i)
$$

损失函数和 Gatys 一样！ TV 范数正则化！
