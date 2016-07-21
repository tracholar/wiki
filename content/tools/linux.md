---
title: "Linux 基础知识"
layout: page
date: 2016-07-18
---
[TOC]

## 关于
作为一个合格的工程师，怎能不懂Linux，所以学习记录一些知识点。


## 常用命令集合
### 文件系统相关
- 重定向：`2>&1`表示将stderr重定向到stdout，`2>/dev/null`表示重定向stderr到空洞，也就是不打印出stderr。
  后面这个在运行hadoop和spark的时候有用。

### 操作系统相关
- 后台运行加一个`&`即可，例如`sleep 100 &`

### 字符串工具
- grep 字符匹配

```bash
## 从log.txt中查找keyword出现的行
grep keyword log.txt
## 查询多个模式
grep -E 'keyword|otherword' log.txt
grep -e keyword -e otherword log.txt

```
