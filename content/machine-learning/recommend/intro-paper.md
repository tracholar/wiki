---
title: "推荐论文集快速浏览"
layout: page
date: 2019-05-20
---
[TOC]

## 论文列表
- <https://github.com/hongleizhang/RSPapers>

## A Survey on Session-based Recommender Systems
- 基于session的推荐系统
- 基于内容的推荐(找相似内容)和基于协同过滤(利用用户行为找相似的人/物),偏静态,无法快速捕获用户的实时兴趣点的变化
- 将session作为推荐的基本单位而不是将用户作为基本单位
- session的概念: A session is a set of items (e.g., referring to any objects, e.g., products, songs or movies) that are collected or consumed in one event (e.g., a transaction) or in a certain period of time or a collection of actions or events (e.g., listening to a song) that happened in a period of time (e.g., one hour).
- 即在一个事件(比如交易)中被获取或消费的一系列item的集合, 或者在一段时间内发生的动作或事件的集合
- session推荐系统:Given partially known session information, e.g., part of a session or recent historical sessions, an SBRS aims to predict the unknown part of a session or the future sessions based on modelling the complex relations embedded within a session or between sessions.
- 即根据session的一部分来预测未知的部分、或者未来的session事件以及session之间的关联
- 两个方向: 推荐下一个/多个item; 推荐下一个session
- 基于session推荐: 基于session上下文预测target,有时也加入item特征和user特征
- 