---
title: "log4j 配置和使用"
layout: page
date: 2019-10-18
---
[TOC]


## 几种不同的包和区别
- `java.util.logging`
- `sl4j`


## 配置
- 文件 `log4j.properties` 放在`resource` 根目录下
- `log4j.rootLogger = [LEVEL], appender1, appender2 ...` 设置日志级别和多个appender
- appender设置
    - `log4j.appender.[appenderName] = [appender class name]`  设置append的处理类
        - `org.apache.log4j.ConsoleAppender` 输出到控制台
        - `org.apache.log4j.FileAppender` 输出到固定文件
        - `org.apache.log4j.RollingFileAppender` 指定文件大小，达到大小创建新文件
        - `org.apache.log4j.DailyRollingFileAppender` 每天一个文件
        - `org.apache.log4j.WriterAppender` 不知道
    - `log4j.appender.[appenderName].layout` layerout设置
        - `org.apache.log4j.HTMLLayout`（以HTML表格形式布局）  
        - `org.apache.log4j.PatternLayout`（可以灵活地指定布局模式）
        - `org.apache.log4j.SimpleLayout`（包含日志信息的级别和信息字符串）
        - `org.apache.log4j.TTCCLayout`（包含日志产生的时间、线程、类别等等信息）
    - `log4j.appender.D.layout.ConversionPattern` 指定模式
        - %m   输出代码中指定的消息
        - %p   输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL 
        - %r   输出自应用启动到输出该log信息耗费的毫秒数
        - %c   输出所属的类目，通常就是所在类的全名 
        - %t   输出产生该日志事件的线程名 
        - %n   输出一个回车换行符，Windows平台为“\r\n”，Unix平台为“\n” 
        - %d   输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy MMM dd HH:mm:ss , SSS}，输出类似：2002年10月18日  22 ： 10 ： 28
        - %l   输出日志事件的发生位置，包括类目名、发生的线程，以及在代码中的行数。举例：Testlog4.main(TestLog4.java: 10 ) 
        
        
## 一个模板
- log4j.properties

```properties
### 设置###
log4j.rootLogger = DEBUG, stdout, E
### 输出信息到控制抬 ###
log4j.appender.stdout = org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target = System.err
log4j.appender.stdout.layout = org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern = [%p] %d{yyyy-MM-dd HH:mm:ss} - %c - %m%n
log4j.appender.stdout.Threshold = DEBUG

### 输出到文件
log4j.appender.E = org.apache.log4j.RollingFileAppender
log4j.appender.E.File = log/info.log
log4j.appender.E.Append = true
log4j.appender.E.Threshold = INFO 
log4j.appender.E.MaxFileSize = 10240KB
log4j.appender.E.MaxBackupIndex = 3
log4j.appender.E.layout = org.apache.log4j.PatternLayout
log4j.appender.E.layout.ConversionPattern = %d{yyyy-MM-dd HH:mm:ss} [%l:[%p]] %m%n
```