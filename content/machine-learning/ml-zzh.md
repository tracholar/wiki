---
title: "机器学习 - 周志华"
layout: page
date: 2016-06-28
---
[TOC]

## 关于
南大周志华老师写得机器学习是国内少有的中文机器学习教材，很多人推荐，
所以看看。

## 绪论

- NFL定理


## 集成学习
### 集成学习基本思想
利用很多个独立的（或者不同的）弱分类器，进行投票得到一个强的分类器。该理论可以由下面的推导得到

设每个弱分类器的性能为$(\epsilon)$，即
$$
P(h_i(x) \neq f(x)) = \epsilon
$$
这里$(h_i)$是弱分类器的判决函数，$(f)$是要学习的未知函数。
那么$(T)$个这样的分离器采用简单投票策略的分类器$(H)$判决错误，要求一半以上的判错，概率为
$$
P(H(x) \neq f(x)) = \sum_{k=0}^{T/2} \binom{T}{k} (1-\epsilon)^k \epsilon^{T-k}  \\\\
                  \le \exp(-\frac{1}{2}T(1-\epsilon)^2)
$$
后面这个式子基于[Hoeffding 不等式](https://en.wikipedia.org/wiki/Hoeffding%27s_inequality)

可以看出，只要T充分大，就可以使得误差足够小！！
个体学习器的准确性和多样性确实一个矛盾体，准确性高了之后，要增加多样性就会牺牲准确性。

两种类型：
- 个体学习期之前强依赖，需要串行，Boosting
- 不强依赖，可并行，Bagging 和 随机森林

### Hoeffding 不等式的推导
Hoeffding不等式是，如果随机变量$(X_i)$独立，切都在0到1之间，那么有
$$
P(\bar{X} - E \bar{X} > t) \le e^{-2 n t^2} \\
P(\bar{X} - E \bar{X} < -t) \le e^{-2 n t^2}
$$
$(\bar{X})$ 是这n个随机变量的均值。

[Hoeffding 引理](https://en.wikipedia.org/wiki/Hoeffding%27s_lemma)：随机变量的指数函数的期望

如果随机变量$(X)$满足均值为0，且$(a \le X \le b)，那么其指数函数的期望的上界为
$$
E e^{\lambda X} \le \exp(\frac{1}{8} \lambda^2(b-a)^2)
$$
该引理证明可以参考维基百科，它的基本思路是将闭区间上的指数函数扩大为链接两端点的线性函数，
然后对两边求期望，这样讲期望去掉了。剩下的就是求去掉期望后的函数的上界（指数界）的问题了。

[Markov 不等式](https://en.wikipedia.org/wiki/Markov%27s_inequality)：随机变量概率和期望的不等式

随机变量$(X)$大于0，那么对正数$(a)$有
$$
P(X \ge a) \le E(X)/a
$$
证明很简单，将概率转换为示性函数的期望表示即可。因为
$$
I(X \ge a) \le X / a
$$
两边求期望就是了。

利用这个引理和 Markov 不等式可以证明前面的不等式，
$$
P(\bar{X} - E \bar{X} > t) = P(e^{s(\bar{X} - E \bar{X})} > e^{st}) \\\\
            \le e^{-st} E(e^{s(\bar{X} - E \bar{X})})   \\\\
            \le e^{-st} e{\frac{1}{8 n} s^2 }
$$
注意最后一个不等式是将前一个式子展开成n个乘积后，再缩放的。
因为上式对所有的s都成立，所以可以取一个最小的作为它的上界，
利用二次函数的性质可得上界为Hoeffding 不等式的右边。

利用Hoeffding 不等式，令其中的独立随机变量为$(y_i = I(h_i(x) = f(x)))$，即每个分类器
是否判决正确。设$(\bar{y}})$是这T个变量的均值，那么有
$$
P(H(x) \neq f(x)) = P(\bar(y) < 0.5) = P(\bar{y} - E \bar{y} < 0.5 - \epsilon) \\\\
    \le \exp(-2 T (0.5-\epsilon)^2)
$$
这就是前面集成学习基本理论里面的那个不等式。

### Boosting
