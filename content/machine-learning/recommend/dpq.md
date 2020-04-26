---
title: "Learning Multi-granular Quantized Embeddings for Large-Vocab Categorical Features in Recommender Systems"
layout: page
date: 2020-04-23
---
[TOC]


## 关于
- 解决的问题
    1. id太多，embedding参数太多，在线打分困难
        - hash trick
        - low-rank factorization：ALBERT: A Lite BERT for Self-supervised Learn- ing of Language Representations
        - quantization：Differentiable Product Quantization for End- to-End Embedding Compression
    2. 不同频率的item的embedding需要不同的容量(长度/状态数)
    
## Differentiable Product Quantization
- KD encode