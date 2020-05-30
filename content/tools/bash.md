---
title: "bash使用技巧"
layout: page
date: 2016-07-02
---
[TOC]
## 例子
提示符修改
```bash
export PS1='\u@\h \w \$ ' (普通示例)
export PS1='\[\e[0;32m\][\u@\h \w \$]\[\e[m\]' (颜色示例)
export PS1='\t: ' (时间示例)
export PS1='\u@\h [\$(ls | wc -l)]:\$ ' (显示当前目录行下文件数量)
```


## 基本语法
- 流控制 if ... elif ... else
```bash
if condition
then
    statements
[elif condition
    then statements. ..]
[else
    statements ]
fi
```
其中condition可以是一个命令，也可以是一个条件表达式，其中条件判断有很多细节。

- for 循环。

```bash
## use list
for i in 1 2 3 5 6
do
    echo $i
done

for i in {1,3,6,7}
do
    echo $i
done

## use seq command
for i in $(seq 1 2 20)
do
    echo $i
done

## C-style
for ((c=1; c<=5; c++))
do
    echo $i
done

## use file
for f in /etc/*
do
    echo f
done
```

## zip命令


## 命令行参数相关
- 脚本内获取参数的格式为：`$n`。n 代表一个数字，1 为执行脚本的第一个参数，2 为执行脚本的第二个参数，以此类推……
```
  $#	传递到脚本的参数个数
  $*	以一个单字符串显示所有向脚本传递的参数。
        如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数。
  $$	脚本运行的当前进程ID号
  $!	后台运行的最后一个进程的ID号
  $@	与$*相同，但是使用时加引号，并在引号中返回每个参数。
        如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数。
  $-	显示Shell使用的当前选项，与set命令功能相同。
  $?	显示最后命令的退出状态。0表示没有错误，其他任何值表明有错误。
```


## sort
- `sort -u` 去重
- `sort -r` 降序排列
- `sort -o` 对原文件排序后写入原文件，重定向无法实现这个功能
- `sort -n` 数字 
- `sort -t <char> -k <field1>[,field2]` 用 <char>做分隔符，按照第 <field1> 列排序，可以指定多列
- `sort -V` 按照版本排序

## awk
- `-F <fs>` 定义分隔符为字符 <fs>

## for循环
- 批量杀任务
```bash
pid=$(cmd)  # 通过命令获取进程
for p in $pid
do
    echo kill $p
    kill $p
done
```

## 参考
- <https://www.runoob.com/linux/linux-shell.html>