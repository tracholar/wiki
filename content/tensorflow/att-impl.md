---
layout: page
title: "attention 实现细节"
date: 2020-05-29
---
[TOC]

## Attention基本原理
- 以DIN为例，用户行为序列是一个向量序列 keys 的 shape= [None, seq_len, emb_len]
- query是item向量 query，shape = [None, emb_len]
- attention的作用是将用户行为变长向量序列变长一个固定维度(emb_len)的向量，类似于 sumpooling，但是多了一个Attention权重

$$
w_i = softmax(k^T q)_i \\\\
out = \sum_i w_i k_i
$$

## tensorflow 实现细节
- 由于模型训练都是一个batch一个batch做的，那么同一个batch内的序列长度肯定不是一样的
- 这必然导致实际拿到的 keys 不是一个正常的3阶张量。所有有了mask这种东西
- mask张量就是为了将所有样本补齐到相同长度，但实际上只有mask标记为1的那些是有效的
- 看代码

```python
import tensorflow as tf

def attention(keys, keys_mask, query):
    """
    keys [None, seq_len, emb_len]
    keys_mask [None, seq_len]
    query [None, emb_len]
    """ 
    keys_mask_3d = tf.expand_dims(keys_mask, -1)
    query_3d = tf.expand_dims(query, -1)
    score = tf.reduce_sum(keys * query_3d * keys_mask_3d , axis=-1)
    score -= 1.0e9 * tf.cast(keys_mask < 0.5, tf.float32)
    weight = tf.nn.softmax(score)
    weight = tf.expand_dims(weight, -1)
    return tf.reduce_sum(keys * weight, axis=1)
    
```

- 其中`score -= 1.0e9 * tf.cast(keys_mask < 0.5, tf.float32)`的目的是为了将softmax中无效的项置0
