---
title: "多目标识别"
layout: page
date: 2017-05-06
---
[TOC]

## 多目标检测
论文：MULTIPLE OBJECT RECOGNITION WITH VISUAL ATTENTION，Jimmy Lei Ba，Volodymyr Mnih，Koray Kavukcuoglu @DeepMind

![多目标检测](/wiki/static/images/multi-obj01.png)

- 每一步，产生一个 glimpse 的坐标 $(l_n)$ 和 图像块 $(x_n)$，其中初始的 l 由原始图像下采样后，经过 context network 提供的。
