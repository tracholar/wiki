---
title: "强化学习简介"
layout: page
date: 2018-02-02
---
[TOC]

## 关于
Reinforcement Learning: An introduction 读书笔记


## 动态规划(DP)方法
当环境已知时，即状态转移概率$(P(s'|s, a))$和回报$(r(s, a))$也知道的情况下，根据HJB方程

$$
V(s) = \max_a \sum_{s'}P(s'|s, a) [r(s, a) + \gamma V(s')]
$$

因为求最大值操作的存在，上述方程是值函数V的非线性方程，所以无法直接求解。
动态规划方法求解值函数方法有两种：值迭代与策略迭代。

采用值迭代的理论依据是非线性方程的迭代求解方法，从HJB方程来看，值函数V可以看做右边非线性算子的不动点，容易验证当$(\gamma<1)$时，该非线性算子是压缩映象，值迭代比收敛于不动点！它也可以看做往前一步的期望值作为目标，进行迭代。

$$
V_{k+1}(s) = \max_a \sum_{s'}P(s'|s, a) [r(s, a) + \gamma V_k(s')]
$$


另一种方法是交替优化策略和值函数，好处是在策略固定时，HJB方程没有max操作，是线性方程！
策略迭代分为两步

**策略评估**：将选择动作的策略固定，求解策略的值函数，因为策略固定，非线性的HJB方程变成线性方程了！所以可以采用线性方程的所有求解方法进行求解，比如高斯消元法、雅克比迭代法等等。

$$
V^{\pi}(s) = \sum_{s'}P(s'|s, a) [r(s, a) + \gamma V^{\pi}(s')], a=\pi(a|s)
$$

**策略提升**，对于上述评估出来的值函数，提升策略

$$
\pi'(s) = \arg\max_a Q^{\pi}(s, a) \\\\
Q^{\pi}(s, a) = r(s, a) + \gamma V^{\pi}(s')
$$

策略提升可以保证值函数序列是单调递增序列！

## 蒙特卡洛(MC)方法
当环境未知的时候，无法采用动态规划方法求解，需要根据经验数据进行评估。
蒙特卡洛法通过在线学习的方法，将整个动作序列执行至终态，根据实际获得的总回报$(G = \sum_{t=0}^T \gamma^t r(s_t, a_t))$来进行策略评估！当执行多次之后，$(G)$的平均值可以作为该策略下，初始状态s的值函数。这个过程也可以用迭代的方法描述

$$
Q^{\pi}(s, a) \leftarrow Q^{\pi}(s, a) - \alpha[Q^{\pi}(s, a) - G] \\\\
\alpha = 1/N
$$

该迭代过程可以看做用观测到的回报G作为学习目标的随机梯度下降

$$
J(Q^{\pi}) = \frac{1}{2}\sum_i (Q^{\pi}(s, a) - G_i)^2
$$

蒙特卡洛法的特点，必须等到动作序列执行完毕后，才能进行评估。
但是一个序列可以更新多个状态的值，利用马尔科夫链的性质，从这条链中间任何一个状态开始，都可以得到该状态的一个值函数的采样值！

为了有效地估计出Q函数，对每个状态-动作对都需要产生多个样本，因此初始状态需要随机从可能的状态-动作对中随机选择！

![蒙特卡洛随机初始化](/wiki/static/images/rl-mc1.png)

由于所有的初始状态-动作都是随机的，所以初始动作比较无效！但是又不能按照当前的策略选择初始动作，那样将会导致很多状态-动作对没有样本！

为了解决这个问题，可以限制$(\pi(a|s) < 1 - \epsilon + \epsilon/|A(s)|)$，也就是说不让策略只选择最优的动作，还以一定的概率选择其他动作，这样初始动作的选择也可以采用当前最优策略来选了！但是，这样一来，收敛后的策略并不是最优策略了，只是近最优策略！

### Importance Sampling
另外一种解决方案是利用采样(Importance Sampling)实现off-policy，也就是评估的策略不是线上运行的策略！前面的方法评估的策略就是线上运行的策略，叫做on-policy方法。

假设评估的策略是$(\pi(a|s))$，而线上运行的策略是$(b(a|s))$，那么对于动作-状态轨迹$(\tau = (A_1, S_2, A_2, ..., S_T))$，两种策略产生该轨迹的概率之比为

$$
\rho_{t:T-1} = \Pi_t^{T-1} \frac{\pi(A_t|S_t)}{b(A_t|S_t)}
$$

那么，根据用策略$(b)$得到的经验数据，可以估计在策略$(\pi)$下的值函数

$$
V(s) = \frac{\sum_{t \in B(s)}\rho_{t:T(t)-1} G_t}{|B(s)|} \\\\
V(s) = \frac{\sum_{t \in B(s)}\rho_{t:T(t)-1} G_t}{\sum_{t \in B(s)}\rho_{t:T(t)-1}}
$$

$(B(s))$是状态s所处的时间集合。前一种估计无偏但是高方差，后一种有偏但是低方差，但是偏差会随着样本数增加而趋近于0！推荐后一种估计。

## 时间差分(TD)方法
蒙特卡洛方法需要策略执行到终止状态才能评估策略，TD方法只需要1步！核心思想在于自助法，它对值函数的估计是

$$
G_t = R_{t+1} + \gamma V(S_{t+1})
$$

即用原来的值函数取代了后面所有的回报！并采用常数学习率，

$$
V(S_t) \leftarrow V(S_t) + \alpha[R_{t+1} + \gamma V(S_{t+1}) - V(S_t)]
$$

上述迭代可以看做最小化 $(1/2||G_t - V^ * (S_t)||^2)$ 进行随机梯度下降！
误差项$(\delta_t = G_t - V(S_t))$称作 TD error。

### SARSA方法
Sarsa方法就是用TD方法对动作值函数Q(s,a)进行学习！

$$
Q(S_t, A_t) \leftarrow Q(S_t, A_t) - \alpha[R_{t+1} + \gamma Q(S_{t+1}, A_{t+1}) - Q(S_t, A_t)]
$$

可以看到，Sarsa方法用到了两个状态和两个动作，这两个动作都是采样自Q函数策略。如果动作选取有探索，那么Q就不是最优解，一般可以随着学习的推进，不断减小探索到某个很小的值，可以得到近最优解。

SARSA可以看做用 $(Q(S_{t+1}, A_{t+1}) )$ 来估计$(V^ * (S_{t+1}))$！

$$
Q^ * (S_t, A_t) = R_{t+1} + \gamma V^ * (S_{t+1}) \approx R_{t+1} + \gamma Q(S_{t+1}, A_{t+1})
$$

![SARSA方法](/wiki/static/images/sarsa.png)

### Q-learning
也是一种TD学习方法，而且是off-policy，与SARSA不一样的是，它只用到了一个初始动作，不需要根据Q函数策略采样的第二个动作，这是最大的区别！也是Q-learning可以用off-policy学到最优策略的关键！

$$
Q(S_t, A_t) \leftarrow Q(S_t, A_t) - \alpha[R_{t+1} + \gamma \max_a Q(S_{t+1}, a) - Q(S_t, A_t)]
$$

Q-learning可以看做用 $(\max_a Q(S_{t+1}, a) )$ 来估计$(V^ * (S_{t+1}))$！

$$
Q^ * (S_t, A_t) = R_{t+1} + \gamma V^ * (S_{t+1}) \approx R_{t+1} + \gamma \max_a Q(S_{t+1}, a)
$$

### 期望SARSA方法
SAESA方法用当前策略的采样动作$(A_{t+1})$的动作值函数近似$(V^ * (s'))$，因此是有偏而且方差很大！
Q-learning则用$(\max_a Q(s', a))$近似$(V^ * (s'))$，也是有偏的方差也大，而且因为取max导致有过高估计的问题。
期望SARSA则用当前Q函数在当前策略下的期望值 $(\sum_{a} \pi(a|s') Q(s', a))$ 近似$(V^ * (s'))$，因为用到期望值，所以可以减少方差！迭代方程是

$$
Q(S_t, A_t) \leftarrow Q(S_t, A_t) - \alpha[R_{t+1} + \gamma \sum_a \pi(a|S_{t+1}) Q(S_{t+1}, a) - Q(S_t, A_t)]
$$

### Double Q-learning
解决求max操作的过高估计问题，用两个Q函数，交替学习，每次学习时，用Q2来估计$(V^ * (S_{t+1}))$作为目标，对Q1进行梯度下降

$$
Q_1(S_t, A_t) \leftarrow Q_1(S_t, A_t) - \alpha[R_{t+1} + \gamma Q_2(S_{t+1}, a) - Q_1(S_t, A_t)] \\\\
a = \arg\max_a Q_1(S_{t+1}, a)
$$


## n-step TD方法

在单步TD方法中，指向前看了一步就得出$(V^{\pi} (S_t))$的估计值，实际上也可以看多步，这样可以估计更加准确

$$
G_{t:t+n} = R_{t+1} + ... + \gamma^{n-1} R_{t+n} + \gamma^n V(S_{t+n})
$$

可以看出单步TD方法就是n=1的情况，而蒙特卡洛方法可以看做 $(n \rightarrow \infty)$ 的极限！
n-step TD估计偏差来自于最后一项，用当前的值函数估计最优值函数，因为前面的系数是$(\gamma^n)$是指数衰减的，所以n步TD方法的偏差随指数衰减！

n-step TD方法迭代方程是

$$
V(S_t) \leftarrow V(S_t) + \alpha[G_{t:t+n} - V(S_t)]
$$

![n-step TD](/wiki/static/images/n-td.png)

**SARSA**：将SARSA对Q的估计用n步回报替换就可以得到n步SARSA方法

$$
Q(S_t, A_t) \leftarrow Q(S_t, A_t) - \alpha\left( [R_{t+1} + ... + \gamma^{n-1}R_{t+n} + \gamma^n Q(S_{t+n}, A_{t+n})] - Q(S_t, A_t) \right)
$$

**Q-learning**：同理将Q-learning对Q的估计用n步回报替换就可以得到n步Q-learning方法！

## TD($(\lambda)$)方法
基本出发点：将n步回报加权平均作为对回报的估计，距现在越久的权重越小。

$$
G_t^{\lambda} = (1-\lambda)\sum_{n=1}^{\infty} \lambda^{n-1} G_{t:t+n}
$$

可以看出当$(\lambda = 0)$时，就是单步TD方法，当$(\lambda = 1)$时，就是蒙特卡洛方法。

后向更新算法，迭代地利用 TD error进行更新

$$
Z_t(s) = \begin{cases}
    \gamma \lambda Z_{t-1}(s), \quad s \neq S_t \\\\
    \gamma \lambda Z_{t-1}(s) + 1, \quad s = S_t
    \end{cases} \\\\
\delta_t = R_{t+2} + \gamma V_t(S_{t+1}) - V_t(S_t) \\\\
V_t(S_t) \leftarrow V_{t-1}(S_t) + \alpha \delta_t Z_t(S_t)
$$


## 策略梯度理论
直接建模策略函数

$$
\nabla J(\theta) = \sum_s \mu_{\pi}(s) \sum_a q_{\pi}(s, a) \nabla_{\theta} \pi(a|s,\theta) = E_{\tau \sim \pi_{\theta}} \nabla_{\theta} \pi_{\theta}(\tau) r(\tau)
$$

两种表述，前面一个等式是状态-动作表述，后一个等式是状态-动作序列表述。策略梯度法实际上相当于用回报作为样本权重的极大似然估计。

REINFORCE：**蒙特卡罗策略梯度**

$$
\nabla J(\theta) = E_{\pi}\left[   \gamma^t\sum_a  q_{\pi}(S_t, a) \nabla_{\theta} \pi(a|S_t,\theta)    \right]  \quad \text{(对状态采样)}\\\\
= E_{\pi}\left[   \gamma^t q_{\pi}(S_t, A_t) \nabla_{\theta} \log \pi(A_t|S_t,\theta)    \right]      \quad \text{(对动作采样)}\\\\
= E_{\pi}\left[   \gamma^t G_t \nabla_{\theta} \log \pi(A_t|S_t,\theta)    \right]      \quad \text{(用采样的回报替换q函数)}\\\\
= E_{\pi}\left[   \gamma^t (G_t - V(S_t)) \nabla_{\theta} \log \pi(A_t|S_t,\theta) \right]  
$$

等二个等式，用到了关系

$$
Eq_{\pi}(S_t, A_t)\nabla_{\theta} \log \pi(A_t|S_t,\theta)  = \sum_a  \pi(a|S_t)q_{\pi}(S_t, a)\nabla_{\theta} \log \pi(A_t|S_t,\theta)
$$

最后一个等式是因为被剪掉的部分期望值为0.

因此，可以利用单个样本$((S_t, A_t))$估计策略梯度，得到策略梯度随机梯度上升迭代公式

$$
\theta_{t+1} = \theta_{t} + \alpha \gamma^t G_t \nabla_{\theta} \log \pi(A_t|S_t,\theta_t)
$$

这里的$(G_t)$采用的是单条链的最终回报，所以是用蒙特卡罗法估计策略梯度！

用$(\delta_t = G_t - V(S_t; w))$替换$(G_t)$可以减小梯度估计的方差，并且使用TD方法估计$(G_t = R_{t+1} + \gamma V(S_{t+1}; w))$。这就是 **Actor Critic** 算法：

$$
\theta_{t+1} = \theta_{t} + \alpha \gamma^t \delta_t \nabla_{\theta} \log \pi(A_t|S_t,\theta_t)
$$

![actor critic](/wiki/static/images/actor-critic.png)
