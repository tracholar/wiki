---
layout: page
title: "Python 日期时间相关模块总结"
date: 2015-10-22 20:00
---
[TOC]

##简介

Python标准库提供了对日期、时间、日历进行操作的模块`time, datetime, calendar`。
其中`time`模块基本上是对Unix时间戳的操作和处理，以及涉及到操作系统
相关的操作，`datetime`模块则是对日期和时间进行的处理封装，支持时间
之间的运算，在对日期和时间的处理上比`time`模块要方便。


## time 模块
`time`模块在python标准库中，被放在了通用操作系统服务目录下，
由此可见这个模块跟操作系统有着很大的关系。
因为这个原因，某些函数是与操作系统平台有关的。
由于是基于Unix时间戳，导致时间表示的范围
被限定在1970-2038年之间。
这个模块中的基本数据结果是`struct_time`，实际上是一个有名字的元组。

这个模块提供的时间操作函数主要是时间戳，时间字符串和`struct_time`三种数据中的相互转换。
还有一些操作系统跟时间有关的系统调用。

### 基本用法

生成Unix时间戳，单位是秒
```python
>>> ts = time.time()
>>> ts
1445495655.495
```

时间戳是一个浮点数，可以通过内置函数转换为其他格式。

时间戳转换为`struct_time`
```python
>>> time.gmtime(ts)
time.struct_time(tm_year=2015, tm_mon=10, tm_mday=22, tm_hour=6, tm_min=34, tm_sec=15, tm_wday=3, tm_yday=295, tm_isdst=0)

>>> time.localtime(ts)
time.struct_time(tm_year=2015, tm_mon=10, tm_mday=22, tm_hour=14, tm_min=34, tm_sec=15, tm_wday=3, tm_yday=295, tm_isdst=0)
```

时间戳转换为方便阅读的字符串   
```python
>>> time.ctime(ts)
'Thu Oct 22 14:34:15 2015'
```

`struct_time`转换为字符串   
```python
>>> st = time.localtime(ts)
>>> time.asctime(st)
'Thu Oct 22 14:34:15 2015'

>>> time.strftime('%Y-%m-%d', st)
'2015-10-22'
```

`struct_time`转换为时间戳  
```python
>>> time.mktime(st)
1445495655.0
```

时间字符串转`struct_time`    
```python
>>> time.strptime('2015-10-22','%Y-%m-%d')
time.struct_time(tm_year=2015, tm_mon=10, tm_mday=22, tm_hour=0, tm_min=0, tm_sec=0, tm_wday=3, tm_yday=295, tm_isdst=-1)
```

与时间有关的系统调用，如`time.sleep`。



## datetime 模块
datetime模块提供对时间和日期的封装，并提供他们之间的数学运算。
该模块包含4个类，用得多的是`datetime.timedelta`和`datetime.datetime`。    
```python
object
    timedelta     # 主要用于计算时间跨度
    tzinfo        # 时区相关
    time          # 只关注时间
    date          # 只关注日期
        datetime  # 同时有时间和日期
```

date只包含年月日3个属性，datetime还包含时、分、秒、毫秒。

获取现在时间   
```python
>>> datetime.today()
datetime.datetime(2015, 10, 22, 15, 8, 54, 88000)
>>> datetime.now()
datetime.datetime(2015, 10, 22, 15, 8, 41, 304000)
```

与时间戳的转换    
```python
>>> datetime.fromtimestamp(ts)
datetime.datetime(2015, 10, 22, 14, 34, 15, 495000)
```

利用`datetime.combine(date,time)`可以将date和time组合为datetime。
利用`datetime.strptime`和`datetime.strftime`
可以在时间字符串和datetime对象间相互转换。   
```python
>>> dt = datetime.strptime('2015-10-12','%Y-%m-%d')
>>> dt
datetime.datetime(2015, 10, 12, 0, 0)
>>> dt.strftime('%d/%m/%Y')
'12/10/2015'
```

对datetime对象做部分修改    
```python
>>> dt.replace(year=2016)
datetime.datetime(2016, 10, 12, 0, 0)
```

转换为timetuple也就是time.struct_time   
```python
>>> dt.timetuple
time.struct_time(tm_year=2015, tm_mon=10, tm_mday=12, tm_hour=0, tm_min=0, tm_sec=0, tm_wday=0, tm_yday=285, tm_isdst=-1)
```

`datetime.timedelta`对象和`datetime.time`对象的属性是类似的，只不过前者是时间差。
`total_seconds`方法返回总的秒数。

`timedelta`和`datetime`之间的数学运算可以归纳为    
```python
timedelta = datetime - datetime
datetime = datetime + timedelta
```
例如   
```python
>>> delta = timedelta(hours=1)
>>> delta
datetime.timedelta(0, 3600)
>>> 
>>> dt + delta
datetime.datetime(2015, 10, 12, 1, 0)
```

## calendar 模块
这个模块主要提供日历的一些操作，可以很方便生成一个文本日历。    
```python
>>> print calendar.month(2015,10)

    October 2015
Mo Tu We Th Fr Sa Su
          1  2  3  4
 5  6  7  8  9 10 11
12 13 14 15 16 17 18
19 20 21 22 23 24 25
26 27 28 29 30 31
```

calendar包含一个`Calendar`对象，描述了日历数据的结构和一些设置操作。
而格式化任务交给了两个子类`TextCalendar`和`HTMLCalendar`。


## 总结
时间模块的比较：
如果对时间操作是在字符串和时间戳间转换的话，用time模块。
如果需要对时间进行比较复杂的数学运算的话，用datetime模块。

