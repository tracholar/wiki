---
layout: page
title: "Python 教程"
date: 2016-09-10 12:32
---
[TOC]

## 关于
深感需要一门语言作为个人核心语言，python 很不错，所以打算深入学习这门语言。
于是，刷一遍python官方文档很有必要！
[官方文档地址](https://docs.python.org/2/tutorial/index.html)

## 安装
WINDOW平台直接安装编译好的二进制码就行，按照正常软件安装流程安装。

MAC 可以先安装 brew, 然后 `brew install python` 一般系统自带 python

Linux 安装，一般自带python。这里介绍一个local模式。

首先下载 python 对应版本的源代码，然后编译。编译时指定 `--prefix=你要安装的目录`
通过将编译后的bin目录加到环境变量`PATH`最前面的方式，覆盖系统路径中的python。
在.bashrc中加入代码 `export PATH=/path-to-local-python/bin/:$PATH`

## 数据结构
### List
- 可以用来做stack，使用 `append`, `pop` 方法！
- 也可以用来做队列，但是不够高效！可以使用 `collections.deque`，它被设计为在双向`append,pop`都很高效！
