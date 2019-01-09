---
title: "Electron 开发跨平台应用程序"
layout: page
date: 2019-01-03
---
[TOC]


## 关于
- 项目地址: <https://github.com/sindresorhus/awesome-electron>
- 文档: <https://electronjs.org/docs/tutorial/quick-start>
- 你所见到的很多APP都用的是这个工具开发的,比如 Atom, VS Code, 大象 etc

## 快速入门
- 参考 
    - <https://electronjs.org/docs/tutorial/quick-start>
    - <https://electronjs.org/docs/tutorial/first-app>

- 安装, 需要先安装`node`, 和 `npm`

## 基本概念
```javascript
const electron = require('electron')
const { app, BrowserWindow } = require('electron')
```

`electron` 包含两个对象 `app` 和 `BrowserWindow`, `app` 可以用来管理应用程序的生命周期, `BrowserWindow` 用来创建窗口。

```javascript
//创建窗口对象
function createWindow () {
  // Create the browser window.
  win = new BrowserWindow({ width: 800, height: 600 })

  // and load the index.html of the app.
  win.loadFile('index.html')
}

// 将创建窗口动作注册到app完成初始化之后
app.on('ready', createWindow)

```