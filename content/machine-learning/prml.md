---
title: "Pattern Recognition and Machine Learning - Bishop"
layout: page
date: 2016-05-30
---
[TOC]

# 关于 
Bishop写得PRML无疑是机器学习领域的权威著作，是加强机器学习理论知识必看的书籍之一。
这里是我在阅读这本书时记录的笔记和自己的一些初步思考。


# 引言
## 曲线拟合
设曲线拟合的目标变量为$(t)$，$(N)$个训练集为
$( \mathbf{x} = (x_1, ..., x_N)^T )$，对应的目标值为
$( \mathbf{t} = (t_1, ..., t_N)^T )$。假设估计误差服从
高斯分布
$$
p(t|x, \mathbf{w}, \beta) = \mathcal{N} (t| y(x, \mathbf{w}), \beta^{-1}) 
$$
这里的$(\mathbf{w})$是待估计的参数，$(\beta^{-1})$对应于
估计误差的方差。此时，按照最大似然准则估计的参数就是最小二乘
法的结果，也就是最小化均方误差。而$(\beta^{-1})$的最大似然估计为
模型预测值与实际值的均方差。
$$
\frac{1}{\beta} = \frac{1}{N} \sum_{n=1}^N [y(x_n,\mathbf{w}_{ML}) - t_n]^2.
$$

如果我们对参数的具有一定先验知识，可以进一步采用最大后验概率估计，
得到更好的结果。作为一个特例，如果认为
$$
p(\mathbf{w}|\alpha) = \mathcal{N}(\mathbf{w} | 0, \alpha^{-1} \mathbf{I})   \\\\
					= (\frac{\alpha}{2\pi})^{(M+1)/2} \exp(-\frac{\alpha}{2} \mathbf{w}^T \mathbf{w}).
$$
那么此时的最大后验概率估计为带$(L_2)$正则项的估计，
它最小化
$$
\frac{\beta}{2} \sum_{n=1}^N [y(x_n, \mathbf{w}) -t_n]^2 + \frac{\alpha}{2} \mathbf{w}^T \mathbf{w}
$$
也就是说正则项是对模型参数的一种先验知识，$(L_2)$正则项代表高斯先验。
那$(L_1)$代表laplace先验(待求证)？



## 第11章 采样方法
- 计算期望 $(E[f] = \int f(z)p(z) dz)$
- 通过采样一系列$(z_i ~ p(z))$ ,从而近似地用有限和来近似上述期望$(\hat{f} = \frac{1}{L}\sum_{l=1}^Lf(z_l))$
- 估计误差可以用上述统计量的方差来刻画

### 基本采样方法
- 一维分布变换关系 $(p(y) = p(z) \frac{dz}{dy})$
- 分布f(y)可以通过从[0,1]间均匀随机变量z,通过变换$(F^{-1}(z))$得到, F是y的累积分布函数
- 指数分布$(y = F^{-1}(z) = -\frac{1}{\lambda} ln(1-z))$
- 对于多维分布,通过雅克比矩阵 $(p(y_1, ..., y_M) = p(z_1, ..., z_M)|\frac{\partial(z_1,...,z_M)}{\partial(y_1,...,y_M)}|)$
- 高斯分布生成: Box-Muller方法
    - $(z_1,z_2)$独立采样自服从(-1,1)之间的均匀分布
    - 只保留在单位圆内的点,即满足$(z_1^2 + z_2^2 \le 1)$,从而得到单位圆内的均匀分布
    - 构造新的变量$(y_1 = z_1(\frac{-2 ln r}{r^2})^{1/2}, y_2 = z_2 (\frac{-2 ln r}{r^2})^{1/2})$. 其中r是半径, 那么y1和y2就是两个独立的服从标准正太分布的随机变量
    - 假设两个随机变量z1,z2服从正太分布,那么对应的极坐标r和$(\theta)$对应的分布是 $(r e^{-\frac{r^2}{2}})$ 和 $(\frac{1}{2\pi})$的均匀分布。显然这两个分布可以通过均匀分布通过逆变换的方法容易得到。
    $$
    r = \sqrt{-2 ln U_1} \\\\
    \theta = 2\pi U_2
    $$
    如果是从单位圆内的变换过来,那么容易验证对应的$(\theta)$即满足第二个均匀分布,而半径s分布为$(2s)$,所以根据概率恒等关系有
    $$
    2s ds = r e^{-\frac{r^2}{2}} dr
    $$
    解这个微分方程可得 $(r = \sqrt{ln s} )$, 同时$(cos(\theta) = x/s, sin(\theta) = y/s)$,于是有 
    $$
    z_1 = r cos(\theta) = x \sqrt{ln s}/s, \\\\
    z_2 = r sin(\theta) = y \sqrt{ln s}/s
    $$
- 一般的高斯分布$(\mu, \Sigma)$,将协方差矩阵分解为$(LL^T)$,那么对独立的标准正太分布向量z,做变换$(\mu + L z)$ 即可得到目标分布

```python
def urnd():
    return random.random()
    
def gaussian_rnd():
    r = sqrt(- 2 * log(urnd()))
    theta = 2*pi*urnd()
    return r * cos(theta)
```

### 拒绝采样
- 目标分布p(z)不简单(比如没有解析表达式,或者有但是逆变换很困难etc),但是p(z)对给定的z还是要容易算出来
- 对于已有的某种分布q(z),选取足够大的k使得$(kq(z) \le p(z))$
- 每次采样的时候,首选从q中采样一个z,然后随机一个(0, kq(z))均匀分布u,如果$(u>p(z))$则拒绝该样本,否则就得到目标分布的一个采样。容易验证这样采出来的z分布为 $(q(z) \times p(z)/kq(z) = p(z))$

![拒绝采样](/wiki/static/images/prml-reject-samping.png)

- kq尽可能和目标分布接近,否则采样效率很低,比如下述例子

```python
## 利用拒绝采样a>1和b>1的beta分布随机变量
def reject_samping_beta(a, b):
    assert a>1 and b>1
    reject = True
    z = 0
    while reject:
        z = urnd()
        p = z**(a-1) * (1-z) ** (b-1) / beta(a, b)
        u = urnd()
        reject = u > p
    return z
```


