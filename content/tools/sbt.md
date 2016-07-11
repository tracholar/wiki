---
title: "SBT - scala构建工具"
layout: page
date: 2016-07-11
---
[TOC]


## 关于
sbt是scala的交互式构建工具，类似于java的maven。

## 构建项目
### 从单个文件构建
最简单的方式是从只包含一个scala源文件的目录构建项目。
例如在目录`helloword`创建一个`HW.scala`文件

```scala
object Hi {
  def main(args: Array[String]) = println("Hi!")
}
```

然后运行`sbt`，和`run`，或者直接运行`sbt run`命令。

sbt自动寻找下列目录

- Sources in the base directory
- Sources in src/main/scala or src/main/java
- Tests in src/test/scala or src/test/java
- Data files in src/main/resources or src/test/resources
- jars in lib

### 构建文件`build.sbt`
位于项目根目录，一个简单的构建文件如下

```scala
lazy val root = (project in file(".")).
  settings(
    name := "hello",
    version := "1.0",
    scalaVersion := "2.11.7"
  )
```
如果是要打包到jar文件，name和version是必须的。

添加依赖

```scala
val derby = "org.apache.derby" % "derby" % "10.4.1.3"

lazy val commonSettings = Seq(
  organization := "com.example",
  version := "0.1.0",
  scalaVersion := "2.11.7"
)

lazy val root = (project in file(".")).
  settings(commonSettings: _*).
  settings(
    name := "hello",
    libraryDependencies += derby
  )
```
依赖库的写法是：
```
groupID % artifactID % revision
```
如果将依赖的库放在lib目录下，就不需要添加该依赖。
### 目录结构


- 源码
sbt的源码目录结构与maven一样。

```
src/
  main/
    resources/
       <files to include in main jar here>
    scala/
       <main Scala sources>
    java/
       <main Java sources>
  test/
    resources
       <files to include in test jar here>
    scala/
       <test Scala sources>
    java/
       <test Java sources>
```

- 构建定义文件
包含根目录的`build.sbt`，其他构建文件放在project目录下。

- 构建输出文件目录是`target/`，在`.gitignore`中应该排除该目录

## 两种构建模式
- 交互式模式，输入`sbt`命令后面不跟参数，然后进入交互式环境，然后运行命令构建。
- 批处理模式，跟参数罗。`sbt clean compile "testOnly TestA TestB"`
- 常见命令，更多参考

```
clean	删除所有生成的文件 （在 target 目录下）。
compile	编译源文件（在 src/main/scala 和 src/main/java 目录下）。
test	编译和运行所有测试。
console	进入到一个包含所有编译的文件和所有依赖的 classpath 的 Scala 解析器。输入 :quit， Ctrl+D （Unix），或者 Ctrl+Z （Windows） 返回到 sbt。
run <参数>*	在和 sbt 所处的同一个虚拟机上执行项目的 main class。
package	将 src/main/resources 下的文件和 src/main/scala 以及 src/main/java 中编译出来的 class 文件打包成一个 jar 文件。
help <命令>	显示指定的命令的详细帮助信息。如果没有指定命令，会显示所有命令的简介。
reload	重新加载构建定义（build.sbt， project/*.scala， project/*.sbt 这些文件中定义的内容)。在修改了构建定义文件之后需要重新加载。
```
