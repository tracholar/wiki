---
title: "go语言"
layout: page
date: 2016-06-29
---
[TOC]


## 安装环境
只试过MAC OS环境，所以就说这个，不同环境应该差不多。
安装GO运行环境，然后配置两个环境变量｀GOPATH｀和｀GOBIN｀


## 基础

### 基础语法

- 程序入口包是`main`
- 导出名的首字母必须为大写！
- 批量import方式
```go
import (
	"fmt"
	"math"
)
```
- 函数签名方式，可以将相同类型的形参类型申明合并
```go
func add(x int, y int) int {
	#TODO
	return x+y
}
```
- 函数可以返回多值。
```go
func swap(x, y string) (string, string) {
	return y, x
}
```
- 赋值语法`a := "hello world."`
- 命名返回值：Go 的返回值可以被命名，并且就像在函数体开头声明的变量那样使用。
  返回值的名称应当具有一定的意义，可以作为文档使用。
  没有参数的 return 语句返回各个返回变量的当前值。这种用法被称作“裸”返回。
  直接返回语句仅应当用在像下面这样的短函数中。在长的函数中它们会影响代码的可读性。
```go
func getsum(sum int) (x, y int){
	sum = x+y
	return
}
```
- 变量声明关键字`var`，类型放在最后。`=`用于初始化变量，初始化使用
  表达式可以省略类型。
```go
var c, python, java bool
var i, j int = 1, 2
var ruby, Go, haskell = true, false, "helloword"
```
- 短声明变量，在函数中， `:=` 简洁赋值语句在明确类型的地方，可以用于替代 var 定义。
- 基本数据类型
```go
bool

string

int  int8  int16  int32  int64
uint uint8 uint16 uint32 uint64 uintptr

byte // uint8 的别名

rune // int32 的别名
     // 代表一个Unicode码

float32 float64

complex64 complex128
```
- 零值
变量在定义时没有明确的初始化时会赋值为 零值 。
零值是：
	- 数值类型为 0 ，
	- 布尔类型为 false ，
	- 字符串为 "" （空字符串）。
- 表达式 T(v) 将值 v 转换为类型 T 。
  与 C 不同的是 Go 的在不同类型之间的项目赋值时需要显式转换.
- GO类型推导，变量的类型由右值推导得出。
```go
func main() {
	v := "HaHa" // change me!
	fmt.Printf("v is of type %T\n", v)
}
```
- 常量的定义`const Pi = 3.14`，常量不能使用 := 语法定义。`=`表示定义，而`:=`表示赋值？
- Go 只有一种循环结构—— for 循环。循环初始化语句和后置语句都是可选的。
```go
sum := 0
for i := 0; i < 10; i++ {
	sum += i
}
```
- for 是 Go 的 “while”
```go
for sum < 1000 {
	sum += sum
}
```
- 死循环
```go
for {
}
```
- if 条件判断
```go
if x < 0 {
	return sqrt(-x) + "i"
}

if x >10 {
	...
} else {
	...
}
```
> GO 的循环条件和判断条件都不需要用小括号括起来，但是括起来也是可以的。
> 亲测通过，另外for和if条件表达式中定义的变量的作用域仅在该语句块中有效。

- switch分支语句, switch 的条件从上到下的执行，当匹配成功的时候停止。不像C需要break语句
- 没有条件的 switch
  没有条件的 switch 同 switch true 一样。
  这一构造使得可以用更清晰的形式来编写长的 if-then-else 链。
```go
	switch {
	case t.Hour() < 12:
		fmt.Println("Good morning!")
	case t.Hour() < 17:
		fmt.Println("Good afternoon.")
	default:
		fmt.Println("Good evening.")
	}
```
- defer 语句会延迟函数的执行直到上层函数返回。
  延迟调用的参数会立刻生成，但是在上层函数返回前函数都不会被调用。
- defer栈：延迟的函数调用被压入一个栈中。当函数返回时， 会按照后进先出的顺序调用被延迟的函数调用。
- Go 具有指针。 指针保存了变量的内存地址。
  类型 `*T` 是指向类型 `T` 的值的指针。其零值是 `nil` 。
  Go指针和C指针的区别在于Go没有指针运算。
- 结构体，和C差不多，利用关键字`type`定义声明。
```go
type Vertex struct {
	X int
	Y int
}
```
结构体也可以通过指针访问，还是用符号`.`访问成员

- 结构体文法。 结构体文法表示通过结构体字段的值作为列表来新分配一个结构体。
  使用 Name: 语法可以仅列出部分字段。（字段名的顺序无关。）
  特殊的前缀 & 返回一个指向结构体的指针。
```go
var (
	v1 = Vertex{1, 2}  // 类型为 Vertex
	v2 = Vertex{X: 1}  // Y:0 被省略
	v3 = Vertex{}      // X:0 和 Y:0
	p  = &Vertex{1, 2} // 类型为 *Vertex
)
```
- 数组，类型 [n]T 是一个有 n 个类型为 T 的值的数组。
```go
var a [10]int
```

- slice，一个slice会指向一个序列的值，并且包含长度信息，`len(s)`返回序列`s`的长度。
- slice的slice，类似于二维数组，初始化代码
```go
game := [][]string{
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
		[]string{"_", "_", "_"},
	}
```
- slice切片操作，类似于python代码
```go
s[lo:hi]
s[:hi]
s[lo:]
```
- make函数构造slice, `make(type, value [, capacity])`. 函数`cap()`获取slice的容量。
```go
a := make([]int, 5)
```
- slice的零值是`nil`。
- slice添加元素，`append(slice, value1, ..., valueN)`方法，添加元素的时候，slice的容量会自动增加。


- slice用法：<https://blog.go-zh.org/go-slices-usage-and-internals>
- `range` 迭代，第一个为下标，第二个为值的拷贝。可以用`_`来忽略下标
```go
var pow = []int{1, 2, 4, 8, 16, 32, 64, 128}
for i, v := range pow {
		fmt.Printf("2**%d = %d\n", i, v)
}
for _, v := range pow {
}
for idx := range pow {
}
```
- map 映射键到值。 类似于python的字典
map 在使用之前必须用 make 来创建；值为 nil 的 map 是空的，并且不能对其赋值。
```go
m = make(map[string]int)
m["Bell Labs"] = 8
```
- map 的操作
在 map m 中插入或修改一个元素：
`m[key] = elem`
获得元素：
`elem = m[key]`
删除元素：
`delete(m, key)`
通过双赋值检测某个键存在：
`elem, ok = m[key]`
如果 key 在 m 中， ok 为 true。否则， ok 为 false，并且 elem 是 map 的元素类型的零值。
同样的，当从 map 中读取某个不存在的键时，结果是 map 的元素类型的零值。

- 函数可以作为值传递
- 函数闭包
```go
// fibonacci 函数会返回一个返回 int 的函数。
func fibonacci() func() int {
	var a0, a1 = 0, 1
	return func() int{
		var tmp = a0 + a1
		a0 = a1
		a1 = tmp
		return a0
	}
}
```

- go没有类！！！然而，仍然可以在结构体类型上定义方法。
方法接收者 出现在 func 关键字和方法名之间的参数中。
```go
func (v *Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}
v.Abs()
```
- 你可以对包中的 任意 类型定义任意方法，而不仅仅是针对结构体。
但是，不能对来自其他包的类型或基础类型定义方法。
```go
type MyFloat float64

func (f MyFloat) Abs() float64 {
	if f < 0 {
		return float64(-f)
	}
	return float64(f)
}

func main() {
	f := MyFloat(-math.Sqrt2)
	fmt.Println(f.Abs())
}
```
- 指针作为函数接收者，有两个原因需要使用指针接收者。首先避免在每个方法调用中拷贝值（如果值类型是大的结构体的话会更有效率）。
  其次，方法可以修改接收者指向的值。
- 接口是由一组方法定义的集合。接口区分类型本身的方法和类型指针绑定的方法。
```go
type Abser interface {
	Abs() float64
}
```
- 隐式接口？什么鬼东西
- Stringers
一个普遍存在的接口是 fmt 包中定义的 Stringer。
```go
type Stringer interface {
    String() string
}
```
Stringer 是一个可以用字符串描述自己的类型。`fmt`包 （还有许多其他包）使用这个来进行输出。
- 错误接口
```go
type error interface {
    Error() string
}
```
- Reader接口






[1] Go学习网页<https://tour.go-zh.org/>
