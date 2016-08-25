---
title: "Linux 基础知识"
layout: page
date: 2016-07-18
---
[TOC]
## 关于
作为一个合格的工程师，怎能不懂Linux，所以学习记录一些知识点。

## 常用命令集合
### 文件系统相关
- 重定向：`2>&1`表示将stderr重定向到stdout，`2>/dev/null`表示重定向stderr到空洞，也就是不打印出stderr。
  后面这个在运行hadoop和spark的时候有用。也可以将stdout和stderr重定向到不同文件，例如：        
```bash
some-cmd 1>stdout.txt 2>stderr.txt
```

### 权限相关
- `sudo`:      
在`sudo`出现之前，使用`su`命令提升权限，缺点是必须知道超级用户的密码。`sudo`可以将用户的名字、可以使用的特殊命令、按照那种用户组执行等信息保存在
文件中（通常是`/etc/sudoers`）
sudo [-bhHpV][-s ][-u <用户>][指令]
或
sudo [-klv]
参数[编辑]
　　-b 　在后台执行指令。
　　-h 　显示帮助。
　　-H 　将HOME环境变量设为新身份的HOME环境变量。
　　-k 　结束密码的有效期限，也就是下次再执行sudo时便需要输入密码。
　　-l 　列出目前用户可执行与无法执行的指令。
　　-p 　改变询问密码的提示符号。
　　-s 　执行指定的shell。
　　-u <用户> 　以指定的用户作为新的身份。若不加上此参数，则预设以root作为新的身份。
　　-v 　延长密码有效期限5分钟。
　　-V 　显示版本信息。
   -S   从标准输入流替代终端来获取密码
### 操作系统相关
后台可靠运行的几种方法：          
- 后台运行加一个`&`即可，例如`sleep 100 &`，只能一直保持会话，如果shell会话关了，这个子进程也会关掉。     
- nohup 方式：    
我们知道，当用户注销（logout）或者网络断开时，终端会收到 HUP（hangup）信号从而关闭其所有子进程。因此，我们的解决办法就有两种途径：要么让进程忽略 HUP 信号，要么让进程运行在新的会话里从而成为不属于此终端的子进程。
只需在要处理的命令前加上 nohup 即可，标准输出和标准错误缺省会被重定向到 nohup.out 文件中。一般我们可在结尾加上"&"来将命令同时放入后台运行，也可用">filename 2>&1"来更改缺省的重定向文件名。           
- setsid 方式：    
nohup 无疑能通过忽略 HUP 信号来使我们的进程避免中途被中断，但如果我们换个角度思考，如果我们的进程不属于接受 HUP 信号的终端的子进程，那么自然也就不会受到 HUP 信号的影响了。setsid 就能帮助我们做到这一点。               
使用的时候，也只需在要处理的命令前加上 setsid 即可。
- 当我们将"&"也放入“()”内之后，我们就会发现所提交的作业并不在作业列表中，也就是说，是无法通过jobs来查看的。让我们来看看为什么这样就能躲过 HUP 信号的影响吧。                     
```bash   
subshell 示例
[root@pvcent107 ~]# (ping www.ibm.com &)
[root@pvcent107 ~]# ps -ef |grep www.ibm.com
root     16270     1  0 14:13 pts/4    00:00:00 ping www.ibm.com
root     16278 15362  0 14:13 pts/4    00:00:00 grep www.ibm.com
[root@pvcent107 ~]#
```
从上例中可以看出，新提交的进程的父 ID（PPID）为1（init 进程的 PID），并不是当前终端的进程 ID。因此并不属于当前终端的子进程，从而也就不会受到当前终端的 HUP 信号的影响了。
- 如果已经提交了任务，但是没加nohup或者setsid，怎么补救？可以用 `disown` 命令来补救。
    - `disown -h jobspec` 是某个作业忽略HUP信号
    - `disown -ah` 是所有作业忽略HUP信号
    - `disown -rh` 是正在运行的作业忽略HUP信号
- 更多技巧，参考IBM文档
1. IBM文档 <https://www.ibm.com/developerworks/cn/linux/l-cn-nohup/>
### 字符串工具
- grep 字符匹配
```bash
## 从log.txt中查找keyword出现的行
grep keyword log.txt
## 查询多个模式
grep -E 'keyword|otherword' log.txt
grep -e keyword -e otherword log.txt
```

`grep -E` 扩展了默认的正则式，可以使用一下特性：

- `[]`字符选择
- `+`1个和多个
- `?`有或无
- `{}`注意花括号要加反斜杠`\`转义，因为bash中花括号有特殊含义


### sort 命令
将文件的每一行作为一个单位，进行排序，从首字符开始，按照ASCII码进行比较（默认情况）。            
- u 参数，去重
- r 参数，降序
- o 参数，排序后写入原文件，重定向会将文件内容清空，达不到要求，可以用这个参数
- n 参数，表示不按照ASCII码排序，而是按照数值大小
- t 参数 和 k 参数，用来排序csv格式，t 指明分隔符 k 指明排序的列序号
- f 参数，忽略大小写
- c 参数，检查是否排好序，输出第一个乱序行信息，返回1；C参数，也是检查，但不输出乱序行信息
- M 参数，以月份排序，会识别月份（只对英文吧）
- b 参数，忽略每一行前面所有空白

### watch 命令
用来周期性地执行某个程序，全屏显示执行结果。常用来 `tail log-file`，是的，我就是要干这个事情才搜到这个命令的。

#### 常用参数：

- `-n 或 --interval` 指定间隔秒钟数，缺省为2
- `-d 或 --differences` 高亮变化的区域，如果`-d=cumulative`会将每次变动积累并高亮
- `-t 或 -no-title` 关闭顶部时间显示

最后接你要周期执行的命令即可。
#### 例子
```bash
# 监控log
watch -n 1 -d tail log-file

# 显示网络连接变化
watch -n 1 -d netstat -ant

# 每秒高亮显示http连接数变化
watch -n 1 -d 'pstree|grep http'
```

### `wc` 命令
用来统计文件的文本行数、单词数、字节数等。

参数：-c, --bytes
打印字节数（print the byte counts）

参数：-m, --chars
打印字符数（print the character counts）

参数：-l, --lines
打印行数（print the newline counts）

参数：-L, --max-line-length
打印最长行的长度（print the length of the longest line）

参数：-w, --words
打印单词数（print the word counts）

### `tar` 打包命令
常用参数 `-v` 显示处理过程。`-c` 创建, `-f` 指定文件, `-r` 追加, `-x`解压， `-z` gzip压缩。

常用组合有：

- `tar cf files.tar files` 将files打包到tar包中
- `tar rf file.tar file` 将文件添加到tar包中，原来已经有个tar包了
- `tar tf file.tar` 测试tar完整性
- `tar xf file.tar` 解压tar包
- `tar zcf file.tar.gz files` 将files打包并压缩到 file.tar.gz
- `tar zxf file.tar.gz` 解压包



## VIM
### 三种模式
- 插入模式：可以输入文本，按`i`进入
- 编辑模式：按`ESC`进入，可以移动和操纵文本
- 命令模式：可以执行冒号命令，常用的有`:w [filename]`写入文件，`:q`退出vim。
### 移动光标
在编辑模式下操作。

- 基本移动：
    - k   上
    - j   下
    - h   左
    - l   右
- 大范围移动          
    - ctrl + f 前进(forward)一页
    - ctrl + b 后退(back)一页
- 更大范围移动
    - `*`和`#` 搜索当前光标所在的 **单词** ，跳转到下/上个地方
    - `(`和`)` 移动到前/后一个 **句子** 的开始，以英文句号为准，对中文貌似没用，相当于段落跳转
    - `{`和`}` 在 **段落** 间移动
    - `fa` 前进到字符a，a可换成其他字符
    - `gg` 文件起始位置
    - `G` 最后一行起始位置
    - `ngg` n行起始位置
- 当前页跳转
    - `H` 当前页的header，`3H`当前页第3行
    - `M` 当前页的middle
    - `L` 当前页最后一行 ，`3L`当期那也倒数第3行
- 行内移动
    - `g_` 行尾，空字符不算
    - `^` 行首，空字符不算
    - `$` 行尾
    - `0` 行首
    - `w` 下一个word的开头
    - `e` 下一个word的结尾
    - `b` 前一个word的开头
- 搜索匹配
    - `/str`，在编辑模式下输入`/`，后面跟要搜索字符串str，然后回车搜索。
    - `?str`，反向搜索字符串str
    - `n`，搜索到匹配后继续向后搜索
    - `N`，搜索到匹配后，继续向前搜索
### 编辑命令
- 替换与删除：用`d`删除行，用`x`删除字符，用`r`替换字符。
    - `rc` 用字符c替换当前字符
    - `nrc` 用c替换前n个字符
    - `x` 删除光标字符
    - `nx` 删除光标右侧n个字符
    - `dw` 删除光标右侧的word
    - `ndw` 删除n个word
    - `db` 删除光标左侧一个word
    - `ndb` 删除光标左侧n个word
    - `dd` 删除一行
    - `ndd` 删除n行
    - `d$` 删除字符到行尾
    - `d0` 删除字符到行首
    - `J` 删除本行回车，合并到下行
- 用输入的文本替换，进入插入模式
    - `s` 输入文本替换当期字符，n个版`ns`
    - `S` 删除当前行进入编辑模式，n个版`nS`
    - `cw` 替换右侧一个字，n个版`ncw`
    - `cW`,`c$` 替换右侧所有字
    - `cb` 替换左侧一个字，n个版`ncb`
    - `cd` 替换当前行，n个版`ncd`，试过但是无效？！
    - `c$` 替换到行尾所有
    - `c0` 替换到行首所有
- 撤销
    - `u` undo，撤销最后一个操作
    - `.` redo，重复最有一个操作，或者`CTRL + R`
- 复制`y`粘贴`p`
    - `yy` 复制一行到缓冲区，n行版`nyy`
    - `+y` 复制到系统剪贴板，n行版`+nyy`，mac貌似无效
    - `p` 将缓冲区内容粘贴到光标后面
    - `P` 到前面
### 插入模式
- 插入命令有：
    - `i` 光标左侧插入
    - `I` 行开头插入
    - `a` 右侧插入
    - `A` 行结尾插入
    - `o` 下一行插入
    - `O` 上一行插入
- 退出插入模式命令`ESC, ctrl + [`，将进入编辑模式

### 命令模式
编辑模式下按`:`号，进入命令模式，输入命令后回车即可运行命令。           
- 文件命令
    - `:e path-to-file` 打开文件
    - `:w` 保存当前文件
    - `:w path-to-file` 另存为
    - `:q` 退出，强制版`:q!`
    - `:wq` 保存并退出
    - `:f filename` 重命名文件
    - `:r filename` 读取filename到光标后面
- 行号与文件
    - `:12` 移动到第12行
    - `:12w filename` 第12行写入到文件
    - `:12,32w filename` 将第12-32行写入到文件，这里可以用`$`表示最后一行，用`.`表示当前行，还可以用`.+5`表示当前行后的第5行
- 命令模式下的搜索
    - `:/str/`正向搜索`str`
    - `:?str?`反向搜索`str`
    - `:/str/w file`  正向搜索，并将第一个包含字符串 str 的行写入 file 文件
    - `:/str1/,/str2/w file ` 正向搜索，并将包含字符串 str1 的行至包含字符串 str2 的行写
- 搜索正则表达式
```bash
^                放在字符串前面，匹配行首的字；
$                放在字符串后面，匹配行尾的字；
\<               匹配一个字的字头；
\>               匹配一个字的字尾；
.                匹配任何单个正文字符；
[str]            匹配 str 中的任何单个字符；
[^str]           匹配任何不在 str 中的单个字符；
[a-b]            匹配 a 到 b 之间的任一字符；
*                匹配前一个字符的 0 次或多次出现；
\                转义后面的字符。
```        
- 正文替换:`:s`命令
    - `:%s/str1/str2/ ` 用字符串 str2 替换行中首次出现的字符串 str1
    - `:s/str1/str2/g ` 用字符串 str2 替换行中所有出现的字符串 str1

    1.  g 放在命令末尾，表示对搜索字符串的每次出现进行替换,不止匹配每行中的第一次出现；不加 g，表示只对搜索字符串的首次出现进行替换；g 放在命令开头，表示对正文中所有包含搜索字符串的行进行替换操作;
    2.  s 表示后面跟着一串替换的命令；
    3.  % 表示替换范围是所有行，即全文。

统计出现次数命令：`:%s/str1/&/gn`

- 删除正文命令：略
- 恢复意外退出未保存的文件`:recover`命令

- 执行shell命令：`:!some-command`，执行完后回到vim
- 分屏
    - `:split`缩写`:sp`上下分屏
    - `:vsplit`缩写`:vsp` 左右分屏
    - 分屏后切换`ctrl + w  [hjkl]`，也可以直接用`Ctrl+w+w`顺序切换
    - 设置分屏尺寸
        - `ctrl + w =` 所有屏幕等高或等宽
        - `ctrl + w -` 减少高度
        - `ctrl + w +` 增加高度
    - 可以在分屏中用`:q`关闭文件即可关闭分屏
    - `:only` 关闭当前分屏外所有屏幕

- 标签页
    - `:tabnew [filename]` 创建新标签文件名可选，有表示打开文件, 和`:tabe`差不多
    - `:tabc` 关闭当前标签close
    - `:tabo` 关闭当前标签外的所有标签only
    - `:tabn` 下一个标签next
    - `:tabp` 跳转到前一个标签preview
    - `:tabr` 和 `:tabfir` 到第一个 first
    - `:tabl` 最后一个last
    - `:tabdo` 为每个标签执行命令

- 选项设置，命令`:set option`

```bash
autoindent        设置该选项，则正文自动缩进
ignorecase        设置该选项，则忽略规则表达式中大小写字母的区别
number            设置该选项，则显示正文行号
ruler             设置该选项，则在屏幕底部显示光标所在行、列的位置
tabstop           设置按 Tab 键跳过的空格数。例如 :set tabstop=n，n 默认值为 8
mk                将选项保存在当前目录的 .exrc 文件中
```

### 外部工具集成
- `vim + diff = vimdiff` 使用方法`vimdiff f1 f2` 或者 `vim -d`
    - 在已经打开的文件中执行命令`:diffsplit filename2`。
    `:diffpatch`，`diffsplit`默认上下分屏，可以用`:vert diffsplit`实现左右分屏
    - `:diffoff`关闭diff的颜色
    - `:diffu[pdate]` 更新diff
- `xxd`二进制编辑
    - `vim -b` 编辑二进制文件
    - `%!xxd` 在vim内切换到二进制编辑
    - `%!xxd -r` 切回到文本编辑模式

### 参考
1. <http://www.jianshu.com/p/bcbe916f97e1>
