---
title: "protobuff 快速入门"
layout: page
date: 2018-12-17
---
[TOC]
google代码仓库 <https://github.com/protocolbuffers/protobuf>

## 定义消息结构

libsvm.proto libffm数据, field可选, rowid和label也是可选项。

```proto
syntax = "proto2";

package tutorial;

option java_package = "com.tracholar.protobuf";
option java_outer_classname = "Libsvm";


message LabelPoint {
    message Point {
        required int32 x = 1; // idx
        required float y = 2; // value
        optional int32 f = 3; // field
    }
    repeated Point feature = 1;
    optional float label = 2;
    optional string rowid = 3;
}

message DataSet {
    repeated LabelPoint lp = 1;
}
```

### Field Rules
- required: 必须有的字段.
- optional: 可选的字段.
- repeated: 可以有多个的字段,对应于 List, 保序.

### 基础数据类型
<https://developers.google.com/protocol-buffers/docs/proto>

int32, float, string, 枚举类型

```proto
enum EnumNotAllowingAlias {
  UNKNOWN = 0;
  STARTED = 1;
}
```

## 编译生成目标语言代码

编译生成Java 代码 `protoc --java_out=<JAVA代码输出路径>  libsvm.proto`。

编译器 `protoc` 可以直接从官网下载 <https://github.com/protocolbuffers/protobuf/releases> 
找到 `protoc-x.x.x-<OS>.zip` 下载解压即可。

把编译器输出的文件拖到合适的目录即可,也可以在 `--java_out` 参数中指定输出目录。

以JAVA为例,需要在工程的pom.xml文件中添加依赖才能使用protobuf的API

```xml
<dependency>
    <groupId>com.google.protobuf</groupId>
    <artifactId>protobuf-java</artifactId>
    <version>3.5.1</version>
</dependency>
```

测试序列化与反序列化, 序列化的对象都是通过 `Builder` 来创建。

- 序列化

```java
public class TestLibsvm {
    public static void main(String[] args) {
        Libsvm.DataSet.Builder dataSet = Libsvm.DataSet.newBuilder();
        Random r = new Random();
        for(int i=0; i< 1000; i++){
            Libsvm.LabelPoint.Builder lp = Libsvm.LabelPoint.newBuilder();
            for(int j=0; j<5; j++){
                Libsvm.LabelPoint.Point pb = Libsvm.LabelPoint.Point
                        .newBuilder()
                        .setF(j)
                        .setX(r.nextInt())
                        .setY(r.nextFloat())
                        .build();
                lp.addFeature(pb);
            }
            //lp.setRowid(r.nextLong() + "");
            dataSet.addLp(lp);
        }
        Libsvm.DataSet dataset = dataSet.build();
        try {
            dataset.writeTo(new FileOutputStream("train.data"));
        }catch (IOException e){
            e.printStackTrace();
        }
    }
}
```

- 反序列化: 反序列化非常简单,直接调用消息对象的静态方法 `parseFrom` 即可, 

```java
Libsvm.DataSet.parseFrom(new FileInputStream("train.data"))
```

上述序列化与反序列化都是针对中间结果是二进制数据,如果中间数据使用文本格式,需要用 TextFormat 对象转换。以python为例

```python
text_format.MessageToString(message[, as_one_line=True[, as_utf8=True]])
text_format.Parse(pbstring, message)
```

而在Java和C++中,可以直接调用 `toString()` 和 `toDebugString()` 这种方法实现。<https://stackoverflow.com/questions/33557965/print-human-friendly-protobuf-message>


## gRPC
