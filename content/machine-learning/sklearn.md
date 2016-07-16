---
title: "sklearn - python 机器学习库"
layout: page
date: 2016-07-13
---
[TOC]

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
