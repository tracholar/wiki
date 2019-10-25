---
title: "动态加载JAR时的classloader的问题"
layout: page
date: 2019-10-17
---
[TOC]


## 问题
动态加载a.jar， 其中的一个类A使用了框架中的类PA的一个方法
