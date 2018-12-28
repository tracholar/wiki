---
title: "【KDD2018-Airbnb】Real-time Personalization using Embeddings for Search Ranking at Airbnb"
layout: page
date: 2018-12-23
---
[TOC]

## 摘要 & 导言
- Airbnb 问题的特殊性
    - 是一个双边市场,既需要考虑考虑买家的体验,还需要考虑卖家的体验
    - 用户很少有复购行为
- 99%的转化来自于实时搜索排序和推荐
- 方法:pairwise regression, 下单的作为正例,拒绝的作为负例 
    - lambda rank
    - 联合优化买家和卖家的排序 ??
    - 利用listing embeddings, low-dimensional vector representations计算用户交互的list与候选list的相似性,作为个性化特征放到排序模型中
- 利用点击等行为学习短期兴趣;而利用下单学习长期兴趣
    - 问题: 下单行为过于稀疏, 一个用户平均一年旅行1-2次; 长尾用户只有一次下单
    - 解决方法: 不是对userid做embedding,而是在用户类型维度做embedding,类型通过多对一的规则映射得到
- 论文创新点:
    - 实时个性化
    
    
- 相关论文:
    - Yahoo
        1. MihajloGrbovic,NemanjaDjuric,VladanRadosavljevic,FabrizioSilvestri,Ri-cardo Baeza-Yates, Andrew Feng, 
           Erik Ordentlich, Lee Yang, and Gavin Owens. 2016. Scalable semantic matching of queries to ads in sponsored 
           search advertis- ing. In SIGIR 2016. ACM, 375–384.
        2. MihajloGrbovic,VladanRadosavljevic,NemanjaDjuric,NarayanBhamidipati, Jaikit Savla, Varun Bhagwan, and Doug Sharp. 2015. 
           E-commerce in your inbox: Product recommendations at scale. In Proceedings of the 21th ACM SIGKDD 
           International Conference on Knowledge Discovery and Data Mining.
        3. Dawei Yin, Yuening Hu, Jiliang Tang, Tim Daly, Mianwei Zhou, Hua Ouyang, Jianhui Chen, Changsung Kang, 
           Hongbo Deng, Chikashi Nobata, et al. 2016. Ranking relevance in yahoo search. In Proceedings of the 22nd ACM SIGKDD.
    - Etsy
        1. Kamelia Aryafar, Devin Guillory, and Liangjie Hong. 2016. An Ensemble-based Approach to Click-Through 
           Rate Prediction for Promoted Listings at Etsy. In arXiv preprint arXiv:1711.01377.
    - Criteo
        1. Thomas Nedelec, Elena Smirnova, and Flavian Vasile. 2017. Specializing Joint Representations for the task 
           of Product Recommendation. arXiv preprint arXiv:1706.07625 (2017).
    - Linkedin
        1. Benjamin Le. 2017. Deep Learning for Personalized Search and Recommender Systems. In Slideshare:
           <https://www.slideshare.net/BenjaminLe4/deep-learning-for-personalized-search-and-recommender-systems>.
        2. Thomas Schmitt, François Gonard, Philippe Caillou, and Michèle Sebag. 2017. Language Modelling for 
           Collaborative Filtering: Application to Job Applicant Matching. In IEEE International Conference on 
           Tools with Artificial Intelligence.
    - Tinder
        1. SteveLiu.2017.PersonalizedRecommendationsatTinder:TheTinVecApproach. In Slideshare: 
           <https://www.slideshare.net/SessionsEvents/dr-steve-liu-chief-scientist-tinder-at-mlconf-sf-2017>.
    - Tumblr
        1. MihajloGrbovic,VladanRadosavljevic,NemanjaDjuric,NarayanBhamidipati, and Ananth Nagarajan. 2015. 
           Gender and interest targeting for sponsored post advertising at tumblr. In Proceedings of the 21th 
           ACM SIGKDD International Conference on Knowledge Discovery and Data Mining. ACM, 1819–1828.
    - Instacart
        1. Sharath Rao. 2017. Learned Embeddings for Search at Instacart. In Slideshare: 
           <https://www.slideshare.net/SharathRao6/learned-embeddings-for-search-and-discovery-at-instacart>.
    - Facebook
        1. Ledell Wu, Adam Fisch, Sumit Chopra, Keith Adams, Antoine Bordes, and Jason Weston. 2017. 
           StarSpace: Embed All The Things! arXiv preprint arXiv:1709.03856.
           
## METHODOLOGY
- listing embedding 短期实时个性化
- user-type & listing type embeddings 长期个性化

### Listing Embeddings(List指一个项目?还是列表?)
- session定义: $( s = (l_1, ..., l_M) )$ 非中断点击序列; 新的session定义为相继两次点击事件相隔30分钟以上。
- skip-gram模型: 损失函数

$$
\mathbf{L} = \sum_{s\in S}\sum_{l_i \in s} \sum_{-m \le j \le m, i \ne 0} P(l_{i+j} | l_i)
$$

利用负采样近似, 正例集合 $((l, c) \in D_p)$ l,c在同一个上下文; 负例集合 $( (l, c) \in D_n )$

$$
\arg\max_{\theta} \sum_{(l, c) \in D_p} \log \frac{1}{1 + e^{- v' _ c v_l}} + \sum_{(l, c) \in D_n} \log \frac{1}{1 + e^{v' _ c v_l}} 
$$


- 将订购的listing作为全局上下文: 
    1. booking sessions 最后有订购
    2. exploratory sessions
- 对于 booking session, 认为最终订购的listing跟之前点击的每一个listing都有相关性,所以可以将它作为之前点击的每一个listing的全局上下文

$$
\arg\max_{\theta} \sum_{(l, c) \in D_p} \log \frac{1}{1 + e^{- v' _ c v_l}} + \sum_{(l, c) \in D_n} \log \frac{1}{1 + e^{v' _ c v_l}}  +  \sum_{l} \log \frac{1}{1 + e^{- v' _ b v_l}}
$$

$( v_b )$ 是订购的listing

相当于增加一些正例!

- 在搜索场景性,用户看到的和搜索的大多是同一个小市场/区域中的房屋,所以正例都是在同一个市场中的房屋对,而负例都是随机采样的,所以负例对大多不在同一个市场中。
  这导致模型难以学到市场内的相似性差异,所以可以额外加入一些同一个市场中的负例对。

$$
\sum_{(l, m_n) \in D_m_n} \log \frac{1}{1 + e^{v' _ m_n v_l}}
$$

$( D_m_n )$ 是采样自l同一个市场中的负例对。

- 冷启动问题: 选出新房源地点附近10miles半径范围内,相同房屋类型,相同价格带的3个有embedding向量的其他房屋,用它们的均值代表新房源的向量
- listing embedding检验: 向量维度32, session数量800M!
- embedding的重点是为了学习房屋特点、结构、类型、观感etc,等难以直接提取的相似性。作者开发了一个评估工具:Similarity Exploration Tool
- 学习到的类型: 船屋、树屋、城堡、小木屋、海景房 <https://youtu.be/1kJSAG91TrI>

### User-type & Listing-type Embeddings
用用户的订购的序列作为session, 来学习跨market的相似性。这种跨market的相似性可以用来解决用户来到一个新的market的时候给他推荐的问题。
但是这种方法由于数据稀疏会带来一下几个问题:

1. 大多数人的历史下单次数很少,甚至只有1个,没法学
2. listing的长尾特性,导致长尾的listing的向量学得不好
3. 由于时间跨度长,原来假设的相邻两个listing是相似的假设并成立,因为用户的偏好、价格等因素以及发生了改变

用 User-type & Listing-type 而不是ID



