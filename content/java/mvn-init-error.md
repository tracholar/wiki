---
title: "Maven打包时异常ExceptionInInitializerError"
layout: page
date: 2020-07-14
---

[TOC]

## 问题
- 用maven打包，出现了异常如下
```bash
WARNING: An illegal reflective access operation has occurred
WARNING: Illegal reflective access by lombok.javac.apt.LombokProcessor to field com.sun.tools.javac.processing.JavacProcessingEnvironment.processorClassLoader
WARNING: Please consider reporting this to the maintainers of lombok.javac.apt.LombokProcessor
WARNING: Use --illegal-access=warn to enable warnings of further illegal reflective access operations
WARNING: All illegal access operations will be denied in a future release
[INFO] ------------------------------------------------------------------------
[INFO] BUILD FAILURE
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 5.255 s
[INFO] Finished at: 2020-07-14T10:32:41+08:00
[INFO] ------------------------------------------------------------------------
[ERROR] Failed to execute goal org.apache.maven.plugins:maven-compiler-plugin:3.1:compile (default-compile) on project recommend-service: Fatal error compiling: java.lang.ExceptionInInitializerError: com.sun.tools.javac.code.TypeTags -> [Help 1]
[ERROR]
[ERROR] To see the full stack trace of the errors, re-run Maven with the -e switch.
[ERROR] Re-run Maven using the -X switch to enable full debug logging.
[ERROR]
[ERROR] For more information about the errors and possible solutions, please read the following articles:
[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoExecutionException
```

## 问题排查
- 通过 `-e` 开关，打开错误堆栈信息，查到 <https://stackoverflow.com/questions/50782986/java-lang-exceptionininitializererror-is-produced-when-lombok-is-used-within-a-s/50792308>
  有解释该错误在新版的 lombok 中已经修复。
```bash
[ERROR] Failed to execute goal org.apache.maven.plugins:maven-compiler-plugin:3.1:compile (default-compile) on project recommend-service: Fatal error compiling: java.lang.ExceptionInInitializerError: com.sun.tools.javac.code.TypeTags -> [Help 1]
org.apache.maven.lifecycle.LifecycleExecutionException: Failed to execute goal org.apache.maven.plugins:maven-compiler-plugin:3.1:compile (default-compile) on project recommend-service: Fatal error compiling
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:213)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:154)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:146)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:117)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:81)
   at org.apache.maven.lifecycle.internal.builder.singlethreaded.SingleThreadedBuilder.build (SingleThreadedBuilder.java:56)
   at org.apache.maven.lifecycle.internal.LifecycleStarter.execute (LifecycleStarter.java:128)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:305)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:192)
   at org.apache.maven.DefaultMaven.execute (DefaultMaven.java:105)
   at org.apache.maven.cli.MavenCli.execute (MavenCli.java:954)
   at org.apache.maven.cli.MavenCli.doMain (MavenCli.java:288)
   at org.apache.maven.cli.MavenCli.main (MavenCli.java:192)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0 (Native Method)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke (NativeMethodAccessorImpl.java:62)
   at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke (DelegatingMethodAccessorImpl.java:43)
   at java.lang.reflect.Method.invoke (Method.java:564)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launchEnhanced (Launcher.java:289)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launch (Launcher.java:229)
   at org.codehaus.plexus.classworlds.launcher.Launcher.mainWithExitCode (Launcher.java:415)
   at org.codehaus.plexus.classworlds.launcher.Launcher.main (Launcher.java:356)
Caused by: org.apache.maven.plugin.MojoExecutionException: Fatal error compiling
   at org.apache.maven.plugin.compiler.AbstractCompilerMojo.execute (AbstractCompilerMojo.java:796)
   at org.apache.maven.plugin.compiler.CompilerMojo.execute (CompilerMojo.java:129)
   at org.apache.maven.plugin.DefaultBuildPluginManager.executeMojo (DefaultBuildPluginManager.java:137)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:208)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:154)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:146)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:117)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:81)
   at org.apache.maven.lifecycle.internal.builder.singlethreaded.SingleThreadedBuilder.build (SingleThreadedBuilder.java:56)
   at org.apache.maven.lifecycle.internal.LifecycleStarter.execute (LifecycleStarter.java:128)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:305)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:192)
   at org.apache.maven.DefaultMaven.execute (DefaultMaven.java:105)
   at org.apache.maven.cli.MavenCli.execute (MavenCli.java:954)
   at org.apache.maven.cli.MavenCli.doMain (MavenCli.java:288)
   at org.apache.maven.cli.MavenCli.main (MavenCli.java:192)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0 (Native Method)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke (NativeMethodAccessorImpl.java:62)
   at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke (DelegatingMethodAccessorImpl.java:43)
   at java.lang.reflect.Method.invoke (Method.java:564)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launchEnhanced (Launcher.java:289)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launch (Launcher.java:229)
   at org.codehaus.plexus.classworlds.launcher.Launcher.mainWithExitCode (Launcher.java:415)
   at org.codehaus.plexus.classworlds.launcher.Launcher.main (Launcher.java:356)
Caused by: org.codehaus.plexus.compiler.CompilerException: java.lang.ExceptionInInitializerError
   at org.codehaus.plexus.compiler.javac.JavaxToolsCompiler.compileInProcess (JavaxToolsCompiler.java:191)
   at org.codehaus.plexus.compiler.javac.JavacCompiler.performCompile (JavacCompiler.java:169)
   at org.apache.maven.plugin.compiler.AbstractCompilerMojo.execute (AbstractCompilerMojo.java:785)
   at org.apache.maven.plugin.compiler.CompilerMojo.execute (CompilerMojo.java:129)
   at org.apache.maven.plugin.DefaultBuildPluginManager.executeMojo (DefaultBuildPluginManager.java:137)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:208)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:154)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:146)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:117)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:81)
   at org.apache.maven.lifecycle.internal.builder.singlethreaded.SingleThreadedBuilder.build (SingleThreadedBuilder.java:56)
   at org.apache.maven.lifecycle.internal.LifecycleStarter.execute (LifecycleStarter.java:128)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:305)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:192)
   at org.apache.maven.DefaultMaven.execute (DefaultMaven.java:105)
   at org.apache.maven.cli.MavenCli.execute (MavenCli.java:954)
   at org.apache.maven.cli.MavenCli.doMain (MavenCli.java:288)
   at org.apache.maven.cli.MavenCli.main (MavenCli.java:192)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0 (Native Method)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke (NativeMethodAccessorImpl.java:62)
   at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke (DelegatingMethodAccessorImpl.java:43)
   at java.lang.reflect.Method.invoke (Method.java:564)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launchEnhanced (Launcher.java:289)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launch (Launcher.java:229)
   at org.codehaus.plexus.classworlds.launcher.Launcher.mainWithExitCode (Launcher.java:415)
   at org.codehaus.plexus.classworlds.launcher.Launcher.main (Launcher.java:356)
Caused by: java.lang.RuntimeException: java.lang.ExceptionInInitializerError
   at com.sun.tools.javac.api.JavacTaskImpl.handleExceptions (JavacTaskImpl.java:163)
   at com.sun.tools.javac.api.JavacTaskImpl.doCall (JavacTaskImpl.java:100)
   at com.sun.tools.javac.api.JavacTaskImpl.call (JavacTaskImpl.java:94)
   at org.codehaus.plexus.compiler.javac.JavaxToolsCompiler.compileInProcess (JavaxToolsCompiler.java:126)
   at org.codehaus.plexus.compiler.javac.JavacCompiler.performCompile (JavacCompiler.java:169)
   at org.apache.maven.plugin.compiler.AbstractCompilerMojo.execute (AbstractCompilerMojo.java:785)
   at org.apache.maven.plugin.compiler.CompilerMojo.execute (CompilerMojo.java:129)
   at org.apache.maven.plugin.DefaultBuildPluginManager.executeMojo (DefaultBuildPluginManager.java:137)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:208)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:154)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:146)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:117)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:81)
   at org.apache.maven.lifecycle.internal.builder.singlethreaded.SingleThreadedBuilder.build (SingleThreadedBuilder.java:56)
   at org.apache.maven.lifecycle.internal.LifecycleStarter.execute (LifecycleStarter.java:128)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:305)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:192)
   at org.apache.maven.DefaultMaven.execute (DefaultMaven.java:105)
   at org.apache.maven.cli.MavenCli.execute (MavenCli.java:954)
   at org.apache.maven.cli.MavenCli.doMain (MavenCli.java:288)
   at org.apache.maven.cli.MavenCli.main (MavenCli.java:192)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0 (Native Method)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke (NativeMethodAccessorImpl.java:62)
   at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke (DelegatingMethodAccessorImpl.java:43)
   at java.lang.reflect.Method.invoke (Method.java:564)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launchEnhanced (Launcher.java:289)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launch (Launcher.java:229)
   at org.codehaus.plexus.classworlds.launcher.Launcher.mainWithExitCode (Launcher.java:415)
   at org.codehaus.plexus.classworlds.launcher.Launcher.main (Launcher.java:356)
Caused by: java.lang.ExceptionInInitializerError
   at lombok.javac.handlers.HandleGetter.<clinit> (HandleGetter.java:298)
   at java.lang.Class.forName0 (Native Method)
   at java.lang.Class.forName (Class.java:427)
   at lombok.core.SpiLoadUtil$1$1.next (SpiLoadUtil.java:111)
   at lombok.javac.HandlerLibrary.loadAnnotationHandlers (HandlerLibrary.java:170)
   at lombok.javac.HandlerLibrary.load (HandlerLibrary.java:155)
   at lombok.javac.JavacTransformer.<init> (JavacTransformer.java:43)
   at lombok.javac.apt.LombokProcessor.init (LombokProcessor.java:86)
   at lombok.core.AnnotationProcessor$JavacDescriptor.want (AnnotationProcessor.java:87)
   at lombok.core.AnnotationProcessor.init (AnnotationProcessor.java:141)
   at lombok.launch.AnnotationProcessorHider$AnnotationProcessor.init (AnnotationProcessor.java:53)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment$ProcessorState.<init> (JavacProcessingEnvironment.java:702)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment$DiscoveredProcessors$ProcessorStateIterator.next (JavacProcessingEnvironment.java:830)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment.discoverAndRunProcs (JavacProcessingEnvironment.java:926)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment$Round.run (JavacProcessingEnvironment.java:1270)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment.doProcessing (JavacProcessingEnvironment.java:1384)
   at com.sun.tools.javac.main.JavaCompiler.processAnnotations (JavaCompiler.java:1263)
   at com.sun.tools.javac.main.JavaCompiler.compile (JavaCompiler.java:935)
   at com.sun.tools.javac.api.JavacTaskImpl.lambda$doCall$0 (JavacTaskImpl.java:104)
   at com.sun.tools.javac.api.JavacTaskImpl.handleExceptions (JavacTaskImpl.java:147)
   at com.sun.tools.javac.api.JavacTaskImpl.doCall (JavacTaskImpl.java:100)
   at com.sun.tools.javac.api.JavacTaskImpl.call (JavacTaskImpl.java:94)
   at org.codehaus.plexus.compiler.javac.JavaxToolsCompiler.compileInProcess (JavaxToolsCompiler.java:126)
   at org.codehaus.plexus.compiler.javac.JavacCompiler.performCompile (JavacCompiler.java:169)
   at org.apache.maven.plugin.compiler.AbstractCompilerMojo.execute (AbstractCompilerMojo.java:785)
   at org.apache.maven.plugin.compiler.CompilerMojo.execute (CompilerMojo.java:129)
   at org.apache.maven.plugin.DefaultBuildPluginManager.executeMojo (DefaultBuildPluginManager.java:137)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:208)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:154)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:146)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:117)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:81)
   at org.apache.maven.lifecycle.internal.builder.singlethreaded.SingleThreadedBuilder.build (SingleThreadedBuilder.java:56)
   at org.apache.maven.lifecycle.internal.LifecycleStarter.execute (LifecycleStarter.java:128)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:305)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:192)
   at org.apache.maven.DefaultMaven.execute (DefaultMaven.java:105)
   at org.apache.maven.cli.MavenCli.execute (MavenCli.java:954)
   at org.apache.maven.cli.MavenCli.doMain (MavenCli.java:288)
   at org.apache.maven.cli.MavenCli.main (MavenCli.java:192)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0 (Native Method)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke (NativeMethodAccessorImpl.java:62)
   at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke (DelegatingMethodAccessorImpl.java:43)
   at java.lang.reflect.Method.invoke (Method.java:564)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launchEnhanced (Launcher.java:289)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launch (Launcher.java:229)
   at org.codehaus.plexus.classworlds.launcher.Launcher.mainWithExitCode (Launcher.java:415)
   at org.codehaus.plexus.classworlds.launcher.Launcher.main (Launcher.java:356)
Caused by: java.lang.ClassNotFoundException: com.sun.tools.javac.code.TypeTags
   at java.lang.ClassLoader.findClass (ClassLoader.java:719)
   at java.lang.ClassLoader.loadClass (ClassLoader.java:589)
   at lombok.launch.ShadowClassLoader.loadClass (ShadowClassLoader.java:418)
   at java.lang.ClassLoader.loadClass (ClassLoader.java:522)
   at java.lang.Class.forName0 (Native Method)
   at java.lang.Class.forName (Class.java:340)
   at lombok.javac.JavacTreeMaker$SchroedingerType.getFieldCached (JavacTreeMaker.java:156)
   at lombok.javac.JavacTreeMaker$TypeTag.typeTag (JavacTreeMaker.java:244)
   at lombok.javac.Javac.<clinit> (Javac.java:154)
   at lombok.javac.handlers.HandleGetter.<clinit> (HandleGetter.java:298)
   at java.lang.Class.forName0 (Native Method)
   at java.lang.Class.forName (Class.java:427)
   at lombok.core.SpiLoadUtil$1$1.next (SpiLoadUtil.java:111)
   at lombok.javac.HandlerLibrary.loadAnnotationHandlers (HandlerLibrary.java:170)
   at lombok.javac.HandlerLibrary.load (HandlerLibrary.java:155)
   at lombok.javac.JavacTransformer.<init> (JavacTransformer.java:43)
   at lombok.javac.apt.LombokProcessor.init (LombokProcessor.java:86)
   at lombok.core.AnnotationProcessor$JavacDescriptor.want (AnnotationProcessor.java:87)
   at lombok.core.AnnotationProcessor.init (AnnotationProcessor.java:141)
   at lombok.launch.AnnotationProcessorHider$AnnotationProcessor.init (AnnotationProcessor.java:53)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment$ProcessorState.<init> (JavacProcessingEnvironment.java:702)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment$DiscoveredProcessors$ProcessorStateIterator.next (JavacProcessingEnvironment.java:830)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment.discoverAndRunProcs (JavacProcessingEnvironment.java:926)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment$Round.run (JavacProcessingEnvironment.java:1270)
   at com.sun.tools.javac.processing.JavacProcessingEnvironment.doProcessing (JavacProcessingEnvironment.java:1384)
   at com.sun.tools.javac.main.JavaCompiler.processAnnotations (JavaCompiler.java:1263)
   at com.sun.tools.javac.main.JavaCompiler.compile (JavaCompiler.java:935)
   at com.sun.tools.javac.api.JavacTaskImpl.lambda$doCall$0 (JavacTaskImpl.java:104)
   at com.sun.tools.javac.api.JavacTaskImpl.handleExceptions (JavacTaskImpl.java:147)
   at com.sun.tools.javac.api.JavacTaskImpl.doCall (JavacTaskImpl.java:100)
   at com.sun.tools.javac.api.JavacTaskImpl.call (JavacTaskImpl.java:94)
   at org.codehaus.plexus.compiler.javac.JavaxToolsCompiler.compileInProcess (JavaxToolsCompiler.java:126)
   at org.codehaus.plexus.compiler.javac.JavacCompiler.performCompile (JavacCompiler.java:169)
   at org.apache.maven.plugin.compiler.AbstractCompilerMojo.execute (AbstractCompilerMojo.java:785)
   at org.apache.maven.plugin.compiler.CompilerMojo.execute (CompilerMojo.java:129)
   at org.apache.maven.plugin.DefaultBuildPluginManager.executeMojo (DefaultBuildPluginManager.java:137)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:208)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:154)
   at org.apache.maven.lifecycle.internal.MojoExecutor.execute (MojoExecutor.java:146)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:117)
   at org.apache.maven.lifecycle.internal.LifecycleModuleBuilder.buildProject (LifecycleModuleBuilder.java:81)
   at org.apache.maven.lifecycle.internal.builder.singlethreaded.SingleThreadedBuilder.build (SingleThreadedBuilder.java:56)
   at org.apache.maven.lifecycle.internal.LifecycleStarter.execute (LifecycleStarter.java:128)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:305)
   at org.apache.maven.DefaultMaven.doExecute (DefaultMaven.java:192)
   at org.apache.maven.DefaultMaven.execute (DefaultMaven.java:105)
   at org.apache.maven.cli.MavenCli.execute (MavenCli.java:954)
   at org.apache.maven.cli.MavenCli.doMain (MavenCli.java:288)
   at org.apache.maven.cli.MavenCli.main (MavenCli.java:192)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke0 (Native Method)
   at jdk.internal.reflect.NativeMethodAccessorImpl.invoke (NativeMethodAccessorImpl.java:62)
   at jdk.internal.reflect.DelegatingMethodAccessorImpl.invoke (DelegatingMethodAccessorImpl.java:43)
   at java.lang.reflect.Method.invoke (Method.java:564)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launchEnhanced (Launcher.java:289)
   at org.codehaus.plexus.classworlds.launcher.Launcher.launch (Launcher.java:229)
   at org.codehaus.plexus.classworlds.launcher.Launcher.mainWithExitCode (Launcher.java:415)
   at org.codehaus.plexus.classworlds.launcher.Launcher.main (Launcher.java:356)
[ERROR]
[ERROR] Re-run Maven using the -X switch to enable full debug logging.
[ERROR]
[ERROR] For more information about the errors and possible solutions, please read the following articles:
[ERROR] [Help 1] http://cwiki.apache.org/confluence/display/MAVEN/MojoExecutionException
```

- 升级 lombok 后，遇到新的错误，通过查询thrift官方文件[EncodingUtils.java](https://github.com/apache/thrift/blob/master/lib/java/src/org/apache/thrift/EncodingUtils.java)
  发现，`setBit`方法在最新版本中存在，但是在`0.8.x`不存在。因此，是新旧版本api的不匹配导致的异常。
- 通过`mvn dependency:tree`查询java包依赖发现有一个`0.8.0`版本的thrift依赖，但是该包通过provide方式
  引入，所以经常诡异地触发该错误。
- 解决方法：修改pom.xml配置文件，把低版本的thrift依赖去掉，解决了该问题！
```bash
Error running child : java.lang.NoSuchMethodError: org.apache.thrift.EncodingUtils.setBit(BIZ)B
```