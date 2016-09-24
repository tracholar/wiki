---
title: "VSM - From Frequency to Meaning: Vector Space Models of Semantics"
layout: page
date: 2016-09-14
---
[TOC]

## 关于
向量空间模型(VSM)综述论文：From Frequency to Meaning: Vector Space Models of Semantics, 2010.

## 导言
- 词的分布式假设（distributional hypothesis ）:

> words that occur in similar contexts tend to have similar meanings (Wittgenstein, 1953; Harris, 1954; Weaver, 1955; Firth, 1957; Deerwester, Dumais, Landauer, Furnas, & Harsh- man, 1990)

- 三类矩阵：term–document, word–context, and pair–pattern matrices
    - event frequencies 而不是 adjacency matrix（基于词典的方法 wordnet）
- 未来工作：新的矩阵，高阶张量！
- 应用（lead algorithm）：
    - measuring semantic relatedness: Pantel & Lin, 2002a; Rapp, 2003; Turney, Littman, Bigham, & Shnayder, 2003.
    - measuring the similarity of semantic relations: Lin & Pantel, 2001; Turney, 2006; Nakov & Hearst, 2008.

**案例**

IQ测试 or 性格测试： subject-item matrix!

向量分析方法：**因子分析**！

向量空间模型里面向量的元素来自于 **事件频率统计**！！

- **Latent Semantic Analysis (LSA)** Deerwester et al., 1990; Lan- dauer & Dumais, 1997
- **Hyperspace Analogue to Language (HAL)** (Lund, Burgess, & Atchley, 1995; Lund & Burgess, 1996)

## 语义向量空间模型
- 假设
    - **statistical semantics hypothesis**: statistical patterns of human word usage can be used to figure out what people mean.
    - **bag of words hypothesis**
    - **distributional hypothesis**
    - **extended distributional hypothesis**
    - **latent relation hypothesis**

### 文档的相似性：The Term–Document Matrix，Salton et al. (1975)
- 行向量对应于一个term，通常是一个词；列向量是一个document，例如一个网页！
- **bag** 是指一个集合，不同的是可以有重复，但是元素的顺序没有意义，不同顺序是等价的！
一个bag可以用一个向量来表示，向量每个元素表示对应的bag元素出现的次数，例如{a,a,b,c,c,c,}可以表示为<2,1,3>.
一系列的bag可以用一个矩阵X表示，矩阵的一列对应于一个bag向量，而一行对应于一个唯一的元素，$(x_{ij})$为第j个bag中元素i出现的次数。
- term-document中，一个文档被表达为一个 bag of word，每一个列向量对应于bag的向量表达。

> In information retrieval, the bag of words hypothesis is that we can estimate the relevance of documents to a query by representing the documents and the query as bags of words. (Salton et al., 1975)

- 矩阵的每一列 $(x_{:j})$代表文档$(d_j)$的一种向量化表达，虽然没有考虑词的顺序、短语、句子等语义结构，但是仍然在搜索引擎中工作的很好！
- 而每一行 $(x_{i})$代表term $(w_i)$ 的一种签名！可以用来度量 term 的相似性！Deerwester et al. (1990)
- 一种解释：the topic of a document will probabilistically influence the author’s choice of words when writing the document. 直接导致LDA模型的出现！

### 词的相似性：The Word–Context Matrix
- 行向量是词，列向量是上下文，context 可以是词、短语、句子、段落、章节、文档等更多可能性
- 上下文可以参考 Sahlgren’s (2006) thesis
- 矩阵相似的行向量代表相似的词！但是主要的上下文通常是其他词！
- 共现频率 Weaver (1955) co-occurrence frequency，用来消歧意

> distributional hypothesis in linguistics is that words that occur in similar contexts tend to have similar meanings (Harris, 1954)

### 关系的相似性：The Pair–Pattern Matrix
- 行向量是词对，例如 mason:stone and carpenter : wood；列向量是词对出现的模式，例如 “X cuts Y ” and “X works with Y ”. Lin and Pantel (2001)，用来判定模式的相似性
- 用来推理，一个句子是另一个句子的解释。
- 行向量：词对的相似性，Turney et al. (2003)

> **extended distributional hypothesis**, that patterns that co-occur with similar pairs tend to have similar meanings. Lin and Pantel (2001)
>
> **The latent relation hypothesis** is that pairs of words that co-occur in similar patterns tend to have similar semantic relations (Turney, 2008a)

关系的相似性不能消减为属性的相似性（word-context matrix)

- 高阶张量：
    - term–document–language third-order tensor：多语言信息检索
    - word–word–pattern tensor：词相似性
    - verb–subject–object tensor：

### Types and Tokens
token-document matrix，里面相同的词但是出现在不同地方的词作为不同的token；
type-duocument matrix，则把相同词合并了。

前者可以用在词消歧义上，一词多义！

问题，这种token-document matrix完全看不出有什么意义啊

### 五个假设
- Statistical semantics hypothesis：词的统计模式可以用来表明含义
- Bag of words hypothesis
- Distributional hypothesis
- Extended distributional hypothesis
- Latent relation hypothesis


## Linguistic Processing for VSM
对数据的预处理：tokenize，normalize（将词不同的形式归一化），annotate the raw text（将相同的形式标记为不同的含义：eg 动词，名词）

Grefenstette (1994)：三步走：tokenization, surface syntactic analysis, and syntactic attribute extraction.

### Tokenization
英语等西班牙语系可以通过天然的分割符空格进行分割！
而汉语等非西班牙语系则不同！

精确的Tokenizer还需要处理标点符号！连字符，multi-word terms（e.g., Barack Obama and ice hockey）。
停止词，高频却无意义的词，代词等。停止词表：SMART system (Salton, 1971)

### Normalization
- case folding
- stemming

一般而言，归一化将导致精确度降低，召回率提高。
如果数据量少，一定要用归一化，提高召回率；
但如果数据量很大，精确度更重要，可以不归一化！

### Annotation
- part-of-speech tagging
- word sense tagging
- parsing

降低召回率，提高精确度！


## Mathematical Processing for Vector Space Models
Lowe (2001) 4步走：1、统计频率，2、频率变换（加权），3、平滑，4、计算相似性。

### 频率统计
关键技术：Hash Table；数据库；搜索引擎索引。

### 加权频率变换
- TF-IDF 用倒文档频率作为权值
- 文档长度：因为相同的情况下，长文档更容易被匹配到！因为词多！
- term 的权重，两个很相近的词同时出现在一个文档中，除了可以将他们归一化到同一个词，也可以减少他们的权重！
- 特征选择也可以看做一种加权手段：Forman (2003)
- Pointwise Mutual Information（PMI，Church & Hanks, 1989; Turney, 2001）
- Positive PMI（PPMI）：将PMI小于0的值置0！当用word-context矩阵度量语义相似性地时候，效果更好！

假设word-context 矩阵 F，行向量$(f_i)$，列向量$(f_{:j})$。新矩阵 X 是PPMI矩阵，定义为

$$
p_{ij} = \frac{f_{ij}}{\sum_i \sum_j f_{ij}}    \\\\
p_{i\*} = \frac{\sum_j f_{ij}}{\sum_i \sum_j f_{ij}}    \\\\
p_{\* j} = \frac{\sum_i f_{ij}}{\sum_i \sum_j f_{ij}}    \\\\
pmi_{ij} = \log\left( \frac{p_{ij}}{p_{i\*} p_{\* j}}  \right)    \\\\
x_{ij} = \begin{cases}
    pmi_{ij} & if pmi_{ij} > 0 \\\\
    0    &  \text{otherwise}
\end{cases}
$$

PMI 的问题，对小概率事件有偏！特例：当i和j统计依赖，$(p_{ij} = p_{i\*} = p_{\* j})$，
那么PMI变为 $(\log(1/p_{i\*}))$。

一种解决方案是（Pantel & Lin, 2002a），对$(f_{ij}, f_{i\*}, f_{\* j})$进行平滑处理

$$
\delta_{ij} = \frac{f_{ij}}{f_{ij} + 1} \frac{\min(f_{\* j}, f_{i\*})}{\min(f_{\* j}, f_{i\*}) + 1}  \\
newpmi_{ij} = \delta_{ij} pmi_{ij}
$$

另一种解决方案是对概率进行拉普拉斯平滑！即对每一个$(f_{ij} \rightarrow f_{ij} + k)$。

### 平滑矩阵
- 限制向量成分：只保留PMI超过某个阈值的项，其他置0.
- truncated SVD：应用到document similarity就是 Latent Semantic Indexing (LSI)；应用到 word similarity 就是 Latent Semantic Analysis (LSA)

rank-k 矩阵近似，最小化富比尼范数$(||X - \hat{X}||\_F)$！(Golub&VanLoan,1996)

- Latent Meaning: Deerwester et al. (1990) and Landauer and Dumais (1997) ，认为k个最大的奇异值是隐层语义，对应的两个矩阵分别代表term和document与不同隐变量的相关度。
- Noise reduction：是对矩阵X的平滑！Rapp (2003)
- High-order co-occurrence: Landauer and Dumais (1997)，Lemaire and Denhiere (2006)
- Sparsity reduction：类似于矩阵补全！

- SVD实现：
    - svdlibc:<http://tedlab.mit.edu/~dr/svdlibc/>. Rohde
    - Brand’s (2006)
    - Gorrell’s (2006)

- 高阶张量类似算法：parallel factor analysis，canonical decomposition，Tucker decomposition
- 其他平滑方法：
    -  Nonnegative Matrix Factorization (NMF) (Lee & Seung, 1999), Probabilistic Latent Semantic Indexing (PLSI) (Hofmann, 1999), Iter- ative Scaling (IS) (Ando, 2000), Kernel Principal Components Analysis (KPCA) (Scholkopf, Smola, & Muller, 1997), Latent Dirichlet Allocation (LDA) (Blei et al., 2003), and Discrete Component Analysis (DCA) (Buntine & Jakulin, 2006).
- SVD 隐含地假设词频是高斯分布，然而并不是，PMI比PPMI更接近高斯分布！

### 比较向量
- 向量夹角余弦值！
- 距离的度量可以转换为相似度
- 距离度量：欧式距离，曼哈顿距离，**Hellinger, Bhattacharya,** and **Kullback-Leibler**
- 在 Bullinaria and Levy (2007) 试验中，余弦相似度效果最好！
- 其他度量：Dice and Jaccard coe cients (Manning et al., 2008).


三类：Weeds et al. (2004)

1. high-frequency sensitive measures (cosine, Jensen-Shannon, $(\alpha)$-skew, recall),
2. low-frequency sensitive measures (precision), and
3. similar-frequency sensitive methods (Jaccard, Jaccard+MI, Lin, harmonic mean).

### 有效的比较
- 稀疏矩阵乘法优化
- 将低于阈值的项减为0，也可以极大的减少计算量
- 分布式实现：MapReduce，Elsayed, Lin, and Oard (2008)
- 随机算法：
    - random indexing
    - Locality sensitive hashing（LSH）

## 3个开源VSM系统

- Term-Document Matrix： Lucene. 结合 Nutch，Solr可以做一个搜索系统了！

- Word–Context Matrix: Semantic Vectors

- Pair–Pattern Matrix: Latent Relational Analysis in S-Space

## 应用
- Term–Document Matrices：
    - 文档检索，跨语言检索：截断SVD可以提高精度和召回！！！问题在于要解决大规模问题的计算量！其他技巧有协同过滤和PageRank
    - 文档聚类
    - 文档分类：主题，语义，垃圾邮件
    - 文章自动打分
    - 文档分割：将文档分割为几个不同的主题
    - QA 问答系统
    - Call routing，客服？
- Word–Context Matrices：
    - 词相似性：TOEFL
    - 词聚类
    - 词分类
    - 词典自动生成
    - 词消歧义
    - 上下文评写纠错
    - 查询扩展：搜索引擎扩展查询词为相近的词：使用 session 上下文和click 上下文
    - 文本广告：点击付费广告：bidterm 扩展
    - 信息提取（**I** nformation **E** xtraction): 名字实体识别（NER），relation extraction, event extraction, and fact extraction
- Pair–Pattern Matrices
    - 关系相似性
    - 模式相似性
    - 关系聚类
    - 关系分类
    - 关系搜索
    - 自动词典生成
    - Analogical mapping：SAT测试 a:b::c:d

## 其他方法 to 语义分析
- 概率语言模型
- 词典：图

## VSM的未来
- 批评：没有考虑词的顺序
- 80%的含义来自于词！！？Landauer (2002)


## 问题
- **随机投影**
- **LSH** SIMIHash等
