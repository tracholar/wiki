---
title: "使用命令行构建智能合约"
layout: page
date: 2018-01-23
tags: ["区块链"]
---

[TOC]


## 使用命令行构建智能合约

这个页面将帮助你在以太坊命令行建立一个Hello，World合约。如果您不知道如何使用命令行，我们建议您跳过本教程，而是使用图形用户界面构建自定义标记。

智能合约是以太坊区块链上的账户持有对象。它们包含代码功能，可以与其他合同进行交互，做出决定，存储数据，并将ether发送给其他人。契约是由他们的创造者定义的，但他们的执行，以及他们所提供的服务，都是由以太网本身提供的。只要整个网络存在，它们就会存在并且可执行，只有被编程为自毁，它们才会消失。

你可以用合同做什么？事实上，你几乎可以做任何事情，但是对于我们的入门指南，我们来做一些简单的事情：首先你将创建一个经典的“Hello
World”合约，然后你可以建立你自己的密码令牌发送给任何你喜欢的人。一旦你掌握了这一点，那么你将通过众筹筹集资金，如果成功，将提供一个完全透明和民主的组织，只会服从自己的公民，永远不会摆脱它的宪法，不能被检查或关闭。而所有这些都在不到300行的代码中。

在你开始之前：

- [安装以太坊CLI](getch-eth.html)
- [详细了解合同](https://github.com/ethereum/go-ethereum/wiki/Contracts-and-Transactions)

进入`geth`控制台之前，请确认GUI已关闭。 运行`geth`开始同步过程（第一次运行可能需要一段时间）。

那么现在就开始吧。

### 你的第一个公民：Greeter

现在你已经掌握了以太坊的基本知识，让我们进入你的第一个严肃的合同。Frontier是一个很大的开放领域，有时你可能会感到孤独，所以我们的第一步就是创造一个自动的伴侣，当你感到孤独的时候迎接你。我们会称他为“Greeter”。

Greeter是一个智能的数字实体，它存在于区块链中，并能够根据其输入与任何与之交互的人进行交谈。它可能不是一个演讲者，但它是一个很好的倾听者。这是它的代码：


```JavaScript
    contract mortal {
        /* Define variable owner of the type address */
        address owner;

        /* This function is executed at initialization and sets the owner of the contract */
        function mortal() { owner = msg.sender; }

        /* Function to recover the funds on the contract */
        function kill() { if (msg.sender == owner) selfdestruct(owner); }
    }

    contract greeter is mortal {
        /* Define variable greeting of the type string */
        string greeting;

        /* This runs when the contract is executed */
        function greeter(string _greeting) public {
            greeting = _greeting;
        }

        /* Main function */
        function greet() constant returns (string) {
            return greeting;
        }
    }
```

你会注意到这个代码中有两个不同的合约：“mortal”和“greeter”。这是因为Solidity（我们使用的高级合同语言）具有继承性，这意味着一个契约可以继承另一个契约的特征。这对于简化编码非常有用，因为合同的常见特征不需要每次重写，所有合同都可以用更小，更易读的块来编写。所以只要声明迎宾者是凡人，你就继承了“凡人”契约的所有特征，并使迎宾者的代码简单易读。

继承特征“凡人”仅仅意味着迎宾合同可以被其所有者杀死，清理区块链，并在不再需要合同时收回锁定的资金。以太坊的契约默认为不死的，没有所有者，这意味着一旦被部署，作者就没有特殊的特权了。在部署之前考虑这一点。

### Solc编译器

在你能够部署它之前，你需要两件事情：编译代码和应用程序二进制接口，它是一个JavaScript对象，它定义了如何与合约进行交互。

这两个事情你都可以通过使用编译器完成。你可以使用solidity编译器。

如果你还没有安装编译器，那么你需要安装一个。你可以在这里找到[安装Solidity的说明](http://solidity.readthedocs.io/en/develop/installing-solidity.html)。

> **译者注**
> solc编译器安装方法：直接利用npm包管理器安装
> `npm install -g solc`

#### 编译你的合同

如果你没有得到以上的固体，那么你需要安装它。你可以在这里找到安装Solidity的说明。

现在你已经安装了编译器，你需要编译合约来获取编译后的代码和应用程序二进制接口。



    solc -o target --bin --abi Greeter.sol


这将创建两个文件，一个文件包含已编译的代码，另一个文件在名为target的目录中创建应用程序二进制接口。



    $tree
    .
    ├── Greeter.sol
    └── target
       ├── Greeter.abi
       ├── Greeter.bin
       ├── Mortal.abi
       └── Mortal.bin


你会看到有为两个合同创建的文件;但是因为Greeter包括Mortal，所以你不需要部署Mortal来部署Greeter。

您可以使用这两个文件来创建和部署合同。



    var greeterFactory = eth.contract(<contents of the file Greeter.abi>)

    var greeterCompiled = "0x" + "<contents of the file Greeter.bin"


你现在已经编译了你的代码，并把它提供给Geth。现在您需要准备好进行部署，包括设置一些变量，例如您希望使用的问候语。将下面的第一行编辑为比“Hello
World！”更有趣的内容并执行这些命令：


```JavaScript
    var _greeting = "Hello World!"

    var greeter = greeterFactory.new(_greeting,{from:eth.accounts[0],data:greeterCompiled,gas:47000000}, function(e, contract){
        if(!e) {

          if(!contract.address) {
            console.log("Contract transaction send: TransactionHash: " + contract.transactionHash + " waiting to be mined...");

          } else {
            console.log("Contract mined! Address: " + contract.address);
            console.log(contract);
          }

        }
    })
```

#### 使用在线编译器

如果你没有安装solC，你可以简单地使用在线编译器。将上面的源代码复制到在线solidity编译器，然后您的编译代码应该出现在左侧窗格中。将`greeter`合同和`mortal`合同中标有Web3
deploy的框中的代码复制到单个文本文件中。现在，在该文件中，将第一行更改为您的问候语：


```JavaScript
    var _greeting = "Hello World!"
```

现在您可以将结果文本粘贴到您的geth窗口中，或者使用`loadScript("yourFilename.js")`导入文件。等待30秒，你会看到这样的消息：



    Contract mined! address: 0xdaa24d02bad7e9d6a80106db164bad9399a0423e


您可能必须使用您在开始时选择的密码来“解锁”发送交易的帐户，因为您需要支付部署合同的天然气费用。
`personal.unlockAccount(web3.eth.accounts[0], "yourPassword")`。

该合同估计需要约18万个气体部署（根据在线固体编译器），在撰写本文时，测试网上的气体价格为20 gwei（等于（20000000000
wei，或0.00000002 ether）每单位有很多有用的统计数据，包括网络统计页面的最新天然气价格。

请注意，这些费用并不支付给以太坊开发者，而是交给了矿工们，那些计算机正在努力寻找新的区块并保证网络安全的同行。天然气价格是由当前的计算供求市场决定的。如果天然气价格太高，你可以成为一名矿工，降低你的要价。

在不到一分钟的时间内，你应该有一个合同地址的日志，这意味着你已经成功地部署了你的合同。您可以使用以下命令验证部署的代码（将被编译）：



    eth.getCode(greeter.address)


如果它返回“0x”以外的任何内容，那么恭喜！你的小家伙活着！如果再次创建合同（通过执行另一个eth.sendTransaction），它将被发布到一个新的地址。

### 运行Greeter

为了打电话给你的机器人，只需在你的终端上输入以下命令：



    greeter.greet();


由于这个调用在区块链上没有任何变化，所以它立即返回并且没有任何天然气成本。你应该看到它返回你的问候语：



    'Hello World!'


#### 让其他人与您的代码进行交互

为了让其他人运行你的合同，他们需要两件事情：合同所在的地址和ABI（应用程序二进制接口），这是一种用户手册，描述了它的功能名称以及如何给你的JavaScript控制台。为了让他们每个人都运行这些命令：



    greeterCompiled.greeter.info.abiDefinition;
    greeter.address;


如果使用基于浏览器的工具进行编译，则可以从标有“接口”的`greeter`和`mortal`合同的字段中获取ABI。

然后，您可以实例化一个JavaScript对象，该对象可用于在连接到网络的任何计算机上调用合约。替换'ABI'（一个数组）和'Address'（一个字符串）在JavaScript中创建一个契约对象：



    var greeter = eth.contract(ABI).at(Address);


这个特殊的例子可以通过简单的调用来实例化：


```JavaScript
    var greeter2 = eth.contract([{constant:false,inputs:[],name:'kill',outputs:[],type:'function'},{constant:true,inputs:[],name:'greet',outputs:[{name:'',type:'string'}],type:'function'},{inputs:[{name:'_greeting',type:'string'}],type:'constructor'}]).at('greeterAddress');
```

将greeterAddress替换为合同的地址。

提示：如果你的机器上没有正确的安装编译器，你可以从在线编译器获取ABI。为此，请使用下面的代码，仔细地用编译器中的abi替换greeterCompiled.greeter.info.abiDefinition。

#### 清理后自己：

你必须非常高兴才能签下第一份合同，但是当业主继续写下更多的合同时，这种激动有时会消失，导致在区块链上看到放弃的合同。未来，区块链租金可能会实施，以增加区块链的可扩展性，但现在，成为一个好公民，并人道地放弃你的废弃机器人。

交易将需要发送到网络，并在下面的代码运行后支付块链更改的费用。自毁是由网络补贴的，所以它的成本要比平常的交易少得多。



    greeter.kill.sendTransaction({from:eth.accounts[0]})


这只能由合同所有者发送的交易触发。您可以验证该行为是否完成，只要看看是否返回0：



    eth.getCode(greeter.address)


请注意，每个合约都必须执行自己的kill子句。在这种特殊情况下，只有创建合同的帐户才能杀死它。

如果你不添加任何杀人条款，它可能永远独立于你和任何地球上的边界永远活着，所以在你实际使用之前，请检查你的当地法律对此的看法，包括对技术出口的任何可能的限制，任何有关数字众生公民权利的立法。对待你的机器人人道。
