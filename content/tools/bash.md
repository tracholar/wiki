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





