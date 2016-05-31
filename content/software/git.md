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

将tag推送到远程服务器，[参见][1]。
推送所有的tags
```bash
git push --tags
```
只推送单个tag
```bash
git push origin <tagname>
```






[1]: http://stackoverflow.com/a/5195913/4349983

