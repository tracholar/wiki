---
title: "Spring 快速入门"
layout: page
date: 2018-12-17
---

[TOC]

## 概念
- Bean: Java可重用对象, 可以复用, 单例模式
- IOC（控制反转，Inverse Of Control）, DI（依赖注入，Dependency Injection），两者是一样的


## Bean 可重用对象
下述BasicBean的属性value可以通过 getter 和 setter 获取和设置。

```java
public class BasicBean {
    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    private String value = "";
}
```

如果我们要创建一个 `BasicBean` 对象,并设置`value`的值为 `Hello Test Bean`, 可以按照Java的基本语法实现如下

```java
BasicBean bean = new BasicBean();
bean.setValue("Hello Test Bean");
```

但是,如果我们要在很多不同的函数中使用同一个 `BasicBean` 对象,我们就需要实现单例模式来做。

Spring里面提供一种更方便的方式, 通过XML配置文件和`BeanFactory`实现对象可重用。

```xml
<?xml version = "1.0" encoding = "UTF-8"?>

<beans xmlns = "http://www.springframework.org/schema/beans"
       xmlns:xsi = "http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation = "http://www.springframework.org/schema/beans
   http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
   
    <bean id = "test_bean" class="com.tracholar.web.demo.BasicBean">
        <property name="value" value="Hello Test Bean" />
    </bean>
</beans>
```

`id` 是唯一标识, `class` 对应该对象的类, `property` 对应的是属性。有了这个配置文件后, 重复使用这个对象就很方便了。

```java
BeanFactory factory = new ClassPathXmlApplicationContext("Beans.xml");
BasicBean bean = (BasicBean) factory.getBean("test_bean");
System.out.println(bean.getValue()); // Hello Test Bean
```

当然,Bean也可以配置成多例模式,这样每次得到的都是一个新的对象。

利用 `@Autowired` 注解,可以避免属性的 `get` 和 `set` 方法

```java
public class BasicBean {
    @Autowired
    private String value = "";
}
```


## SpringBoot 简单 Web APP
HTTP的Get, POST 等请求最终是由 Controller 处理, 首先实现一个这样的处理类

```java 
@RestController
public class GreetingController {

    private static final String template = "Hello, %s!";

    @RequestMapping("/greeting")
    public BasicBean greeting(@RequestParam(value="name", defaultValue="World") String name) {
        BasicBean bean = new BasicBean();
        bean.setValue(String.format(template, name));

        return bean;
    }
    
}
```
Annotation `@RestController` 表明这个类用于处理HTTP请求, `@RequestMapping("/greeting")`表明处理的路径为 `/greeting`,
`RequestParam`可以帮你配置一些参数的属性,比如默认值。SpringBoot 会自动将返回的对象转换成JSON,所以请求返回的结果将是一个JSON。

`@RequestMapping` 会将GET请求和POST请求都发到这个方法,如果要分别处理,可以用 `@GetMapping` 和 `@PostMapping` 分别指定。
`@RequestMapping` 也可以加到整个类, 比如对整个类加上注解 `@RequestMapping("/home")`, 那么这个类的方法都是在子路径下, `greeting`对应的路径就是 `/home/greeting`。

有了 Controller 之后,我们启动一个简单的Server,就可以测试了。

```java
@SpringBootApplication
public class TestWebDemo {
    public static void main(String[] args){
        SpringApplication.run(TestWebDemo.class, args);
    }
    
    
    @Bean
    public EmbeddedServletContainerFactory servletContainer() {
        TomcatEmbeddedServletContainerFactory factory =
                new TomcatEmbeddedServletContainerFactory();
        return factory;
    }
    /* 上述方法等同于在配置文件中的设置
    <bean id="xxx" class = "org.springframework.boot.context.embedded.tomcat.TomcatEmbeddedServletContainerFactory" />
    
    */
}
```

`@SpringBootApplication` Annotation 相当于以下3个Annotation共同的效果:

1. `@Configuration` 将这个类作为Bean的定义类, Bean的定义不但可以通过配置文件,也可以通过配置类中用`@Bean`标记的方法, 返回的结果就是一个Bean。
2. `@EnableAutoConfiguration`   作用从classpath中搜索所有META-INF/spring.factories配置文件然后，将其中
    org.springframework.boot.autoconfigure.EnableAutoConfiguration key对应的配置项加载到spring容器。
3. `@EnableWebMvc`
4. `@ComponentScan` 在同一个包中扫描其他组件, 例如Controller。

Spring会自动查找所有的Controller, 所以不用做额外的配置。启动运行后, 从浏览器打开 `localhost:8080`,可以看到返回的JSON

```json
{
    "value": "Hello, World!"
}
```

## 定时执行任务
参考: <https://spring.io/guides/gs/scheduling-tasks/>

首先创建一个定时执行模块, 用 `@Scheduled` 注解标注定是执行的方法, `fixedRate` 参数指定间隔毫秒数。

```java
@Component
public class ScheduledTasks {
    private static final Logger log = LoggerFactory.getLogger(ScheduledTasks.class);

    private static final SimpleDateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");

    @Scheduled(fixedRate = 1000)
    public void reportTime(){
        log.info("Now: {}", dateFormat.format(new Date()));
    }
}
```

但是spring默认不起用Scheduled模块,需要在 `TestWebDemo` 上再加个`@EnableScheduling`注解。执行后结果如下

```bash
2018-12-27 15:04:04.568  INFO 76968 --- [pool-1-thread-1] com.tracholar.web.demo.ScheduledTasks    : Now: 15:04:04
2018-12-27 15:04:05.567  INFO 76968 --- [pool-1-thread-1] com.tracholar.web.demo.ScheduledTasks    : Now: 15:04:05
2018-12-27 15:04:06.567  INFO 76968 --- [pool-1-thread-1] com.tracholar.web.demo.ScheduledTasks    : Now: 15:04:06
2018-12-27 15:04:07.567  INFO 76968 --- [pool-1-thread-1] com.tracholar.web.demo.ScheduledTasks    : Now: 15:04:07
2018-12-27 15:04:08.568  INFO 76968 --- [pool-1-thread-1] com.tracholar.web.demo.ScheduledTasks    : Now: 15:04:08
2018-12-27 15:04:09.568  INFO 76968 --- [pool-1-thread-1] com.tracholar.web.demo.ScheduledTasks    : Now: 15:04:09
```

## 文件上传
参考: <https://spring.io/guides/gs/uploading-files/>



```java
@Controller
public class FileUploader {
    Logger logger = LoggerFactory.getLogger(this.getClass());

    @PostMapping("/upload")
    public String uploadHandler(@RequestParam("file") MultipartFile file,
                                RedirectAttributes redirectAttributes) throws IOException{
        logger.info(file.getOriginalFilename());
        File f = new File("/tmp/www_upload/" + file.getOriginalFilename());
        logger.info("Move to " + f.getAbsolutePath());
        file.transferTo(f);
        redirectAttributes.addFlashAttribute("message", "You successfully upload " + file.getOriginalFilename());
        return "redirect:/";
    }
}

```

`MultipartFile` 对象用于表示文件, `@Controller` 注解和 `@RestController` 都表示这个类是一个控制器, 
但是`@Controller`返回的是一个 `ModelAndView`,如果方法返回的是string,表示是页面名字, 参考下面的 **页面模板**。
`@RestController` 等于加了 `@ResponseBody` 注解的 `@Controller`, 也就是方法返回的是内容本身,不会加载某个页面。

## 页面模板
Spring Boot 的页面模板和资源文件放在 `resources` 下面,

- `resources/templates` 存放前端html页面
- `resources/static` 存放静态文件, **这个目录对应于HTTP请求时的根目录**。例如: `resources/static/style.css` 可以通过URL `/style.css`直接引用!

如果没有这两个文件夹,可以自己创建。
在Controller中返回一个页面非常简单, 返回页面的文件名(不包含后缀)即可, 也可以返回 `ModelAndView` 对象。
但是,要使得引擎生效,需要增加`spring-boot-starter-thymeleaf`的依赖。`thymeleaf` 引擎语法参考 <https://www.thymeleaf.org/doc/tutorials/2.1/thymeleafspring.html>

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-thymeleaf</artifactId>
    <version>1.5.15.RELEASE</version>
</dependency>
```

```java
@Controller
public class FileUploader {
    Logger logger = LoggerFactory.getLogger(this.getClass());

    @GetMapping("/upload")
    public ModelAndView showUploadPage() {
        return new ModelAndView("upload_file");
    }
    @PostMapping("/upload")
    public String uploadHandler(@RequestParam("file") MultipartFile file,
                                RedirectAttributes redirectAttributes) throws IOException{
        logger.info(file.getOriginalFilename());
        File f = new File("/tmp/www_upload/" + file.getOriginalFilename());
        logger.info("Move to " + f.getAbsolutePath());
        file.transferTo(f);
        redirectAttributes.addFlashAttribute("message", "You successfully upload " + file.getOriginalFilename());
        return "redirect:/";
    }
}

```

## Bean 自动配置
配置文件: `resources/application.properties` 配置文件的语法参考 <http://blog.didispace.com/springbootproperties/>

```
my.conf.files=www.baidu.com,www.google.com
```


```java
@Component
@ConfigurationProperties(prefix = "my.conf")
public class MyConfig {
    public List<String> getFiles() {
        return files;
    }

    public void setFiles(List<String> files) {
        this.files = files;
    }

    private List<String> files = new ArrayList<>();

}

@RestController
@RequestMapping("/conf")
public class MyConfigController {
    @Autowired
    private MyConfig myConfig;

    @RequestMapping("/")
    public Object getConf(){
        return myConfig.getFiles();
    }
}
```

注解`ConfigurationProperties`表明对这个类属性进行配置,配置文件中key的前缀是`my.conf`, 
在使用这个类时,加上 `@Autowired` 注解, spring就会从注册组件中寻找对应的类,并创建对象,这个过程叫自动装配, 对象统一由spring来管理。
参考 spring 依赖注入 <https://www.jianshu.com/p/3942cce05f71>。

> 首先想说说IoC（Inversion of Control，控制倒转）。这是spring的核心，贯穿始终。
> 所谓IoC，对于spring框架来说，就是由spring来负责控制对象的生命周期和对象间的关系。
> 这是什么意思呢，举个简单的例子，我们是如何找女朋友的？常见的情况是，我们到处去看哪里有长得漂亮身材又好的mm，
> 然后打听她们的兴趣爱好、qq号、电话号、ip号、iq号………，想办法认识她们，投其所好送其所要，然后嘿嘿……这个过程是复杂深奥的，
> 我们必须自己设计和面对每个环节。传统的程序开发也是如此，在一个对象中，如果要使用另外的对象，就必须得到它（自己new一个，或者从JNDI中查询一个），
> 使用完之后还要将对象销毁（比如Connection等），对象始终会和其他的接口或类藕合起来
> 
> 那么IoC是如何做的呢？有点像通过婚介找女朋友，在我和女朋友之间引入了一个第三者：婚姻介绍所。
> 婚介管理了很多男男女女的资料，我可以向婚介提出一个列表，告诉它我想找个什么样的女朋友，比如长得像李嘉欣，身材像林熙雷，唱歌像周杰伦，
> 速度像卡洛斯，技术像齐达内之类的，然后婚介就会按照我们的要求，提供一个mm，我们只需要去和她谈恋爱、结婚就行了。
> 简单明了，如果婚介给我们的人选不符合要求，我们就会抛出异常。整个过程不再由我自己控制，而是有婚介这样一个类似容器的机构来控制。
> Spring所倡导的开发方式就是如此，所有的类都会在spring容器中登记，告诉spring你是个什么东西，你需要什么东西，
> 然后spring会在系统运行到适当的时候，把你要的东西主动给你，同时也把你交给其他需要你的东西。所有的类的创建、销毁都由 spring来控制，
> 也就是说控制对象生存周期的不再是引用它的对象，而是spring。对于某个具体的对象而言，以前是它控制其他对象，现在是所有对象都被spring控制，
> 所以这叫控制反转。如果你还不明白的话，我决定放弃。
  

## 表单认证
创建可认证表单数据对象, 用 `javax.validation.constraints.*` 中的Annotation标注约束条件。

```java
public class PersonData {
    @NotNull
    @Size(min = 5, max = 30)
    private String name;

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @NotNull
    @Min(10)
    @Max(100)
    private Integer age;
}
```

利用 `@Valid` 注解对请求参数标注, 用`BindingResult`获取约束条件判断结果。

```java
@Controller
@RequestMapping("/person")
public class PersonController {
    @GetMapping("/")
    public String form(){
        return "person";
    }
    @PostMapping("/")
    @ResponseBody
    public Object checkPersonInfo(@Valid PersonData person, BindingResult bindingResult){
        if (bindingResult.hasErrors()) {
            return bindingResult.getAllErrors();
        }

        return person;
    }
}
```


问题: 为什么Controller的方法可以自动指定这么多不同类型的参数?


