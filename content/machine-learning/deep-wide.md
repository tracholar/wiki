---
title: "Deep & Wide model : Google"
layout: page
date: 2016-12-13
---
[TOC]

## 关于
Google 宽广度模型

用深度模型学习特征组合，将离散特征全部 embedding  到低维向量，加上连续特征一起，
用DNN建模。深度模型和浅层模型联合优化。

线下AUC和线上效果不一致？单独深度模型线下AUC略低，但是线上效果确提高了。
