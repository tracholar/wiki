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


## dependencies 依赖管理
### 手动管理
手动将库的jar包复制到lib目录下就可以了。如果要更改默认路径，需要修改`unmanagedBase`，
例如修改到`custom_lib/`目录可以用下述命令

```scala
unmanagedBase := baseDirectory.value / "custom_lib"
```
更多的控制可以通过重载unmanagedJars这个task，默认的实现是

```scala
unmanagedJars in Compile := (baseDirectory.value ** "*.jar").classpath
```
如果要添加多个路径到默认路径，可以这样写

```scala
unmanagedJars in Compile ++= {
    val base = baseDirectory.value
    val baseDirectories = (base / "libA") +++ (base / "b" / "lib") +++ (base / "libC")
    val customJars = (baseDirectories ** "*.jar") +++ (base / "d" / "my.jar")
    customJars.classpath
}
```
这里对路径的语法，参考后面的路径



### 自动管理
sbt支持三种自动管理方式，都是通过Apache ivy来实现的。

- Declarations in your project definition
- Maven POM files (dependency definitions only: no repositories)
- Ivy configuration and settings files

可以通过下述语句声明依赖，其中configuration是可选的。
多个依赖可以通过`Seq`将每一个依赖作为一个元素进行添加，注意链接操作符号的区别，
libraryDependencies是一个 Seq ?

```scala
libraryDependencies += groupID % artifactID % revision % configuration
libraryDependencies ++= Seq(
  groupID %% artifactID % revision,
  groupID %% otherID % otherRevision
)
```

> If you are using a dependency that was built with sbt, double the first % to be %%
>
> sbt uses the standard Maven2 repository by default.

revision除了可以使用常规的完整版本号外，还可以使用 "latest.integration", "2.9.+", or "[1.0,)"这种形式。

#### resolvers
可以通过设置resolvers来添加依赖库获取的位置，格式是
`resolvers += name at location`，location可以是合法的URI，例如

```scala
resolvers += "Sonatype OSS Snapshots" at "https://oss.sonatype.org/content/repositories/snapshots"
resolvers += "Local Maven Repository" at "file://"+Path.userHome.absolutePath+"/.m2/repository"
externalResolvers := Resolver.withDefaultResolvers(resolvers.value, mavenCentral = false)
```

#### configuration
- 指定URL

```scala
libraryDependencies += "slinky" % "slinky" % "2.1" from "https://slinky2.googlecode.com/svn/artifacts/2.1/slinky.jar"
libraryDependencies += "org.apache.felix" % "org.apache.felix.framework" % "1.8.0" intransitive()
libraryDependencies += "org.testng" % "testng" % "5.7" classifier "jdk15"
libraryDependencies +=
  "org.lwjgl.lwjgl" % "lwjgl-platform" % lwjglVersion classifier "natives-windows" classifier "natives-linux" classifier "natives-osx"
libraryDependencies +=
    "log4j" % "log4j" % "1.2.15" exclude("javax.jms", "jms")
libraryDependencies +=
      "org.apache.felix" % "org.apache.felix.framework" % "1.8.0" withSources() withJavadoc()
```

-

### 使用pom xml文件添加依赖
```
externalPom()
externalPom(Def.setting(baseDirectory.value / "custom-name.xml"))
```

### 路径
#### 创建文件和路径
sbt 0.10+ 使用`java.io.File` 文件类型。
创建文件方法

```scala
val source: File = file("/home/user/code/A.scala")
def readme(base: File): File = base / "README"
```
sbt 添加了`/`方法，对应于两参数构造函数。

`baseDirectory` task 返回bese目录绝对路径。

#### 路径finder
一个路径finder返回一个`Seq[File]`。例如

```scala
def scalaSources(base: File): Seq[File] = {
  val finder: PathFinder = (base / "src") ** "*.scala"
  finder.get
}
```
The `**` method accepts a `java.io.FileFilter` ，筛选目录及子目录下所有文件。
如果只访问该目录可以使用 `*` 函数。
惰性求值使得需要调用`.get`才能计算结果。

name filter 使用 `*`表示0个或多个字符。用`||`表示多个filter的或，用`--`表示排除。

```scala
val base = baseDirectory.value
(base / "src") * "*Test*.scala"
(base / "src") ** ("*.scala" || "*.java")
(base/"src"/"main"/"resources") * ("*.png" -- "logo.png")
```

组合多个finder `+++`, 排除结果可以用 `---`。
finder有一个filter方法，用于进一步筛选

```scala
(base / "lib" +++ base / "target") * "*.jar"
( (base / "src") ** "*.scala") --- ( (base / "src") ** ".svn" ** "*.scala")
( (base / "src") ** "*") filter { _.isDirectory }
base filter ClasspathUtilities.isArchive
 ```

 - to string 转换
    - toString
    - absString ，
    - getPaths ， 返回Seq[String]

## 插件
插件用来扩展构建定义，可能是一个新的task。

添加插件申明，在项目根目录下的 `/project` 目录添加 `.sbt` 文件，然后在其中添加语句

```scala
addSbtPlugin("com.typesafe.sbt" % "sbt-site" % "0.7.0")
```

## 调试
传入 `-jvm-debug <port>  Turn on JVM debugging, open at the given port.`参数即可远程调试，例如 `sbt -jvm-debug 5005 run`
