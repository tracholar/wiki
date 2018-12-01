---
title: "Tensorflow 编译吐血历史"
layout: page
date: 2018-11-07
---
[TOC]

## 关于
因为要在CentOS6中运行TensorFlow,但是TensorFlow依赖的Glibc版本都比较高,所以只能从源码编译,蛋疼!

因为编译TensorFlow需要bazel,而bazel官方编译好的包也需要CentOS7,所以bazel也需要从源码编译!


bazel 需要Java8,所以如果没有安装Java8的还需要先安装Java8

## 安装Java8
1. 去Oracle官网下一个:<https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html> 
   或者用我的百度云镜像 <https://pan.baidu.com/s/161G0d3GYsw5dY6En4SHpjA>
2. 下载完解压即可,然后配置JAVA_HOME环境变量,鉴于你的系统可能有多个JAVA版本,建议你只在脚本中或者在命令行中临时设置这个变量

```bash
export JAVA_HOME=path-to-java-directory
export PATH=path-to-java-directory/bin:$PATH
```

我的做法是将上述保存到env.sh中,然后在 compile.sh 中加载 `source env.sh`

通过 `java -version` 确认当前使用的是刚下载的java8,而不是系统的java7等版本。

## 编译Bazel
参考:<https://docs.bazel.build/versions/master/install-compile-source.html>

设置好JAVA8环境变量之后, GCC版本也不能太低,安装高版本GCC6.0

执行编译命令 `./compile.sh`,然后发现错误:

```bash
bazel-out/host/bin/third_party/protobuf/3.4.0/js_embed: /usr/lib64/libstdc++.so.6: version `GLIBCXX_3.4.21' not found (required by bazel-out/host/bin/third_party/protobuf/3.4.0/js_embed)
```

参考文章<https://www.jianshu.com/p/6f9652a9410d>将libstdc++改为静态链接

```bash 
tools/cpp/unix_cc_configure.bzl:222
  "-lstdc++"     改为          "-l:libstdc++.a"
```

## 编译TensorFlow

1. 运行配置`./configure`
2. 编译pip包 `bazel build --config=opt //tensorflow/tools/pip_package:build_pip_package`

配置的时候,OPENCL关闭,jemalloc也要关闭,将所有都关闭,哈哈

编译时增加 -lrt 参数,参考 <https://www.cnblogs.com/lasclocker/p/9578623.html>

## 运行测试
将上述编译好的三个文件 `libtensorflow_framework.so, libtensorflow.jar, libtensorflow_jni.so` 复制到一个目录中,然后创建测试代码 `HelloTF.java`

```java 
import org.tensorflow.Graph;
import org.tensorflow.Session;
import org.tensorflow.Tensor;
import org.tensorflow.TensorFlow;

public class HelloTF {
  public static void main(String[] args) throws Exception {
    try (Graph g = new Graph()) {
      final String value = "Hello from " + TensorFlow.version();

      // Construct the computation graph with a single operation, a constant
      // named "MyConst" with a value "value".
      try (Tensor t = Tensor.create(value.getBytes("UTF-8"))) {
        // The Java API doesn't yet include convenience functions for adding operations.
        g.opBuilder("Const", "MyConst").setAttr("dtype", t.dataType()).setAttr("value", t).build();
      }

      // Execute the "MyConst" operation in a Session.
      try (Session s = new Session(g);
           Tensor output = s.runner().fetch("MyConst").run().get(0)) {
        System.out.println(new String(output.bytesValue(), "UTF-8"));
      }
    }
  }
}
```

编译 `javac -cp libtensorflow.jar HelloTF.java`

执行 `java -cp libtensorflow.jar:.    -Djava.library.path=.  HelloTF` 如果加载so失败,可以增加参数 `-Dorg.tensorflow.NativeLibrary.DEBUG=1`
