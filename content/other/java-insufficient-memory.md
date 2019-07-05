---
title: "磁盘空间满了"
layout: page
date: 2019-07-05
---
[TOC]

## 错误日志

```bash
Java HotSpot(TM) 64-Bit Server VM warning: Insufficient space for shared memory file:
Try using the -Djava.io.tmpdir= option to select an alternate temp location.
```

执行 `df -hl` 检查磁盘空间是否满了。显然 `/dev/vda1`满了

```bash
Filesystem      Size  Used Avail Use% Mounted on
/dev/vda1        25G   24G   15M 100% /
tmpfs            16G     0   16G   0% /dev/shm
/dev/vdc1       493G  151G  317G  33% /opt
```