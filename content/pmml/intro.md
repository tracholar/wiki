---
title: "PMML 预测模型教程"
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

### 基本框架分析
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

`Header`  头部信息，只包含说明信息，对预测逻辑没有影响，通常包括：
    - 包含的属性
        - copyright 版权
        - description 描述文本
        - modelVersion 模型版本
    - 包含的子元素
        - Application 描述生成PMML文件的软件相关信息，本例子说明这个PMML是由 **JPMML-XGBoost** 软件生成的，该软件版本是 **1.2-SNAPSHOT**。
        - Timestamp 生成的时间戳
        - Annotation 可选，描述模型版本更新信息

`DataDictionary` 数据字典，描述字段信息，包括模型的输入字段和输出字段，这里`_target`是输出字段，其他三个是输入字段。每一个字段用 `DataField` 元素描述[ref](http://dmg.org/pmml/v4-3/DataDictionary.html#xsdElement_DataField)。`DataField` 有三个必须的属性：`name` 字段或特征名字，`optype` 操作类型，`dataType` 数据类型。 `DataDictionary` 只负责字段的定义，对字段的处理比如缺失值处理应该在模型的`MiningField` 区域定义。

`optype` 是操作类型，有三个可选值：`categorical` 类别变量，只能进行相等的判断; `ordinal` 序数变量还可以进行顺序比较; `continuous` 只有这个操作类型才能进行算术运算，这种类型的变量应用比较多。

`dataType` 是数据类型，大约有十几种类型[[ref]](http://dmg.org/pmml/v4-3/DataDictionary.html#xsdType_DATATYPE)。


`PMML` 可选的元素有4个，分别是：[ref](http://dmg.org/pmml/v4-3/GeneralStructure.html)。包括 `string`, `interger`, `float`, `double`, `date` 等常见的数据类型。

1. `MiningBuildTask` 可以包含任意XML值，用于表达模型训练时的相关信息，对模型预测没有影响。
2. `TransformationDictionary` 变换字典，用于定义各种变换。
3. `MODEL-ELEMENT` 这是个模型元素集合，用来表达模型的参数和预测逻辑。具体使用时，可以是这个集合里面任意一种元素，在这个例子里面，用得就是 `MiningModel` 这个元素，还可以是`GeneralRegressionModel`等18个元素中的任意一个，可以参看[链接](http://dmg.org/pmml/v4-3/GeneralStructure.html#xsdGroup_MODEL-ELEMENT)。
4. `Extension` 扩展信息

`MODEL-ELEMENT` 大约包括18个不同的模型，每一个模型都有几个相同的属性。`functionName` 用于定义该模型是回归、分类还是聚类等等，是必须的属性。PMML里面一共定义了7种类型[ref](http://dmg.org/pmml/v4-3/GeneralStructure.html)，常用的有回归`regression`、分类`classification`、聚类`clustering`。另外两个可选的属性是：`modelName` 和 `algorithmName`，只用于提示，对模型预测没有实质性影响。
`MODEL-ELEMENT` 都包含了这样一些元素：`MiningSchema`（必须）、 `Output`、 `Targets`等，这些在后面的章节将会详细介绍。

### 模型分析
找到 `MiningModel` 区块，可以看到这个元素的主要结构如下(非主要结构已被省略)：

```xml
<MiningModel functionName="regression" x-mathContext="float">
    <MiningSchema>
        <MiningField name="_target" usageType="target"/>
        <MiningField name="sepal width (cm)"/>
        <MiningField name="petal width (cm)"/>
        <MiningField name="petal length (cm)"/>
    </MiningSchema>
    <Segmentation multipleModelMethod="modelChain">
        <Segment id="1">
            <True/>
            <MiningModel functionName="regression" x-mathContext="float">
                ...
            </MiningModel>
        </Segment>
        <Segment id="2">
            <True/>
            <RegressionModel functionName="regression" normalizationMethod="logit" x-mathContext="float">
                ...
            </RegressionModel>
        </Segment>
    </Segmentation>
</MiningModel>
```

`MiningModel` 实际上是一种通用的模型，通常用于模型的融合。它包含了一个特有子的元素 `Segmentation`，用于融合多个模型。本文的例子里面顶层的模型（即`MiningModel`）包含了两个模型。多个模型的融合方式是`modelChain`，即前一个模型的输出作为后一个模型的输入。融合方式由`Segmentation`的属性`multipleModelMethod`指定。实际上，因为XGBoost用的是回归树，然后将所有树的输出结果相加得到第一个模型的输出；第二个模型只是对第一个模型的输出做了一个简单的`logit`变换，将原始值转换为概率值。

`MiningSchema` 是所有模型都必须有的子元素，包括模型内的子模型也都有。对于所有输入这个模型的数据，都必须经过 `MiningSchema`，这个元素里面包含了这个模型用到的所有字段，相比 `DataDictionary`，`MiningSchema`可以为每个模型定义特有的一些信息，还包括缺失值异常值处理等特征预处理操作。[ref](http://dmg.org/pmml/v4-3/MiningSchema.html)

#### MiningField
每一个字段由`MiningField`定义，它包含以下属性：

- `name` 必须，字段名字
- `usageType` 字段用途，默认是`active`即用作输入特征，值`target` 表示该字段是监督学习模型的目标，也是模型预测结果。其他取值参考[ref](http://dmg.org/pmml/v4-3/MiningSchema.html)
- `optype` 操作类型，一般在数据字典中定义，这里可以重载这个属性
- `outliers` 异常值处理方式：`asIs` `asMissingValues` `asExtremeValues`（极端值通过属性`lowValue`和`highValue`定义）
- `missingValueReplacement` 缺失值替换值，如果有这个属性，在进入模型前先用这个值替换
- `missingValueTreatment` 只是提示替换的值的来源，对PMML预测没有影响

```xml
<MiningField name="foo" missingValueReplacement="3.14" missingValueTreatment="asMean"/>
```

#### Segmentation
多个模型用 `Segmentation` 来组织，每一个模型都被包括在子元素 `Segment` 中。 `Segmentation` 只有一个属性 `multipleModelMethod` 用来表明多个模型的组合方式，可以取得值如下[ref](http://dmg.org/pmml/v4-3/MultipleModels.html#xsdElement_Segmentation)

- `modelChain` 模型链，Predicates的值为TRUE的模型按照顺序打分。模型的输出字段 `OutputFields` 里面的字段，可以作为后续模型的输入。
- `sum` 求和，将多个模型的预测值求和。
- `average` 平均，将多个模型的预测值平均。
- `majorityVote` 投票
- `weightedMajorityVote`, `weightedAverage`, `max`, `median`

每一个`Segment`包含属性`id`和`weight`，`weight`可选属性，在加权融合的情况下才有用。每一个`Segment`包含的子元素有 [`PREDICATE`](http://dmg.org/pmml/v4-3/TreeModel.html#xsdGroup_PREDICATE) 和 [`MODEL-ELEMENT`](http://dmg.org/pmml/v4-3/GeneralStructure.html#xsdGroup_MODEL-ELEMENT)。这个例子中的`PREDICATE`是`<True/>`，表明使用这个模型计算预测值。模型元素有两个，一个是`MiningModel`，另一个是`TreeModel`。

## 输出变换

```xml
<Output>
    <OutputField name="rawResult" dataType="double" feature="predictedValue" />
    <OutputField name="_target" dataType="double" feature="transformedValue" >
        <!--  pow(10, s+0.5) - 1 -->
        <Apply function="round">
            <Apply function="-">
                <Apply function="pow">
                    <Constant dataType="double">10.0</Constant>
                    <Apply function="+">
                        <FieldRef field="rawResult"></FieldRef>
                        <Constant dataType="double">0.5</Constant>
                    </Apply>
                </Apply>
                <Constant dataType="double">1.0</Constant>
            </Apply>
        </Apply>
    </OutputField>
</Output>
```

## 参考
1. https://www.ibm.com/developerworks/cn/opensource/ind-PMML1/index.html
