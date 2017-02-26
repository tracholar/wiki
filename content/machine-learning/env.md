---
title: "Deep learning 环境搭建大坑"
layout: page
date: 2016-06-30 00:00
---

## 关于
暂时记录了windows和mac系统下遇到的问题。





## MAC SIP权限问题
经常因为SIP无法取得最高权限导致安装出错，关闭SIP权限的方法：
1. 重启MAC，在重启过程中按住command＋R键，进入安全模式，因为只有在安全模式下才能关闭SIP安全特性
2. 在顶部菜单打开终端，在终端输入命令 `csrutil disable`，可以查看这个命令。开启可以使用`enable`参数。
3. 重启系统。


## theano


## tensorflow

linux 安装pip可能找不到对应的版本，需要从tensor的github提供的带 non 的whl包安装。
可能安装后，import会报glibc版本找不到，需要下载 gnu/glibc 对应的版本，编译并安装！

glibc 安装，在一个方便的镜像下载glibc源码，安装

```bash
mkdir build
cd build
../configure --prefix=path-to-install
make & make install
```
