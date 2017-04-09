---
title: "文本摘要相关算法汇总"
layout: page
date: 2017-03-27
---
[TOC]

## 关于
汇总文本摘要相关的模型、算法即评估指标

## 关键短语提取：review
论文：Automatic Keyphrase Extraction: A Survey of the State of the Art，Kazi Saidul Hasan and Vincent Ng

- 定义：自动选择出文档中的重要的、表达主题的短语！确定文档中最有表达能力的少量关键词！
    - Peter Turney. 2000. Learning algorithms for keyphrase extraction. Information Retrieval, 2:303–336.
    - Takashi Tomokiyo and Matthew Hurst. 2003. A lan- guage model approach to keyphrase extraction. In Proceedings of the ACL Workshop on Multiword Ex- pressions, pages 33–40.
    - Zhiyuan Liu, Peng Li, Yabin Zheng, and Maosong Sun. 2009b. Clustering to find exemplar terms for keyphrase extraction. In Proceedings of the 2009 Conference on Empirical Methods in Natural Lan- guage Processing, pages 257–266.
    - Zhuoye Ding, Qi Zhang, and Xuanjing Huang. 2011. Keyphrase extraction from online news using binary integer programming. In Proceedings of the 5th In- ternational Joint Conference on Natural Language Processing, pages 165–173.
    - Xin Zhao, Jing Jiang, Jing He, Yang Song, Palakorn Achanauparp, Ee-Peng Lim, and Xiaoming Li. 2011. Topical keyphrase extraction from Twitter. In Proceedings of the 49th Annual Meeting of the Association for Computational Linguistics: Human Language Technologies, pages 379–388.
    - Zhiyuan Liu, Wenyi Huang, Yabin Zheng, and Maosong Sun. 2010. Automatic keyphrase extrac- tion via topic decomposition. In Proceedings of the 2010 Conference on Empirical Methods in Natural Language Processing, pages 366–376.

- 应用：文档快速、高精度的检索；提升自然语言处理的其他任务：文本摘要，文本分类，观点挖掘，文档索引；
    - Yongzheng Zhang, Nur Zincir-Heywood, and Evangelos Milios. 2004. World Wide Web site summariza- tion. Web Intelligence and Agent Systems, 2:39–53.
    - Ga ́bor Berend. 2011. Opinion expression mining by exploiting keyphrase extraction. In Proceedings of the 5th International Joint Conference on Natural Language Processing, pages 1162–1170.

- 影响关键词提取的几个主要因素：
    - 长度：长文档的候选词更多
    - 结构一致性：科技文档的结构非常一致，可以利用abstract提取关键词！
    - 主题的变化：科技文档的主题在同一个文档中基本不变，但是对话则经常随时间变化！
    - 主题想关心：非正式文档的多个主题可能并不相关。

- 关键词提取方法：
    - 利用一些启发式方法提取一个关键词列表
    - 利用监督或者无监督学习确定一个关键词是否是正确的关键词

- 候选词选择：
    - 启发式的规则，减少候选词数目
    - 停止词列表：移除停止词
    - 保留特定词性的词：名词、形容词、动词etc
    - 利用其它信息：允许维基百科词条的 n-gram
    - 保留满足特定词法模式的 n-gram
    - 其它减枝技术
- 监督学习方法：Task Reformulation，feature design
- Task Reformulation：
    - 二分类标注：给定一个关键词和文档，预测该关键词是否是该文档的关键词；缺点是不能确定哪些词更有表达能力！
        - Peter Turney. 1999. Learning to extract keyphrases from text. National Research Council Canada, In- stitute for Information Technology, Technical Report ERB-1057.
        - Peter Turney. 2000. Learning algorithms for keyphrase extraction. Information Retrieval, 2:303–336.
        - Ian H. Witten, Gordon W. Paynter, Eibe Frank, Carl Gutwin, and Craig G. Nevill-Manning. 1999. **KEA**: Practical automatic keyphrase extraction. In Pro- ceedings of the 4th ACM Conference on Digital Li- braries, pages 254–255.
    - 排序方法：pairwise的排序方法效果明显优于二分类的方法 **KEA**！
        - Xin Jiang, Yunhua Hu, and Hang Li. 2009. A ranking approach to keyphrase extraction. In Proceed- ings of the 32nd International ACM SIGIR Confer- ence on Research and Development in Information Retrieval, pages 756–757.
- 特征设计：1. 文档内特征；2. 文档外特征
    - 统计特征：
        1. tf*idf,不解释
        2. 第一次出现的位置距文档开头的归一化距离；通常关键词出现在文档的头部
        3. 短语在训练集中作为关键词的次数
        4. 其他统计信息：短语长度，短语跨度（第一次出现和最后一次出现的距离）
    - 结构特征：短语出现在科技文档不同章节的频率，出现在网页metadata的频率etc
    - 句法特征：当有其他特征时，这类特征没啥用
        - 将短语编码为 POS 序列，例如编码为 动词-名词；形容词-名词 etc
        - 词法后缀序列，貌似只有拉丁语系才有， full-tion, less-tion etc
    - 维基百科类特征：是否作为维基百科词条？etc
    - 是否作为搜索关键词？
    - 两个候选词的语义相关性特征！

- 无监督学习方法：
    - Graph-Based Ranking:
        - 一个词是重要的：1，与大量其他候选词是相关的；2，候选词是重要的！
        - 词的关系通过共生矩阵来描述（实际上现在可用过词向量来描述啦）
        - 一个文档的词用一个图来描述，图的节点是词，边的权重是词的关系。一个节点的score由他的邻居的score决定！选出TOP个节点即可！
        - TextRank：Rada Mihalcea and Paul Tarau. 2004. TextRank: Bringing order into texts. In Proceedings of the 2004 Conference on Empirical Methods in Natural Language Processing, pages 404–411.
        - 缺点在于选取的词无法覆盖文档的全部主要信息！
    - Topic-Based Clustering
        - 将候选词按主题聚类
        - KeyCluster: 利用维基百科和共生矩阵聚类相似的词，对每一个类（主题）选出最靠近中心的词！缺点在于并不是所有的主题都重要！这种方法给每一个主题相同的权重！
        - Topical PageRank：利用textrank对每个主题内的词排序，词的最终score是在各个主题中的score的加权和，权重是该主题在文档中的概率！
        - CommunityCluster：保留重要主题下的所有候选词！
    - Simultaneous Learning
    - Language Modeling
- 评估
    - 典型方法：
        1. to create a mapping between the keyphrases in the gold standard and those in the system output using exact match
        2. score the output using evaluation metrics such as precision (P), recall (R), and F-score (F).
    - BLEU，METEOR, NIST, and ROUGE 解决精确匹配的问题
    - R-precision

## Keyword Extraction from a Single Document using Word Co-occurrence Statistical Information
论文：Keyword Extraction from a Single Document using Word Co-occurrence Statistical Information，Y. MATSUO，M. Ishizuka，2003

基本思想：只需要单个文档（长文档），首先提取高频词，如果一个词与高频词的共现关系通过卡方检验，就认为是可能的关键词。

TFIDF：在该文档经常出现，但是在整个语料中出现得不那么频繁的词！

- automatic term recognition，automatic indexing，automatic keyword extraction
- 本文的方法只需要一篇文档，不需要语料
    1. 选出高频词，统计高频词出现的频率
    2. 统计词与高频词的共现矩阵
- 如果一个词经常与一个高频词子集共现，那么这个词就有高的概率是关键词！这种偏差用卡方统计量来度量
- 如果一个词w跟高频词的任何子集都没有特殊的共现关系，那么共现矩阵中w的分布期望值应该就是高频词本身的分布。
反之，则实际分布与这种期望分布存在较大偏差，可以用卡方统计量度量这种偏差。

$$
\chi^2(w) = \sum_{g \in G} \frac{(freq(w, g) - n_w p_g)^2}{n_w p_g}
$$

这里w是某个待检验的词，$(g \in G)$ 是高频词，G是高频词组成的集合。$(n_w)$是w在共现矩阵中出现的总数，
$(p_g)$是高频词g在高频词中的归一化频率。

- 优化：
    - 针对长短句不同带来的共现偏差，重新定义$(p_g)$为 g出现的句子中词的数目/文档的总词数目; $(n_w)$定义为w出现过的句子中的总词数目！
    - 增加鲁棒性，防止某个词只跟某一个特定的高频词高度共现，方法是减去这个高度共现的部分，即最大值。

$$
\chi'^2(w) = \chi^2(w) - \max_{g \in G} \frac{(freq(w, g) - n_w p_g)^2}{n_w p_g}
$$

- 词聚类：将高频词相似的词聚类！词的相似度基于 [Jensen-Shannon divergence](https://en.wikipedia.org/wiki/Jensen%E2%80%93Shannon_divergence) 度量！然后采用pairwise聚类，利用交互信息量度量。

## TextRank
论文：TextRank: Bringing Order into Texts，Rada Mihalcea and Paul Tarau，2004

带权 PageRank

$$
WS(V_i) = (1-d) + d \sum_{V_j \in IN(V_i)} \frac{w_{ji}}{\sum_{V_k \in OUT(V_j)} w_{jk}} WS(V_j)
$$

WS 是定点的 PageRank score。随机初始化，然后迭代收敛！

- 将文本作为一个图：每一个词做顶点，如果两个词出现在同一个上下文窗（大小认为设定，试验中窗大小为2比较好），那么就有一条边。只使用形容词和名词！
- 多个词通过后处理得到，例如两个词A，B都在TOPN中，并且这两个词相邻，那么AB就是一个新的关键词
- 关键词评估指标，P，R，F1
- 句子的相似性通过公共词数目定义，评估指标 ROUGE

$$
similarity(S_i, S_j) = \frac{|\\{w_k| w_k \in S_i , w_k \in S_j \\}|}{\log{|S_i|} + \log{|S_j|}}
$$

## KeyCluster
论文：Clustering to Find Exemplar Terms for Keyphrase Extraction，Zhiyuan Liu, Wenyi Huang, Yabin Zheng and Maosong Sun

- 关键词的几个目标：可解释性；相关性；对主题的覆盖率
- 无监督聚类方法：
    - 首先将文档的 term 按照语义聚类，每一类用一个代表 term 表达，每一个类的中心 term
- 算法流程：
    - 候选词选择：过滤停止词等无意义词
    - 计算 term 间的语义相关性度量：Wikipedia的tfidf，pmi，ngd
    - 基于相关性将term聚类：Hierarchical Clustering，Spectral Clustering，Affinity Propagation
    - 使用每个类的代表词提取关键词：选出 代表词 的组合短语作为关键词

## Topical PageRank
论文：Automatic Keyphrase Extraction via Topic Decomposition

基本思想，pagerank的时候，只关注某一个主题，求出每个term在该主题先的rank后，然后按照文档的主题分布加权得到最终的rank。
