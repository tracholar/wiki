---
title: "优化算法"
layout: page
date: 2016-07-02
---
[TOC]

## 梯度下降
- 批量梯度下降
- 随机梯度下降 SGD
- mini-batch 梯度下降

mini-batch梯度下降的问题
- 学习率难以选择，过小收敛太慢，过大会导致震荡
- 自动降低学习率需要预先指定条件，不是自适应的
- 算法对所有的参数采用相同的学习率
- 难以跳出鞍点，这是致命的问题


优化算法

### 动量方法
可以加速相关方向的收敛和抑制不相关方向的震荡。动量实际上是对梯度的指数平滑
$$
v_t = \gamma v_{t-1} + (1-\gamma) \nabla_\theta J(\theta). \\
\theta = \theta - \eta v_t.
$$

### Nesterov accelerated gradient
采用预测的点的梯度，而不是当前梯度。
$$
v_t = \gamma v_{t-1} + (1-\gamma) \nabla_\theta J(\theta - \gamma v_{t-1}). \\
\theta = \theta - \eta v_t.
$$

### Adagrad
解决了两个问题，自适应学习率 和 对不同频次特征采用不同的学习率，适应于稀疏特征 。
更新权值的方程为
$$
\theta_{t+1} = \theta_t - \frac{\eta}{\sqrt{G_t + \epsilon}} \odot g_t.
$$
$(G_t)$是对过去的梯度的平方的累积，因此学习率一直在减少。

### Adadelta (Google Inc, New York Univ, Matthew D.Zeiler)
改善Adagrad单调递减的学习率，它不是累计所有的梯度，而是设置了一个固定的时间窗$(w)$.
平滑平方误差$(E[g^2]\_t)$
$$
E[g^2]\_t = \gamma E[g^2]\_{t-1} + (1-\gamma) g_t^2. 
$$
权值更新方程为
$$
\Delta \theta_t = - \frac{\eta}{\sqrt{E[g^2]\_t +\epsilon}} \odot g_t.
$$
平滑步长的平方
$$
E[\Delta \theta^2]\_t = \gamma E[\Delta \theta^2]\_{t-1} + (1-\gamma) \Delta \theta^2_t.
$$
利用这个重新设计学习率使得单位一致
$$
\Delta \theta_t = - \frac{\sqrt{E[\Delta \theta^2] + \epsilon}}{\sqrt{E[\theta^2]\_t + \epsilon}} \odot g_t.
$$


### RMSprop (Hinton)
第一种Adadelta。

### Adam, adaptive moment estimation.
算法
$$
m_t = \beta_1 m_{t-1} + (1-\beta_1) g_t. \\
v_t = \beta_2 v_{t-1} + (1-\beta_2) g_t^2.
$$

$$
\hat{m}\_t = \frac{m_t}{1 - \beta_1^t}. \\
\hat{v}\_t = \frac{v_t}{1 - \beta_2^t}.
$$

更新方程
$$
\theta_{t+1} = \theta_t - \frac{\eta}{\sqrt{\hat{v}\_t + \epsilon}} \hat{m}\_t.
$$


### Additional strategies for optimizing SGD
每一次的循环都要打散数据的顺序。Zaremba and Sutskever训练LSTM的时候，
发现按照一定顺序反而能提高性能？

### Batch normalization
- 可以使用更高的学习率
- 移除或使用较低的dropout
- 降低L2权重















## Reference
1. <http://sebastianruder.com/optimizing-gradient-descent/>
2. <https://arxiv.org/pdf/1502.03167v3.pdf>
3. <http://blog.csdn.net/happynear/article/details/44238541>

