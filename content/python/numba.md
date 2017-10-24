---
title: "Numba 加速"
layout: page
date: 2017-10-17
---
[TOC]

Numba是一个用于python计算加速的软件包，通过增加Annotation注释的方式，不需要修改原始代码，使用方便。
下面通过一个例子展示使用方法。

```python
import numpy as np
from numba import jit

def sum2d(arr):
    M, N = arr.shape
    result = 0.0
    for i in range(M):
        for j in range(N):
            result += arr[i,j]
    return result

@jit
def sum2d_jit(arr):
    M, N = arr.shape
    result = 0.0
    for i in range(M):
        for j in range(N):
            result += arr[i,j]
    return result

arr = np.random.randn(10000,10000)
```

```python
%time  sum2d(arr)
```

    CPU times: user 20.4 s, sys: 34.1 ms, total: 20.4 s
    Wall time: 20.4 s

```python
%time sum2d_jit(arr)
```

    CPU times: user 122 ms, sys: 513 µs, total: 122 ms
    Wall time: 123 ms

```python
%time np.sum(arr)
```

    CPU times: user 77 ms, sys: 1.12 ms, total: 78.1 ms
    Wall time: 77.2 ms

通过numba优化的代码可以比原始python代码快将近两个数量级，和numpy的性能接近！
