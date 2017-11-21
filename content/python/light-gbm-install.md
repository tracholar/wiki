---
title: "MAC上安装light gbm库"
layout: page
date: 2017-10-30
---
[TOC]

## 步骤
参考<https://stackoverflow.com/questions/44937698/lightgbm-oserror-library-not-loaded>

1. `brew install cmake`
2. `brew install gcc --without-multilib`
3. `git clone --recursive https://github.com/Microsoft/LightGBM ; cd LightGBM`
4. `mkdir build ; cd build`
5. `cmake .. -DCMAKE_C_COMPILER=/usr/local/Cellar/gcc/6.2.0/bin/gcc-6  -DCMAKE_CXX_COMPILER=/usr/local/Cellar/gcc/6.2.0/bin/g++-6  -DOpenMP_C_FLAGS=-fopemmp`  注意把gcc的两个路径改成你机器上的路径，DOpenMP_C_FLAGS标记是因为cmake脚本没有自动找到opemmp的路径，手动指定
6. `make -j`
7. `cd python-packages`
8. `sudo python setup.py install`
