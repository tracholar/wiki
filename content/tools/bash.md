---
title: "bash使用技巧"
layout: page
date: 2016-07-02
---
[TOC]
## 例子
提示符修改
```bash
export PS1='\u@\h \w \$ ' (普通示例)
export PS1='\[\e[0;32m\][\u@\h \w \$]\[\e[m\]' (颜色示例)
export PS1='\t: ' (时间示例)
export PS1='\u@\h [\$(ls | wc -l)]:\$ ' (显示当前目录行下文件数量)
```


## 基本语法
- 流控制 if ... elif ... else
```bash
if condition
then
    statements
[elif condition
    then statements. ..]
[else
    statements ]
fi
```
其中condition可以是一个命令，也可以是一个条件表达式，其中条件判断有很多细节。
