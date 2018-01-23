---
title: "以太坊网络的命令行工具"
layout: page
date: 2018-01-23
tags: ["区块链"]
---

[TOC]

这些是区块链开发者的工具。 命令行工具将允许您连接服务器或在以太坊区块链上运行应用程序 - 或者您自己的私人区块链。


## 客户端
为了安全起见 ，为以太坊创建了三个独立的实现。 客户端具有几乎相同的功能，因此您选择的客户端可以在平台、语言以及您计划的网络使用方面进行个人选择。

如果您正在构建一个需要对以太坊网络提供最大正常运行时间保证的业务，那么我们建议您至少运行两个客户端的一个实例以确保可靠性。

## Geth

![geth](https://ethereum.org/images/icons/gopher.png)

Go语言的实现被称为Geth 。 Geth已经过安全审计，将成为面向最终用户的 **Mist Browser** 的未来基础，所以如果你有网站开发经验，并且有兴趣为dapps构建前端，你应该试用Geth。

### MAC OS
安装Homebrew ，并确保它是最新的：

```bash
brew update
brew upgrade
```

然后使用这些命令来安装以太坊：

```bash
brew tap ethereum/ethereum
brew install ethereum
```

有关更多信息，请参阅[Mac OSX Geth的完整文档](https://github.com/ethereum/go-ethereum/wiki/Installation-Instructions-for-Mac)

### WINDOWS
下载最新的稳定[二进制文件](https://geth.ethereum.org/downloads/) ，解压缩，下载压缩文件，从压缩包中提取geth.exe，打开一个命令终端并输入：

```bash
chdir <path to extracted binary>
open geth.exe
```

有关更多信息，请参阅[Windows Geth上的完整文档](https://github.com/ethereum/go-ethereum/wiki/Installation-instructions-for-Windows)

### LINUX
在Ubuntu上，执行这些命令：

```bash
sudo apt-get install software-properties-common sudo add-apt-repository -y ppa:ethereum/ethereum sudo apt-get update sudo apt-get install ethereum
```

有关其他环境和更多说明，请参阅[Geth linux的完整文档](https://github.com/ethereum/go-ethereum/wiki/Building-Ethereum)

## ETH
![Logo for C ++](https://ethereum.org/images/icons/cpp.png)

C++实现称为Eth 。 如果您希望通过并行运行两种不同的实现来增加安全性，或者认真对待GPU挖掘，那么C++“Eth”客户端就是为您服务的。

### MAC
安装Homebrew ，然后确保它是最新的：

```bash
brew update
brew upgrade
```

然后使用这些命令来安装cpp-ethereum：

```
brew tap ethereum/ethereum
brew install cpp-ethereum
brew linkapps cpp-ethereum
```

[cpp-ethereum文档](http://www.ethdocs.org/en/latest/ethereum-clients/cpp-ethereum/index.html)提供了有关OS X Homebrew软件包和从Source构建OS X的详细信息。

### 在LINUX上安装：
如果您使用Apt-get，请将其粘贴到终端中：

```
apt-get install cpp-ethereum
```

cpp-ethereum文档详细介绍了关于Ubuntu的PPA以及从Source构建Linux的信息 。

### 在WINDOWS上安装
cpp-ethereum文档包含有关从源代码构建Windows的详细信息。

## Python

Python的实现被称为Pyethapp。 如果您有兴趣了解以太坊如何工作以及如何扩展它，那么这个客户端的代码基础可能是最具可读性的，并且拥有一个具有快速开发周期的合同测试程序库。 这并不意味着高端的使用，因为这个客户端的性能没有那么高的清晰度和可靠性。 如果你是一个Python开发者，希望构建分布式的应用程序，或者对以太坊有兴趣进行研究或学术研究，那么这是一个很好的客户端：我们邀请你来看看并为之作出贡献 。

### 其他选项
- 奇偶校验技术 （ Parity Technologies）实施了一个Rust实施
- 由Blockapps开发的一个Haskell实现
- 如果您有兴趣开发一个完全在Web浏览器中运行的轻型应用程序，那么我们推荐使用EthereumJS作为基础。
- 如果您想创建一个小型硬件项目，请查看Raspberry Pi的实现
- 如果你想为非Ubuntu Linux安装geth，那么我们建议你从源代码开始构建
- 如果你想在Mac上有更多的灵活性，试试Homebrew


## 运行
Geth和Eth是运行完整的以太坊节点的多用途命令行工具。 它们提供了多个接口： 命令行子命令和选项， JSON-RPC服务器和交互式控制台 。

对于本指南，我们将重点介绍控制台，这是一个包含您可能需要的所有主要功能的JavaScript环境。 根据您的客户端，粘贴以下任一命令：

Geth

```
geth console
```

Eth

Eth仍然有一个内置的控制台，但它很快就会被移除。 开始使用

```
eth
```

然后使用`geth attach` （如果你也有geth）或者下面的npm控制台。 Eth可能需要一些时间才能启动。

```
npm install -g ethereum-console
ethconsole
```

第一次启动命令行时，您将获得许可证。 在您使用之前，您必须先接受此许可证，请仔细阅读。

*注意：如果您只想测试技术并玩耍，请勿使用主网络。 进一步阅读以了解如何部署私有测试网络，而不用花费太多时间。*

### 连接到一个私人测试网
有时你可能不想连接到现场公共网络; 相反，您可以选择创建自己的私人测试网。 如果您不需要测试公共合同，只想尝试或开发技术，这非常有用。 由于您是您专用网络的唯一成员，因此您有责任查找所有块，验证所有交易并执行所有智能合约。 这使得开发更容易，因为您可以灵活控制个人区块链中的交易。

Geth

```
geth --datadir ~/.ethereum_private init ~/dev/genesis.json
geth --fast --cache 512 --ipcpath ~/Library/Ethereum/geth.ipc --networkid 1234 --datadir ~/.ethereum_private console
```

Eth

```
eth --private 12345 --genesis-json ~/test/genesis.json --db-path ~/.ethereum_experiment
```

将12345替换为您想要用作网络ID的任意数字。 最好改变生成块的内容，因为如果有人不小心使用真正的链连接到你的测试网络，你的本地副本将被认为是一个旧的分支，并更新为“真实”的分支 。 改变数据地址也会改变您本地的区块链副本，否则，为了成功地挖掘一个区块，您需要对付区块链本地副本中存在的最后一个区块 - 这可能需要几个小时。

如果你想创建一个私人网络，你应该出于安全原因使用不同的起源块（genesis block: 一个包含Ether所有交易的数据库）。 你可以阅读我们的博客文章如何生成您的文件 。 在不久的将来，我们将提供更好的方法来获得其他的起源块。

这些命令可以防止任何人不知道您选择的密码，网络ID和起源文件连接到您或为您提供不需要的数据。 如果你想连接到其他节点，并创建一个多台计算机的小型私人网络，他们都将需要使用相同的网络ID和一个相同的起源块。 您还必须帮助每个节点找到其他节点。 要做到这一点，首先你需要自己的节点URL：

```
admin.nodeInfo.NodeUrl
```

该命令将会返回你的节点url - 记下它，然后在其他客户端上，告诉他们通过执行这个命令来添加你的节点：

```
admin.addPeer("YOURNODEURL")
```

您不需要将每个客户端添加到彼此，因为一旦连接，他们将共享关于他们连接到的任何其他同伴的信息。

如果您使用的是Eth，那么只需找出您的IP并执行以下命令：

```
web3.admin.net.connect("YOURIP:30303")
```

### 日志
如果你正在运行Geth，你会注意到在你的控制台上弹出了很多日志条目 - 有时在你输入的时候。 这是因为所有警告和进度信息都由客户端实时记录到您的终端中。 如果要将日志保存到可以稍后查看的文件，请使用以下命令：

```
geth console 2>>geth.log
```

Geth支持多个终端窗口，你可以用一个日志和另一个控制台启动一个新的日志。 这将给你与原来的控制台完全相同的功能，但没有混乱。 要做到这一点，打开一个新的终端窗口并输入：

```
geth attach
```

控制台具有自动完成和历史命令支持。 您可以通过按Tab键完成一个命令，然后geth会自动补全当前的语句，当有多个补全选择时显示可用补全列表。 您可以使用向上和向下箭头键导航您的命令历史记录。

### 了解有关运行节点的更多信息
- [备份还原](http://ethdocs.org/en/latest/account-management.html#backup-and-restore-accounts)
- [连接到网络](http://ethdocs.org/en/latest/network/connecting-to-the-network.html)

## 用法示例
### 创建帐户
为了使用以太坊网络，你需要生成一个帐户。 有很多方法可以解决这个问题 ，但最简单的方法是通过控制台。

注意：如果您是在奥运期间或开发早期运行以太网， 请不要重复使用 Frontier客户端软件1.0发布之前生成的密钥 ，否则可能会受到重播攻击（replay attacks） 。 备份这些密钥，并使用Frontier发布客户端创建新密钥。

```
personal.newAccount("Write here a good, randomly generated, passphrase!")
```

注意：拿起一个很好的密码并写下来。 如果您丢失了用于加密帐户的密码，则无法访问该帐户。 重复：没有安全网。 如果没有有效的密码，无法访问您的帐户，并且此处没有“忘记我的密码”选项。 有关详细信息，请参阅此XKCD 。

**不要忘记你的 PASSPHRASE！**

您可以创建尽可能多或尽可能少的帐户。 按照惯例我们称您创建您的主要帐户的第一个帐户。 您可以使用以下命令查看所有帐户：

```
web3.eth.accounts
```

账户的排序反映了他们创建的时间。 密钥文件存储在`DATADIR/keystore`下，可以通过复制其中包含的文件在客户端之间传输。 这些文件使用密码进行加密，如果包含任何数量的ether，则应备份。 但是，请注意，如果您传输个别密钥文件，则提交的帐户顺序可能会发生变化，您可能无法在同一个位置结束同一个帐户。 所以请注意，只要您不将外部密钥文件复制到您的密钥库，依靠帐户索引就是合理的。

### 获取帐户的余额
控制台上的所有命令实际上都是在JavaScript中运行，因此您可以创建变量和函数。 您也可以将任何“eth”函数编写为“web3.eth”，因为它实际上是主“web3”对象的一部分。

试试这个例子：

```JavaScript
var primaryAccount = web3.eth.accounts[0]
```

您现在有一个名为`primaryAccount`的变量，您可以在其他调用中使用该变量。 要获得任何帐户的余额，请使用`eth.getBalance`函数，如下所示：

```JavaScript
web3.eth.getBalance(primaryAccount)
```

你的余额应该返回0，因为你刚创建它。 为了进行下一步操作，您需要在帐户中拥有一些以太网帐户，以便支付gas费用。 在下一节中，您将了解什么是gas，以及如何与网络进行交互。

### 一次检查所有余额
命令行工具是JavaScript环境，这意味着您可以像创建JavaScript一样创建函数。 例如，如果您想要一次查看所有帐户的余额，请使用此JavaScript代码段。

它将遍历每个帐户，并在以太网中打印其余额，可以使用以下代码：

```JavaScript
function checkAllBalances() {
     web3.eth.getAccounts(function(err, accounts) {
          accounts.forEach(function(id) {
               web3.eth.getBalance(id, function(err, balance) {
                    console.log("" + id + ":\tbalance: " + web3.fromWei(balance, "ether") + " ether");
                });
        });
    });
};
```

一旦你执行上面的行，你需要检查所有的余额是调用下面的函数：

```JavaScript
checkAllBalances()
```

提示：如果您有许多这样的小巧方便的脚本，您可以将它们保存到一个文件中，然后使用loadScript一次加载它们：

```JavaScript
loadScript('/some/script/here.js')
```
