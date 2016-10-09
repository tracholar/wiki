---
layout: page
title: "requests包"
date: 2016-10-08
---
[TOC]

## 关于
如果你还在为urllib包而烦恼，不妨试试Python Requests包。
这个包的特点是，简单明了！

## 快速入门
参考[官方文档](http://docs.python-requests.org/zh_CN/latest/user/quickstart.html)。

### 基本操作
- GET: `requests.get("https://github.com/timeline.json")`
- POST:
- PUT
- HEAD
- DELETE
- OPTIONS

- 传递URL参数：

```python
payload = {'key1': 'value1', 'key2': 'value2'}
r = requests.get("http://httpbin.org/get", params=payload)
print r.url
```

- 响应内容 `r.text`（文本）, `r.content`（二进制），编码`r.encoding`。requests会自动帮你解码 `gzip`！
- JSON响应 `r.json()`
- 原始套接字

```python
r = requests.get(url, stream=True)
r.raw
r.iter_content(chunk_size)
```

- 定制HTTP头 `requests.get(url, headers=headers)`
- POST multipart-encoded

```python
files = {'file': open('report.xls', 'rb')}
r = requests.post(url, files=files)
```

- Cookie `r.cookie` 是一个字典
