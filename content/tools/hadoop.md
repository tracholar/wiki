---
title: "Hadoop"
layout: page
date: 2016-07-12
---
[TOC]

## 关于
作为大数据工作者，怎能不懂hadoop。


## Hadoop shell
利用hadoop shell 访问HDFS文件系统，官方文档见<https://hadoop.apache.org/docs/r1.0.4/cn/hdfs_shell.html>。
这里记录一些常用命令和技巧。

调用文件系统(FS)Shell命令应使用 `bin/hadoop fs <args>`的形式。 所有的的FS shell命令使用URI路径作为参数。URI格式是scheme://authority/path。对HDFS文件系统，scheme是hdfs，对本地文件系统，scheme是file。其中scheme和authority参数都是可选的，如果未加指定，就会使用配置中指定的默认scheme。一个HDFS文件或目录比如/parent/child可以表示成hdfs://namenode:namenodeport/parent/child，或者更简单的/parent/child（假设你配置文件中的默认值是namenode:namenodeport）。大多数FS Shell命令的行为和对应的Unix Shell命令类似，不同之处会在下面介绍各命令使用详情时指出。出错信息会输出到stderr，其他信息输出到stdout。

- <args> 需要添加`-`号，例如`cat`命令调用形式为`hadoop fs -cat some-path`
- 与bash相同的命令有，`cat`, `chmod`, `chown`, `du`(显示文件大小), `ls`, `lsr`(ls -R), `mkdir` (mkdir -p 创建所有不存在的父目录),
  `mv`, `rm`, `rmr` (rm -R), `stat`, `tail`, `test -[ezd]`，
- 特有的命令
    - `dus`， 显示文件大小
    - `touchz`，创建一个0字节的空文件
    - `text`， 将源文件输出为文本格式。允许的格式是zip和TextRecordInputStream。
- 与本地文件系统交互的命令
    - `copyFromLocal`   将本地路径拷贝到HDFS中
    - `copyToLocal`     将HDFS中的路径原样拷贝到本地
    - `put`，可以用`-`代表标准输入，例如`hadoop fs -put - hdfs://host:port/hadoop/hadoopfile `
    - `get`， 例子 `hadoop fs -get /user/hadoop/file localfile`
    - `getmerge`, 获取分布式文件目录中所有的文件并合并到一个本地文件中，使用方法：`hadoop fs -getmerge <src> <localdst> [addnl]`，addnl是可选的，用于指定在每个文件结尾添加一个换行符。
