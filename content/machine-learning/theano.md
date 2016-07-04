---
title: "Theano"
layout: page
date: 2016-07-04
---
[TOC]

## theano中的broadcast
theano和numpy中的broadcast不同，
theano中需要对能够broadcast的变量进行编译前申明，
默认情况下，theano中的`matrix/tensor`和所有的`shared variable`都不不可以
broadcast的，`vector`和`matrix`进行计算时是可以进行broadcast的。
`shared variable`可以在创建的时候声明可以broadcast。
```python
bval = np.array([[10, 20, 30]])
bshared = theano.shared(bval, broadcastable=(True, False))
```

## theano两层神经网络代码例子
```python
def nn_cost_func():
    import theano
    from theano import tensor as T
    w1 = T.dmatrix('w1')
    b1 = T.dvector('b1')
    w2 = T.dmatrix('w2')
    b2 = T.dvector('b2')
    x = T.dmatrix('x')
    y = T.dmatrix('y')
    h = T.nnet.sigmoid(T.dot(x,w1)+b1)
    yp = T.nnet.softmax(T.dot(h, w2)+b2)
    J = T.mean(T.sum(y * T.log(yp), axis=1))
    dJ = T.grad(J,[w1,b1,w2,b2])
    fJ = theano.function([x,y,w1,b1,w2,b2], J)
    fdJ = theano.function([x,y,w1,b1,w2,b2], dJ)
    return fJ, fdJ
```
