---
title: "TOPIC MODEL - 主题模型"
layout: page
date: 2017-04-06
---
[TOC]

## 主题模型的意义
Topic modeling provides methods for automatically organizing, understanding, searching, and summarizing large electronic archives.

1. Discover the hidden themes that pervade the collection.
2. Annotate the documents according to those themes.
3. Use annotations to organize, summarize, and search the texts.

## Latent Dirichlet allocation(LDA)
- 文档包含多个主题
- 每一个主题是在词上的一个分布，可以表达为词的直方图
- 每一个文档是多个主题的混合，可以表达为主题的直方图
- 每一个词是从某个主题中采样得到
- 但是我们只能观察到文档，其他的都是隐变量！
- 我们的目标是推断出这些隐变量！


<img src="/wiki/static/images/toptic-model01.png" />



## 参考文献
1. Probabilistic Topic Models, ICML2012 Tutorial: <http://www.cs.columbia.edu/~blei/talks/Blei_ICML_2012.pdf>
2.
