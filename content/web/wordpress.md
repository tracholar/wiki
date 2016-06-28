---
title: "wordpress开发指南"
layout: page
date: 2016-06-08
---
[TOC]

## 分类目录

### 为分类目录添加元数据
钩子
```php
//新建分类目录
add_action('category_add_form_fields', 'cat_add_form_fields_cb');
//编辑分类目录
add_action( 'category_edit_form_fields', 'cat_edit_form_fields_cb');

// 保存
add_action( 'edited_category', 'cat_form_save_fields_cb');
add_action( 'create_category', 'cat_form_save_fields_cb');
```

创建和更新元数据函数
```php
get_term_meta( $term_id, $meta_key, $single);
add_term_meta($term_id, $meta_key, $meta_value);
update_term_meta($term_id, $meta_key, $meta_value);
```


