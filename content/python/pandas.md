---
layout: page
title: "Pandas 中的坑"
date: 2015-10-22 20:00
---
[TOC]

## DataFrame 的`index`操作
索引操作多而杂，现总结如下。
参考 [Indexing and Selecting Data](http://pandas.pydata.org/pandas-docs/stable/indexing.html)。
- `.loc`基于label(如果只有一个索引则为行的index，行有限)的索引。
    - 单个标签，如`a`，多个标签列表`['a', 'b', 'c']`。注意，如果提供两个索引，即行索引加列索引，在python中处理为一个tuple。
    - slice object with label，` 'a' : 'f'  `. 这种形式的索引叫做`slice object`。例如 `df.loc['a':'d']`
    - 一个boolean数组. `df.loc[df.A > 0.5]`
    - 一个返回上述索引的单参数（该参数是`df`本身）函数. `df.loc[lambda x: x.A > 0.5 ]`

```python
df1 = pd.DataFrame(np.random.randn(6, 4),
                    index=list('abcdef'),
                    columns=list('ABCD'))

    A	B	C	D
a	0.333368	0.953575	0.189191	0.186499
b	0.344776	0.940556	0.624198	0.278640
c	0.269827	0.449311	0.679678	0.769818
d	0.910729	0.024516	0.745065	0.399805
e	0.868005	0.822731	0.908870	0.376258
f	0.141232	0.983130	0.730339	0.782900

df.loc['a']
df.loc['a', 'B']
df.loc[['a','b']]
df.loc[['a','b'], ['B','C']]
```



- `.iloc` 则是基于序号的索引(还是行优先)，从0到`length - 1`。
    - 一个整数，`df.iloc[5]`, `df.iloc[5,3]`
    - 整数列表，`df.iloc[[2,5,3]]`, `df.iloc[[2,5,3], [3,2]]`
    - slice object with int， `df.iloc[1:3]`, `df.iloc[1:3, :2]`
    - 一个boolean数组， `df.iloc[list(df.A>0.5)]`，注意这里如果写成`df.iloc[df.A > 0.5]`是有问题的。不清楚为啥，可能与`df.A > 0.5`是一个`Series`有关，而`Series`在迭代的时候更像`dict`。
    - 一个返回上述索引的单参数函数。与`.loc`类似。

- `.ix` 则相当于上述两个之和，两种index都能处理。
- `[]`或者`__getitem__()`和上面的行优先相反，他是列优先，而且不能实现多维索引。此外，还可以传入slice object（label和int都可以）和 boolean数组来筛选特定的行
    - 列名label，`df['A']`
    - 列名列表，`df[['A','B']]`
    - slice object，`df[:3]`，`df['a':'c']`
    - boolean array，`df[df.A>0.5]`
- `.head`和`.tail`函数访问前（后）几行
- 以属性方式访问，对DataFrame是列名，对Series是键。例如`df.A`, `df.A.a`
- `.at`,`.iat`和`.get_value`，获取单个值，注意两者的区别。`df.at['a','A'],  df.get_value('a','A')`。他们等价于`df.loc['a','A']`。这个比采用`[]`速度要快，遍历的时候推荐用这个。

## 遍历操作
### 函数式编程方式，apply方式（个人比较推荐这种方式）
- tablewise 函数 `.pipe()`，`.pipe(f, args ...)`将DataFrame作为函数`f`的第一个参数传过去，其他参数也原样传递。

```python
def f(x, a):
    return np.log(x) / np.log(a)
df.pipe(f, 3)  # 等价于 f(df, 3)
```
- 一行一列方式应用函数 `.apply()`，用参数`axis`指定行(取值0)或列(取值1)

```python
df.apply(np.mean)   # 对每一列求均值
df.apply(np.mean, axis=1)  # 对每一行求均值
```
- elementwise `.applymap()`，求每一个元素的ID，`df.applymap(id)`

### iteration 迭代
- `for i in obj` 方式，对不同数据结构不同
    - `Series` : 代表值
    - `DataFrame` : 代表列label，即列名
    - `Panel` : item label

- `.iteriems()`，对DataFrame相当于对列迭代。
    - `Series`: (index, value)
    - `DataFrame` : (column, Series)
    - `Panel` : (item, DataFrame)

- `df.iterrow()`，对DataFrame的每一行进行迭代，返回一个Tuple `(index, Series)`
- `df.itertuples()`，也是一行一行地迭代，返回的是一个namedtuple，通常比`iterrow`快，因为不需要做转换

```python
for idx, row in df.iterrows():
    print idx, row

for row in df.itertuples():
    print row

for c, col in df.iteritems():
    print c, col
```

- `.dt`， 对Datetime类型的Series可以通过这个对象访问`hour, second, day`等值。
  还可以通过`strftime`格式化时间

```python
s = pd.Series(pd.date_range('20130101 09:10:12', periods=4))
s.dt.hour
```


- 迭代方式通常性能较差，可以通过几种方式进行优化。
    - 将运算编程向量化操作
    - 采用`.apply`等函数式编程方式
    - 将操作的内循环等耗费时间的操作用cython编写

### 向量化string操作
通过访问Series的`.str`属性，例如`s.str.lower()`可以将每一个元素变成小写。可以在`.str`属性上使用
所有的字符串函数，就相当于每一个元素使用那样。

## DataFrame的修改

## 可视化plot


## TIPS
- `df.loc['a','A'] = 5`是有效的，但是`df.loc['a']['A'] = 5`对`df`是没有影响的。
