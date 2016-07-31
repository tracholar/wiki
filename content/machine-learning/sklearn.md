---
title: "sklearn - python 机器学习库"
layout: page
date: 2016-07-13
---
[TOC]


## 预处理模块
### `LabelBinarizer`
在one-vs-all里面经常用，将数值或者字符类型的label，转换为一个向量，向量每一维对应其中一个label值，
就像one-hot编码。

```python
>>> from sklearn import preprocessing
>>> lb = preprocessing.LabelBinarizer()
>>> lb.fit([1, 2, 6, 4, 2])
LabelBinarizer(neg_label=0, pos_label=1, sparse_output=False)
>>> lb.classes_
array([1, 2, 4, 6])
>>> lb.transform([1, 6])
array([[1, 0, 0, 0],
       [0, 0, 0, 1]])
```

属性`classes_`保存`fit`后的类列表，提供正变换`transform`，逆变换`inverse_transform`。

### `LabelEncoder`
用来将数值类型或者字符串类型编码为数字 0 到 n_classes-1。

```python
>>> from sklearn import preprocessing
>>> le = preprocessing.LabelEncoder()
>>> le.fit([1, 2, 2, 6])
LabelEncoder()
>>> le.classes_
array([1, 2, 6])
>>> le.transform([1, 1, 2, 6])
array([0, 0, 1, 2]...)
>>> le.inverse_transform([0, 0, 1, 2])
array([1, 1, 2, 6])
```

## 决策树


### 树的可视化
利用`tree.export_graphviz`函数，可以将树导出为图文件。借助于`IPython`可以在notebook里面
将树可视化显示出来。需要安装`graphviz`命令行工具，在mac里面可以通过brew安装，命令是`brew install graphviz`。
如果你在终端里面输入`dot`，没有提示找不到命令，那么就安装好了。

```python
from IPython.display import Image
from StringIO import StringIO
import pydot
from sklearn import tree

def tree2png(clf, cols):
    dot_data = StringIO()
    tree.export_graphviz(clf, out_file=dot_data, feature_names=cols,filled=True, rounded=True,  
                             special_characters=True)
    g = pydot.graph_from_dot_data(dot_data.getvalue())[0]
    return Image(g.create_png())
```

## 特征离散化
从scikit-learn 0.1.7版本开始可以通过树分类器的`.apply`
方法，获取输出叶子节点的序号，然后利用`onehot`转换器转换为
稀疏的离散特征。

利用决策树分类器对特征进行离散化的例子

```python
from sklearn import tree
from sklearn.tree import DecisionTreeClassifier

clf = DecisionTreeClassifier(min_samples_leaf=10)
clf.fit(X_train, Y_train)


from sklearn.preprocessing import OneHotEncoder
onehot = OneHotEncoder()

## 因为决策树apply输出是一个一位数组，所以要reshape
## 如果是randomforest或者gbdt，那么输出的是一个二维数组，
## 每一个决策树有一个输出。这里都是说二分类器。
leaf_numbers = clf.apply(X_train).reshape((-1,1))
onehot.fit(leaf_numbers)
onehot_features = onehot.transform(leaf_numbers)

```
