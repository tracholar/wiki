---
title: "Collections源代码"
layout: page
date: 2019-10-28
---
[TOC]


## 类继承关系
- `Iterable`
    - `Iterator<T> iterator()`
- `Collection<E> extends Iterable<E>`
    - `int size()`
    - `boolean isEmpty()`
    - `boolean contains(Object o)`
    - `Iterator<E> iterator()`
    - `Object[] toArray()`
    - `T[] toArray(T[] a)`
    - `boolean add(E e)`
    - `boolean remove(Object o)`
    - `boolean containsAll(Collection<?> c)`
    - `boolean addAll(Collection<? extends E> c)`
    - `boolean removeAll(Collection<?> c)`
    - `boolean retainAll(Collection<?> c)`
    - `void clear()`
    - `boolean equals(Object o)`
    - `int hashCode()`