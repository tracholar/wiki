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
