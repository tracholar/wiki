---
title: "YARN学习"
layout: page
date: 2016-12-25
---
[TOC]

## 关于
YARN 是HADOOP2代，相比第一代产品，可以开发除了 MAPREDUCE 外更复杂的分布式程序，
XGBoost on yarn 就是一个例子！

## 开发指南
<img src="/wiki/static/images/yarn-structure.png" style="float:left;width:500px" />

YARN 用 ResourceManager 来分配资源，NodeManager 来管理计算节点！
ApplicationMaster 用于跟踪任务，管理每一个在YARN上运行的程序实例，
通过调用 ResourceManager 和 NodeManager 进行管理。

要使用一个 YARN 集群，首先需要来自包含一个应用程序的客户的请求。ResourceManager 协商一个容器的必要资源，启动一个 ApplicationMaster 来表示已提交的应用程序。通过使用一个资源请求协议，ApplicationMaster 协商每个节点上供应用程序使用的资源容器。执行应用程序时，ApplicationMaster 监视容器直到完成。当应用程序完成时，ApplicationMaster 从 ResourceManager 注销其容器，执行周期就完成了。

具体而言，就是要创建一个 ApplicationMaster类，例如[XGBoost的例子](https://github.com/dmlc/dmlc-core/blob/master/tracker/yarn/src/main/java/org/apache/hadoop/yarn/dmlc/ApplicationMaster.java)。


创建一个 [application submission client](https://github.com/dmlc/dmlc-core/blob/master/tracker/yarn/src/main/java/org/apache/hadoop/yarn/dmlc/Client.java)，
提交应用给 YARN 的 ResourceManager(RM)。
这个步骤通过创建一个 YarnClient 实例实现！

Following are the important interfaces:

- Client<-->ResourceManager

By using YarnClient objects.

- ApplicationMaster<-->ResourceManager

By using AMRMClientAsync objects, handling events asynchronously by AMRMClientAsync.CallbackHandler

- ApplicationMaster<-->NodeManager

Launch containers. Communicate with NodeManagers by using NMClientAsync objects, handling container events by NMClientAsync.CallbackHandler


一个简单的例子：


```java
// Create yarnClient
YarnClient yarnClient = YarnClient.createYarnClient();
yarnClient.init(conf);
yarnClient.start();

YarnClientApplication app = yarnClient.createApplication();
GetNewApplicationResponse appResponse = app.getNewApplicationResponse();

/**
- **ApplicationSubmissionContext**: defines all the information needed by the RM to launch the AM. A client needs to set the following into the context:
    - Application info: id, name
    - Queue, priority info: Queue to which the application will be submitted, the priority to be assigned for the application.
    - User: The user submitting the application
    - ContainerLaunchContext: The information defining the container in which the AM will be launched and run. The ContainerLaunchContext, as mentioned previously, defines all the required information needed to run the application such as the local *Resources (binaries, jars, files etc.), Environment settings (CLASSPATH etc.), the Command to be executed and security T*okens (RECT).
*/
ApplicationSubmissionContext appContext = app.getApplicationSubmissionContext();
ApplicationId appId = appContext.getApplicationId();

appContext.setKeepContainersAcrossApplicationAttempts(keepContainers);
appContext.setApplicationName(appName);
```

ApplicationMaster 通过 AMRMClientAsync.CallbackHandler 处理资源相关事件，通常在 onContainersAllocated 事件启动任务。


## 参考
1. IBM博客 <https://www.ibm.com/developerworks/cn/data/library/bd-hadoopyarn/index.html>
2. [Hadoop: Writing YARN Applications](https://hadoop.apache.org/docs/current/hadoop-yarn/hadoop-yarn-site/WritingYarnApplications.html)
