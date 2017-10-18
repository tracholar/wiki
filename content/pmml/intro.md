---
title: "PMML 预测模型"
layout: page
date: 2016-07-21
---
[TOC]

## 什么是PMML
PMML 是一种基于XML的标准语言，用于表达数据挖掘模型，可以用来在不同的应用程序中交换模型。
一种非常有用的应用场景是在生产环境中部署用各种建模工具训练出来的模型。
目前最新的标准是4.3 <http://dmg.org/pmml/pmml-v4-3.html>。

PMML 文件的结构遵从了用于构建预测解决方案的常用步骤，包括：

1. 数据词典，这是一种数据分析阶段的产品，可以识别和定义哪些输入数据字段对于解决眼前的问题是最有用的。这可以包括数值、顺序和分类字段。
2. 挖掘架构，定义了处理缺少值和离群值的策略。这非常有用，因为通常情况，当将模型应用于实践时，所需的输入数据字段可能为空或者被误呈现。
3. 数据转换，定义了将原始输入数据预处理至派生字段所需的计算。派生字段（有时也称为特征检测器）对输入字段进行合并或修改，以获取更多相关信息。例如，为了预测停车所需的制动压力，一个预测模型可能将室外温度和水的存在（是否在下雨？）作为原始数据。派生字段可能会将这两个字段结合起来，以探测路上是否结冰。然后结冰字段被作为模型的直接输入来预测停车所需的制动压力。
4. 模型定义，定义了用于构建模型的结构和参数。PMML 涵盖了多种统计技术。例如，为了呈现一个神经网络，它定义了所有的神经层和神经元之间的连接权重。对于一个决策树来说，它定义了所有树节点及简单和复合谓语。
5. 输出，定义了预期模型输出。对于一个分类任务来说，输出可以包括预测类及与所有可能类相关的概率。
6. 目标，定义了应用于模型输出的后处理步骤。对于一个回归任务来说，此步骤支持将输出转变为人们很容易就可以理解的分数（预测结果）。
7. 模型解释，定义了将测试数据传递至模型时获得的性能度量标准（与训练数据相对）。这些度量标准包括字段相关性、混淆矩阵、增益图及接收者操作特征（ROC）曲线图。
8. 模型验证，定义了一个包含输入数据记录和预期模型输出的示例集。这是非常重要的一个步骤，因为在应用程序之间移动模型时，该模型需要通过匹配测试。这样就可以确保，在呈现相同的输入时，新系统可以生成与旧系统同样的输出。如果实际情况是这样的话，一个模型将被认为经过了验证，且随时可用于实践。

一个通用的PMML文件结构如下(参考<http://dmg.org/pmml/v4-3/GeneralStructure.html>)：

```xml
<?xml version="1.0"?>
<PMML version="4.3"
  xmlns="http://www.dmg.org/PMML-4_3"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">

  <Header copyright="Example.com"/>
  <DataDictionary> ... </DataDictionary>

  ... a model ...

</PMML>
```

## 使用PMML发布预测模型
下面以目前应用广泛的 XGBoost 模型为例，介绍使用PMML发布预测模型。

首先，我们需要有一个XGBoost模型，为此，可以以Iris数据集训练一个简单的二分类模型（只用其中的两类）。 然后利用 XGBoost 训练得到模型文件。


```python
import xgboost as xgb
from sklearn.datasets import load_iris

iris = load_iris()
mask = iris.target < 2
X = iris.data[mask,:]
y = iris.target[mask]

params = {
    'objective' : 'reg:logistic',
    'num_round' : 10,
    'max_depth' : 3
}
dtrain = xgb.DMatrix(X, label=y)
evallist = [(dtrain, 'train')]
bst = xgb.train(params, dtrain, evals=evallist)
bst.save_model('xgb.bin')
```

    [0]	train-rmse:0.364576
    [1]	train-rmse:0.27088
    [2]	train-rmse:0.203626
    [3]	train-rmse:0.154579
    [4]	train-rmse:0.118482
    [5]	train-rmse:0.091745
    [6]	train-rmse:0.071832
    [7]	train-rmse:0.056919
    [8]	train-rmse:0.045683
    [9]	train-rmse:0.037156

然后生成一个特征映射文件，因为模型文件中没有特征名，只有特征id。

```python
f = open('fmap.txt', 'w')
for i, fn in enumerate(iris.feature_names):
    f.write('%d\t%s\t%s\n' % (i, fn, 'q'))
f.close()
```

利用 jpmml-xgboost 项目<https://github.com/jpmml/jpmml-xgboost>提供的工具，进行转换。
你也可以直接下载我已经[编译好的jar包](https://github.com/tracholar/wiki/raw/master/src/machine-learning/converter-executable-1.2-SNAPSHOT.jar)。
然后执行下述命令，即可得到转换后的PMML文件 xgb.pmml.xml。

```bash
!java -jar converter-executable-1.2-SNAPSHOT.jar --model-input xgb.bin  --fmap-input fmap.txt  --pmml-output xgb.pmml.xml
```

得到PMML文件xgb.pmml.xml后，我们就可以在生产环境部署了。
PMML模型的部署可以使用 <https://github.com/jpmml/jpmml-evaluator> 进行部署，
可以很容易应用到分布式环境！

下面是一段预测的代码：

```java
InputStream is = new FileInputStream("path-to-pmml-file");
PMML pmml = PMMLUtil.unmarshal(is);

// 这里的LocatorTransformer是为了将模型转换为可序列化的对象，如果不需要在分布式环境(如Spark)使用模型，就可以不用转换
LocatorTransformer lt = new LocatorTransformer();
lt.applyTo(pmml);

Evaluator evaluator = ModelEvaluatorFactory.newInstance().newModelEvaluator(pmml);

// 预测
List<InputField> fields = evaluator.getActiveFields();

Map<FieldName, Double> input = new HashMap<>();
for(InputField field : fields){
    input.put(field.getName(), 1.2); //对每一个特征指定对应的值
}
Map<FieldName, ?> results = evaluator.evaluate(input);
List<TargetField> output = evaluator.getTargetFields();
Object value = results.get(output.get(0).getName());
```

## PMML文件分析
打开 xgb.pmml.xml 文件，我们可以看到一个实际可用的PMML文件结构。

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<PMML xmlns="http://www.dmg.org/PMML-4_3" version="4.3">
	<Header>
		<Application name="JPMML-XGBoost" version="1.2-SNAPSHOT"/>
		<Timestamp>2017-10-17T03:41:51Z</Timestamp>
	</Header>
	<DataDictionary>
        <DataField name="_target" optype="continuous" dataType="float"/>
		<DataField name="sepal width (cm)" optype="continuous" dataType="float"/>
		<DataField name="petal length (cm)" optype="continuous" dataType="float"/>
		<DataField name="petal width (cm)" optype="continuous" dataType="float"/>
	</DataDictionary>
	<MiningModel functionName="regression" x-mathContext="float">
        ...
    </MiningModel>
</PMML>

```
PMML文件是基于XML格式的文本文件，有且只有一个根节点`PMML`。
`PMML` 根节点除了 xmlns 属性外，有且只有一个属性 `version`，它的值表明PMML标准的版本。

`PMML` 子元素有两个是必须的，`Header` 和 `DataDictionary`。

1. `Header`  头部信息，只包含说明信息，对预测逻辑没有影响，通常包括：
    - 包含的属性
        - copyright 版权
        - description 描述文本
        - modelVersion 模型版本
    - 包含的子元素
        - Application 描述生成PMML文件的软件相关信息，本例子说明这个PMML是由 **JPMML-XGBoost** 软件生成的，该软件版本是 **1.2-SNAPSHOT**。
        - Timestamp 生成的时间戳
        - Annotation 可选，描述模型版本更新信息
2. `DataDictionary` 数据字典，描述字段信息，包括模型的输入字段和输出字段，这里`_target`是输出字段，其他三个是输入字段。
   每一个字段用 `DataField` 元素描述[ref](http://dmg.org/pmml/v4-3/DataDictionary.html#xsdElement_DataField)。
   `DataField`

`PMML` 可选的元素有4个，分别是：[ref](http://dmg.org/pmml/v4-3/GeneralStructure.html)

1. `MiningBuildTask` 可以包含任意XML值，用于表达模型训练时的相关信息，对模型预测没有影响。
2. `TransformationDictionary`
3. `MODEL-ELEMENT` 这是个模型元素集合，用来表达模型的参数和预测逻辑。具体使用时，可以是这个集合里面任意一种元素，在这个例子里面，用得就是 `MiningModel` 这个元素，还可以是`GeneralRegressionModel`等18个元素中的任意一个，可以参看[链接](http://dmg.org/pmml/v4-3/GeneralStructure.html#xsdGroup_MODEL-ELEMENT)。
4. `Extension`


## 参考
1. https://www.ibm.com/developerworks/cn/opensource/ind-PMML1/index.html
