---
title: "scala语言"
layout: page
date: 2016-07-05
---
[TOC]

## 基础语法与数据结构
- 不变量`val`，变量`var`
- 基础类型：
    - Int
    -
- `def`创建函数，类型标签
```scala
def addOne(m: Int): Int = m + 1
```
- 匿名函数 `(x:Int) => x+1`
- 函数的部分应用
```scala
def adder(m:Int, n:Int) = m + n
val add2 = adder(2, _:Int)
```
- 柯理化函数
```scala
def muliply(m: Int) (n: Int) = m * n
```
- 可变长参数
```scala
def cap(args String*) = {
    args.map {
        arg => arg.capitalize
    }
}
```
- 类 `class`
- 构造函数不是特殊的方法，他们是除了类的方法定义之外的代码。
```scala
class Calculator(brand: String) {
  /**
   * A constructor.
   */
  val color: String = if (brand == "TI") {
    "blue"
  } else if (brand == "HP") {
    "black"
  } else {
    "white"
  }

  // An instance method.
  def add(m: Int, n: Int): Int = m + n
}
```
- Scala是高度面向表达式的：大多数东西都是表达式而非指令。
- 继承
```scala
class ScientificCalculator(brand: String) extends Calculator(brand) {
  def log(m: Double, base: Double) = math.log(m) / math.log(base)
}
```
- 抽象类
```scala
abstract class Shape {
        def getArea():Int    // subclass should define this
}
```
- Traits 特质
很像接口，通过`with`关键字，一个类可以扩展多个特质。
```scala
trait Car {
  val brand: String
}

trait Shiny {
  val shineRefraction: Int
}
class BMW extends Car with Shiny {
  val brand = "BMW"
  val shineRefraction = 12
}
```
- 泛型，方法和trait都可以引入类型参数
```scala
trait Cache[K, V] {
  def get(key: K): V
  def put(key: K, value: V)
  def delete(key: K)
}
def remove[K](key: K)
```
如何实现像java那样的父类占位符。

- apply方法
```scala
class Bar {
    def apply() = 0
}
val bar = new Bar
bar()
// res: Int = 0
```
- 单例对象，工厂模式
```scala
object Timer {
  var count = 0

  def currentCount(): Long = {
    count += 1
    count
  }
}
Timer.currentCount()
```
- 函数即对象.
函数是一些特质的集合。具体来说，具有一个参数的函数是Function1特质的一个实例。这个特征定义了apply()语法糖，让你调用一个对象时就像你在调用一个函数。
```scala
object addOne extends Function1[Int, Int] {
    def apply(m: Int): Int = m + 1
}
class AddOne extends (Int => Int) {
  def apply(m: Int): Int = m + 1
}
```
- 包




## 构建工具 SBT
- 安装命令`brew install sbt`
- 项目布局
    - 项目 – 项目定义文件
        - project/build/.scala – 主项目定义文件
        - project/build.properties – 项目、sbt和Scala版本定义
        - src/main – 你的应用程序代码出现在这里，在子目录表明代码的语言（如src/main/scala, src/main/java）
        - src/main/resources – 你想要添加到jar包中的静态文件（如日志配置）
        - src/test – 就像src/main，不过是对测试
        - lib_managed – 你的项目依赖的jar文件。由sbt update时填充
        - target – 生成物的目标路径（如自动生成的thrift代码，类文件，jar包）
