---
title: "Thrift 快速入门"
layout: page
date: 2018-12-17
---

Thrift是一种接口描述语言和二进制通讯协议，它被用来定义和创建跨语言的服务。
它被当作一个远程过程调用框架来使用，是由Facebook为“大规模跨语言服务开发”而开发的。

> 远过程调用(RPC), 可以通过像调用本地函数一样,调用远程服务。

## Helloword

helloword.thrift

```thrift

service HelloWorldServer
{
    string call(1: string name)
}
```

