---
title: "seaborn 绘图工具"
layout: page
date: 2016-07-13
---
[TOC]

## 关于
kaggle 上很多人用这个工具做图，图片还不错，比`ggplot`貌似要更加符合matlab风格。
网站地址 <https://stanford.edu/~mwaskom/software/seaborn>
## 配置
### 主题风格
默认风格已经不错，如果要配置风格可以使用包的`axes_style()`和`set_style()`命令。
五中默认主题是`darkgrid, whitegrid, dark, white, ticks`，默认是`darkgrid`。
关键词汇总有`grid`代表有网格，没有的代表没有网格。

```python
import seaborn as sns
sns.set_style("whitegrid")
data = np.random.normal(size=(20,6)) + np.arange(6) / 2
sns.boxplot(data=data)
```
### 移除上边和右边的边框
对于`white, ticks`主题，通常可以移除上边和右边的边框，通过`sns.despine()`命令就可以了。


### 设置局部式样
使用 `sns.axes_style()` 函数和 `with` 环境。        
```python
with sns.axes_style("darkgrid"):
    plt.subplot(211)
    sinplot()
plt.subplot(212)
sinplot(-1)
```

重写其他配置属性：      
```python
sns.set_style("darkgrid", {"axes.facecolor": ".9"})
```

所有的配置属性可以通过`.axes_style()`，传入空参数得到：     
```python
sns.axes_style()

{'axes.axisbelow': True,
 'axes.edgecolor': '.8',
 'axes.facecolor': 'white',
 'axes.grid': True,
 'axes.labelcolor': '.15',
 'axes.linewidth': 1.0,
 'figure.facecolor': 'white',
 'font.family': [u'sans-serif'],
 'font.sans-serif': [u'Arial',
  u'Liberation Sans',
  u'Bitstream Vera Sans',
  u'sans-serif'],
 'grid.color': '.8',
 'grid.linestyle': u'-',
 'image.cmap': u'Greys',
 'legend.frameon': False,
 'legend.numpoints': 1,
 'legend.scatterpoints': 1,
 'lines.solid_capstyle': u'round',
 'text.color': '.15',
 'xtick.color': '.15',
 'xtick.direction': u'out',
 'xtick.major.size': 0.0,
 'xtick.minor.size': 0.0,
 'ytick.color': '.15',
 'ytick.direction': u'out',
 'ytick.major.size': 0.0,
 'ytick.minor.size': 0.0}
 ```

### 设置绘图上下文
内置四中上下文：`paper, notebook, talk, poster`，默认是`notebook`。
上下文可以通过`sns.set_context('paper')`这种形式进行设置。


`set_context`函数还可以指定两个参数，`font_scale`和`rc`参数，用来指定字体大小和其他运行参数。其他运行参数？
`sns.set_context("notebook", font_scale=1.5, rc={"lines.linewidth": 2.5})`

函数`sns.set()`将所有配置设置为默认情况(传入空参数)，或者更多其他参数，包括`rc`参数。

## Color palettes
`sns.color_palette()`获取和设置color palette
使用hls循环颜色系统，可以指定第一个参数为`hls`，或者使用`hls_palette()`函数。
绘制颜色画板的函数是 `sns.palplot(pal)`。
一种更亮的版本是 husl 系统。

### Color Brewer
类似于color map ？ 好多颜色，先不看了。看不下去了。

### 使用命名好的颜色
xkcd 颜色命名有954中颜色，可以通过字典 `sns.xkcd_rgb` 查看。 使用方式也是通过这个字典。
利用这个字典，可以将颜色名字列表转换为颜色画板。

```python
plt.plot([0, 1], [0, 1], sns.xkcd_rgb["pale red"], lw=3)
plt.plot([0, 1], [0, 2], sns.xkcd_rgb["medium green"], lw=3)
plt.plot([0, 1], [0, 3], sns.xkcd_rgb["denim blue"], lw=3);

colors = ["windows blue", "amber", "greyish", "faded green", "dusty purple"]
sns.palplot(sns.xkcd_palette(colors))
```

交互式颜色选择工具 <http://www.luminoso.com/colors/>

### 线性渐变颜色画板
```python
sns.palplot(sns.color_palette("Blues"))
sns.palplot(sns.color_palette("BuGn_r"))
sns.palplot(sns.color_palette("GnBu_d"))
```

更多画板工具后面再详细地添加吧。


## 可视化数据集
### 绘制分布图 `distplot`
- `distplot()`，它会绘制histogram 直方图，并且会拟合一个 [kernel density estimate (KDE)](https://en.wikipedia.org/wiki/Kernel_density_estimation)。            
```python
x = np.random.normal(size=100)
sns.distplot(x);
```
- Histograms 直方图： 将 `distplot` 的参数设置为 `kde=False` 就可以去掉KDE，`rugplot()`
也可以通过制定参数 `rug=True` 来实现。rug图是像条形码一样的图，用竖线的疏密来表达数据的多少。

- KDE图，将`distplot` 的参数设置为 `hist=False`即可，也可以通过 `.kdeplot()` 来实现。

- fit，制定fit参数

### 两个变量分布图 `jointplot`
- 散点图， `.jointplot(x= 'x', y='y', data=data)`
- 两变量直方图，hexbins plot，添加参数 `kind='hex'`
- 两变量KDE图，`kind="kde"`，也可以通过`kdeplot`
- 高level KDE图， `sns.kdeplot(df.x,df.y, shade=True, n_levels=60)`
- `jointplot` 返回一个 `JoinGrid`

- `.pairplot` 观察两两变量之间的关系。可以指定第三个变量为 `hue`，使得用颜色来区分第三个变量。

### 线性回归图
- `regplot() and lmplot()`，线性回归图，`lmplot()`功能比较强大。
- `lmplot` 的 `order`参数可以指定回归的阶数。
- `robust=True` 可以指定去掉异常值。
- `scatter_kws={"s": 80}` 这个参数可以指定scatter 的属性
- `hue` 指定第三个变量作为颜色画图
- `col` 指定第三个变量作为列分隔
- `row` 指定第三个变量作为行分隔

- 在其他作图上下文做回归图， 指定作图类型为：`kind="reg"`

### category 变量
- `sns.stripplot` 其中一个变量是category类型
- 因为tripplot作图时，数据点会重叠，所以可以指定参数`jitter=True`，增加一点随机偏移，让数据点不重叠
- 另一种防止重叠的方法使用函数 `sns.swarmplot`，会采用防止重叠算法，可以指定第三个变量为`hue`
- `.boxplot()` 会将另一个连续变量的分布也显示出来
- `sns.violinplot` 会将另一个变量的KDE也显示出来
- 用 `scale` 指定第三个变量
- 用 `hue` 指定第三个变量，同时指定 `split=Ture`
- 统计估计 `barplot`
- 对category进行统计 `sns.countplot`，类似于`df.count_values().plot`
- `sns.pointplot` 不知道干啥的

## Grid
- FaceGrid
- PairGrid
