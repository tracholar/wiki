---
title: "Git常用命令汇总"
layout: page
date: 2016-05-31 20:51
---

[TOC]

## 关于
这里汇集了git常用命令和用法，便于速查，不定期更新。

## 打tag
将当前版本添加tag`<tagname>`
```bash
git tag <tagname>
```
列出标签
```bash
$ git tag
v0.1
v1.3

$ git tag -l 'v1.4.2.*'
v1.4.2.1
v1.4.2.2
v1.4.2.3
v1.4.2.4
```

将tag推送到远程服务器，参见[1-2]，推送所有的tags
```bash
git push --tags
```
只推送单个tag
```bash
git push origin <tagname>
```

1. <http://stackoverflow.com/a/5195913/4349983>
2. <https://git-scm.com/book/zh/v1/Git-%E5%9F%BA%E7%A1%80-%E6%89%93%E6%A0%87%E7%AD%BE>

