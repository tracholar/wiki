---
title: "Python Subprocess"
layout: page
date: 2019-02-20
---
[TOC]

处理子进程, 可以替代 `os.system`,并提供更丰富的操作。

参考: 

1. <https://docs.python.org/2/library/subprocess.html>
2. <https://segmentfault.com/a/1190000009176351>

## 基本函数
- `subprocess.call(args, *, stdin=None, stdout=None, stderr=None, shell=False)`
- `class subprocess.Popen(args, bufsize=0, executable=None, stdin=None, stdout=None, stderr=None, preexec_fn=None, close_fds=False, shell=False, cwd=None, env=None, universal_newlines=False, startupinfo=None, creationflags=0)`

- 执行基本的shell命令

```python
subprocess.call('ls -l', shell=True)
subprocess.call(['ls', '-l'])
subprocess.Popen('ls -l', shell=True)
subprocess.Popen(['ls', '-l'])
```

- 和子进程交互

```python
# 读取子进程的stdout
p = subprocess.Popen('ls -l', shell=True, stdout=subprocess.PIPE)
print p.stdout.read()

# 发送数据给子进程的stdin
p = subprocess.Popen('cat', shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
stdout, stderr = p.communicate('data1\ndata2\ndata3')
```

- 返回值
```python
from subprocess import *

output = check_output(["mycmd", "myarg"]) # output=`mycmd myarg`
```

