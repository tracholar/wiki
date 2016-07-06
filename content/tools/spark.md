---
title: "Spark"
layout: page
date: 2016-07-05
---
[TOC]


## 安装
从Spark官网下载安装包，然后解压即可。非常简单


## 启动主机和worker
进入spark目录，然后运行脚本
```bash
./sbin/start-master.sh
```
即可。进程会在后台运行，你可以通过 <http://localhost:8080> 进行监控。


启动worker的脚本是
```bash
./bin/spark-class org.apache.spark.deploy.worker.Worker spark://IP:PORT
```
其中IP和PORT可以在监控页面看到。


关闭worker很简单，直接关闭worker运行的shell或者ctr + c中断即可。
关闭主机需要运行脚本
```bash
./sbin/stop-master.sh
```
