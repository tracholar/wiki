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

- 程序入口包是｀main｀
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












[1] Go学习网页<https://tour.go-zh.org/>












