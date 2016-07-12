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

## 构建定义

一个构建定义是一个Project，拥有一个类型为 Setting[T] 的列表，Setting[T] 是会影响到 sbt 保存键值对的 map 的一种转换，T 是每一个 value 的类型。
参考前面的构建定义示例代码。

每一项 Setting 都定义为一个 Scala 表达式。在 settings 中的表达式是相互独立的，而且它们仅仅是表达式，不是完整的 Scala 语句。????WHAT
这些表达式可以用 val，lazy val，def 声明。 build.sbt 不允许使用顶层的 object 和 class。它们必须写到 project/ 目录下作为完整的 Scala 源文件。

### 键
有三种类型的 key：

- SettingKey[T]：一个 key 对应一个只计算一次的 value（这个值在加载项目的时候计算，然后一直保存着）。
- TaskKey[T]：一个 key 对应一个称之为 task 的 value，每次都会重新计算，可能存在潜在的副作用。
- InputKey[T]：一个 key 对应一个可以接收命令行参数的 task。

键的类型

- 内置键， build.sbt 会隐式包含 import sbt.Keys._ ，所以可以通过 name 取到 sbt.Keys.name。
- 自定义键，创建方法：settingKey，taskKey 和 inputKey 创建自定义 keys.

```scala
lazy val hello = taskKey[Unit]("一个 task 示例")
```

## Tasks 任务
一个简单的hello任务如下，在build.sbt文件中加入下列代码

```scala
lazy val hello = taskKey[Unit]("Prints 'Hello World'")

hello := println("hello world!")
```
然后执行 `sbt hello` 就可以看到结果了。

一个任务首先需要定义一个`taskKey[T]`，在这个例子中返回空类型，每一个任务是一个scala函数，可以
返回一个结果。可以在其他任务中通过`.value`属性访问另一个task的结果。


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


## Task
