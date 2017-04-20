---
title: "CentOS 上搭建PPTP VPN"
layout: page
date: 2017-04-19
---
[TOC]

## 关于
记录PPTP VPN 搭建流程，亲测成功。
我是参考这个地方<http://www.dabu.info/centos6-4-structures-pptp-vpn.html>

## 过程
- 安装ppp和iptables

```bash
yum install -y perl ppp iptables
```

- 安装 pptpd

首先查看ppp版本，不同版本对应的pptpd版本也不一样

```bash
yum list installed ppp
```

找到对应的ppp版本:<http://poptop.sourceforge.net/yum/stable/packages/>

```
ppp 2.4.4——————>pptpd 1.3.4
ppp 2.4.5——————>pptpd 1.4.0
```

我的电脑是i686, 2.4.5版本，所以找到对应的pptpd版本链接为 <http://poptop.sourceforge.net/yum/stable/packages/pptpd-1.4.0-1.fc12.i686.rpm>

推荐用 rpm 安装

```bash
rpm -Uvh http://poptop.sourceforge.net/yum/stable/packages/pptpd-1.4.0-1.fc12.i686.rpm
yum install pptpd
```

手动安装参考原始链接。

- 修改配置pptpd

```
cp /etc/ppp/options.pptpd /etc/ppp/options.pptpd.bak
vi /etc/ppp/options.pptpd
```

添加DNS解析

```
ms-dns 8.8.8.8
ms-dns 8.8.4.4
```

- 配置用户与密码：

```
cp /etc/ppp/chap-secrets   /etc/ppp/chap-secrets.bak
vi /etc/ppp/chap-secrets
```

加入一行，空格隔开，注意后面的*号

```
你的登陆用户名 pptpd 你的登陆密码 *
```

- 配置pptpd ip转发

```
cp /etc/pptpd.conf     /etc/pptpd.conf.bak
vi /etc/pptpd.conf
```

加入下面两行，并以空行结尾这个文件。下面的代码不需要更改其中的IP

```
localip 192.168.9.1
remoteip 192.168.9.11-30 //表示vpn客户端获得ip的范围
```

- 配置流量转发

```
vi /etc/sysctl.conf
```


将`net.ipv4.ip_forward = 0` 改成 `net.ipv4.ip_forward = 1`

保存修改 `/sbin/sysctl -p`

- 启动pptpd服务

```bash
/sbin/service pptpd start
# 或者 service pptpd start
```

到此，可以测试pptp拨号了，应该可以成功拨号，用户名和密码填上面配置的用户名和密码！
但是还没有网络访问权限。

- 配置网络流量转发

```
iptables -t nat -A POSTROUTING    -s 192.168.9.0/24 -j SNAT --to-source  你的服务器公网IP
```

保存转发规则，并重启服务

```
/etc/init.d/iptables save
/sbin/service iptables restart
service pptpd restart
```

至此，就可以使用 VPN 科学上网了！

- 设置开机启动

```
chkconfig pptpd on
chkconfig iptables on
```

- 如果你嫌安装麻烦，可以试试 shadesocks, openvpn等易用的VPN方案。
