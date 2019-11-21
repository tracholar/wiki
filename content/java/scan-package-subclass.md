---
title: "原生实现扫描包下的子类"
layout: page
date: 2019-11-21
---
[TOC]

## 问题
- 自动扫描某个包下面的某个特定类型的子类
- 例如，想扫描包`com.tracholar.demo`下面所有 `IFiler` 的子类

## 实现原理
- java编译后的每个类都是一个文件，类似于`com/tracholar/demo/Someclass.clss`，通过文件扫描的方式来实现类的扫描

```java
public class AutoLoadUtil {
    private static <T> List<Class<?>> scan(String packageName,
                                                     Class<T> cls) {
        List<String> classes = new LinkedList<>();
        try {
            URL url = AutoLoadUtil.class.getProtectionDomain().getCodeSource().getLocation();
            Vfs.Dir dir = Vfs.fromURL(url);

            try {
                for (final Vfs.File file : dir.getFiles()) {

                    String path = file.getRelativePath();
                    String fqn = path.replace('/', '.');
                    if (fqn.startsWith(packageName) && fqn.endsWith(".class")) {
                        classes.add(fqn.substring(0, fqn.indexOf(".class")));
                    }

                }
            }catch (Exception e){

            }finally {
                dir.close();
            }
        }catch (Exception ex){
            ex.printStackTrace();
        }finally {

        }
        List<Class<?>> foundClasses = classes.stream().map(c -> {
            try{
                return Class.forName(c);
            }catch (ClassNotFoundException ex){
                ex.printStackTrace();
                return null;
            }
        }).filter(c -> !Modifier.isAbstract(c.getModifiers())
                && cls.isAssignableFrom(c))
          .collect(Collectors.toList());

        return foundClasses;
    }

    public AutoLoadUtil(){
        //for test
        List<Class<?>> renders = AutoLoadUtil.scan(IRender.class.getPackage().getName(), ITemplateParam.class);
        System.out.println("找到 " + renders.size() + " 个类！");
        renders.stream().forEach(e -> {
            System.out.println(e.getName());
        });
    }
}
```
