---
title: "PyTorch"
layout: page
date: 2018-12-23
---
[TOC]

## 张量定义
张量定义跟numpy的数组定义类似

```python
import torch
torch.randn(5,6)
torch.ones(5,1)

```


## 自动微分
`torch.Tensor` 的属性 `requires_grad` 如果设为 `True`, 那么会自动跟踪所有的运算。

停止跟踪梯度方法 `.detach()`

```python
>>> import torch
>>>
>>> x = torch.ones(2, 2, requires_grad=True)
>>> y = x ** 2 + 4
>>> y
tensor([[5., 5.],
        [5., 5.]], grad_fn=<AddBackward>)
```

每一个Tensor都有一个`.grad_fn`属性,指向创建这个张量的函数,除了用户自己创建的

```python
>>> out = y.mean()
>>> out
tensor(5., grad_fn=<MeanBackward1>)
```


当你调用 `.backward()` 方法时,所有的梯度将会自动计算,并放在 `.grad` 属性中。

```python
>>> out.backward()
>>> x.grad
tensor([[0.5000, 0.5000],
        [0.5000, 0.5000]])
```

当输出是向量时,需要指定 `.backward(v)` 参数才能得到雅克比矩阵。

```python
>>> x = torch.ones(4, requires_grad=True)
>>> out = x * 2
>>> out.backward(torch.tensor([1, 1, 1, 1], dtype=torch.float))
>>> x.grad
tensor([2., 2., 2., 2.])
```

禁止跟踪梯度

```python
with torch.no_grad():
    print((x ** 2).requires_grad)
```

## 参考
1. <https://pytorch.org/tutorials/beginner/blitz/autograd_tutorial.html>
2. <https://pytorch.org/tutorials/beginner/blitz/neural_networks_tutorial.html>