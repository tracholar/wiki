---
title: "合约"
layout: page
date: 2018-02-01
tags: ["区块链"]
---

[TOC]

## 什么是合同？

契约是代码（其功能）和数据（其状态）的集合 位于以太坊区块链上的特定地址。合同账户是 能够在他们自己之间传递消息以及实际上图灵
完成计算。合约在以太坊特有的区块链上生存 二进制格式称为以太坊虚拟机（EVM）字节码。

合同通常用一些高级语言编写，例如Solidity，然后编译成字节码 上传到区块链上。

也可以看看

还有其他的语言，特别是Serpent和LLL，在本文档的以太坊高级语言部分中有进一步描述。

Dapp开发资源列出了集成开发环境，帮助您使用这些语言开发的开发工具，提供测试和部署支持等功能。

## 以太坊高级语言¶

以Ethereum虚拟机（EVM）执行的特定于以太坊的二进制格式（EVM字节码）形式签署合同。然而，契约通常是用更高级别的语言编写的，然后使用EVM编译器将其编译成字节代码以部署到区块链。

以下是开发人员可用于为以太坊编写智能合约的不同高级语言。

### Solidity¶

Solidity是一种类似于JavaScript的语言，它允许您开发合同并编译为EVM字节码。它目前是以太坊的旗舰语言，也是最受欢迎的。

  * Solidity文档 - Solidity是用于编写合同的旗舰Ethereum高级语言。
  * Solidity在线实时编译器
  * 标准化合约API
  * 有用的应用程序模式 - 对于应用程序开发非常有用的代码片段。

### 蛇¶

Serpent是一种类似于Python的语言，可用于开发契约并编译为EVM字节码。它旨在最大限度地简化和清理，将低级语言的许多效率优势与易于使用的编程风格相结合，同时为合约编程添加特定领域特定功能。蛇是使用LLL编译的。

  * 蛇在ethereum维基上
  * 蛇EVM编译器

### LLL¶

Lisp Like Language（LLL）是一种类似于Assembly的低级语言。它意味着非常简单和简约;基本上就是直接在EVM中进行编码的一个小包装。

  * LIBLLL在GitHub
  * LLL的例子

### Mutan（已弃用）¶

Mutan是由Jeffrey Wilcke设计和开发的静态类型C语言。它不再被维护。

## 写一份合同¶

如果没有Hello World程序，没有语言是完整的。在内部操作 以太坊环境下，Solidity没有明显的“输出”字符串的方式。
我们可以做的最接近的是使用日志事件来放置一个字符串 blockchain：



    contract HelloWorld {
            event Print(string out);
            function() { Print("Hello, World!"); }
    }


此合约将在Print类型的区块链上创建一个日志条目 参数“Hello，World！”每次执行。

也可以看看

Solidity文档有更多的例子和指导编写Solidity代码。

## 编译合同¶

可靠性合同的汇编可以通过一些 机制。

  * 通过命令行使用solc编译器。
  * 在JavaScript控制台提供的web3.eth.compile.solidity中使用 geth或eth（这仍然需要solc编译器 安装）。
  * 在线Solidity实时编译器。
  * 流星戴斯科斯莫建设稳固的合同。
  * 混合IDE。
  * 以太坊钱包。

注意

关于solc和编译Solidity合同代码的更多信息可以在这里找到。

### 在geth中设置solidity编译器

如果你启动你的geth节点，你可以检查哪些编译器 可用。



    > web3.eth.getCompilers();
    ["lll", "solidity", "serpent"]


该命令返回一个字符串数组，指示哪些编译器是 目前可用。

注意

solc编译器与cpp-ethereum一起安装。或者， 你可以自己构建它。

如果您的solc可执行文件位于非标准位置，则可以指定一个 使用th - solc标志的solc可执行文件的自定义路径。



    $ geth --solc /usr/local/bin/solc


或者，您可以通过控制台在运行时设置此选项：



    > admin.setSolc("/usr/local/bin/solc")
    solc, the solidity compiler commandline interface
    Version: 0.2.2-02bb315d/.-Darwin/appleclang/JIT linked to libethereum-1.2.0-8007cef0/.-Darwin/appleclang/JIT
    path: /usr/local/bin/solc


### 编译一个简单的合同¶

我们来编一个简单的合同来源：



    > source = "contract test { function multiply(uint a) returns(uint d) { return a * 7; } }"


这个合同提供了一个单一的方法乘以a 正整数a并返回* 7。

您已经准备好使用geth JS控制台来编译solidity代码 eth.compile.solidity（）：



    > contract = eth.compile.solidity(source).test
    {
      code: '605280600c6000396000f3006000357c010000000000000000000000000000000000000000000000000000000090048063c6888fa114602e57005b60376004356041565b8060005260206000f35b6000600782029050604d565b91905056',
      info: {
        language: 'Solidity',
        languageVersion: '0',
        compilerVersion: '0.9.13',
        abiDefinition: [{
          constant: false,
          inputs: [{
            name: 'a',
            type: 'uint256'
          } ],
          name: 'multiply',
          outputs: [{
            name: 'd',
            type: 'uint256'
          } ],
          type: 'function'
        } ],
        userDoc: {
          methods: {
          }
        },
        developerDoc: {
          methods: {
          }
        },
        source: 'contract test { function multiply(uint a) returns(uint d) { return a * 7; } }'
      }
    }


注意

编译器也可以通过RPC，因此通过 web3.js到任何浏览器中的Ðapp连接 通过RPC / IPC进行通信。

以下示例显示了如何通过JSON-RPC将geth连接到 使用编译器。


```
    $ geth --datadir ~/eth/ --loglevel 6 --logtostderr=true --rpc --rpcport 8100 --rpccorsdomain '*' --mine console  2>> ~/eth/eth.log
    $ curl -X POST --data '{"jsonrpc":"2.0","method":"eth_compileSolidity","params":["contract test { function multiply(uint a) returns(uint d) { return a * 7; } }"],"id":1}' http://127.0.0.1:8100
```

一个源的编译器输出将为您提供每个合同对象 代表一份合同。实际的回报值 eth.compile.solidity是契约对象对的契约名称映射。
由于我们的合同名称是test，eth.compile.solidity（source）.test 将会给你包含合同对象的测试合同 以下领域：

码


信息


资源


语言


languageVersion


compilerVersion


abiDefinition

    [Application Binary Interface Definition](https://github.com/ethereum/wiki/wiki/Ethereum-Contract-ABI)
userDoc

    [NatSpec Doc](https://github.com/ethereum/wiki/wiki/Ethereum-Natural-Specification-Format) for users.
developerDoc

    [NatSpec Doc](https://github.com/ethereum/wiki/wiki/Ethereum-Natural-Specification-Format) for developers.

编译器输出的直接结构（编码和信息） 反映了两种截然不同的部署路径。编译的EVM代码 通过合同创建交易发送到区块链
休息（信息）将理想地生活在可公开验证的分散式云上 元数据补充区块链上的代码。

如果您的来源包含多个合同，则输出将包含一个条目 对于每个合同，相应的合同信息对象可以被检索 合同的名称作为属性名称。你可以尝试通过检查
最新的GlobalRegistrar代码：



    contracts = eth.compile.solidity(globalRegistrarSrc)


## 创建和部署一个合同¶

在开始本节之前，请确保您同时拥有一个未锁定的帐户 以及一些资金。

现在，您将通过将事务处理以前一节中的EVM代码作为数据发送到空白地址来在区块链上创建合同。

注意

使用在线Solidity实时可以轻松完成此操作 编译器或Mix IDE程序。



    var primaryAddress = eth.accounts[0]
    var abi = [{ constant: false, inputs: { name: 'a', type: 'uint256' } }]
    var MyContract = eth.contract(abi)
    var contract = MyContract.new(arg1, arg2, ..., {from: primaryAddress, data: evmByteCodeFromPreviousSection})


所有二进制数据都以十六进制格式进行序列化。十六进制字符串总是有一个 十六进制前缀0x。

注意

请注意，arg1，arg2，...是合约的参数 构造函数，以防它接受任何。如果合同不需要任何 构造函数参数，那么这些参数可以省略。

值得指出的是，这一步需要您付费执行。你的 账户余额（您作为发件人在发件人字段中）将会是 一旦交易完成，根据EVM的气体规则减少
成块。过了一段时间，你的交易应该出现在一个 确认它所带来的状态是一个共识。你的合同 现在居住在区块链上。

异步的做法是这样的：



    MyContract.new([arg1, arg2, ...,]{from: primaryAccount, data: evmCode}, function(err, contract) {
      if (!err && contract.address)
        console.log(contract.address);
    });


## 与合同交互¶

与合约的交互通常使用抽象层来完成 作为eth.contract（） 函数，它返回一个javascript对象与所有的合约功能
在JavaScript中可用作可调用函数。

描述合同可用功能的标准方式是ABI 定义。 这个对象是一个描述呼叫签名和返回值的数组 为每个可用的合约功能。



    var Multiply7 = eth.contract(contract.info.abiDefinition);
    var myMultiply7 = Multiply7.at(address);


现在在ABI中指定的所有函数调用都可用 合同实例。您可以在合同实例上调用这些方法 以两种方式之一。



    > myMultiply7.multiply.sendTransaction(3, {from: address})
    "0x12345"
    > myMultiply7.multiply.call(3)
    21


当使用sendTransaction调用时，函数调用通过发送来执行 交易。这将花费乙醚发送和通话将被记录 永远在区块链上。以这种方式进行的呼叫的返回值是
交易的散列。

当使用调用进行调用时，该功能在EVM和本地执行 函数的返回值与函数一起返回。打电话给在这 方式不记录在区块链上，因此不能修改内部
合同的状态。这种呼叫方式被称为常数 函数调用。以这种方式进行的呼叫不会花费任何代价。

如果您只对返回值和使用感兴趣，您应该使用呼叫 sendTransaction如果你只关心对状态的副作用 合同。

在上面的例子中，没有副作用，因此sendTransaction 只燃烧气体，增加宇宙的熵。

## 合约元数据

在前面的章节中，我们解释了你如何创建合同 blockchain。现在我们将处理其余的编译器输出结果 合同元数据或合同信息。

与合同交互时，您并未创建您可能需要的合同 文档或查看源代码。合同作者受到鼓励 通过在区块链上注册来提供这些信息
通过第三方服务，如EtherChain。管理API提供 方便的方法来获取这个包的任何合同，选择 寄存器。



    // get the contract info for contract address to do manual verification
    var info = admin.getContractInfo(address) // lookup, fetch, decode
    var source = info.source;
    var abiDef = info.abiDefinition


使这项工作的基本机制是：

  * 合同信息被上传到可以通过URI识别的地方 是可公开访问的
  * 任何人都可以找出什么是只知道合约的URI 地址

这些要求是通过使用2步区块链注册表来实现的。首先 步骤在合同中将合同代码（哈希）与内容哈希注册 称为HashReg。第二步注册一个带有内容哈希的url
UrlHint合同。这些注册合同 是边疆释放的一部分，并进入了宅基地。

通过使用这种方案，知道合同的地址来查找URL并获取实际合同元数据信息包就足够了。

所以如果你是一个认真的合同创造者，其步骤如下：

  1. 将合约本身部署到区块链
  2. 获取合同信息json文件。
  3. 将合同信息json文件部署到您选择的任何网址
  4. 注册codehash - >内容哈希 - >网址

JS API通过提供帮助程序使这个过程非常简单。呼叫 admin.register从合约中提取信息，写出它的json
序列化在给定的文件中，计算文件的内容哈希值 最后把这个内容哈希注册到合同的代码哈希。一旦您
部署该文件到任何网址，您可以使用admin.registerUrl进行注册 在区块链上的内容哈希链接也是如此。 （注意，如果是
固定内容寻址模型被用作文档存储，url-hint是no 需要更长的时间）


        source = "contract test { function multiply(uint a) returns(uint d) { return a * 7; } }"
    // compile with solc
    contract = eth.compile.solidity(source).test
    // create contract object
    var MyContract = eth.contract(contract.info.abiDefinition)
    // extracts info from contract, save the json serialisation in the given file,
    contenthash = admin.saveInfo(contract.info, "~/dapps/shared/contracts/test/info.json")
    // send off the contract to the blockchain
    MyContract.new({from: primaryAccount, data: contract.code}, function(error, contract){
      if(!error && contract.address) {
        // calculates the content hash and registers it with the code hash in `HashReg`
        // it uses address to send the transaction.
        // returns the content hash that we use to register a url
        admin.register(primaryAccount, contract.address, contenthash)
        // here you deploy ~/dapps/shared/contracts/test/info.json to a url
        admin.registerUrl(primaryAccount, hash, url)
      }
    });


## 测试合同和交易¶

通常你需要采取低级别的测试和调试策略 合同和交易。本节介绍一些调试工具和 您可以使用的做法。为了测试合同和交易没有
真正的结果，你最好在私人区块链上测试它。这可以 通过配置替代网络ID（选择唯一的整数）来实现 和/或禁用对等体。建议练习，为了测试你使用一个
替代数据目录和端口，以便您甚至不会意外冲突 与您的实时运行的节点（假设使用默认运行 你的虚拟机调试模式下的配置文件和最高的日志记录 详细级别建议：



    geth --datadir ~/dapps/testing/00/ --port 30310 --rpcport 8110 --networkid 4567890 --nodiscover --maxpeers 0 --vmdebug --verbosity 6 --pprof --pprofport 6110 console 2>> ~/dapp/testint/00/00.log


在您提交任何交易之前，您需要设置您的私人测试 链。请参阅测试网络。



    // create account. will prompt for password
    personal.newAccount();
    // name your primary account, will often use it
    primary = eth.accounts[0];
    // check your balance (denominated in ether)
    balance = web3.fromWei(eth.getBalance(primary), "ether");



    // assume an existing unlocked primary account
    primary = eth.accounts[0];

    // mine 10 blocks to generate ether

    // starting miner
    miner.start(4);
    // sleep for 10 blocks (this can take quite some time).
    admin.sleepBlocks(10);
    // then stop mining (just not to burn heat in vain)
    miner.stop();
    balance = web3.fromWei(eth.getBalance(primary), "ether");


创建交易后，您可以使用以下几行强制处理它们：



    miner.start(1);
    admin.sleepBlocks(1);
    miner.stop();


您可以使用



    // shows transaction pool
    txpool.status
    // number of pending txs
    eth.getBlockTransactionCount("pending");
    // print all pending txs
    eth.getBlock("pending", true).transactions


如果您提交了合同创建交易，您可以检查所需的代码是否实际插入到当前区块链中：



    txhash = eth.sendTansaction({from:primary, data: code})
    //... mining
    contractaddress = eth.getTransactionReceipt(txhash);
    eth.getCode(contractaddress)
