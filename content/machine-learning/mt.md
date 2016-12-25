---
title: "机器翻译"
layout: page
date: 2016-12-16
---
[TOC]

## SCORE
### BLEU
### WER

## 机器翻译：联合训练 alignment and translater
Bahdanau D, Cho K, **Bengio Y**. Neural machine translation by jointly learning to align and translate[J]. arXiv preprint arXiv:1409.0473, 2014.

Encoder - Decoder 当句子很长的时候，难以学习，因为需要把一个很长的序列压缩为一个固定长度的向量。

attention 向量是变长的，这个问题怎么解决？


## 低频词
论文：Luong M, Sutskever I, Le Q V, et al. Addressing the Rare Word Problem in Neural Machine Translation[C]. meeting of the association for computational linguistics, 2014: 11-19.


## 低频词：混合模型
论文：Luong M, Manning C D. Achieving Open Vocabulary Neural Machine Translation with Hybrid Word-Character Models[C]. meeting of the association for computational linguistics, 2016: 1054-1063.

解决的问题：encoder + decoder + attention 模型，对低频词效果不好，
常将低频词全部映射为一个<UNK>的特殊词，然后通过后续规则解决。

方法：在 encoder 和 decoder 测加了一个对<UNK>的 Character Model，这个模型和翻译模型联合优化！

<img src="/wiki/static/images/nmt-hm.png" style="width:400px; float:left" />
