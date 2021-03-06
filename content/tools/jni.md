---
title: "开发JNI native程序"
layout: page
date: 2018-11-29
---
[TOC]

## 关于
JAVA可以通过JNI即 Java Native Interface 来调用原生库,下面以一个例子展示使用方法。

## 实战
1. 创建Java类`Hello.java`

```java 
package com.tracholar.jni.demo;

public class Hello {
    // 声明native方法, 遵循Java类型和声明方法
    public native static void say_hi();

    static {
        // 加载动态库, 动态库后面会生成, 库名字和文件名之间对应关系是
        // 库名:   hello
        // 文件名(windows): libhello.dll
        //        mac     : libhello.dylib
        //        linux   : libhello.so
        
        System.loadLibrary("hello");
    }

    public static void main(String[] args){
        say_hi();
    }
}
```

2. 生成头文件 

```bash
javac -d . src/main/java/com/tracholar/jni/demo/Hello.java
javah com.tracholar.jni.demo.Hello
``` 

将生成一个头文件 `com_tracholar_jni_demo_Hello.h`

```cpp
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_tracholar_jni_demo_Hello */

#ifndef _Included_com_tracholar_jni_demo_Hello
#define _Included_com_tracholar_jni_demo_Hello
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_tracholar_jni_demo_Hello
 * Method:    say_hi
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_tracholar_jni_demo_Hello_say_1hi
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif

```

3. 编写C/C++代码,实现这个函数

```c++
#include<stdio.h>
#include "com_tracholar_jni_demo_Hello.h"


JNIEXPORT void JNICALL Java_com_sankuai_itc_dmspa_tool_demo_Hello_say_1hi
  (JNIEnv * env, jclass c){
    //jcharArray arr = (*env)->NewCharArray(env, 14);
    //(*env)->SetCharArrayRegion(env, &arr, 0, 14, "hello world!\n");
    //return arr;
    printf("hello world!\n");
  }
```

4. 编译C代码, 编译时需要包含 jni.h 所在的目录,在 MAC系统下, 是 `/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers/`,
   在其他系统一般在JDK所在的某个子目录,可以自行搜索。
   
```bash
# 只编译生成目标代码, 便于后续生成动态链接库
gcc  -c -fPIC hello.c  -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers/ -o hello.o

# 生成动态库
# (linux) 
# gcc hello.o -shared -o libhello.so
# (MAC)
gcc hello.o -dynamiclib -o libhello.dylib
```

5. 执行Java类,指定 `java.library.path` 参数为动态链接库所在目录

`java  -Djava.library.path=.  com.tracholar.jni.demo.Hello`

> hello world!

## 加载动态库的两个方法
1. 第一个方法是用 `System.loadLibrary(String libname)` libname会通过一个native方法`public static native String mapLibraryName(String libname);`
   转换成文件名,在MAC系统下`hello`会转换成 `libhello.dylib`
2. 第二个方法是用 `System.load(String filename)` 加载,注意这里的filename必须是绝对路径! MAC下也可以直接加载so文件。

```java 
public class Hello {
    public native static void say_hi();
    
    public static void main(String[] args){
        try {
            String filename = new File(".").getCanonicalPath() + File.separator + "hello.so";
            System.load(filename);
            say_hi();
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}

```

## JNI 数据类型
