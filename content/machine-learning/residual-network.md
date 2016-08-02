---
title: "残差网络"
layout: page
date: 2016-08-02
---
[TOC]

## 关于
Residual Networks 残差网络，何凯明，孙剑 @MSRA。

## 残差网络2015年的论文导读
### 摘要
152层残差网络，是 VGG net的8倍，但是复杂度更低，效果更好。
    - ImageNet 测试集错误率为 3.57%
    - COCO object detection dataset 28% 相对提升
    - ILSVRC & COCO 2015 competitions 第一名，on the tasks of ImageNet detection, ImageNet localization,
COCO detection, and COCO segmentation

### 导言
- 深度卷积网络（CNN）是图像分类问题的重大突破，
它可以自动学习底层/中层/高层特征。
特征的层级可以通过stack的方式（增加深度）得到提升。

CNN的重要论文：

1. Y. LeCun, B. Boser, J. S. Denker, D. Henderson, R. E. Howard,
W. Hubbard, and L. D. Jackel. Backpropagation applied to handwritten
zip code recognition. Neural computation, 1989
2. A. Krizhevsky, I. Sutskever, and G. Hinton. Imagenet classification
with deep convolutional neural networks. In NIPS, 2012.
3. P. Sermanet, D. Eigen, X. Zhang, M. Mathieu, R. Fergus, and Y. LeCun.
Overfeat: Integrated recognition, localization and detection
using convolutional networks. In ICLR, 2014
4. M. D. Zeiler and R. Fergus. Visualizing and understanding convolutional
neural networks. In ECCV, 2014.


近期研究表明，堆叠的深度是至关重要的因素。

1. K. Simonyan and A. Zisserman. Very deep convolutional networks
for large-scale image recognition. In ICLR, 2015
2. C. Szegedy, W. Liu, Y. Jia, P. Sermanet, S. Reed, D. Anguelov, D. Erhan,
V. Vanhoucke, and A. Rabinovich. Going deeper with convolutions.
In CVPR, 2015

ImageNet 的最佳结果都是很深的模型，从13层到30层。深度模型对其他的图像任务也有帮助。

1. K. He, X. Zhang, S. Ren, and J. Sun. Delving deep into rectifiers:
Surpassing human-level performance on imagenet classification. In
ICCV, 2015.
2. S. Ioffe and C. Szegedy. **Batch normalization**: Accelerating deep
network training by reducing internal covariate shift. In ICML, 2015
3. O. Russakovsky, J. Deng, H. Su, J. Krause, S. Satheesh, S. Ma,
Z. Huang, A. Karpathy, A. Khosla, M. Bernstein, et al. Imagenet
large scale visual recognition challenge. arXiv:1409.0575, 2014.

学习深度模型最大的问题在于 vanishing gradient，梯度消减！导致模型无法收敛。
利用这些技巧，几十层深度的模型也可以通过BP算法+SGD进行训练。

1. Y. Bengio, P. Simard, and P. Frasconi. Learning long-term dependencies
with gradient descent is difficult. IEEE Transactions on Neural
Networks, 5(2):157–166, 1994.
2. X. Glorot and Y. Bengio. Understanding the difficulty of training
deep feedforward neural networks. In AISTATS, 2010.

梯度消减的问题被很大程度上通过 normalized initialization 和 intermediate normalization layers 解决了

1. Y. LeCun, L. Bottou, G. B. Orr, and K.-R. Muller. Efficient backprop. ¨
In Neural Networks: Tricks of the Trade, pages 9–50. Springer, 1998.
2. A. M. Saxe, J. L. McClelland, and S. Ganguli. Exact solutions to
the nonlinear dynamics of learning in deep linear neural networks.
arXiv:1312.6120, 2013.
3. K. He, X. Zhang, S. Ren, and J. Sun. Delving deep into rectifiers:
Surpassing human-level performance on imagenet classification. In
ICCV, 2015.
4. S. Ioffe and C. Szegedy. Batch normalization: Accelerating deep
network training by reducing internal covariate shift. In ICML, 2015.

随着层数的加深，模型的性能逐渐饱和，然后迅速恶化。这个问题并不是由于过拟合，更深的模型导致更差的性能！
理论上来讲，深层模型应该可以做到比浅层模型更好的性能，可以设想多余的层是恒等变换，那么深层模型结果和浅层一样。
但是实际上的结果并非如此。

残差网络并不直接拟合目标，而是拟合残差。假设潜在的目标映射为$(\mathcal{H}(x))$，我们让非线性层学习残差
$(\mathcal{F}(x):=\mathcal{H}(x) - x)$，并提供一条短路（或直连）通道，使得输出为$(\mathcal{F}(x)+x)$。
我们假设优化残差比原始映射要简单！（假设！！！！）
在极端情况下，可以让非线性层置0，使得直接输出输入值。（我的思考：存在正则项的时候，这个确实更优，那是不是就证明残差网络不会比浅层网络更差了呢？！）
短路连接在这里可以跳过一层或者多层。单位短路通道（即短路通道直接输出输入的值）不增加计算复杂度也不增加额外的参数。
整个网络可以采用 end-to-end 使用SGD+BP算法，可以采用现有的求解器就能实现。


1. C. M. Bishop. Neural networks for pattern recognition. Oxford
university press, 1995.
2. B. D. Ripley. Pattern recognition and neural networks. Cambridge
university press, 1996.
3. W. Venables and B. Ripley. Modern applied statistics with s-plus.
1999

- ImageNet 论文： O. Russakovsky, J. Deng, H. Su, J. Krause, S. Satheesh, S. Ma,
Z. Huang, A. Karpathy, A. Khosla, M. Bernstein, et al. Imagenet
large scale visual recognition challenge. arXiv:1409.0575, 2014.
- CIFAR-10 论文：A. Krizhevsky. Learning multiple layers of features from tiny images.
Tech Report, 2009.

> We present successfully trained models on this dataset (CIFAR-10) with
over 100 layers, and explore models with over 1000 layers.
> Our ensemble has 3.57% top-5 error on the ImageNet test set, and won the 1st place in the ILSVRC
2015 classification competition.
> The extremely deep representations also have excellent generalization performance
> on other recognition tasks, and lead us to further win the 1st places on: ImageNet detection, ImageNet localization,
> COCO detection, and COCO segmentation in ILSVRC & COCO 2015 competitions.

### 相关的研究工作
#### Residual Representations.
- VLAD：H. Jegou, F. Perronnin, M. Douze, J. Sanchez, P. Perez, and
C. Schmid. Aggregating local image descriptors into compact codes.
TPAMI, 2012
- Fisher Vector：F. Perronnin and C. Dance. Fisher kernels on visual vocabularies for
image categorization. In CVPR, 2007.
- 这两种表达被应用在图像检索和分类中：

1. K. Chatfield, V. Lempitsky, A. Vedaldi, and A. Zisserman. The devil
is in the details: an evaluation of recent feature encoding methods.
In BMVC, 2011
2. A. Vedaldi and B. Fulkerson. VLFeat: An open and portable library
of computer vision algorithms, 2008

- 在矢量量化中，编码残差比编码原始矢量更加有效。

1. H. Jegou, M. Douze, and C. Schmid. Product quantization for nearest
neighbor search. TPAMI, 33, 2011.

- 低级视觉和计算机图形学中，为了解决PDE，采用Multigrid方法。。。。不懂，所以略。

#### Shortcut Connection
已被研究多日了（哈哈哈哈），早起的多层感知器研究在输入输出间单独加了一个线性层。
在另外两篇论文中，一些中间层直接连接到一个辅助的分类器，通过这种方式减少梯度消减。

1. C.-Y. Lee, S. Xie, P. Gallagher, Z. Zhang, and Z. Tu. Deeply supervised
nets. arXiv:1409.5185, 2014.
2. R. K. Srivastava, K. Greff, and J. Schmidhuber. Highway networks.
arXiv:1505.00387, 2015

等等其他，略。

highway networks 在短路链接采用了门函数，该门函数有参数需要通过数据学习。
当门关掉（为0值）时，网络就是传统的神经网络，而不是残差网络。

### Deep Residual Learning


## 参考
1. [Deep Residual Learning for Image Recognition](https://arxiv.org/abs/1512.03385)
2. [Deep	Residual	Networks](http://icml.cc/2016/tutorials/icml2016_tutorial_deep_residual_networks_kaiminghe.pdf)
