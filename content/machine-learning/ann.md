---
title: "近似最近邻搜索"
layout: page
date: 2017-04-10
---
[TOC]

## 综述
论文：An Investigation of Practical Approximate Nearest Neighbor Algorithms，Ting Liu, Andrew W. Moore, Alexander Gray and Ke Yang，2004

- 最近邻搜索方案：
    - Voronoi diagrams：1-2维，F. P. Preparata and M. Shamos. Computational Geometry. Springer-Verlag,
    - kd树、metric trees、ball-trees：中等维度10s
        - J. H. Friedman, J. L. Bentley, and R. A. Finkel. An algorithm for finding best matches in loga- rithmic expected time. ACM Transactions on Mathematical Software, 3(3):209–226, September 1977.
        - J. K. Uhlmann. Satisfying general proximity/similarity queries with metric trees. Information Processing Letters, 40:175–179, 1991.
        - S. M. Omohundro. Efficient Algorithms with Neural Network Behaviour. Journal of Complex Systems, 1(2):273–347, 1987.
- $(1+\epsilon)$-k最近邻搜索：返回的点最大距离不超过第k个近邻距离的 $(1+\epsilon)$ 倍！


### Metric Tree， Spill Tree
1. metric tree 将样本按照二叉树结构保存，根节点代表所有的样本，它的两个子节点将样本分割成不想交的两部分；
用 v 代表节点，N(v)代表v节点的样本，左右子节点用 v.lc v.rc 表示
2. 分割：对于节点v，找到两个 pivot 点 v.lpv v.rpv，使得这两个节点的距离是集合的距离，即 $(||v.lpv - v.rpv|| = \max_{p1,p2 \in N(v)} ||p1 - p2||)$，最优 pivot 寻找是二次复杂度，可以近似用线性复杂度找 pivot：
    1. 先随机找一个点p
    2. 找到 N(v) 中距p最远的点作为 v.lpv
    3. 再找距v.lpv 最远的点作为 v.rpv
3. 找到两个 pivot 点后，然后将所有点点投影到这两个点的连线上 $(u = v.rpv - v.lpv)$，找到投影的中值 A 作为分割点，投影小于A的分到左子树，大于A的分到右子树；为计算效率计，可以直接用中点代替，即 $(1/2(v.rpv - v.lpv))$
4. 每一个节点 v 保留 N(v) 的覆盖超球信息，球心 v.center, 半径 v.r.
5. 搜索（MT-DFS）：深度优先搜索，如果待查找的q投影在A左边，先找左子树，反之先找右子树；保留k个已找到的最近邻，设这些点的距q最大距离为r；如果节点 v 的所有点距离q都大于r就可以减枝，不再找v和v的子树。可以通过条件 $(||v.center - q|| - v.r \ge r)$ 判断！

- metric tree 的计算通过减枝减少寻找数目，但是如果无法减枝的节点，需要不断回溯，使得查找性能不高。
- spill tree 则放弃精确查找，不回溯提高性能。但是精度将难以接受。
- 提高精度：分裂的时候两个子树可以有交集，相交部分为 $(\tao)$, overlapping buffer. 通过这种方法提高精度
- 混合搜索：$(\tao)$ 的引入带来新的问题，可能某些节点重叠部分太多，导致左右子树包含了全部数据！为此，可以设定一个阈值 $(\rho<1)$（典型值70%），如果任何一个子节点包含超过这个比率的样本，则对这个节点不进行 spill分割，而进行常规的不交叠分割，并标记为 nonoverlapping 节点，其他的节点标记为overlapping节点。搜索的时候，只对 nonoverlapping 节点回溯！
- 维度超过30后，速度就会变得很慢！通过随机投影到一个低维空间后，再利用混合搜索！随机投影带来的精度损失，可以通过多次投影找回！
- [Johnson-Lindenstrauss 定理](https://en.wikipedia.org/wiki/Johnson%E2%80%93Lindenstrauss_lemma)：W. Johnson and J. Lindenstrauss. Extensions of lipschitz maps into a hilbert space. Contemp. Math., 26:189–206, 1984.
- LSH：A. Gionis, P. Indyk, and R. Motwani. Similarity Search in High Dimensions via Hashing. In Proc 25th VLDB Conference, 1999.

## 大规模最近邻搜索
Muja M, Lowe D G. Scalable nearest neighbor algorithms for high dimensional data[J]. IEEE Transactions on Pattern Analysis and Machine Intelligence, 2014, 36(11): 2227-2240.

- 三类最近邻搜索方法：
    - partitioning trees
    - hashing
    - neighboring graph

### partitioning trees
- kd-tree：低维空间高效，但是维度高了效果急剧下降！
- 多个 randomized k-d trees
