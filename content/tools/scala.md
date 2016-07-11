---
title: "scala语言"
layout: page
date: 2016-07-05
---
[TOC]

## 基础语法
- 不变量`val`，变量`var`
- 基础类型：
    - Int

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
- 包，和Java的一样
- 模式匹配
匹配值
```scala
val times = 1

times match {
    case 1 => "one"
    case 2 => "two"
    case _ => "some others"
}

times match {
    case i if i == 1 => "one"
    case i if i == 2 => "two"
    case _ => "some others"
}
```
匹配类型
```scala
def bigger(o: Any): Any = {
  o match {
    case i: Int if i < 0 => i - 1
    case i: Int => i + 1
    case d: Double if d < 0.0 => d - 0.1
    case d: Double => d + 0.1
    case text: String => text + "s"
  }
}
```
匹配类成员
```scala
def calcType(calc: Calculator) = calc match {
  case _ if calc.brand == "hp" && calc.model == "20B" => "financial"
  case _ if calc.brand == "hp" && calc.model == "48G" => "scientific"
  case _ if calc.brand == "hp" && calc.model == "30B" => "business"
  case _ => "unknown"
}
```
- 样本类 case class
```scala
case class Calculator(brand: String, model: String)
```
case classes are designed to be used with pattern matching. Let’s simplify our calculator classifier example from earlier.
样本类就是被设计用在模式匹配中的。让我们简化之前的计算器分类器的例子。
```scala
def calcType(calc: Calculator) = calc match {
  case Calculator("hp", "20B") => "financial"
  case Calculator("hp", "48G") => "scientific"
  case Calculator("hp", "30B") => "business"
  case Calculator(ourBrand, ourModel) => "Calculator: %s %s is of unknown type".format(ourBrand, ourModel)
}
```
我们也可以将匹配的值重新命名。
```scala
  case c@Calculator(_, _) => "Calculator: %s of unknown type".format(c)
```
- 异常， try ... catch ... finally

## 基本数据结构
- List 列表
```scala
val numbers = List(1,2,3,4)
```
- Set 集
```scala
scala> Set(1,2,1)
res0: scala.collection.immutable.Set[Int] = Set(1, 2)
```
- Tuple 元组
```scala
val hostPort = ("localhost", 80)
hostPort._1  // localhost
hostPort._2  // 80
```
与样本类不同，元组不能通过名称获取字段，而是使用位置下标来读取对象；而且这个下标基于1，而不是基于0。
在创建两个元素的元组时，可以使用特殊语法：`1 -> 2`，见映射
- Map 映射，类似于python的字典，c的hash_map
```scala
Map(1 -> 2)  // 值映射
Map("foo" -> "bar")  // 字符串映射
Map(1 -> Map("foo" -> "bar"))  // 映射到映射
Map("timesTwo" -> {timesTwo(_)})   // 映射到函数
```
`Map`中要获取键对应的值，需要使用`Map.get`方法。
- 选项 Option
Option 是一个表示有可能包含值的容器。
Option 本身是泛型的，有两个子类 `Some[T]` 或 `None`。
在模式匹配中会用到。
```scala
val result = res1 match {
    case Some(n) => n*2
    case None => 0
}
```
Option基本的接口是这样的：
```scala
trait Option[T] {
  def isDefined: Boolean
  def get: T
  def getOrElse(t: T): T
}
```
Option本身是泛型的，并且有两个子类： Some[T] 或 None。
`Map.get`使用`Option`作为其返回值，表示这个方法也许不会
返回你请求的值。
类似于Haskell的`Maybe`？

### 函数组合子 Functional Combinators
- map 组合子
例子：`List(1,2,3,4) map {i:Int => i*i}`，
或者这样调用
```scala
numbers.map((i:Int) => i * 2)
```
- foreach， 很像map，但是没有返回值。仅用于有副作用的函数？
```scala
numbers.foreach((i:Int) => i * 2)
```
- filter，一处任何传入函数计算结果为`false`的元素。
- zip，将两个列表的内容聚合到一个对偶列表中。
```scala
List(1,2,3).zip(List("a","b","c"))  //[(1,a),(2,b),(3,c)]
```
- `partition`, 使用给定的谓词函数（返回true和false的函数）分割列表，返回tuple
- `find`，返回集合中第一个匹配谓词函数的元素
- `drop` 和 `dropWile`，`drop`删除前i个元素，`dropWhile`将删除
  元素直到不满足条件为止。
- foldLeft， 左折叠。需要传入一个初始值和一个二元函数
- foldRight，右折叠
- flatten，展平。
- flatMap，等价于 flatten . map

### 函数组合
- `compose`组合其它函数形成新的函数`f(g(x))`。
```scala
val fg = f _ compose g _
```
`println`是啥？为甚不能组合。
- `andThen`，与`compose`很像，只是执行顺序相反，先执行第一个。

### 偏函数
不是部分应用函数，篇函数是指只能接受该类型的某些特定的值。
`isDefinedAt`用来确定该函数能否接受一个给定的参数。
```scala
val one: PartialFunction[Int, String] = { case 1 => "one" }
one.isDefinedAt(1)   // true
one.isDefinedAt(2)   // false
```
PartialFunctions可以使用orElse组成新的函数，得到的PartialFunction反映了是否对给定参数进行了定义。
```scala
scala> val two: PartialFunction[Int, String] = { case 2 => "two" }
two: PartialFunction[Int,String] = <function1>

scala> val three: PartialFunction[Int, String] = { case 3 => "three" }
three: PartialFunction[Int,String] = <function1>

scala> val wildcard: PartialFunction[Int, String] = { case _ => "something else" }
wildcard: PartialFunction[Int,String] = <function1>

scala> val partial = one orElse two orElse three orElse wildcard
partial: PartialFunction[Int,String] = <function1>

scala> partial(5)
res24: String = something else

scala> partial(3)
res25: String = three

scala> partial(2)
res26: String = two

scala> partial(1)
res27: String = one

scala> partial(0)
res28: String = something else
```

## 类型，静态类型
- 参数化多态，秩1多态性rank-one。下面是一个错误的例子，将会报编译错误。

```scala
def foo[A, B](f: A->List[A], b: B) = f(b)
def foo[A](f: A->List[A], b: Int) = f(i)
```

- 类型推断
Hindley Milner算法。 Scala编译器为我们做类型推断，
使得可以不明确指定返回类型。

```scala
def id[T](x : T) = x
val x = id("hey")
```

- 变性 Variance，如果T'是T的子类，那么Container[T']和Container[T]的关系呢？
    - 协变， C[T']也是C[T]的子类， [+T]
    - 逆变， C[T']是C[T]的父类， [-T]
    - 不变， 没有关系， [T]

逆变的例子，函数特质。参数用父类，调用用子类，表明以父类为类型参数的函数
是以子类为类型参数的函数的子类。有点绕，理解一下。

- 边界，指定泛型的大类型？`T <: SomeType` 指定T是SomeType的子类。

```scala
scala> def cacophony[T](things: Seq[T]) = things map (_.sound)
<console>:7: error: value sound is not a member of type parameter T
       def cacophony[T](things: Seq[T]) = things map (_.sound)
                                                        ^

scala> def biophony[T <: Animal](things: Seq[T]) = things map (_.sound)
biophony: [T <: Animal](things: Seq[T])Seq[java.lang.String]

scala> biophony(Seq(new Chicken, new Bird))
res5: Seq[java.lang.String] = List(cluck, call)
```

`T :> SomeType` 指定T是SomeType的超类。
List 同样 定义了`::[B >: T](x: B)` 来返回一个List[B]，例如下面这个例子中，
flock是Bird类型，Bird是Animal的子类。`::`操作后返回的是超类Animal的列表。

```scala
scala> new Animal :: flock
res59: List[Animal] = List(Animal@11f8d3a8, Bird@7e1ec70e, Bird@169ea8d2)
```

- 量化 Quantification。
有时候，不关心类型变量时，可以用通配符取而代之，注意区分变量和类型变量。
个人理解：下面这个例子与类型无关，只与List的接口有关，所以不影响类型推导系统。

可以为通配符指定边界。

```scala
def count[A](l: List[A]) = l.size
def count(l: List[_]) = l.size

def hashcodes(l: Seq[_ <: AnyRef]) = l map (_.hashCode)
```







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
