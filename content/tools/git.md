---
title: "Git常用命令汇总"
layout: page
date: 2016-05-31 20:51
---

[TOC]

## 关于
这里汇集了git常用命令和用法，便于速查，不定期更新。

## 提交
- 给 git commit 加上 -a 选项，Git 就会自动把所有已经跟踪过的文件暂存起来一并提交，从而跳过 git add 步骤：

## 打tag
将当前版本添加tag`<tagname>`
```bash
git tag <tagname>
```
列出标签
```bash
$ git tag
v0.1
v1.3

$ git tag -l 'v1.4.2.*'
v1.4.2.1
v1.4.2.2
v1.4.2.3
v1.4.2.4
```

将tag推送到远程服务器，参见[1-2]，推送所有的tags
```bash
git push --tags
```
只推送单个tag
```bash
git push origin <tagname>
```

1. <http://stackoverflow.com/a/5195913/4349983>
2. <https://git-scm.com/book/zh/v1/Git-%E5%9F%BA%E7%A1%80-%E6%89%93%E6%A0%87%E7%AD%BE>

## 分支
```bash
## 创建dev分支
git branch dev
## 切换到dev分支
git checkout dev
## 上述两条命令也可以合并为一条
git checkout -b dev

## 在dev分支作修改后，再切换回master分支
git checkout master
## 还可以创建多个其他分支
git checkout -b issu35
git checkout -b issu37
## 在master分支中，合并dev分支
## 可与选择只合并某一个分支
git merge issu35  
## 合并后，可以删除issu35分支
git branch -d issu35
## 如果合并分支时，发生冲突，解决冲突后再合并
## 可以通过命令查看冲突的地方
git status
```

## `.gitignore`文件
文件 .gitignore 的格式规范如下：

- 所有空行或者以注释符号 `＃` 开头的行都会被 Git 忽略。
- 可以使用标准的 `glob` 模式匹配。
- 匹配模式最后跟反斜杠（`/`）说明要忽略的是目录。
- 要忽略指定模式以外的文件或目录，可以在模式前加上惊叹号（!）取反。

所谓的 glob 模式是指 shell 所使用的简化了的正则表达式。

- 星号（`*`）匹配零个或多个任意字符；
- [abc] 匹配任何一个列在方括号中的字符（这个例子要么匹配一个 a，要么匹配一个 b，要么匹配一个 c）；
- 问号（?）只匹配一个任意字符；如果在方括号中使用短划线分隔两个字符，表示所有在这两个字符范围内的都可以匹配（比如 [0-9] 表示匹配所有 0 到 9 的数字）。

我们再看一个 .gitignore 文件的例子：

```
# 此为注释 – 将被 Git 忽略
# 忽略所有 .a 结尾的文件
*.a
# 但 lib.a 除外
!lib.a
# 仅仅忽略项目根目录下的 TODO 文件，不包括 subdir/TODO
/TODO
# 忽略 build/ 目录下的所有文件
build/
# 会忽略 doc/notes.txt 但不包括 doc/server/arch.txt
doc/*.txt
# ignore all .txt files in the doc/ directory
doc/**/*.txt
```

A `**/` pattern is available in Git since version 1.8.2.

如果不小心把文件加到缓存区，可以先通过如下命令删除，然后再将模式加到`.gitignore`文件中。
使用`git rm`的`--cached`参数。后面可以列出文件或者目录的名字，也可以使用 glob 模式。

```bash
git rm --cached readme.txt
git rm log/\*.log
```

注意到星号 * 之前的反斜杠 `\`，因为 Git 有它自己的文件模式扩展匹配方式，所以我们不用 shell 来帮忙展开（译注：实际上不加反斜杠也可以运行，只不过按照 shell 扩展的话，仅仅删除指定目录下的文件而不会递归匹配。上面的例子本来就指定了目录，所以效果等同，但下面的例子就会用递归方式匹配，所以必须加反斜杠。）。此命令删除所有 log/ 目录下扩展名为 .log 的文件。类似的比如：

```bash
$ git rm \*~
```

会递归删除当前目录及其子目录中所有 ~ 结尾的文件。
