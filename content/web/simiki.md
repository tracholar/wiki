---
title: "在github pages中使用simiki指南"
layout: page
date: 2016-05-25
---

# 起因
开始使用simiki是因为一篇博客——[程序员的知识管理](http://blog.jobbole.com/101321/)，
这篇博客对我启发很大。是的，程序员或者一般的工程师经常需要配置一些开发环境之类的
工作，如果能够将这些过程记录下来，日后再配置的时候会减少很多不必要的时间浪费。
此外，如果能将平常一些琐碎的知识记录下来也是不错的。使用simiki可以将这些工作通过
wiki来实现，并且可以将数据保存在本地，不用担心数据丢失之类的风险。

**请不要FORK我的WIKI！！** 因为这个WIKI里面的内容大多是我原创的，请不要FORK，谢谢。


# 工具准备
为了使用simiki，需要准备好基本环境。

1. 安装python，不同的平台安装方式不同，都很容易。

2. 安装simiki库及其依赖库，我使用pip进行安装，只需要一条命令`pip install simiki`即可。

3. 注册github账户，并且创建`<username>.github.io`代码项目。完成之后，你应该可以通过该
   子域名访问到自己的page页面，具体细节请参考[官方文档](https://pages.github.com/)。

# 环境配置过程

在github中创建`wiki`项目，并创建`gh-pages`分支[1]。
```
git clone git@github.com:tracholar/wiki.git
git checkout -b gh-pages
git rm -rf .
```

切换到master分支初始化simiki，生成content和themes目录和几个文件。并在output目录生成静态文件。
```
git checkout master
simiki init
simiki g
```

# 部署
## windows中的部署
为了部署方便，在master分钟中将output目录过滤掉，
在.gitignore文件中添加一行
```
output
```
即可。此外，需要将output目录push到gh-pages分支，可以利用我写的一个
批处理脚本[deploy.bat](https://github.com/tracholar/wiki/raw/master/deploy.sh)。
这个脚本提供两个功能，初始化和部署。
```
deploy [option]
	-i  初始化
	message  以message作为git commit信息提交并推送到github
```
如果你要使用请把脚本中库的URL改成你的URL。
初始化之前请删除前面生成的output目录，然后执行以下命令。
```
deploy -i
```
然后重新提交部署
```
deploy init-version
```
然后就可以在你的项目页面中看到wiki了，对我来说是 tracholar.github.io/wiki。

> Tips: 注意文件夹和文件命名统一用小写，否则你会后悔，因为windows不区分大小写
> 而linux是区分的。

## linux/MAC 中的部署
还没试过，等试过之后再写，你可以参考[官方指南](http://simiki.org/docs/deploy.html)
使用`ghp-import`部署，更方便，只是不支持windows。

公司发了mac，我又写了个bash脚本[deploy.sh](https://github.com/tracholar/wiki/raw/master/deploy.sh)。
使用方法跟windows上的一样，不过要`chmod +x deploy.sh`给脚本增加执行权限。
当然，你也需要把git仓库的地址改成你自己的地址。


# 我的效果
请访问[https://tracholar.github.io/wiki](https://tracholar.github.io/wiki)观看。

**请不要FORK我的WIKI！！** 因为这个WIKI里面的内容大多是我原创的，请不要FORK，谢谢。




[1][https://help.github.com/articles/creating-project-pages-manually/](https://help.github.com/articles/creating-project-pages-manually/)
