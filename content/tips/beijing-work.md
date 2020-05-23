---
layout: page
title: "北京工作居住证申请系统"
date: 2020-05-19
---
[TOC]


# 关于
- 在MAC下申请北京工作居住证，在填报系统的时候发现各种坑，政府网站就是这样，BUG太多。按照本教程可以成功填报。

# 准备工具
- chrome浏览器
- 安装插件 [tampermonkey](http://www.tampermonkey.net/) ，用于注入js脚本

# 分步骤

## 登陆系统
- 进入系统正常登陆，没有什么问题 <http://219.232.200.39/uamsso/>

## 进入居住证填报系统
- 点击后会出现乱码，空白页
- 解决方法是进入空白页后，点击链接 <http://219.232.200.39/uamsso/SSOSecService?LinkType=online&LinkID=666>  或者创建一个新标签页，地址栏输入上述地址也可以

## 填报系统按钮点击无反应
- 利用插件 tampermonkey 创建如下脚本，然后即可正常填报了。

```javascript
// ==UserScript==
// @name         工作居住证网站脚本
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       zuoyuan@tracholar.com
// @match        http://219.232.200.42/yjrc/*
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    var scriptNodes = document.getElementsByTagName('script');

    var n = scriptNodes.length;
    for(var i=0; i < n; i++){
        var script = scriptNodes[i].text.replace('window.onload', 'window_onload');
        var node = document.createElement('script');
        node.text = script;
        document.head.append(node);

        console.log(script);

    }

})();
```
