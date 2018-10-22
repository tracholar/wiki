---
title: "Spark reducebykey"
layout: page
date: 2018-10-22
---
[TOC]


spark reduceByKey 是默认包含了map端combine操作的, 不过如果key是array除外,这种无法使用map段合并
