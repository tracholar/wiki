---
title: "NetSMF: Large-Scale Network Embedding as Sparse Matrix Factorization"
layout: page
date: 2019-10-01
---
[TOC]



## 图
- 参考:
    - Graph sparsification by effective resistances
    - 
- G = (V, E, w), 顶点, 边, 权重
- 定义对角阵S为 $(s_{ii} = \sum_j w_{ij})$ ,非对角线为0, 图的(未归一化)拉普拉斯矩阵 L = S - W
- 拉普拉斯矩阵的性质
    1. 对称, 因此具有实数特征值
    2. 每一行之和为0, $(L\bf{1} = 0)$, 因此至少存在一个特征值为0的特征向量1
    3. 半正定 $(x^TLx = \sum_{(i,j) \in E} w_{ij}(x_i - x_j)^2)$, 因此特征值非负
- 完全图的拉普拉斯矩阵, 完全图是指任意两个顶点都存在边, 即有 $(L_{ij})$ 都非0
    - 特征值为0和N,N是顶点数目
    - 特征向量是1和$(e_i - e_j)$, $(e_i)$ 表示单位向量

- 图等效电阻: 无向联通图可以看做一个电路, 权重$(w_{ij})$ 看做边上的电导
    - 设每条边都关联一个电流,每个定点关联一个电压,边的权重是电导, 那么电流向量i和电压向量v关系为
    - 每条边上的电流 $(i = WBv)$ B是边到顶点的关联矩阵,bij表示第i条边和第j个定点的关系(+1表示出边,-1表示入边,0表示无关), W是一个以边权重为对角线的对角阵
    - 每个顶点流出的电流是该定点关联的边上电流流入之和$(i_{ext} = B^T i)$
    - 结合上述两式有每个顶点流出的电流与每个定点的电压可以通过拉普拉斯矩阵关联起来 $(i_{ext} = B^TWBv = Lv)$
    - 定义$(L^+)$ 为拉普拉斯矩阵的彭罗斯伪逆,那么$(v = L^+ i_{ext})$
    - 为计算边e=(u, v)的等效电阻, 可以注入单位电流$(i_{ext} = (\chi_u-\chi_v)^T = b_e^T)$,那么电压差即为等效电阻
    $$
    R_e = v_u - v_v = (\chi_u-\chi_v)^T v = b_e L^+ b_e^T
    $$
    因此,等效电阻的关键是计算拉普拉斯矩阵的伪逆,即对拉普拉斯矩阵做谱分解。

## Random-Walk Molynomial Sparsification
- 利用random-walk 将图变成稀疏图
- 谱相似，两个图的拉普拉斯矩阵是相似的，拉普拉斯矩阵相似是指，对任意x，
$$
(1-\epsilon) x^T \hat{L}x \le x^T L x \le (1+\epsilon) x^T \hat{L} x
$$    
那么这两个图就叫$(1+\epsilon)$-谱相似
- 稀疏化的方法还没搞懂

## SVD
- truncated SVD
- redsvd <https://github.com/cequencer/redsvd>



