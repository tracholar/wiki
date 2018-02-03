---
title: "强化学习算法实现(I)"
layout: page
date: 2018-02-02
---
[TOC]

## 环境构建
MDP的环境是指转移概率$(P(s'| s, a))$和回报函数$(r(s, a))$！
在环境已知的动态规划算法中，用于迭代算法当中。在环境位置的TD方法中用于模拟。

构建一个简单的环境，有nS个状态，0，1，...，nS-1；其中nS-1是终止状态。
该环境下一共两个动作：0向左运动，1向右运动，每个动作都有概率p0不动，p1的概率会往反方向运动, 1-p0-p1概率正常运动。

|0 | 1 | 2 | ... | 9 |
|--|--|----|-----|---|
|←·→|←·→|←·→|←·→|终点|

```python
import numpy as np

nS = 10
nA = 2  #不要改这个参数
Done = nS - 1
p0 = 0.1
p1 = 0.1
P = np.zeros((nS, nA, nS)) # 转移概率
R = np.zeros((nS, nA, nS)) - 1.0 # 回报都是-1
gamma = 1

for s in range(nS):
    if s == Done: # 终止态转移概率都为0
        continue
    for a in range(nA):
        inc = a * 2 - 1 # 步长        
        P[s, a, s] += p0 # 不动
        P[s, a, max(0, s - inc)] += p1 # 反方向
        P[s, a, max(0, s + inc)] += 1 - p0 - p1 # 正常运动
```

## 值迭代
迭代法求解非线性方程，值迭代迭代方程

$$
V(s) = \max_a \sum_{s'}P(s'|s, a) [r(s, a) + \gamma V(s')]
$$

```python
V = np.zeros(nS)
for it in range(1000):
    converage = True
    for s in range(nS):
        if s == Done:  # 终止状态不迭代
            V[s] = 0
            continue

        # HJB 非线性方程
        maxV = max(sum(P[s, a, ss]*(R[s, a, ss] + gamma * V[ss]) for ss in range(nS)
                        ) for a in range(nA)
                   )

        if V[s] != maxV: # 测试值迭代是否收敛
            converage = False
            V[s] = maxV
    if converage:
        break

print 'iteral steps:', it
print V

>>>
iteral steps: 57
[-12.65306123 -11.40306123  -9.99681123  -8.57102998  -7.14280732
  -5.71427949  -4.28571351  -2.85714276  -1.42857142   0.        ]

```

## 策略迭代
- 策略迭代分两步
    1. 第一步固定策略，求解值函数，叫策略评估，因为策略固定了，HJB方程由非线性方程变成线性方程，可以采用迭代解法或者高斯消元法求解；
    2. 第二步叫策略提升，对值函数构造新的更优策略！

```python
pi = np.zeros(nS, dtype=int) #初始策略全部往左

for it in range(100):
    V = np.zeros(nS)
    # 策略评估，解线性方程，雅克比迭代法
    for _ in range(100):
        converage = True
        for s in range(nS):
            if s == Done:
                V[s] = 0
                continue

            # HJB 线性方程
            a = pi[s]
            v = sum(P[s, a, ss]*(R[s, a, ss] + gamma * V[ss]) for ss in range(nS))

            if V[s] != v:
                converage = False
                V[s] = v
        if converage:
            break

    # 策略提升
    converage = True
    for s in range(nS):
        maxA = np.argmax([sum(P[s, a, ss]*(R[s, a, ss] + gamma * V[ss]) for ss in range(nS)) for a in range(nA)])
        if maxA != pi[s]:
            converage = False
            pi[s] = maxA
    print 'iter',it, 'pi =',pi

    if converage:
        break

print 'pi =', pi
print 'V =', V

>>>
iter 0 pi = [0 0 0 0 0 0 0 1 1 0]
iter 1 pi = [0 0 0 0 0 1 1 1 1 0]
iter 2 pi = [0 0 0 1 1 1 1 1 1 0]
iter 3 pi = [1 1 1 1 1 1 1 1 1 0]
iter 4 pi = [1 1 1 1 1 1 1 1 1 0]
pi = [1 1 1 1 1 1 1 1 1 0]
V = [-12.65306123 -11.40306123  -9.99681123  -8.57102998  -7.14280732
  -5.71427949  -4.28571351  -2.85714276  -1.42857142   0.        ]
```

## 蒙特卡罗法
当环境未知时，即转移概率未知，无法利用HJB方程求解值函数。蒙特卡罗法通过运行到结束得到回报，去更新动作值函数——Q函数！

```python
Q = np.zeros((nS, nA))
pi = np.random.randint(0, nA, nS)

def go_next(s, a):
    r = np.random.rand()
    i = 0
    p = 0
    while True:
        if r < p + P[s, a, i]:
            return i
        p += P[s, a, i]
        i += 1
    return len(P[s, a])

alpha = 0.01
for it in range(1000):
    if it % 50 == 0:
        print 'iter', it, 'pi=', pi

    # 策略评估：根据目前策略仿真一条状态-动作路径，更新Q函数
    for s in range(nS):
        for a in range(nA):
            # 仿真一条状态-动作路径
            history = []
            ss = s
            while ss != Done:
                ss_next = go_next(ss, a)
                history.append((ss, a, R[ss,a,ss_next], ss_next))
                ss = ss_next
                a = pi[ss] #更新动作
                #print ss

            Gt = 0
            # 对出现的所有(s, a)对更新Q函数，复用这条路径
            for i in reversed(range(len(history))):
                ss, aa, rr, _ = history[i]
                Gt = gamma * Gt + rr
                Q[ss, aa] += alpha * (Gt - Q[ss, aa])

    # 策略提升：根据更新后的Q函数，更新策略
    for s in range(nS):
        pi[s] = np.argmax(Q[s, :])
print 'V=', np.max(Q, axis=1)
print 'pi=', pi

>>>
iter 0 pi= [1 0 1 0 0 1 1 0 1 0]
iter 50 pi= [0 1 0 1 0 1 1 0 1 0]
iter 100 pi= [0 1 0 1 0 1 1 0 1 0]
iter 150 pi= [1 0 1 1 0 1 1 0 1 0]
iter 200 pi= [1 0 1 1 0 1 1 1 1 0]
iter 250 pi= [1 0 1 1 0 1 1 1 1 0]
iter 300 pi= [1 0 1 1 0 1 1 1 1 0]
iter 350 pi= [0 1 1 1 1 1 1 1 1 0]
iter 400 pi= [0 1 1 1 1 1 1 1 1 0]
iter 450 pi= [0 1 1 1 1 1 1 1 1 0]
iter 500 pi= [0 1 1 1 1 1 1 1 1 0]
iter 550 pi= [1 1 1 1 1 1 1 1 1 0]
iter 600 pi= [1 1 1 1 1 1 1 1 1 0]
iter 650 pi= [1 1 1 1 1 1 1 1 1 0]
iter 700 pi= [1 1 1 1 1 1 1 1 1 0]
iter 750 pi= [1 1 1 1 1 1 1 1 1 0]
iter 800 pi= [1 1 1 1 1 1 1 1 1 0]
iter 850 pi= [1 1 1 1 1 1 1 1 1 0]
iter 900 pi= [1 1 1 1 1 1 1 1 1 0]
iter 950 pi= [1 1 1 1 1 1 1 1 1 0]
V= [-12.60595021 -11.20499113  -9.82580041  -8.41347716  -6.9761059
  -5.5099145   -4.19408818  -2.89429492  -1.45856045   0.        ]
pi= [1 1 1 1 1 1 1 1 1 0]
```

## Q-Learning
Q-learning是TD方法的一种，也是用得最多的一种，因为他可以很方便off-policy学习，不需用借助重要性采样。
相比蒙特卡罗法可以不需要等到运行结束就可以更新Q函数。

```python
Q = np.zeros((nS, nA))
pi = np.random.randint(0, nA, nS)

def go_next(s, a):
    r = np.random.rand()
    i = 0
    p = 0
    while True:
        if r < p + P[s, a, i]:
            return i
        p += P[s, a, i]
        i += 1
    return len(P[s, a])

alpha = 0.01
epsilon = 0.9 # 探索
for it in range(100):
    if it % 10 == 0:
        print 'iter', it, 'epsilon=', epsilon, 'V[0]=', max(Q[0])

    # 根据目前策略仿真一条状态-动作路径，并同时更新Q函数
    for s in range(nS):
        if s == Done:
            continue
        for a in range(nA):
            # 仿真一条状态-动作路径
            ss = s
            while ss != Done:
                ss_next = go_next(ss, a)
                Gt = R[ss,a,ss_next] + gamma * max(Q[ss_next, :])
                Q[ss, a] += alpha * (Gt - Q[ss, a])  # Q-learning 迭代步骤
                ss = ss_next

                a = pi[ss] #选择动作
                if np.random.rand() < epsilon: # 探索
                    a = np.random.randint(0, nA)
                #print ss

    epsilon = max(0.01, epsilon *0.99)

for s in range(nS):
    pi[s] = np.argmax(Q[s, :])

print 'V=', np.max(Q, axis=1)
print 'pi=', pi

>>>
iter 0 epsilon= 0.9 V[0]= 0.0
iter 10 epsilon= 0.813943867508 V[0]= -5.50430863461
iter 20 epsilon= 0.736116243838 V[0]= -9.58192560014
iter 30 epsilon= 0.665730336049 V[0]= -11.372140159
iter 40 epsilon= 0.602074582713 V[0]= -12.0806787351
iter 50 epsilon= 0.544505460424 V[0]= -12.3537691746
iter 60 epsilon= 0.492440978152 V[0]= -12.5909546081
iter 70 epsilon= 0.44535479364 V[0]= -12.657326121
iter 80 epsilon= 0.402770892387 V[0]= -12.5917214982
iter 90 epsilon= 0.36425877541 V[0]= -12.7107205284
V= [-12.70433436 -11.41177551  -9.8564413   -8.59335991  -7.20906867
  -5.70400336  -4.41177384  -2.82674456  -1.35446586   0.        ]
pi= [1 1 1 1 1 1 1 1 1 0]
```
