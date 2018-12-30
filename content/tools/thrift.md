---
title: "Thrift 快速入门"
layout: page
date: 2018-12-17
---
[TOC]

## 关于
Thrift是一种接口描述语言和二进制通讯协议，它被用来定义和创建跨语言的服务。
它被当作一个远程过程调用框架来使用，是由Facebook为“大规模跨语言服务开发”而开发的。

> 远过程调用(RPC), 可以通过像调用本地函数一样,调用远程服务。

## 安装
MAC `brew install thrift`

thrift采用thrift文件 + 编译器的方式, 最后生成目标语言的代码,这些代码直接嵌入到你的项目中就可以使用了。

## Helloword
参考: <https://thrift.apache.org/> <https://www.ibm.com/developerworks/cn/java/j-lo-apachethrift/index.html>

helloword.thrift

```thrift

service HelloWorldServer
{
    string call(1: string name)
}
```


编译 `thrift --gen <language> <Thrift filename>`

以JAVA为例, 执行编译命令 `thrift --gen java helloword.thrift` 可以得到一个 `HelloWorldServer.java` 文件, 将它放到适当的目录即可。
在JAVA工程的pom.xml文件中,增加 libthrift 依赖。

```xml
<dependency>
  <groupId>org.apache.thrift</groupId>
  <artifactId>libthrift</artifactId>
  <version>0.11.0</version>
</dependency>
```

服务端实现 `HelloWorldServerImpl.java` 实现接口

```java
public class HelloWorldServerImpl implements HelloWorldServer.Iface {
    @Override
    public String call(String name) throws TException{
        return "Hello, " + name;
    }
}
```

启动一个简单的服务器

```java
public class HelloWorldServerThread {
    public static final int port = 9120;
    public static void main(String[] args) throws Exception{
        ServerSocket socket = new ServerSocket(port);
        TServerSocket tsocket = new TServerSocket(socket);
        HelloWorldServer.Processor processor = new HelloWorldServer.Processor(new HelloWorldServerImpl());
        TServer server = new TSimpleServer(new TServer.Args(tsocket).processor(processor));
        System.out.println("Starting the simple server...");
        server.serve();
    }
}
```

启动一个简单的客户端

```java
public class HelloWorldClient {
    public static void main(String[] args) throws Exception{
        TTransport transport = new TSocket("localhost",  HelloWorldServerThread.port);
        transport.open();

        TProtocol protocol = new TBinaryProtocol(transport);
        HelloWorldServer.Client client = new HelloWorldServer.Client(protocol);
        System.out.println(client.call("world!"));
    }
}
```

输出结果: `Hello, world!`


### python实现
需要安装 `pip install thrift`

```server
from helloword import HelloWorldServer

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import logging

logging.basicConfig(level=logging.INFO,
                    format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S',
                    )
                    
class HelloWorldHandler():
    def __init__(self):
        self.log = {}

    def call(self, name):
        logging.info("name = " + str(name))
        return "Hello, " + str(name)

if __name__ == '__main__':
    handler = HelloWorldHandler()
    processor = HelloWorldServer.Processor(handler)
    transport = TSocket.TServerSocket(host='0.0.0.0', port=9120)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    ## 多线程server
    server = TServer.TThreadedServer(processor, transport, tfactory, pfactory)

    logging.info('server info : ' + str(server))
    server.serve()
```