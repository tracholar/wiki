---
title: "编译tensorflow"
layout: page
date: 2018-02-26
---
[TOC]

## 关于
记录从源码编译tensorflow的过程

## 环境
- CentOS 6 `rpm --query centos-release`
- GLIBC 版本2.12； `strings /lib64/libc.so.6 |grep GLIBC_`

## 安装
### 安装JDK8
因为bazel需要JDK8。你可以从Oracle官网下载，也可以下载我的备份 <https://pan.baidu.com/s/1jJoMjuY>

下载后解压到某个目录，例如在我的机器上是 `~/jdk1.8.0_161`，确保这个目录下有 `/bin` 这个子目录。

配置环境变量

```
export JAVA_HOME=~/jdk1.8.0_161
export PATH=$JAVA_HOME/bin:$PATH
```

执行`java -version`命令，查看是否配置正确

```
java version "1.8.0_161"
Java(TM) SE Runtime Environment (build 1.8.0_11-b12)
Java HotSpot(TM) 64-Bit Server VM (build 25.11-b03, mixed mode)
```

### 安装bazel
可以直接通过软件包安装，参考官网文档<https://docs.bazel.build/versions/master/install.html>

下面介绍从源码安装步骤，首先下载源文件<https://github.com/bazelbuild/bazel/releases>，并解压到`bazel/`目录。
然后运行脚本`./compile.sh`

## 参考资料
- https://www.tensorflow.org/install/install_sources
