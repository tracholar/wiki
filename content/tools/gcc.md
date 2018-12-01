---
title: "Linux基本安装"
layout: page
date: 2017-03-09 00:00
---

[TOC]

## 源码安装gcc6
以 redhat 环境安装gcc6为例：

- 首先下载 gcc-6.3.0.tar.gz
- 解压 `tar xzf gcc-6.3.0.tar.gz`
- 下载必须的依赖 `./contrib/download_prerequisites` ，会直接从 gnu 官网下载，国内可能很慢，
可以修改这个文件中的命令，使用镜像的源，推荐一个源挺快的 `https://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/infrastructure/`，也可以使用国内的其他源，比如 USTC, THU 的镜像。
- 在gcc目录外键一个目标文件目录 `cd ..  &  mkdir objdir`
- 配置gcc `../gcc-6.3.0/configure --prefix=$HOME/gcc-6.3.0` ，可以将 prefix参数修改为你想要的位置
- 多线程make `make -j4` 修改j参数调整线程数，这个过程需要较长时间，建议用 nohup 后台编译。
- 安装 `make install`


## 源码安装 Python27
- 下载 Python-2.7.13.tgz
- 解压 `tar xzf Python-2.7.13.tgz` 然后进入解压后目录 `cd Python-2.7.13`
- 配置 `./configure --prefix=$HOME/local/usr/Python-2.7.13  --enable-shared` 可以修改 prefix 指定安装目录，使能共享库；
如果要安装 TensorFlow，需要使能 ucs4 支持，增加参数 `--enable-unicode=ucs4` 即可。
- 安装 `make -j4 && make install`

## 安装 TensorFlow
可能存在TensorFlow的 glibc 和 libcxx 版本与系统自带的版本不一致，会导致 import 异常，我是参考这个链接解决的：  <https://stackoverflow.com/questions/33655731/error-while-importing-tensorflow-in-python2-7-in-ubuntu-12-04-glibc-2-17-not-f>。可能不一定适合你的情况，但是原理是一样的，即手动下载 tf 支持的 glibc 和libcxx版本，然后设置环境变量 `LD_LIBRARY_PATH` 使 python 加载正确的版本。我的命令如下

```bash
mkdir ~/libcenv
cd ~/libcenv
wget http://launchpadlibrarian.net/137699828/libc6_2.17-0ubuntu5_amd64.deb
wget http://launchpadlibrarian.net/137699829/libc6-dev_2.17-0ubuntu5_amd64.deb
### 论坛中的那个链接此时已经失效，换一个版本也可以，应该只要版本高于tf要求的libcxx版本即可，没有测试过，不负责任
wget ftp://rpmfind.net/linux/centos/7.3.1611/os/x86_64/Packages/libstdc++-4.8.5-11.el7.x86_64.rpm
ar p libc6_2.17-0ubuntu5_amd64.deb data.tar.gz | tar zx
ar p libc6-dev_2.17-0ubuntu5_amd64.deb data.tar.gz | tar zx
rpm2cpio libstdc++-4.8.5-11.el7.x86_64.rpm| cpio -idmv
```

done！

最后一步，我安装python的时候忘记指定`--enable-unicode=ucs4`，导致报错 `undefined symbol: PyUnicodeUCS4_AsUTF8String`，重新编译 python即可！对此的解释可以参考 <http://blog.csdn.net/taolinke/article/details/50472451>

运行时要指定环境变量，你也可以通过shell函数来做

`LD_LIBRARY_PATH="$HOME/libcenv/lib/x86_64-linux-gnu/:$HOME/libcenv/usr/lib64/" $HOME/libcenv/lib/x86_64-linux-gnu/ld-2.17.so $HOME/local/usr/Python-2.7.13/bin/python`


## gcc常用选项
- `-I<path>` 增加 include 目录,可以多次使用
- `-L<path>` 增加动态库搜索目录,可以多次使用,链接时的目录
- `-Wl,-rpath,<path>` 增加动态库搜索目录,可以多次使用,运行时的目录


参考<http://www.runoob.com/w3cnote/gcc-parameter-detail.html>