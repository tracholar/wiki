---
title: "命令行模块"
layout: page
date: 2019-05-19
---

[TOC]

## argparse模块
基本模板

```python
import argparse
parser = argparse.ArgumentParser(description='描述信息')
parser.add_argument('integers', metavar='N', type=int, nargs='+',
                    help='an integer for the accumulator')
args = parser.parse_args()
print(args.integers)
```

- 主动传参 `parser.parse_args` 默认parse `sys.argv`,也可以主动传参
- 添加参数的方法 `ArgumentParser.add_argument(name or flags...[, action][, nargs][, const][, default][, type][, choices][, required][, help][, metavar][, dest])`

```python
# 可选参数
parser.add_argument('-f', '--foo')

# 位置参数, 不需要输入参数名, 根据位置来判断
parser.add_argument('bar')

# flag选项, 保存常数
# --foo
# foo = 42
parser.add_argument('--foo', action='store_const', const=42)

# 多选项
# --foo 1 --foo 2
# foo = [1, 2]
parser.add_argument('--foo', action='append')

# 多值
# --foo 1 2
# foo = [1,2]
parser.add_argument('--foo', nargs=2)
parser.add_argument('--foo', nargs='*')

# 默认值
# foo = 42
parser.add_argument('--foo', default=42)

# 指定类型
# --foo 34
parser.add_argument('--foo', type=int)

# 文件类型
# --foo file.txt --out out.txt
parser.add_argument('--foo', type=file) # read file
parser.add_argument('--out', type=argparse.FileType('w')) # write file
parser.add_argument('--cb', type=callable) # 自定义类型, type是一个函数

# 指定选择范围
# --move rock
# move = rock
parser.add_argument('move', choices=['rock', 'paper', 'scissors'])

# 必选选项
parser.add_argument('--foo', required=True)

# 帮助信息
parser.add_argument('--foo', action='store_true',
                     help='foo the bars before frobbling')

# 改变目标变量的名字
# --foo 42
# bar = 42
parser.add_argument('--foo', dest='bar')

# 将选项增加到已有对象上
# --foo 42
# C.foo = 42
class C(object):
    pass
c = C()
parser.parse_args(namespace=c)

```

自定义action[ref](https://docs.python.org/2.7/library/argparse.html#argparse.Action): 输入密码,将输入密码隐藏[ref](https://stackoverflow.com/questions/29948567/how-to-hide-password-in-python)

```python
class Password(argparse.Action):
    def __call__(self, parser, namespace, values, option_string):
        if values is None:
            values = getpass.getpass()
        setattr(namespace, self.dest, values)

parser = argparse.ArgumentParser()
parser.add_argument('-p', action=Password, nargs='?', dest='password')
args = parser.parse_args()
```



更多选项: <https://docs.python.org/2.7/library/argparse.html>

## 安装
```pip install Click
```
