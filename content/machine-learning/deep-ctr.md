---
title: "CTR预估中的深度模型"
layout: page
date: 2019-01-01
---
[TOC]

## 数据集
Electronics Dataset, 亚马逊1996-2014年间的评论数据, Electronics 只是其中电子产品这个类别的数据,它包括两个文件,一个是评论数据,另一个是电子产品的元数据。

- 评论数据:
    - reviewerID 评论用户ID
    - asin 产品ID
    - reviewerName 评论用户名字
    - helpful 有用的评分
    - reviewText 评论文本
    - overall 对产品的打分
    - summary 评论摘要
    - unixReviewTime 评论时间戳
    - reviewTime 评论时间
    
```json
{"reviewerID": "A3BY5KCNQZXV5U", "asin": "0594451647", "reviewerName": "Matenai", "helpful": [3, 3], "reviewText": "This product really works great but I found the following items you need to keep in mind:- You must have your power adapter connected for it to work...it plugs in the the bottom. It appears it needs power from the nook power adapter to operate.- The plug fits in loosely and you cannot move the Nook around much without holding the adapter in place.- On initial plugin it seems you need to rock it around to get the connection but then it seems solid.- It works with a 25ft high quality HDMI cable so you can put the NOOK across the room with you. Not tested with cheap cables.Warning...I found that my LG SmartTV 3D from a few years back does not work with this adapter but it does not seem to work with many things...bad software. This adapter works fine with other HDMI devices I have used like monitors and I am sure other TVs.Gave it five stars because it really is nice to extend the screen and use your Nook as a streaming server to your TV. Nice they made such a device.", "overall": 5.0, "summary": "This works great but read the details...", "unixReviewTime": 1390176000, "reviewTime": "01 20, 2014"}
```

- 产品元数据:
    - asin 产品ID
    - title 产品名字
    - price 价格
    - imUrl 图片URL
    - related 相关产品(推荐结果)
    - salesRank 
    - brand 品牌
    - categories 类别
    
    
```javascript
{'asin': '0594287995', 'imUrl': 'http://g-ecx.images-amazon.com/images/G/01/x-site/icons/no-img-sm._CB192198896_.gif', 'categories': [['Electronics', 'eBook Readers & Accessories', 'Covers']], 'title': 'Kate Spade Rain or Shine Magazine Cover for Nook Simple Touch'}
```


- <http://jmcauley.ucsd.edu/data/amazon/>
- 评论数据 <http://snap.stanford.edu/data/amazon/productGraph/categoryFiles/reviews_Electronics_5.json.gz>
- 元数据 <http://snap.stanford.edu/data/amazon/productGraph/categoryFiles/meta_Electronics.json.gz>

## 数据预处理
在这次实验中, 只用到 reviewerID, asin, unixReviewTime, categories 这4个字段, 并且简单起见, categories只用到最后一个类别。对每一个用户,评论的产品作为正样本, 再随机采样相等数量的其他产品作为负样本, 最后按照时间对他评论的产品排序,最后一个作为测试集(正样本和负样本一致)。这样,保证正样本和负样本都覆盖了所有用户。

预处理流程参考代码,我复用了来自DIN论文的代码 <>

基本信息: user_count: 192403	item_count: 63001	cate_count: 801	example_count: 1689188


